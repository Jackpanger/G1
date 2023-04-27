#ifndef mmu_h
#define mmu_h

#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <queue>

namespace mmu {
    using namespace std;
    const unsigned int MAX_VPAGES = 64;
    unsigned int MAX_FRAMES;
    bool OPTION_O = true;
    bool OPTION_P = true;
    bool OPTION_F = true;
    bool OPTION_S = true;
    bool OPTION_x = true;
    bool OPTION_y = true;
    bool OPTION_f = true;
    bool OPTION_a = true;
    unsigned long inst_count = 0;
    unsigned long ctx_switches = 0, process_exits = 0;
    unsigned long long cost = 0;

    enum cost_map {
        LOAD_STORE = 1,
        CONTEXT_SWITCH = 130,
        EXIT = 1230,
        MAPS = 350,
        UNMAPS = 410,
        INS = 3200,
        OUTS = 2750,
        FINS = 2350,
        FOUTS = 2800,
        ZEROS = 150,
        SEGV = 440,
        SEGPROT = 410
    };

    struct Pstats {
        unsigned long  unmaps = 0;
        unsigned long  maps = 0;
        unsigned long  ins = 0;
        unsigned long  outs = 0;
        unsigned long  fins = 0;
        unsigned long  fouts = 0;
        unsigned long  zeros = 0;
        unsigned long  segv = 0;
        unsigned long  segprot = 0;
    };
    typedef struct {
        unsigned int frame_id: 7;
        unsigned int proc_id: 4;
        unsigned int vpage: 6;
        unsigned int mapped:1;
        unsigned int age=0;
    } frame_t;

    typedef struct {
        unsigned present: 1;
        unsigned write_protect: 1;
        unsigned modified: 1;
        unsigned referenced: 1;
        unsigned pageout: 1;
        unsigned frame: 7;
        unsigned file_mapped: 1;
    } pte_t; // can only be total of 32-bit size and will check on this

    vector <frame_t> frame_table;

    typedef struct {
        unsigned int start_vpage: 6;
        unsigned int end_vpage: 6;
        unsigned int write_protect: 1;
        unsigned int file_mapped: 1;
    } vma_t;

    struct process_t {
        int pid;
        vector <pte_t> page_table;
        vector <vma_t> vmas;
        Pstats pstats;
    };

    vector <process_t> process_set;
    queue<unsigned int> frame_free_list;

    class FileLoader {
    private:
        ifstream input;
        istringstream iss;
        bool get_valid_line() {
            string line;
            while (getline(input, line)) {
                if (line[0] == '#')
                    continue;
                iss.clear();
                iss.str(line);
                return true;
            }
            return false;
        }

    public:
        FileLoader(string file) : input(file) {
            get_valid_line();
            int num_process;
            iss >> num_process;
            while (num_process--) {
                process_t proc;
                get_valid_line();
                int num_vma;
                iss >> num_vma;
                while (num_vma--) {
                    get_valid_line();
                    unsigned int start_vpage, end_vpage;
                    bool write_protected, file_mapped;
                    iss >> start_vpage >> end_vpage >> write_protected >> file_mapped;
                    vma_t vma{start_vpage, end_vpage, write_protected, file_mapped};
                    proc.vmas.push_back(vma);
                }
                proc.page_table.resize(MAX_VPAGES);
                process_set.push_back(proc);
            }
        }

        bool get_next_instruction(char &operation, unsigned int &vpage) {
            if (get_valid_line()) {
                iss >> operation >> vpage;
                return true;
            }
            return false;
        }
    };

    class Pager {
    public:
        virtual unsigned int select_victim_frame() = 0; // virtual base class
        virtual void set_age(unsigned int frame){}; // virtual base class
    };



