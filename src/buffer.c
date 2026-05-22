#include "../include/editor.h"
#include <stdio.h>

void initBuffer(void){
    for(int i=0; i<MAX_LINES; i++){
        textbuffer[i].statement[0] = '\0';
        textbuffer[i].next = NIL;
        textbuffer[i].prev = NIL;
    }
    head = NIL;
    tail = NIL;
    freeIndex = 0;
    operationCount = 0;
}

int validLineCount(void){
    int count = 0;
    int current = head;

    while(current != NIL){
        count++;
        current = textbuffer[current].next;
    }
    return count;
}

int getBufferIndexByScreenLine(int screenLine){
    int current = head;
    int row = 0;

    if(screenLine < 0){
        return NIL;
    }

    while(current != NIL && row < screenLine){
        current = textbuffer[current].next;
        row++;
    }
    return current;
}

void debugPrintBuffer(void){
    int current = head;

    printf("Head = %d\n", head);
    printf("Tail = %d\n", tail);
    printf("Free Index = %d\n\n", freeIndex);

    while(current != NIL)
    {
        printf("Current Index: %d\n", current);
        printf("Prev: %d\n", textbuffer[current].prev);
        printf("Text: %s\n", textbuffer[current].statement);
        printf("Next: %d\n", textbuffer[current].next);

        current = textbuffer[current].next;
    }
}
