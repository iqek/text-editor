#include <stdio.h>
#include <string.h>
#include "../include/editor.h"

int main(void) {
    char command;
    char filename[256];

    initBuffer();

    printf("CSE232 Text Editor\n");
    printf("Commands: E filename, P, I, D, R, S, G, Q\n");

    while (1) {
        printf("\n> ");
        scanf(" %c", &command);

        if (command == 'E' || command == 'e') {
            scanf("%255s", filename);

            if (editFile(filename) == 0) {
                printf("File opened: %s\n", filename);
            } else {
                printf("Error: Could not open file.\n");
            }
        }

        else if (command == 'P' || command == 'p') {
            print();
        }

        else if (command == 'I' || command == 'i') {
            printf("Insert command will be connected with UI later.\n");
        }

        else if (command == 'D' || command == 'd') {
		int screenLine;
		printf("Enter line number to delete: ");
		if(scanf("%d", &screenLine) != 1){
			printf("Error: Invalid line number.\n");
			while(getchar() != '\n');
		}else{
			int index = getBufferIndexByScreenLine(screenLine);

			if (index == NIL){
				printf("Error: No such line.\n");
			}else{
				if(deleteLine(index) == 0){
					printf("Deleted screen line %d buffer index %d.\n",screenLine, index);
					debugPrintBuffer();
				}else{
					printf("Error: Delete failed.\n");
				}
			}
		}
        }

        else if (command == 'R' || command == 'r') {
            printf("Replace command will be connected with UI later.\n");
        }
        else if (command == 'S' || command == 's') {
            if (saveFile() == 0) {
                printf("File saved.\n");
            } else {
                printf("Error: Could not save file.\n");
            }
        }

        else if (command == 'G' || command == 'g') {
            garbageCollection();
            printf("Garbage collection completed.\n");
        }

        else if (command == 'Q' || command == 'q') {
            printf("Quitting editor.\n");
            break;
        }

        else {
            printf("Unknown command. Try Again.\n");
        }
    }

    return 0;
}
