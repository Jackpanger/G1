#ifndef sched_h
#define sched_h
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <stack>
namespace sched
{
    using namespace std;
    int rfile;
    int TOTALIO;
    int incre = 0; // increment for eid
    int MAXPRIO = 4;
    int QUANTUM = 10000;

    vector<int> get_randfile(string path)
    {
        ifstream a_file(path);
        vector<int> randvals;
        int item;
        a_file >> rfile;
        while (a_file >> item)
        {
            randvals.push_back(item);
        }
        return randvals;
    }
    typedef enum STATE
    {
        STATE_RUNNING,
        STATE_BLOCKED,
        STATE_READY,
        STATE_CREATED

    } process_state_t;
    enum TRANSITION
    {
        TRANS_TO_READY,
        TRANS_TO_PREEMPT,
        TRANS_TO_RUN,
        TRANS_TO_BLOCK
    };

    struct Process
    {
        const int pid;
        const int AT;
        const int TC;
        const int CB;
        const int IO;
        const int static_priority;

        int state_ts;
        int rm;           // remaining CPU time
        int rmCPUbust;    // remaining CPU burst
        int termTime;     // teminate time
        int iotime;       // io time
        int cwaittime;    // cpu waiting time
        int dynamic_prio; // dynamic priority

        Process(int pid, int AT, int TC, int CB, int IO,
                int static_priority) : pid(pid), AT(AT), TC(TC), CB(CB), cwaittime(0), iotime(0),
                                       IO(IO), static_priority(static_priority), rm(TC), state_ts(AT), rmCPUbust(0) {}
    };
    struct EVENT
    {
        Process *evtProcess;
        int evtTimeStamp;
        TRANSITION transition;
        process_state_t oldstate;
        process_state_t newstate;
        int eid;
        EVENT(Process *evtProcess, int evtTimeStamp, process_state_t oldstate, process_state_t newstate, TRANSITION transition)
            : eid(incre++), evtProcess(evtProcess), evtTimeStamp(evtTimeStamp), oldstate(oldstate), newstate(newstate), transition(transition) {}
    };

    struct des_operator
    {
        bool operator()(EVENT *a, EVENT *b)
        {
            if (a->evtTimeStamp == b->evtTimeStamp)
                return a->eid < b->eid;
            else
                return a->evtTimeStamp < b->evtTimeStamp;
        }
    };
    struct sched_operator
    {
        bool operator()(Process *a, Process *b)
        {
            return a->pid < b->pid;
        }
    };

    struct SRTF_operator
    {
        bool operator()(Process *a, Process *b)
        {
            if (a->rm == b->rm)
                return a->pid < b->pid;
            return a->rm < b->rm;
        }
    };

    class Scheduler
    {
    public:
        string algorithm;
        set<Process *, sched_operator> Done;

