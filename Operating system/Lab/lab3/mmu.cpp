#include "mmu.h"
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

using namespace std;
using namespace mmu;

pte_t page_table[MAX_VPAGES]; // a per process array of fixed size=64 of pte_t not pte_t pointers !
Pager *THE_PAGER = nullptr;
process_t *current_process;
vector<int> randvals;
int ofs = 0;    



frame_t *allocate_frame_from_free_list()
{

    return nullptr;
}

frame_t *get_frame()
{
    frame_t *frame = allocate_frame_from_free_list();
    if (frame == NULL)
        frame = THE_PAGER->select_victim_frame();
    return frame;
}

void simulation(FileLoader &exec)
{
    char operation;
    unsigned int vpage;
    while (exec.get_next_instruction(operation, vpage))
    {
        // handle special case of “c” and “e” instruction // now the real instructions for read and write
        pte_t *pte = &current_process->page_table[vpage];
        if (!pte->present)
        {
            // this in reality generates the page fault exception and now you execute
            // verify this is actually a valid page in a vma if not raise error and next inst
            frame_t *newframe = get_frame();
            //-> figure out if/what to do with old frame if it was mapped
            // see general outline in MM-slides under Lab3 header and writeup below
            // see whether and how to bring in the content of the access page.
        }
        // now the page is definitely present
        // check write protection
        // simulate instruction execution by hardware by updating the R/M PTE bits update_pte(read/modify) bits based on operations.
    }
}

int main(int argc, char *const argv[])
{
    char *fvalue;
    char *avalue;
    char *ovalue;
    int index;
    int num_frames;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "f:a:o:")) != -1)
    {
        switch (c)
        {
        case 'f':
            fvalue = optarg;
            break;
        case 'a':
            avalue = optarg;
            break;
        case 'o':
            ovalue = optarg;
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
    string input = argv[0];
    FileLoader exec = FileLoader(input);
    switch (avalue[0])
    {
    case 'f':
    {
        printf("FIFO pager is used\n");
        THE_PAGER = new FIFOPager();
        break;
    }
    case 'r':
        break;
    case 'c':
        break;
    case 'e':
        break;
    case 'a':
        break;
    case 'w':
        break;
    default:
        fprintf(stderr,
                "Unknown option character `\\x%x'.\n",
                avalue[0]);
        abort();
    }

    for (char &c : string(ovalue))
    {
        switch (c)
        {
        case 'O':
        {
            OPTION_O = true;
            break;
        }
        case 'P':
            OPTION_P = true;
            break;
        case 'F':
            OPTION_F = true;
            break;
        case 'S':
            OPTION_S = true;
            break;
        case 'x':
            OPTION_x = true;
            break;
        case 'y':
            OPTION_y = true;
            break;
        case 'f':
            OPTION_f = true;
            break;
        case 'a':
            OPTION_a = true;
            break;
        default:
            fprintf(stderr,
                    "Unknown option character `\\x%x'.\n",
                    c);
            abort();
        }
    }
    sscanf(fvalue + 1, "%d", &num_frames);
    simulation(exec);
    delete (THE_PAGER);
    if (strcmp(ovalue, "oOPFS"))
    {
        printf("OPFS\n");
    }

    return 0;
}