    class FIFOPager : public Pager {
    private:
        unsigned int hand=0;
    public:
        virtual unsigned int select_victim_frame() {
            hand %= MAX_FRAMES;
            return hand++;
        }
    };
    class RandomPager: public Pager {
    private:
        vector<int> randvals;
        int ofs = 1;
        vector<int> get_randfile(string path) {
            ifstream a_file(path);
            vector<int> randvals;
            int item;
            while (a_file >> item) {
                randvals.push_back(item);
            }
            return randvals;
        }
        unsigned int myrandom(unsigned int bound)
        {
            if (ofs == randvals.size()) ofs = 1;
            return randvals[ofs++] % bound;
        }
    public:
        RandomPager(string rfile): randvals(get_randfile(rfile)){}
        unsigned int select_victim_frame(){
            return myrandom(MAX_FRAMES);
        }
    };

    class CLOCKPager: public Pager{
    private:
        unsigned hand = 0;
    public:
        unsigned int select_victim_frame(){
            while(true){
                frame_t frame = frame_table.at(hand);
                pte_t& pre_pte = process_set.at(frame.proc_id).page_table.at(frame.vpage);
                if (pre_pte.referenced){
                    pre_pte.referenced= false;
                    hand = (hand+1)%MAX_FRAMES;
                }else{
                    unsigned result = hand;
                    hand = (hand+1)%MAX_FRAMES;
                    return result;
                }
            }
        }
    };
    class ESCPager: public Pager{
    private:
        const unsigned int PERIODIC_TIME = 50;
        unsigned long last_time = 0;
        unsigned int hand=0;
    public:
        unsigned int select_victim_frame(){
            int classes[4]={-1,-1,-1,-1};
            int lowest= 4;
            int start = hand;
            bool reset = false;
            if (inst_count-last_time>=PERIODIC_TIME) reset = true;
            while (true){
                frame_t frame = frame_table.at(hand);
                pte_t& pre_pte = process_set.at(frame.proc_id).page_table.at(frame.vpage);
                int class_ = pre_pte.modified+2*pre_pte.referenced;
                lowest = min(lowest,class_);
                if (classes[class_]==-1) classes[class_] = hand;
                if (class_==0&&!reset) break;
                if (reset) pre_pte.referenced= false;
                hand = (hand+1) % MAX_FRAMES;
                if (hand==start) break;
            }
            if (reset) last_time = inst_count;
            hand = (classes[lowest]+1)%MAX_FRAMES;
            return classes[lowest];
        }
    };
    class AGING: public Pager{
    private:
        unsigned int hand = 0;
    public:
        void set_age(unsigned int frame){
            frame_table[frame].age = 0;
        }
        unsigned int select_victim_frame(){
            unsigned int start = hand;
            unsigned int lowest = hand;
            while (true){
                frame_t& frame = frame_table.at(hand);
                pte_t& pre_pte = process_set.at(frame.proc_id).page_table.at(frame.vpage);
                frame.age>>=1;
                if (pre_pte.referenced){
                    frame.age |= 0x80000000;
                    pre_pte.referenced = false;
                }
                if (frame.age<frame_table.at(lowest).age){
                    lowest = hand;
                }
                hand = (hand+1)%MAX_FRAMES;
                if (hand==start) break;
            }
            hand = (lowest+1)%MAX_FRAMES;
            return lowest;
        }
    };
    class Working_Set: public Pager{
    private:
        const unsigned int TAU = 49;
        unsigned int hand = 0;
        vector<unsigned int> last_time;
    public:
        Working_Set(){
            last_time.resize(MAX_FRAMES);
        };
        void set_age(unsigned int frame){
            last_time[frame] = inst_count;
        }
        unsigned int select_victim_frame(){
            unsigned int start = hand;
            unsigned int lowest = hand;
            while (true){
                frame_t& frame = frame_table.at(hand);
                pte_t& pre_pte = process_set.at(frame.proc_id).page_table.at(frame.vpage);
                if (pre_pte.referenced){
                    last_time[hand] = inst_count;
                    pre_pte.referenced = false;
                }
                if (last_time[hand]+TAU<inst_count){
                    lowest = hand;
                    break;
                }
                if (last_time[hand]<last_time[lowest]){
                    lowest = hand;
                }
                hand = (hand+1)%MAX_FRAMES;
                if (hand==start) break;
            }
            hand = (lowest+1)%MAX_FRAMES;
            return lowest;
        }
    };
}
#endif