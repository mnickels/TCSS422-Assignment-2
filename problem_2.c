#include <stdio.h>
#include "pcb.h"
#include "fifoqueue.h"
// should we move this stuff to a .h file?
enum interrupt_type {none, timer};
void create_processes(FIFO_q_p, unsigned char);
void isr(PCB_p, FIFO_q_p);
void scheduler(PCB_p, FIFO_q_p, enum interrupt_type);
void dispatcher(PCB_p, FIFO_q_p);
int main(int, char const **);
unsigned int PRINT_FLAG = 0;
unsigned int SYS_STACK = 0;


void create_processes(FIFO_q_p process_q, unsigned char num_processes) {
    for(int i = 0; i < num_processes; i++) {
        PCB_p temp_pcb = pcb_constructor();
        pcb_set_state(temp_pcb, new);
        fifo_q_enqueue(process_q, temp_pcb);
    }
}

void isr(PCB_p running_process, FIFO_q_p ready_list) {
    if (running_process) {
        pcb_set_state(running_process, interrupted);
        pcb_set_pc(running_process, SYS_STACK);
        // up-call to scheduler
    } // else - CPU is in idle state
    scheduler(running_process, ready_list, timer);
}


void scheduler(PCB_p running_process, FIFO_q_p ready_list, enum interrupt_type type) {
    
    switch(type) {

    	case timer: //timer interrupt
    		if (running_process) {
            	// put running process back in ready queue
            	pcb_set_state(running_process, ready);
            	fifo_q_enqueue(ready_list, running_process);
       		} // else - CPU is in idle state

        	dispatcher(running_process ,ready_list);
        	break;
        
        case none: //schedule a process 
        	fifo_q_enqueue(ready_list, running_process);
        	char * run_str = pcb_to_string(running_process);
        	printf("PROCESS SCHEDULED\n%s\n",run_str);
        	free(run_str);
        	break;
    }
}

void dispatcher(PCB_p running_process, FIFO_q_p ready_list) {
	PCB_p temp = running_process;
	PRINT_FLAG++;    
    running_process = fifo_q_dequeue(ready_list);   // mutates this pointer
    char s[5000];
    char * run_str;
    char * switch_str;

    if(PRINT_FLAG == 4) {
    	printf("FOURTH CALL\n------------------------------------------------\n");
    	
    	run_str = pcb_to_string(temp);
    	switch_str = pcb_to_string(running_process);
    	printf("%s\nSwitching to:\n%s\n", run_str, switch_str);

    	//printf("%s\n", fifo_q_to_string(ready_list, s));
    	
    }
    // switch to next process
    if (running_process) {
        SYS_STACK = pcb_get_pc(running_process);
    } // else - nothing to run, ready queue is empty

    if (PRINT_FLAG == 4) {
    	PRINT_FLAG = 0;
    	printf("%s\nAfter Switch to:\n%s\n", run_str, switch_str);
    	printf("------------------------------------------------\n");	
    	free(run_str);
    	free(switch_str);
    }

}

int main(int argc, char const *argv[]) {
    FIFO_q_p created_list = fifo_q_new();
    FIFO_q_p ready_list = fifo_q_new();

    srand(time(NULL));
    unsigned int cpu_pc = 0;
    int max_num_process = 0; 
    PCB_p running_process;
    for(;;) {

    	int num_to_create = rand() % 6;
    	max_num_process += num_to_create;
    	if(max_num_process > 30) break;	
        // create some new processes and add them to the created queue
        create_processes(created_list, num_to_create);
        // add any new processes to the ready queue
        while(!fifo_q_is_empty(created_list)) {
            scheduler(fifo_q_dequeue(created_list), ready_list, none);
        }

        running_process = fifo_q_dequeue(ready_list);
        //simulate timer quantum for currently running process
        if (running_process) {
            unsigned int cycles_executed = rand() % 1001 + 3000;
            cpu_pc += cycles_executed;
        }
       
        // simulate timer interrupt
        SYS_STACK = cpu_pc;
        isr(running_process, ready_list);
        cpu_pc = SYS_STACK;
    }
   
    // cpu_pc += rand_increment;
    // temp_pcb->context->pc = cpu_pc;
}