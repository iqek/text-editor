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

int editFile(const char *filename);
int saveFile(void);

void initUI(void);
void closeUI(void);
void print(void);
int cursorLine(void);
int cursorChar(void);
int handleInput(void);

int insertLineAfter(int index, const char *line);
int deleteLine(int index);
int replaceChar(int lineIndex, int charIndex, char newChar);

int garbageCollection(void);
void checkAutomaticGC(void);
