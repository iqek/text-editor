#include "editor.h"
#include <string.h>
#include <ncurses.h>

void insert(int index)
{
    if (freeIndex >= MAX_LINES) {
        garbageCollection();
 
        if (freeIndex >= MAX_LINES) {
            mvprintw(LINES - 1, 0, "ERROR: text buffer is full, cannot insert.");
            clrtoeol();
            refresh();
            return;
        }
    }

    char newStatement[MAX_LINES];
    memset(newStatement, 0, sizeof(newStatement));
 
    mvprintw(LINES - 1, 0, "> ");
    clrtoeol();
    echo();
    curs_set(1);
    getnstr(newStatement, MAX_LINES - 1);
    noecho();
    curs_set(0);
 
    int newIdx = freeIndex;
    strncpy(textbuffer[newIdx].statement, newStatement, MAX_LINES - 1);
    textbuffer[newIdx].statement[MAX_LINES - 1] = '\0';
  
    if (index == -1) {
        textbuffer[newIdx].prev = -1;
        textbuffer[newIdx].next = -1;
        head = newIdx;
        tail = newIdx;
 
    } else {
        int nextIdx = textbuffer[index].next;
 
        textbuffer[newIdx].prev = index;
        textbuffer[newIdx].next = nextIdx;
 
        textbuffer[index].next = newIdx;
 
        if (nextIdx != -1) {
            textbuffer[nextIdx].prev = newIdx;
        } else {
            tail = newIdx;
        }
    }
 
    freeIndex++;
 
    print();
}


int replaceChar(int lineIndex, int charIndex, char newChar){
    if(lineIndex >= 0 && charIndex >= 0 && lineIndex < freeIndex && charIndex < strlen(textbuffer[lineIndex].statement)){
        textbuffer[lineIndex].statement[charIndex] = newChar;
        return 0;
    } 
    return -1;
} 


int deleteLine(int index){
    int prevIndex;
    int nextIndex;

    if(index == NIL){
        return -1;
    }

    prevIndex = textbuffer[index].prev;
    nextIndex = textbuffer[index].next;

    if(prevIndex != NIL){
        textbuffer[prevIndex].next = nextIndex;
    }
    else{
        head = nextIndex;
    }

    if(nextIndex != NIL){
        textbuffer[nextIndex].prev = prevIndex;
    }
    else{
        tail = prevIndex;
    }

    textbuffer[index].next = NIL;
    textbuffer[index].prev = NIL;

    operationCount++;

    return 0;
}








