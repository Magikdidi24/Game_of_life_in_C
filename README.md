# Game of life in C

Conway's Game of Life drawn with SDL2, in a 1920x1080 window with 20 px cells. Click cells to turn them on or off, then press space to start or pause. Generation and population are shown in the top left. The grid doesn't wrap around, anything past the border counts as dead.

It needs SDL2 and SDL2_ttf. The font path is hard coded to `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`. If DejaVu Sans is somewhere else on your system change it in main.c, otherwise the program quits at startup.

```bash
make
```
