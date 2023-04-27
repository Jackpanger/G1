#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace mmu;

pte_t page_table[MAX_VPAGES]; // a per process array of fixed size=64 of pte_t not pte_t pointers !
Pager *THE_PAGER = nullptr;
process_t *current_process;
unsigned int current_pid;
Pstats p;
Pstats *pstats = &p;
bool free_frame_list_full = false;


int allocate_frame_from_free_list() {
    if (frame_free_list.empty()) {
        free_frame_list_full = true;
        return -1;
    }
    free_frame_list_full = false;
    int hand = frame_free_list.front();
    frame_free_list.pop();
    return hand;
}

unsigned int get_frame() {
    int frame = allocate_frame_from_free_list();
    if (frame == -1) {
        frame = THE_PAGER->select_victim_frame();

    }
    return frame;
}

bool page_fault_handler(pte_t *pte, unsigned int vpage) {
    for (vma_t vma: current_process->vmas) {
        if (vpage >= vma.start_vpage && vpage <= vma.end_vpage) {
            pte->write_protect = vma.write_protect;
            pte->file_mapped = vma.file_mapped;
            return true;
        }
    }
    return false;
}

void unmap_old_frame(frame_t *newframe){
    unsigned int pid = newframe->proc_id;
    unsigned int pre_vpage = newframe->vpage;
    process_t *pre_proc = &process_set.at(pid);
    pte_t *pre_pte = &pre_proc->page_table[pre_vpage];
    if (OPTION_O) printf(" UNMAP %d:%d\n", pid, pre_vpage);
    pre_proc->pstats.unmaps++;
    cost += cost_map::UNMAPS;
    pre_pte->present = false;
    if (pre_pte->modified) {
        pre_pte->pageout = !pre_pte->file_mapped;
        if (pre_pte->file_mapped) {
            if (OPTION_O)
                printf(" FOUT\n");
            pre_proc->pstats.fouts++;
            cost += cost_map::FOUTS;
        } else {
            if (OPTION_O)
                printf(" OUT\n");
            pre_proc->pstats.outs++;
            cost += cost_map::OUTS;
        }
    }
}
void option_operation(pte_t* pte){
    if (pte->pageout) {
        if (OPTION_O) printf(" IN\n");
        current_process->pstats.ins++;
        cost += cost_map::INS;
    } else if (pte->file_mapped) {
        if (OPTION_O) printf(" FIN\n");
        current_process->pstats.fins++;
        cost += cost_map::FINS;
    } else {
        if (OPTION_O) printf(" ZERO\n");
        current_process->pstats.zeros++;
        cost += cost_map::ZEROS;
    }
}

void exit_process(unsigned int vpage){
    for (int i = 0; i < current_process->page_table.size(); i++) {
        pte_t &pte = current_process->page_table[i];
        pte.pageout = false;
        if (pte.present) {
            pte.present = false;
            frame_table[pte.frame].mapped = false;
            frame_free_list.push(pte.frame);
            if (OPTION_O) printf(" UNMAP %d:%d\n",vpage, i );
            current_process->pstats.unmaps++;
            cost += cost_map::UNMAPS;
            if (pte.file_mapped && pte.modified) {
                if (OPTION_O) printf(" FOUT\n");
                current_process->pstats.fouts++;
                cost += cost_map::FOUTS;
            }
        }
    }
}

