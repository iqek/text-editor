#include "editor.h"
#include <string.h>
#include <stdio.h>

int garbageCollection(void){
    int  validOrder[MAX_LINES];
    int  count = 0;

    int cur = head;
    while (cur != -1 && count < MAX_LINES) { //storing the valid nodes into a temporary array
        validOrder[count++] = cur;
        cur = textbuffer[cur].next;
    }

    if (count == 0) {
    	head = NIL;
    	tail = NIL;
    	freeIndex = 0;
    	operationCount = 0;
    	return 0;
    }
    /*if (count == 0) { //if the list is empty
        head = -1;
        tail = -1;
        freeIndex = 0;
        return -1;
    }*/


    struct node tmp[MAX_LINES]; //copy valid nodes into a temporary array
    for (int i = 0; i < count; i++) {
        strncpy(tmp[i].statement, textbuffer[validOrder[i]].statement, sizeof(tmp[i].statement) - 1);
        tmp[i].statement[sizeof(tmp[i].statement) - 1] = '\0';
    }

    for (int i = 0; i < count; i++) { //copy back into the textbuffer
        strncpy(textbuffer[i].statement, tmp[i].statement, sizeof(textbuffer[i].statement) - 1);
        textbuffer[i].statement[sizeof(textbuffer[i].statement) - 1] = '\0';

        textbuffer[i].prev = (i == 0)         ? -1 : i - 1; //rebuild the pointers
        textbuffer[i].next = (i == count - 1) ? -1 : i + 1;
    }

    int reclaimed = 0;
    for (int i = count; i < MAX_LINES; i++) { //clear unused space
        textbuffer[i].statement[0] = '\0';
        textbuffer[i].next = -1;
        textbuffer[i].prev = -1;
        reclaimed++;
    }

    /*head = 0; //update global pointers
    tail = count - 1;
    freeIndex = count;

    return reclaimed;*/

    head = 0;
    tail = count - 1;
    freeIndex = count;
    operationCount = 0;
    return reclaimed;

}

void checkAutomaticGC(void)
{
    int shouldCollect = 0;

    if (freeIndex >= MAX_LINES) //decide if gc is needed
        shouldCollect = 1;

    if (operationCount >= 10)
        shouldCollect = 1;

    if (shouldCollect) { //run gc
        garbageCollection();
        operationCount = 0;
    }
}
