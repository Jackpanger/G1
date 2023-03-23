#include "sched.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <set>
#include <cstring>

using namespace sched;
using namespace std;

int vflag = 0; // verbose
int tflag = 0; // traces the event execution
int eflag = 0; // shows the eventQ before and after an event is inserted
int pflag = 0; // shows for the E scheduler the decision
int iflag = 0; // runs program in single step mode

int CURRENT_TIME = 0;
bool CALL_SCHEDULER = false;
int LAST_IO_TIME = 0; // Last IO finish time
int ofs = 0;          // ofs for random

vector<int> randvals;
Process *CURRENT_RUNNING_PROCESS = nullptr;

int myrandom(int burst)
{
    if (ofs == randvals.size())
    {
        ofs = 0;
    }
    return 1 + (randvals[ofs++] % burst);
}
class DESLayer
{
private:
    set<EVENT *, des_operator> eventQ;

public:
    DESLayer(string file)
    {
        int AT, TC, CB, IO, pid = 0;
        ifstream input(file);
        while (input >> AT >> TC >> CB >> IO)
        {
            int static_priority = myrandom(MAXPRIO);
            Process *process = new Process(pid++, AT, TC, CB, IO, static_priority);
            EVENT *event = new EVENT(process, AT, STATE_CREATED, STATE_READY, TRANS_TO_READY);
            eventQ.insert(event);
        }
    }
    EVENT *get_event()
    {
        if (eventQ.empty())
        {
            return nullptr;
        }

        EVENT *event = *eventQ.begin();
        eventQ.erase(eventQ.begin());
        return event;
    }
    void put_event(EVENT *evt)
    {
        eventQ.insert(evt);
    }
    // find an event in the des layer
    EVENT *find_event(Process *p)
    {
        for (set<EVENT *, des_operator>::iterator it = eventQ.begin(); it != eventQ.end(); it++)
        {
            EVENT *e = *it;
            if (e->evtProcess->pid == p->pid)
            {
                return e;
            }
        }
        return nullptr;
    }
    // remove a specific event in the des layer
    void rm_event(EVENT *event)
    {
        for (set<EVENT *, des_operator>::iterator it = eventQ.begin(); it != eventQ.end(); it++)
        {
            EVENT *e = *it;
            if (e->eid == event->eid)
            {
                eventQ.erase(it);
                break;
            }
        }
    }
    int get_next_event_time()
    {
        if (eventQ.empty())
        {
            return -1;
        }
        EVENT *event = *eventQ.begin();
        return event->evtTimeStamp;
    }
};