void simulation(FileLoader &exec) {
    char operation;
    unsigned int vpage;
    int t = 0;
    while (exec.get_next_instruction(operation, vpage)) {
        if (OPTION_O) printf("%d: ==> %c %d\n", inst_count++, operation, vpage);

        switch (operation) {
            case 'c': {
                current_process = &process_set.at(vpage);
                current_pid = vpage;
//                ::printf("current_pid: %u\n",current_pid);
                ctx_switches++;
                cost+=cost_map::CONTEXT_SWITCH;
                break;
            }
            case 'w':
            case 'r': {
                // handle special case of “c” and “e” instruction // now the real instructions for read and write
                pte_t *pte = &current_process->page_table[vpage];

                pte->referenced = true;
                cost+=cost_map::LOAD_STORE;
                if (!pte->present) {
                    // this in reality generates the page fault exception and now you execute
                    // verify this is actually a valid page in a vma if not raise error and next inst
                    pte->modified=false;
                    if (page_fault_handler(pte, vpage)) {
                        unsigned int frame_id = get_frame();
                        frame_t *newframe = &frame_table[frame_id];
                        // UNMAP old frame
                        if (free_frame_list_full){
                            unmap_old_frame(newframe);
                        }
                        // map new frame
                        newframe->proc_id = current_pid;
                        newframe->vpage = vpage;
                        newframe->mapped = true;
                        pte->present = true;
                        pte->frame = frame_id;
                        // print option_O
                        option_operation(pte);
                        if (OPTION_O) printf(" MAP %d\n", frame_id);
                        current_process->pstats.maps++;
                        cost += cost_map::MAPS;
                        THE_PAGER->set_age(frame_id);
                    } else {
                        if (OPTION_O) printf(" SEGV\n");
                        current_process->pstats.segv++;
                        cost += cost_map::SEGV;
                    }
                    //-> figure out if/what to do with old frame if it was mapped
                    // see general outline in MM-slides under Lab3 header and writeup below
                    // see whether and how to bring in the content of the access page.
                }
                if (operation == 'w') {
                    if (pte->write_protect) {
                        if (OPTION_O) printf(" SEGPROT\n");
                        current_process->pstats.segprot++;
                        cost += cost_map::SEGPROT;
                    } else {
                        pte->modified = true;
                    }
                }
                break;
            }
            case 'e': {
//                printf("EXIT current process %d\n",current_pid);
                process_exits++;
                cost+= cost_map::EXIT;
                exit_process(vpage);
                break;
            }

        }

        // now the page is definitely present
        // check write protection
        // simulate instruction execution by hardware by updating the R/M PTE bits update_pte(read/modify) bits based on operations.
    }

}
void display(){
    if (OPTION_P) {
        int i;
        for (i = 0; i < process_set.size(); i++) {
            printf("PT[%d]: ", i);
            vector <pte_t> page_table = process_set[i].page_table;
            int j;
            for (j = 0; j < page_table.size(); j++) {
                pte_t pte = page_table[j];
                if (pte.present) {
                    printf("%d:", j);
                    if (pte.referenced) printf("R");
                    else printf("-");
                    if (pte.modified) printf("M");
                    else printf("-");
                    if (pte.pageout) printf("S ");
                    else printf("- ");
                } else {
                    if (pte.pageout) printf("# ");
                    else if (j!=page_table.size()-1) printf("* ");
                    else printf("*");
                }
            }
            printf("\n");
        }

    }
    if (OPTION_F) {
        printf("FT: ");
        for (size_t i = 0; i < frame_table.size(); i++) {
            frame_t frame = frame_table[i];
            if (frame.mapped) {
                printf("%d:%d ", frame.proc_id, frame.vpage);
            } else {
                printf("* ");
            }
        }
        printf("\n");
    }
    if (OPTION_S) {
        for (int i = 0; i < process_set.size(); i++) {
            Pstats pstats = process_set[i].pstats;
            printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
                   i,
                   pstats.unmaps, pstats.maps, pstats.ins, pstats.outs,
                   pstats.fins, pstats.fouts, pstats.zeros,
                   pstats.segv, pstats.segprot);
        }
        printf("TOTALCOST %lu %lu %lu %llu %lu\n",
               inst_count, ctx_switches, process_exits, cost, sizeof(pte_t));
    }
}
int main(int argc, char *const argv[]) {
    char *fvalue;
    char *avalue;
    char *ovalue;
    int num_frames;
    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "f:a:o:")) != -1) {
        switch (c) {
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
    if (argc < 2) {
        fprintf(stderr, "input or rfile is missing`\\x%x'.\n",
                optopt);
        return 1;
    }
    argv += optind;
    string input = argv[0];
    FileLoader exec = FileLoader(input);
    sscanf(fvalue, "%d", &num_frames);
    MAX_FRAMES = num_frames;
    switch (avalue[0]) {
        case 'f': {
            THE_PAGER = new FIFOPager();
            break;
        }
        case 'r':
        {
            THE_PAGER = new RandomPager(argv[1]);
            break;
        }
        case 'c':{
            THE_PAGER = new CLOCKPager();
            break;
        }
        case 'e':{
            THE_PAGER = new ESCPager();
            break;
        }
        case 'a':{
            THE_PAGER = new AGING();
            break;
        }
        case 'w':{
            THE_PAGER = new Working_Set();
            break;
        }
        default:
            fprintf(stderr,
                    "Unknown option character `\\x%x'.\n",
                    avalue[0]);
            abort();
    }
    for (char &c: string(ovalue)) {
        switch (c) {
            case 'O': {
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

    frame_table.resize(MAX_FRAMES);
    int i;
    for (i = 0; i < MAX_FRAMES; i++) {
        frame_free_list.push(i);
    }
    simulation(exec);
    delete (THE_PAGER);
    display();
    return 0;
}
