#pragma once


#define MAX_LINES 100
#define MAX_LEN 40
#define MAX_SCREEN_LINES 30

typedef struct node {
    char statement[MAX_LEN];
    int next;
    int prev;
} Node;

extern Node textbuffer[MAX_LINES];

extern int head;
extern int tail;
extern int freeIndex;

extern char currentFilename[256];

extern int operationCount;

void initBuffer(void);
int getBufferIndexByScreenLine(int screenLine);
int validLineCount(void);
void debugPrintBuffer(void);

int edit(char *filename);
int save(void);

void initUI(void);
void closeUI(void);
void print(void);
int cursorLine(void);
int cursorChar(void);
int handleInput(void);

int insert(int index);
int delete(int index);
int replace(int index);

int garbageCollection(void);
void checkAutomaticGC(void);
