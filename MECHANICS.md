# NES Tetris - Game Mechanics
This file includes notes for how game mechanics in the original NES tetris were implemented. 

Sources:
- https://harddrop.com/wiki/Tetris_(NES,_Nintendo) (best, thank you)
- https://tetris.fandom.com/wiki/Tetris_(NES,_Nintendo)
- https://strategywiki.org/wiki/Tetris_(NES)
- Official design documents. Under "tetris_official_design_guidelines_2009.pdf"
## DAS

- Keep 2 counters for how many **frames** the Left/Right keys have been held down for

- Immediately on key press, move the piece. Then set the counter to **0**

- Once a counter has reached **16**, move the piece. Then, set the counter to **10**