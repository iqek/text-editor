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

        textbuffer[i].prev = (i == 0) ? -1 : i - 1;
        textbuffer[i].  ext = i + 1;

        i++;
    }

    if(i == 0){
        head = -1;
        tail = -1;
        freeIndex = 0;
    }
    else{
        head = 0;
        tail = i - 1;
        textbuffer[tail].next = -1;
        freeIndex = i;
    }

    strncpy(currentFilename, filename, 255);
    currentFilename[255] = '\0';

    fclose(fptr);

    return 0;
}

int saveFile(void){
    FILE *fptr = fopen(currentFilename, "w");

    if(fptr == NULL){
        perror("Error saving file");
        return -1;
    }

    int curr = head;
    while(curr != -1 ){
        fprintf(fptr, /*"%s"*/ "%s\n", textbuffer[curr].statement);
        curr = textbuffer[curr].next;
    }

    fclose(fptr);

    return 0;
}
