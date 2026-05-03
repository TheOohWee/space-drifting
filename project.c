// Fundamentals of Computing CSE 20311
// Mini Final Project Lab 11: Space Drifting
// Name: Amir Tomashpayev

// P.S. couldn't figure out how to make it possible for the ship to move and shoot simultaneously, but maybe it's not possible in gfx (to notice both key clicks)

#include "func.h"
#include <math.h>
#include <stdlib.h> // for randomness
#include <stdio.h>
#include "gfx.h"
#include <unistd.h>
#include <time.h> // for time()

int main() {
    const int w = 800;
    const int h = 700;
    const int hud_h = 40;

    int score = 0; 
    int wave = 1;

    srand(time(NULL)); // seed

    // declaring ship object + 2 arrays of structs we created
    Ship ship;
    Bullet bullets[MAX_BULLETS];
    Asteroid asteroids[MAX_ASTEROIDS];

    init_ship(&ship, w, h);
    init_bullets(bullets);
    init_asteroids(asteroids);

    gfx_open(w, h, "Space Drifting");

    spawn_wave(asteroids, 4, w, h, hud_h, &ship);
    gfx_clear();
    draw_title(w, h);
    gfx_flush();
    gfx_wait();

    while (1) {
        gfx_clear();

        draw_ship(&ship);
        draw_bullets(bullets);
        draw_asteroids(asteroids);
        draw_hud(score, ship.lives, wave, w, hud_h);
        
        gfx_flush();

        while (gfx_event_waiting()) {
            char c = gfx_wait();
            if (c == 'q') break;
            if (c == 'a') rotate_ship(&ship, -1);
            if (c == 'd') rotate_ship(&ship, +1);
            if (c == ' ') shoot_bullet(bullets, &ship);

        }

        update_ship(&ship, w, h, hud_h);
        update_bullets(bullets, w, h, hud_h);
        update_asteroids(asteroids, w, h, hud_h);
        check_bullet_hits(bullets, asteroids, &score, w, h, hud_h);
        check_ship_hits(&ship, asteroids, w, h);

        if (ship.lives <- 0) {
            // show game over screen
            gfx_clear();
            draw_asteroids(asteroids);
            draw_game_over(score, w, h);
            gfx_flush();
            gfx_wait();
            break;
        }
                    
        if (wave_cleared(asteroids)) {
            wave++;
            spawn_wave(asteroids, 3 + wave, w, h, hud_h, &ship);
        }

        usleep(15000);
    }



return 0;
}


