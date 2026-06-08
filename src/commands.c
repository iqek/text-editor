#include "editor.h"
#include <string.h>
#include <ncurses.h>

int insert(int index)
{
    if(freeIndex >= MAX_LINES) {
        garbageCollection();
        if(freeIndex >= MAX_LINES) {
            return -1;
        }
    }

    char line[MAX_LEN];
    echo();
    curs_set(1);
    mvprintw(LINES - 1, 0, "Insert new line: ");
    clrtoeol();
    getnstr(line, MAX_LEN - 1);
    noecho();

    int newIdx = freeIndex;
    strncpy(textbuffer[newIdx].statement, line, MAX_LEN - 1);
    textbuffer[newIdx].statement[MAX_LEN - 1] = '\0';

    if(index == -1) {
        textbuffer[newIdx].prev = -1; textbuffer[newIdx].next = -1; head = newIdx; tail = newIdx;
    }
    else{
        int nextIdx = textbuffer[index].next;

        textbuffer[newIdx].prev = index;
        textbuffer[newIdx].next = nextIdx;

        textbuffer[index].next = newIdx;

        if(nextIdx != -1) {
            textbuffer[nextIdx].prev = newIdx;
        }
        else{
            tail = newIdx;
        }
    }

    freeIndex++;
    operationCount++;
    checkAutomaticGC();

    return 0;
}


int replace(int index){

    int lineIndex = cursorLine();
    int charIndex = index;

    mvprintw(LINES - 1, 0, "New char: ");
    clrtoeol();
    refresh();
    int newChar = getch();

    if(lineIndex >= 0 && charIndex >= 0 && lineIndex < freeIndex && charIndex < (int)strlen(textbuffer[lineIndex].statement) && newChar != ERR){
        textbuffer[lineIndex].statement[charIndex] = newChar;
        return 0;
    } 
    return -1;
} 


int delete(int index){
    int prevIndex;
    int nextIndex;

    if(index == -1){
        return -1;
    }

    prevIndex = textbuffer[index].prev;
    nextIndex = textbuffer[index].next;

    if(prevIndex != -1){
        textbuffer[prevIndex].next = nextIndex;
    }
    else{
        head = nextIndex;
    }

    if(nextIndex != -1){
        textbuffer[nextIndex].prev = prevIndex;
    }
    else{
        tail = prevIndex;
    }

    textbuffer[index].next = -1;
    textbuffer[index].prev = -1;

    operationCount++;
    checkAutomaticGC();

    return 0;
}








