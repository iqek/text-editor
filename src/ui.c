// -lncurses libi ekleme

#include <ncurses.h>
#include <string.h>
#include "editor.h"

//UI
static int selectedScreenLine = 0;
static int selectedChar       = 0;  


static int lenOfCurrentLine(void)
{
    int idx = getBufferIndexByScreenLine(selectedScreenLine);
    if (idx == NIL)
        return 0;
    return (int)strlen(textbuffer[idx].statement);
}


static void clampCursor(int lineCount)
{
    if (selectedScreenLine < 0)
        selectedScreenLine = 0;
    if (lineCount > 0 && selectedScreenLine >= lineCount)
        selectedScreenLine = lineCount - 1;

    int lineLen = lenOfCurrentLine();
    if (selectedChar < 0)
        selectedChar = 0;
    if (selectedChar > lineLen)
        selectedChar = lineLen;
}


void initUI(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    refresh();
}

void closeUI(void)
{
    endwin();
}

void printEditor(void)
{
    clear();

    int row = 0;
    int idx = head;
    int selIdx = getBufferIndexByScreenLine(selectedScreenLine);

    while (idx != NIL && row < MAX_SCREEN_COUNT){
        if (idx == selIdx)
            attron(A_REVERSE);

        mvprintw(row, 0, "%s", textbuffer[idx].statement);

        if (idx == selIdx)
            attroff(A_REVERSE);
        row=row+1;
        idx = textbuffer[idx].next;
    }

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    attron(A_BOLD);
    mvprintw(maxY - 2, 0, "File: %-20s  Lines: %d",
             currentFilename[0] ? currentFilename : "(none)",
             validLineCount());
    attroff(A_BOLD);

    mvprintw(maxY - 1, 0, ">_");

    move(selectedScreenLine, selectedChar);
    refresh();
}

//keremin getchle
int handleInput(void)
{
    int ch = getch();
    int lineCount = validLineCount();

    switch (ch){
        case KEY_UP:
            selectedScreenLine=selectedScreenLine-1;
            clampCursor(lineCount);
            move(selectedScreenLine, selectedChar);
            refresh();
            return ERR;

        case KEY_DOWN:
            selectedScreenLine=selectedScreenLine+1;
            clampCursor(lineCount);
            move(selectedScreenLine, selectedChar);
            refresh();
            return ERR;

        case KEY_LEFT:
            selectedChar=selectedChar-1;
            clampCursor(lineCount);
            move(selectedScreenLine, selectedChar);
            refresh();
            return ERR;

        case KEY_RIGHT:
            selectedChar=selectedChar+1;
            clampCursor(lineCount);
            move(selectedScreenLine, selectedChar);
            refresh();
            return ERR;

        default:
            return ch;
    }
}

int cursorLine(void)
{
    return getBufferIndexByScreenLine(selectedScreenLine);
}

int cursorChar(void)
{
    return selectedChar;
}