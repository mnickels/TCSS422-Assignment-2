#include <stdio.h>
#include "pcb.h"
#include "fifoqueue.h"

// should we move this stuff to a .h file?
enum interrupt_type {none, timer};
void create_processes(FIFO_q_p, unsigned char);
void isr(PCB_p);
void scheduler(PCB_p, enum interrupt_type);
void dispatcher(PCB_p);
int main(int, char const **);
//

unsigned int SYS_STACK = 0;

int main(int argc, char const *argv[]) {
    FIFO_q_p created_list = fifo_q_new();
    FIFO_q_p ready_list = fifo_q_new();

    srand(time(NULL));
    unsigned int cpu_pc = 0;

    PCB_p running_process;

    for(;;) {

        // create some new processes and add them to the created queue
        create_processes(created_list, rand() % 6);

        // add any new processes to the ready queue
        for (int i = 0; i < created_list->length; i++) {
            fifo_q_enqueue(ready_list, fifo_q_dequeue(created_list));
        }

        // simulate timer quantum for currently running process
        if (running_process) {
            unsigned int cycles_executed = rand() % 1001 + 3000;
            cpu_pc += cycles_executed;
        }

        // simulate timer interrupt
        SYS_STACK = cpu_pc;
        isr(running_process);

        break;
    }

    // cpu_pc += rand_increment;
    // temp_pcb->context->pc = cpu_pc;
}

void isr(PCB_p running_process) {
    if (running_process) {
        running_process->state = interrupted;
        running_process->context->pc = SYS_STACK;
        // up-call to scheduler
    } // else - CPU is in idle state
    scheduler(running_process, timer);
}

void scheduler(PCB_p running_process, enum interrupt_type type) {
    if (type == timer) {
        if (running_process) {
            // put running process back in ready queue
            running_process->state = ready;
            fifo_q_enqueue(ready_list, running_process);
        } // else - CPU is in idle state

        dispatcher(running_process);
    }
}

void dispatcher(PCB_p running_process) {
    // switch to next process
    running_process = fifo_q_dequeue(ready_list);   // mutates this pointer
    if (running_process) {
        SYS_STACK = running_process->context->pc;
    } // else - nothing to run, ready queue is empty
}

void create_processes(FIFO_q_p process_q, unsigned char num_processes) {
    for(int i = 0; i < num_processes; i++) {
        PCB_p temp_pcb = pcb_constructor();
        temp_pcb->state = new;
        fifo_q_enqueue(process_q, temp_pcb);
    }
}
