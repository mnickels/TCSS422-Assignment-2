#include <stdio.h>
#include "pcb.h"
#include "fifoqueue.h"

unsigned int SYS_STACK = 0;

int main(int argc, char const *argv[]) {
    FIFO_q_p created_list = fifo_q_new(); 
    FIFO_q_p terminated_list = fifo_q_new(); 
    FIFO_q_p ready_list = fifo_q_new(); 
    FIFO_q_p running_list = fifo_q_new();
    FIFO_q_p blocked_list = fifo_q_new(); 
    FIFO_q_p interrupted_list = fifo_q_new();
    
    srand(time(NULL));
    unsigned int cpu_pc = 0; 
    
    for(;;) {
        
        unsigned int rand_increment  = rand() % 1001 + 3000;
        printf("%d\n",rand_increment);
        int r = rand() % 6;
        
        for(int i = 0; i < r; i++) {
            PCB_p temp_pcb = pcb_constructor();
            fifo_q_enqueue(created_list, temp_pcb);
        }


        SYS_STACK = cpu_pc;
        char s[50000];
        printf("%s\n", fifo_q_to_string(created_list, s));
        break;
    }

    // cpu_pc += rand_increment;
    // temp_pcb->context->pc = cpu_pc;
}