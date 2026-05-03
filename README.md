# Lab 11 - Space Drifting
Amir Tomashpayev

[live demo video](https://www.loom.com/share/56edf6ceb46c4863998f3c55e77169a3) (just practicing, not that deep)

A space-themed game where you fly a ship through waves of asteroids.
The ship moves forward continuously. User can rotate with A/D, shoot with SPACE,
quit with Q. Asteroids split when shot (big -> medium -> small).
User has 3 lives. Each cleared wave spawns more asteroids. We added HUD (heads up display with stats), starter screen with instructions and game over screen, so it's a full game on c. 

Files:
- project.c   : main loop
- func.c/.h   : game functions and structs
- gfx.c/.o/.h : graphics library
- Makefile

Compile: make
Run: ./project
