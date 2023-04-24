#ifndef mmu_h
#define mmu_h
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

namespace mmu
{
    using namespace std;
    const unsigned int MAX_FRAMES = 128;
    const unsigned int MAX_VPAGES = 64;
    vector<process_t> process_set;
    bool OPTION_O = true;
    bool OPTION_P = true;
    bool OPTION_F = true;
    bool OPTION_S = true;
    bool OPTION_x = true;
    bool OPTION_y = true;
    bool OPTION_f = true;
    bool OPTION_a = true;

    typedef struct
    {
        unsigned num_of_frame : 7;
        unsigned present : 1;
        unsigned write_protect : 1;
        unsigned modified : 1;
        unsigned referenced : 1;
        unsigned pageout : 1;
    } pte_t; // can only be total of 32-bit size and will check on this

    typedef struct
    {
        unsigned int proc_id : 4;
        unsigned int vpage : 6;
        unsigned int mapped : 1;
        unsigned int age;
    } frame_t;

    frame_t frame_table[MAX_FRAMES];

    typedef struct
    {
        unsigned int start_vpage : 6;
        unsigned int end_vpage : 6;
        unsigned int write_protect : 1;
        unsigned int file_mapped : 1;
    } VMA_t;

    typedef struct
    {
        pte_t page_table[MAX_VPAGES];
        vector<VMA_t> VMAs;
    } process_t;

    vector<int> get_randfile(string path)
    {
        ifstream a_file(path);
        vector<int> randvals;
        int item;
        while (a_file >> item)
        {
            randvals.push_back(item);
        }
        return randvals;
    }

    class FileLoader
    {
    private:
        ifstream input;
        istringstream iss;
        bool get_valid_line()
        {
            string line;
            while (getline(input, line))
            {
                if (line[0] == '#')
                    continue;
                iss.clear();
                iss.str(line);
                return true;
            }
            return false;
        }

    public:
        FileLoader(string file) : input(file)
        {
            get_valid_line();
            int num_process;
            iss >> num_process;
            while (num_process--)
            {
                process_t proc;
                get_valid_line();
                int num_vma;
                iss >> num_vma;
                proc.VMAs.resize(num_vma);
                while (num_vma--)
                {
                    get_valid_line();
                    unsigned int start_vpage, end_vpage;
                    bool write_protected, file_mapped;
                    iss >> start_vpage >> end_vpage >> write_protected >> file_mapped;
                    VMA_t VMA{start_vpage, end_vpage, write_protected, file_mapped};
                    proc.VMAs.push_back(VMA);
                }
                process_set.push_back(proc);
            }
        }
        bool get_next_instruction(char &operation, unsigned int &vpage)
        {
            if (get_valid_line())
            {
                iss >> operation >> vpage;
                return true;
            }

            return false;
        }
    };
    class Pager
    {
    public:
        virtual frame_t *select_victim_frame() = 0; // virtual base class
    };

    class FIFOPager : public Pager
    {
    private:
        unsigned int hand;

    public:
        virtual frame_t *select_victim_frame()
        {
            hand %= MAX_FRAMES;
            return &frame_table[hand++];
        }
    };
}
#endif