#include "fifoqueue.h"
#include "pcb.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FIFO_q_p fifo_q_new(void) {
    FIFO_q_p this = (FIFO_q_p) calloc(1, sizeof(FIFO_q_s));
    if (!this)
        return NULL;
    this->length = 0;
    this->elementSize = sizeof(Node_p);
    this->front = NULL;
    this->back = NULL;
    return this;
}

int fifo_q_destroy(FIFO_q_p queue) {
    if (!queue)
        return 0;
    while (!fifo_q_is_empty(queue))
        fifo_q_dequeue(queue);
    free(queue);
    return 1;
}

char * fifo_q_to_string(FIFO_q_p queue, char * string) {
	if (!queue || !string)
		return NULL;
	char tempstring[256] = "Q:Count=";
	char buffer[10];
	sprintf(buffer, "%d", queue->length);
	strcat(tempstring, buffer);
	strcat(tempstring, ": ");
	Node_p tempnode = queue->front;
	for (int i = 0; i < queue->length; i++) {
		sprintf(buffer, "%d", tempnode->process->pid);
		strcat(tempstring, "P");
		strcat(tempstring, buffer);
		strcat(tempstring, "->");
        tempnode = tempnode->next;
    }
    strcat(tempstring, "*");
   strcpy(string, tempstring);
   return string;
}

int fifo_q_is_empty(FIFO_q_p queue) {
    if (!queue)
        return 1;
    if (!queue->front || !queue->back)
        return 1;
    if (queue->length == 0)
        return 1;
    return 0;
}

int fifo_q_enqueue(FIFO_q_p queue, PCB_p process) {
    if (!queue || !process)
        return 0;
    Node_p node = calloc(1, sizeof(Node_s));
    if (!node)
        return 0;
    node->process = process;
    node->next = NULL;
    if (queue->back)
        queue->back->next = node;
    queue->back = node;
    if (!queue->front)
        queue->front = node;
    queue->length++;
    return 1;
}

PCB_p fifo_q_dequeue(FIFO_q_p queue) {
    if (fifo_q_is_empty(queue))
        return NULL;
    Node_p front = queue->front;
    Node_p next = front->next;
    PCB_p dequeued_process = queue->front->process;
    free(queue->front);
    queue->front = next;
    queue->length--;
    return dequeued_process;
}
