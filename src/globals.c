#include "../include/editor.h"

Node textbuffer[MAX_LINES];

int head = NIL;
int tail = NIL;
int freeIndex = 0;

char currentFilename[256] = "";
int operationCount = 0;