void Simulation(DESLayer des, Scheduler &sched)
{
    EVENT *evt;
    int i = 0;
    while ((evt = des.get_event()))
    {
        Process *proc = evt->evtProcess;
        CURRENT_TIME = evt->evtTimeStamp;
        int transition = evt->transition;
        int timeInPrevState = CURRENT_TIME - proc->state_ts; // for accounting
        proc->state_ts = CURRENT_TIME;
        delete evt;
        evt = nullptr; // remove cur event obj and don’t touch anymore
        switch (transition)
        { // encodes where we come from and where we go
        case TRANS_TO_READY:
            // must come from BLOCKED or CREATED
            // add to run queue, no event created
            proc->dynamic_prio = proc->static_priority - 1;
            proc->rmCPUbust = 0;
            sched.add_process(proc);
            if (sched.does_preempt() && CURRENT_RUNNING_PROCESS) // if is preemptive prio, go this way
            {
                if (proc->dynamic_prio > CURRENT_RUNNING_PROCESS->dynamic_prio) // check dynamic prio
                {
                    EVENT *e = des.find_event(CURRENT_RUNNING_PROCESS); // find the next event of the current process
                    if (e->evtTimeStamp != CURRENT_TIME) // if the time is same, then we should continue run this process
                    {
                        CURRENT_RUNNING_PROCESS->rm += e->evtTimeStamp - CURRENT_TIME;
                        CURRENT_RUNNING_PROCESS->rmCPUbust += e->evtTimeStamp - CURRENT_TIME;
                        des.rm_event(e);
                        EVENT *next = new EVENT(CURRENT_RUNNING_PROCESS, CURRENT_TIME, STATE_RUNNING, STATE_READY, TRANS_TO_PREEMPT);
                        des.put_event(next);
                    }
                }
            }
            CALL_SCHEDULER = true;
            break;
        case TRANS_TO_PREEMPT: // similar to TRANS_TO_READY
            // must come from RUNNING (preemption)
            // add to runqueue (no event is generated)
            sched.decay(proc);
            sched.add_process(proc);
            CALL_SCHEDULER = true;
            CURRENT_RUNNING_PROCESS = nullptr;
            break;
        case TRANS_TO_RUN:
            int cb, burst;
            if (CURRENT_RUNNING_PROCESS->rmCPUbust == 0)
            {
                // get a random cpu burst
                burst = myrandom(CURRENT_RUNNING_PROCESS->CB);
            }
            else
            {
                // use the previous cpu burst
                burst = CURRENT_RUNNING_PROCESS->rmCPUbust;
            }
            // if is smaller than the remaining, use remaining as burst
            cb = min(burst, CURRENT_RUNNING_PROCESS->rm);
            if (cb > sched.quantum)
            {
                // larger than the quantum, preeempt
                proc->rm -= sched.quantum;
                proc->rmCPUbust = cb - sched.quantum;
                int time = CURRENT_TIME + sched.quantum;
                EVENT *next = new EVENT(proc, time, STATE_RUNNING, STATE_READY, TRANS_TO_PREEMPT);
                des.put_event(next);
            }
            else
            {
                // smaller than the quantum, block
                proc->rm -= cb;
                proc->rmCPUbust = 0;
                int time = CURRENT_TIME + cb;
                EVENT *next = new EVENT(proc, time, STATE_RUNNING, STATE_BLOCKED, TRANS_TO_BLOCK);
                des.put_event(next);
            }
            // create event for either preemption or blocking
            break;
        case TRANS_TO_BLOCK:
            // create an event for when process becomes READY again
            int ib = 0;
            if (proc->rm == 0)
            {
                // process is done
                proc->termTime = CURRENT_TIME;
                sched.done(proc);
            }
            else
            {
                // do io
                ib = myrandom(proc->IO);
                CURRENT_RUNNING_PROCESS->iotime += ib;
                EVENT *next = new EVENT(proc, CURRENT_TIME + ib, STATE_BLOCKED, STATE_READY, TRANS_TO_READY);
                des.put_event(next);
            }
            int io_finish = CURRENT_TIME + ib;
            if (io_finish > LAST_IO_TIME)
            {
                TOTALIO += min(ib, io_finish - LAST_IO_TIME);
                LAST_IO_TIME = io_finish;
            }
            CURRENT_RUNNING_PROCESS = nullptr;
            CALL_SCHEDULER = true;
            break;
        }
        if (CALL_SCHEDULER)
        {

            if (des.get_next_event_time() == CURRENT_TIME)
                continue;           // process next event from Event queue
            CALL_SCHEDULER = false; // reset global flag
            if (CURRENT_RUNNING_PROCESS == nullptr)
            {

                CURRENT_RUNNING_PROCESS = sched.get_next_process();

                if (CURRENT_RUNNING_PROCESS == nullptr)
                {
                    continue;
                }
                CURRENT_RUNNING_PROCESS->cwaittime += CURRENT_TIME - CURRENT_RUNNING_PROCESS->state_ts;
                EVENT *next = new EVENT(CURRENT_RUNNING_PROCESS, CURRENT_TIME, STATE_READY, STATE_RUNNING, TRANS_TO_RUN);
                des.put_event(next);
            }
        }
    }
}

int main(int argc, char *const argv[])
{
    char *svalue;
    int index;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "vtepis:")) != -1)
    {
        switch (c)
        {
        case 'v':
            vflag = 1;
            break;
        case 't':
            tflag = 1;
            break;
        case 'e':
            eflag = 1;
            break;
        case 'p':
            pflag = 1;
            break;
        case 'i':
            iflag = 1;
            break;
        case 's':
            svalue = optarg;
            break;
        case '?':
            if (optopt == 's')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            return 1;
        default:
            abort();
        }
    }
    argc -= optind;
    if (argc < 2)
    {
        fprintf(stderr, "input or rfile is missing`\\x%x'.\n",
                optopt);
        return 1;
    }
    argv += optind;
    string file = argv[0];
    Scheduler *sched;
    switch (svalue[0])
    {
    case 'F':
        sched = new Fscheduler();
        break;
    case 'L':
        sched = new Lscheduler();
        break;
    case 'S':
        sched = new Sscheduler();
        break;
    case 'R':
    {
        int quantum;
        sscanf(svalue + 1, "%d", &quantum);
        if (!quantum)
        {
            sched = new RRscheduler(QUANTUM);
        }
        else
        {
            sched = new RRscheduler(quantum);
        }
        break;
    }
    case 'P':
    {
        int quantum, maxprio;
        if (strchr(svalue + 1, ':'))
        {
            sscanf(svalue + 1, "%d:%d", &quantum, &maxprio);
            sched = new Prioscheduler(quantum, maxprio);
        }
        else
        {
            sscanf(svalue + 1, "%d", &quantum);
            sched = new Prioscheduler(quantum, 4);
        }
        break;
    }

    case 'E':
    {
        int quantum, maxprio;
        if (strchr(svalue + 1, ':'))
        {
            sscanf(svalue + 1, "%d:%d", &quantum, &maxprio);
            sched = new PrePrioscheduler(quantum, maxprio);
        }
        else
        {
            sscanf(svalue + 1, "%d", &quantum);
            sched = new PrePrioscheduler(quantum, 4);
        }
        break;
    }

    default:
        fprintf(stderr,
                "Unknown option character `\\x%x'.\n",
                svalue[0]);
        abort();
    }
    randvals = get_randfile(argv[1]);
    DESLayer *des = new DESLayer(file);
    Simulation(*des, *sched);
    sched->display();
    delete (des);
    return 0;
}
