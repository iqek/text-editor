# text editor

A simple terminal-based text editor written in C for the CSE 232 Systems Programming course. Uses ncurses for the UI and manages text in a linked-list style buffer with garbage collection.

## Build

```bash
make
```

## Usage

```bash
./editor
```

**Outer prompt:**
- `E filename` — open a .txt file
- `P` — enter the editor screen
- `S` — save the current file
- `G` — run garbage collection
- `Q` — quit

**Inside the editor (after P):**
- Arrow keys — move cursor
- `I` — insert a new line after the selected line
- `D` — delete the selected line
- `R` — replace the character under the cursor
- `S` — save
- `G` — garbage collection
- `Q` — return to outer prompt

## Notes

- Max 100 lines, 40 characters per line
- Garbage collection runs automatically after every 10 insertions/deletions or when the buffer is full
