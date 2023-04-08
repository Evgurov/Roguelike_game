# Roguelike game (C++)
## About
A repository with a game in the roguelike genre written by me entirely in C++. The only ready-made code used is pixel-by-pixel rendering of the image using OpenGL. The game is in its infancy, at the moment it is possible to control the character and move through several levels of the dungeon. The character dies when he steps on traps. In the future, the project can be finalized as a pet project.
## Build and run
The project was developed and ran on `Linux`.
To work and build, the `OpenGL` library `glfw3` is required. You can install this by typing in the command line
```
$ sudo apt-get install libglfw3-dev
```
To build the program, the `g++` compiler and the `cmake` and `make` utilities are also required.

In the project folder:
```
$ cmake ./
$ make
$ ./bin/main
```
## Controls
* Press right mouse button to capture/release mouse cursor  
* W, A, S, D - movement  
* Press ESC to exit
* Press E to enter the doors

  <p align="center"><img src="screenshots/Screenshot_1" width="600" /></p>
____
Evgeny Gurov, 2021
