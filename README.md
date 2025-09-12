# Word Blaster

Word Blaster is a fast-paced typing game where your goal is to shoot down falling bombs by typing the words attached to them. As you progress, the waves of bombs become more frequent and numerous, testing your typing speed and accuracy.

## Features

*   **Endless Gameplay:** Face continuous waves of bombs with progressively increasing difficulty.
*   **Scoring System:** Earn points for each bomb you successfully destroy.
*   **High Score Tracking:** The game saves your highest score, so you can always aim to beat your personal best.
*   **Audio:** Includes background music and sound effects for an immersive experience.
*   **Cheat Code:** Feeling overwhelmed? Try to find the secret cheat code for infinite lives!

## How to Play

*   **Start Game:** Press `ENTER` on the main menu to begin.
*   **Shoot Bombs:** Type the word displayed on a bomb. The bullet fires automatically upon completing the word.
*   **Clear Input:** If you make a mistake, press `SPACE` to clear your current typed word and start over.
*   **Restart:** After the game is over, press `ENTER` to play again.
*   **Exit:** Press `ESC` at any time to close the game.

## Building from Source

The game is built using C++ and OpenGL (via `freeglut`).

### Dependencies

*   A C++ compiler (like g++ from MinGW-w64 on Windows).
*   **freeglut:** The OpenGL Utility Toolkit.

You can typically install `freeglut` on Windows by placing the necessary header files (`GL/glut.h`, etc.) in your compiler's include directory and the library files (`freeglut.lib` or `libfreeglut.a`) in the library directory.

The `miniaudio` library is included directly in the source (`miniaudio.h`), so no external audio library is required.

### Compilation Command

Navigate to the project directory in your terminal and run the following command to compile the game:

```sh
g++ main.cpp -o WordBlaster.exe -lfreeglut -lglu32 -lopengl32 -lwinmm
```

This will create an executable file named `WordBlaster.exe`.

### Required Game Files

Make sure the following files and folders are in the same directory as the executable:

*   `res/`
    *   `words.txt` (The word list for the bombs)
    *   `sounds/` (Contains all `.wav` sound files)

## Credits
Some of the game building credit goes to my team members, as this was a group project of one of my university courses.

### Team Members
Team 4 Bit
- Sadman Sakib
- Jannatul Adan Adreeta
- Md. Fahim Muhtashim
- Md. Toha