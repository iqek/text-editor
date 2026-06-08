#include "../include/editor.h"

Node textbuffer[MAX_LINES];

int head = -1;
int tail = -1;
int freeIndex = 0;

char currentFilename[256] = "";
int operationCount = 0;