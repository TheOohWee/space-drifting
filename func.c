// Amir Tomashpayev Mini Final Project
// functions file

#include "func.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "gfx.h"
#include <unistd.h>


// for each struct we do the same
// 1. initiate
// 2. spawn
// 3. update
// 4. draw

// 1. SHIP FUNCTIONS

void init_ship(Ship *s, int w, int h) {
    s->x = w / 2.0;
    s->y = h / 2.0;
    s->dx = 0;
    s->dy = 0;
    s->angle = -M_PI / 2; 
    s->alive = 1; // to show on hud
    s->lives = 3;
    s->invuln = 0;
}

void rotate_ship(Ship *s, int dir) {
    // dir = -1 is leftr and dir = +1 is right
    s->angle += dir * 0.15;
}


// PIVOTED: ENDED UP NOT USING: controls were bad
// we just add a kick in the direction of the tip of the ship 
// multiplier power to make sure it doesn't accelerate to quickly
void thrust_ship(Ship *s) {
    double power = 0.15;
    s->dx += cos(s->angle) * power;
    s->dy += sin(s->angle) * power;

    // cap max speed so the ship doesn't go to crazy 
    double max_speed = 6.0;
    double speed = sqrt(s->dx * s->dx + s->dy * s->dy);
    if (speed > max_speed) {
        s->dx = s->dx / speed * max_speed; // cos by max speed
        s->dy = s->dy / speed * max_speed; // sin by max speed
    }
}

// this is not just for the ship, but for every struct i am gonna make (bullets, asteroids too), so generic pointers
void wrap(double *x, double *y, int w, int h, int hud_h) {
    if (*x < 0) *x += w; 
    if (*x > w) *x -= w;
    if (*y < hud_h) *y += (h - hud_h); // account for hud, btw hud in videogames in heads on display that shows vital stats 
    if (*y > h) *y -= (h - hud_h);
}

void update_ship(Ship *s, int w, int h, int hud_h) {
    double speed = 3.0;
    s->x += cos(s->angle) * speed;
    s->y += sin(s->angle) * speed;
    wrap(&s->x, &s->y, w, h, hud_h);
    if (s->invuln > 0) s->invuln--;
}

void draw_ship(Ship *s) {
    // blink during invuln 
    if (s->invuln > 0 && (s->invuln / 4) % 2 == 0) return;

    // ship is a triangle that's why before i stressed that this is more complicated to implement compared to wasd movement because of the tip 
    // noes + 2 back corners: n + r and l
    double size = 20;
    // i don't wwant an equilateral triangle
    // want more of a real ship looking triangle: isoceles
    // angle 0 is to the right and then clockwise
    // 2.5 was handpicked but it doesn't realluy matter
    double nx = s->x + cos(s->angle) * size;
    double ny = s->y + sin(s->angle) * size;
    double lx = s->x + cos(s->angle + 2.5) * size;
    double ly = s->y + sin(s->angle + 2.5) * size;
    double rx = s->x + cos(s->angle - 2.5) * size;
    double ry = s->y + sin(s->angle - 2.5) * size;

    gfx_color(255, 255, 255);
    gfx_line(nx, ny, lx, ly);
    gfx_line(nx, ny, rx, ry);
    gfx_line(rx, ry, lx, ly);
}

// 2. BULLETS

void init_bullets(Bullet arr[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        arr[i].active = 0;
    }
}

void shoot_bullet(Bullet arr[], Ship *s) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (arr[i].active == 0) {
            arr[i].x = s->x + cos(s->angle) * 16; // start @ nose 
            arr[i].y = s->y + sin(s->angle) * 16;
            arr[i].dx = cos(s->angle) * 8; // bullet speed
            arr[i].dy = sin(s->angle) * 8; // doesn't change direction
            arr[i].life = 60;
            arr[i].active = 1;
            return;
        }
    }
}


void update_bullets(Bullet arr[], int w, int h, int hud_h) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (arr[i].active) {
            arr[i].x += arr[i].dx;
            arr[i].y += arr[i].dy;

            // we kill if off screen
            if (arr[i].x < 0 || arr[i].x > w || arr[i].y > h || arr[i].y > h) {
                arr[i].active = 0;
                continue;
            }

            arr[i].life--;
            if (arr[i].life <= 0) arr[i].active = 0;
        }
    }
}

// i loveveee indexing
void draw_bullets(Bullet arr[]) {
    gfx_color(255, 220, 80);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (arr[i].active) {
            gfx_circle(arr[i].x, arr[i].y, 2);
        }
    }
}


// 3. ASTEROIDS


void init_asteroids(Asteroid arr[]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        arr[i].active = 0;
    }
}

void spawn_asteroid(Asteroid arr[], double x, double y, int size, int w, int hud_h) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (arr[i].active == 0) {
            arr[i].x = x;
            arr[i].y = y;
            // random direction, speed depends on size
            double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
            double speed = 0.8 + (3 - size) * 0.5; // bigger -> slower
            arr[i].dx = cos(angle) * speed;
            arr[i].dy = sin(angle) * speed;
            arr[i].size = size;
            arr[i].active = 1;
            return;
        }
    }
}

