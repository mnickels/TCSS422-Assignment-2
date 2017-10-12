#ifndef PRIORITY_Q_H_INCLUDED
#define PRIORITY_Q_H_INCLUDED

#include "pcb.h"
#include "fifoqueue.h"

#define NUM_PRIORITY_LEVEL 16

typedef struct prio_q {
	FIFO_q_p queues[NUM_PRIORITY_LEVEL];
} PRIO_q_s;

typedef PRIO_q_s * PRIO_q_p;

PRIO_q_p prio_q_new(void);

void prio_q_destroy(PRIO_q_p);

void prio_q_enqueue(PRIO_q_p, PCB_p);

PCB_p prio_q_dequeue(PRIO_q_p);

char * prio_q_to_string(PRIO_q_p, char *);

#endif
