#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/editor.h"

int editFile(const char *filename){
    FILE *fptr = fopen(filename, "r");

    if(fptr == NULL){
        perror("Error opening file");
        return -1;
    }

    initBuffer();

    int i = 0;

    while(i < MAX_LINES && fgets(textbuffer[i].statement, MAX_LEN, fptr)){
        textbuffer[i].statement[strcspn(textbuffer[i].statement, "\n")] = '\0';

        textbuffer[i].prev = (i == 0) ? NIL : i - 1;
        textbuffer[i].next = i + 1;

        i++;
    }

    if(i == 0){
        head = NIL;
        tail = NIL;
        freeIndex = 0;
    }
    else{
        head = 0;
        tail = i - 1;
        textbuffer[tail].next = NIL;
        freeIndex = i;
    }

    strncpy(currentFilename, filename, 255);
    currentFilename[255] = '\0';

    fclose(fptr);

    return 0;
}/*FILE *fptr = fopen(filename, "r");

    if(fptr == NULL){
        perror("Error opening file");
        return -1;
    }

    int i = 0;
    while(i < MAX_LINES && fgets(textbuffer[i].statement, MAX_LEN, fptr)){  //fgets() keeps newlines
        textbuffer[i].prev = i - 1;
        textbuffer[i].next = i + 1;  //kendilerinden onceki ve sonraki satirlarin indexlerini tutuyorlar
        i++;
    }
    textbuffer[i-1].next = NIL;

    head = 0;
    tail = i - 1;
    freeIndex = i;

    strncpy(currentFilename, filename, 255);
    fclose(fptr);

    return 0;
}*/

int saveFile(void){
    FILE *fptr = fopen(currentFilename, "w");

    if(fptr == NULL){
        perror("Error saving file");
        return -1;
    }

    int curr = head;
    while(curr != NIL ){
        fprintf(fptr, /*"%s"*/ "%s\n", textbuffer[curr].statement);
        curr = textbuffer[curr].next;
    }

    fclose(fptr);

    return 0;
}
