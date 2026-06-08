#include <stdio.h>
#include <string.h>
#include "../include/editor.h"
#include <ncurses.h>

int main(void) {
    char command;
    char filename[256];

    initBuffer();

    printf("CSE232 Text Editor\n");
    printf("Commands: E filename, P, S, G, Q\n");

    while (1) {
        printf("\n> ");
        scanf(" %c", &command);

        if (command == 'E' || command == 'e') {
            scanf("%255s", filename);
            if (edit(filename) == 0) {
                printf("File opened: %s\n", filename);
            } else {
                printf("Error: Could not open file.\n");
            }
        }

        else if (command == 'P' || command == 'p') {
            int ch;
            int runningUI = 1;

            initUI();

            while (runningUI) {
                print();
                ch = handleInput();

                if (ch == ERR) {
                    continue;
                }

                if (ch == 'Q' || ch == 'q') {
                    runningUI = 0;
                }
                else if (ch == 'D' || ch == 'd') {
                    int index = cursorLine();
                    if (delete(index) == 0) {
                        print();
                    }
                }
                else if (ch == 'G' || ch == 'g') {
                    garbageCollection();
                    print();
                }
                else if (ch == 'S' || ch == 's') {
                    save();
                    print();
                }
                else if (ch == 'I' || ch == 'i') {
                    int index = cursorLine();
                    if (insert(index) == 0) {
                        print();
                    }
                }
                else if (ch == 'R' || ch == 'r') {
                    if (replace(cursorChar()) == 0) {
                        print();
                    }
                }
            }

            closeUI();
            printf("Returned from editor screen.\n");
        }

        else if (command == 'S' || command == 's') {
            if (save() == 0) {
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