    public:
        int quantum;
        Scheduler(string algorithm, int quantum) : algorithm(algorithm), quantum(quantum){};
        virtual void add_process(Process *p) = 0;
        virtual Process *get_next_process() = 0;
        void decay(Process *p) { p->dynamic_prio--; };
        virtual bool does_preempt() { return false; }; // returns true for ‘E’ scheds, else false.
        virtual int getSize() = 0;
        void done(Process *p)
        {
            Done.insert(p);
        }
        void display()
        {
            printf("%s\n", algorithm.c_str());
            double totalCPU, cpuUtil, ioUtil, totalTT, totalCPUwaiting;
            int maxTermTime = 0;
            for (set<Process *>::iterator it = Done.begin(); it != Done.end(); it++)
            // for (size_t i = 0; i < Done.size(); i++)
            {
                Process *p = *it;
                totalCPU += p->TC;
                int turnaround = p->termTime - p->AT;
                totalTT += turnaround;
                totalCPUwaiting += p->cwaittime;
                maxTermTime = max(maxTermTime, p->termTime);
                printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                       p->pid, p->AT, p->TC, p->CB, p->IO, p->static_priority, p->termTime, turnaround, p->iotime, p->cwaittime);
            }
            cpuUtil = totalCPU / (double)maxTermTime;
            ioUtil = TOTALIO / (double)maxTermTime;
            printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", maxTermTime, 100 * cpuUtil, 100 * ioUtil, totalTT / Done.size(),
                   totalCPUwaiting / Done.size(), 100.0 * Done.size() / maxTermTime);
        }
    };

    class Fscheduler : public Scheduler
    {
    private:
        queue<Process *> runQ;

    public:
        Fscheduler() : Scheduler("FCFS", 10000){};
        int getSize()
        {
            return (int)runQ.size();
        }
        virtual void add_process(Process *p)
        {
            runQ.push(p);
        };
        virtual Process *get_next_process()
        {
            if (runQ.empty())
                return nullptr;
            Process *proc = runQ.front();
            runQ.pop();
            return proc;
        };
    };

    class Lscheduler : public Scheduler
    {
    private:
        stack<Process *> runQ;

    public:
        Lscheduler() : Scheduler("LCFS", 10000){};
        int getSize()
        {
            return (int)runQ.size();
        }
        virtual void add_process(Process *p)
        {
            runQ.push(p);
        };
        virtual Process *get_next_process()
        {
            if (runQ.empty())
                return nullptr;
            Process *proc = runQ.top();
            runQ.pop();
            return proc;
        };
    };

    class Sscheduler : public Scheduler
    {
    private:
        set<Process *, SRTF_operator> runQ;

    public:
        Sscheduler() : Scheduler("SRTF", 10000){};
        int getSize()
        {
            return (int)runQ.size();
        }
        virtual void add_process(Process *p)
        {
            runQ.insert(p);
        };
        virtual Process *get_next_process()
        {
            if (runQ.empty())
                return nullptr;
            Process *proc = *runQ.begin();
            runQ.erase(runQ.begin());
            return proc;
        };
    };

    class RRscheduler : public Scheduler
    {
    private:
        queue<Process *> runQ;

    public:
        RRscheduler(int quantum) : Scheduler("RR " + to_string(quantum), quantum){};
        int getSize()
        {
            return (int)runQ.size();
        }
        virtual void add_process(Process *p)
        {
            runQ.push(p);
        };
        virtual Process *get_next_process()
        {
            if (runQ.empty())
                return nullptr;
            Process *proc = runQ.front();
            runQ.pop();
            return proc;
        };
    };
    class Prioscheduler : public Scheduler
    {
    private:
        vector<queue<Process *> > activeQ, expiredQ;
        // vector<int> bitmap;

    public:
        int expired_size;
        int active_size;
        Prioscheduler(int quantum, int maxPriority) : Scheduler("PRIO " + to_string(quantum), quantum),
                                                      expired_size(0), active_size(0)
        {
            activeQ.resize(maxPriority);
            expiredQ.resize(maxPriority);
            MAXPRIO = maxPriority;
        };
        int getSize()
        {
            return active_size + expired_size;
        }
        virtual void add_process(Process *p)
        {
            // printf("iam here\n");
            if (p->dynamic_prio >= 0)
            {
                                // printf("sta %d \n",p->static_priority);

                activeQ.at(p->dynamic_prio).push(p);
                active_size++;
                // printf("pid %d prio %d size: %d %d\n", p->pid, p->dynamic_prio, active_size, expired_size);
            }
            else
            {
                p->dynamic_prio = p->static_priority - 1;
                expiredQ.at(p->dynamic_prio).push(p);
                expired_size++;
            }
        };
        virtual Process *get_next_process()
        {

            if (expired_size + active_size == 0)
                return nullptr;
            if (active_size == 0)
            {
                activeQ.swap(expiredQ);
                active_size = expired_size;
                expired_size = 0;
            }
            for (size_t i = MAXPRIO - 1; i >= 0; i--)
            {
                // printf("out\n");
                // printf("%d\n",activeQ.at(i).size());
                queue<Process *> q = activeQ.at(i);
                if (q.empty())
                    continue;
                // Process *proc = ;
                activeQ.at(i).pop();
                active_size--;
                // printf("prio size: %d %d done size %d\n", active_size, expired_size, Done.size());
                // printf("pid in  %d priority %d\n", proc->pid, proc->dynamic_prio);

                return q.front();
            }
            return nullptr;
        };
    };

    class PrePrioscheduler : public Scheduler
    {
    private:
        vector<queue<Process *> > activeQ, expiredQ;
        vector<int> bitmap;

    public:
        int expired_size;
        int active_size;
        PrePrioscheduler(int quantum, int maxPriority) : Scheduler("PREPRIO " + to_string(quantum), quantum),
                                                         expired_size(0), active_size(0)
        {
            activeQ.resize(maxPriority);
            expiredQ.resize(maxPriority);
            MAXPRIO = maxPriority;
        };
        int getSize()
        {
            return active_size + expired_size;
        }
        virtual bool does_preempt() { return true; };
        virtual void add_process(Process *p)
        {
            if (p->dynamic_prio >= 0)
            {
                //                 printf("sta %d \n",p->static_priority);
                activeQ.at(p->dynamic_prio).push(p);
                active_size++;
                // printf("pid %d prio %d size: %d %d\n", p->pid, p->dynamic_prio, active_size, expired_size);
            }
            else
            {
                p->dynamic_prio = p->static_priority - 1;
                expiredQ.at(p->dynamic_prio).push(p);
                expired_size++;
            }
        };
        virtual Process *get_next_process()
        {

            if (expired_size + active_size == 0)
                return nullptr;
            if (active_size == 0)
            {
                activeQ.swap(expiredQ);
                active_size = expired_size;
                expired_size = 0;
            }
            for (size_t i = MAXPRIO - 1; i >= 0; i--)
            {
                // printf("out\n");
                // printf("%d\n",activeQ.at(i).size());
                queue<Process *> q = activeQ.at(i);
                if (q.empty())
                    continue;
                // Process *proc = ;
                activeQ.at(i).pop();
                active_size--;
                // printf("prio size: %d %d done size %d\n", active_size, expired_size, Done.size());
                // printf("pid in  %d priority %d\n", proc->pid, proc->dynamic_prio);

                return q.front();
            }
            return nullptr;
        };
    };

}
#endif