// spawn 'count' big asteroids @ edges, away from ship
void spawn_wave(Asteroid arr[], int count, int w, int h, int hud_h, Ship *s) {
    for (int i = 0; i < count; i++) {
        double x, y;
        // keep spawning untin far from ship 
        do {
            x = rand() % w;
            y = hud_h + rand() % (h - hud_h);
        } while ((x - s->x)*(x - s->x) + (y - s->y)*(y - s->y) < 150*150);
        spawn_asteroid(arr, x, y, 3, w, hud_h);
    }
}

void update_asteroids(Asteroid arr[], int w, int h, int hud_h) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (arr[i].active) {
            arr[i].x += arr[i].dx;
            arr[i].y += arr[i].dy;
            wrap(&arr[i].x, &arr[i].y, w, h, hud_h);
        }
    }
}

void draw_asteroids(Asteroid arr[]) {
    gfx_color(180, 180, 180);
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (arr[i].active) {
            int radius = arr[i].size * 12; 
            gfx_circle(arr[i].x, arr[i].y, radius);
        }
    }
}

// 4. ACTUAL PHYSICS BETWEEN COLLISIONS BULLETS AND ASTEROIDS`
void check_bullet_hits(Bullet bullets[], Asteroid asteroids[], int *score, int w, int h, int hud_h) {
    // i am trying to do nested loop here 
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue; // if cell in object pool is empty we skip the whole iteration
        for (int j = 0; j < MAX_ASTEROIDS; j++) {
            if (!asteroids[j].active) continue;

            double dx = bullets[i].x - asteroids[j].x;
            double dy = bullets[i].y - asteroids[j].y;
            double r = asteroids[j].size * 12;
            if (dx*dx + dy*dy < r*r) {
                // hit happens 
                // declare new variables for convenience
                int sz = asteroids[j].size;
                double ax = asteroids[j].x;
                double ay = asteroids[j].y;

                bullets[i].active = 0;
                asteroids[j].active = 0;

                // scoring system for getting rid of asteroids
                if (sz == 3) *score += 20;
                else if (sz == 2) *score += 50;
                else *score += 100;

                if (sz > 1) {
                    spawn_asteroid(asteroids, ax, ay, sz - 1, w, hud_h);
                    spawn_asteroid(asteroids, ax, ay, sz - 1, w, hud_h);
                }
                break;
            }
        }
    }
}

int wave_cleared(Asteroid arr[]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (arr[i].active) return 0;
    }
    return 1;
}



// 5. ship asteroid collision respawn and game over
void check_ship_hits(Ship *s, Asteroid asteroids[], int w, int h) {
    if (s->invuln > 0) return; // if invincible skip

    double ship_r = 12; // pretend the ship is a circle to make it easier 

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;

        double dx = s->x - asteroids[i].x;
        double dy = s->y - asteroids[i].y;
        double r = asteroids[i].size * 12 + ship_r;
        if (dx*dx + dy*dy < r*r) {
            s->lives--;
            s->x = w / 2.0;
            s->y = h / 2.0;
            s->dx = 0;
            s->dy = 0;
            s->angle = -M_PI / 2;
            s->invuln = 120; // 2 sec invincible
            return;
        }
    }
}


void draw_game_over(int score, int w, int h) {
    char buf[64];
    gfx_color(255, 80, 80);
    gfx_text(w/2 - 50, h/2 - 20, "GAME OVER");
    gfx_color(255, 255, 255);
    sprintf(buf, "Final score: %d", score);
    gfx_text(w/2 - 50, h/2 + 10, buf);
    gfx_text(w/2 - 60, h/2 + 30, "Press any key to quit");
}


// 6. draw hud (heads up display that is showing stats)
void draw_hud(int score, int lives, int wave, int w, int hud_h) {
    char buf[64];
    gfx_color(255, 255, 255);

    // separator line below hud
    gfx_line(0, hud_h, w, hud_h);

    sprintf(buf, "SCORE: %d", score);
    gfx_text(20, 25, buf);

    sprintf(buf, "LIVES: %d", lives);
    gfx_text(w/2 - 40, 25, buf);

    sprintf(buf, "WAVE: %d", wave);
    gfx_text(w - 100, 25, buf);
}


// 7. start screen 
void draw_title(int w, int h) {
    gfx_color(255, 255, 255);

    gfx_text(w / 2 - 80, h / 2 - 60, "Fundamentals of Computing: Amir T.");
    gfx_text(w / 2 - 80, h / 2 - 40, "PROJECT: SPACE DRIFTING");
    gfx_text(w / 2 - 80, h / 2 - 10, "A and D to rotate");
    gfx_text(w / 2 - 80, h / 2 + 10, "SPACE to shoot");
    gfx_text(w / 2 - 80, h / 2 + 30, "Q to quit");
    gfx_text(w / 2 - 80, h / 2 + 60, "Press any key to start. Enjoy!");
}
