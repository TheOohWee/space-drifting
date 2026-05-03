// header fo game functions 

#ifndef FUNC_H
#define FUNC_H

// struct + prototypes

// 1. SHIP

// struct for a ship (i didn't want simple wasd movemement)
// to make it more realistic I decided to do angle movement using ad 
// u can see when run
typedef struct {
    double x, y;
    double dx, dy;
    double angle;
    int alive;
    int lives;
    int invuln; // other things self-explanatoruy but this is number of times ship is invincible when respawn after hit the asteroid
} Ship; 


// function prototypes for ship
void init_ship(Ship *s, int w, int h);
void update_ship(Ship *s, int w, int h, int hud_h); // hud_h arg is a height of the hud on top
void draw_ship(Ship *s);
void thrust_ship(Ship *s);
void rotate_ship(Ship *s, int dir);
void wrap(double *x, double *y, int w, int h, int hud_h); // when ship, asteroid or bullet go off the screen i want them to pop back from the opposite side so the game would not feel cramped

// 2. BULLET

#define MAX_BULLETS 30

typedef struct {
    double x, y;
    double dx, dy;
    int life;
    int active;
} Bullet;

void init_bullets(Bullet arr[]);
void shoot_bullets(Bullet arr[], Ship *s);
void update_bullets(Bullet arr[], int w, int h, int hud_h);
void draw_bullets(Bullet arr[]);

// 3. ASTEROIDS

#define MAX_ASTEROIDS 40

typedef struct {
    double x, y;
    double dx, dy;
    int size; // here, i think we can do simple 3 = big, 2 = mid, 1 = small
    int active;
} Asteroid;

void init_asteroids(Asteroid arr[]);
void spawn_asteroid(Asteroid arr[], double x, double y, int size, int w, int hud_h);
void spawn_wave(Asteroid arr[], int count, int w, int h, int hud_h, Ship *s);
void update_asteroids(Asteroid arr[], int w, int h, int hud_h);
void draw_asteroids(Asteroid arr[]);


// 4. BULLET HITS + WAVE CLEARED
void check_bullet_hits(Bullet bullets[], Asteroid asteroids[], int *score, int w, int h, int hud_h);
int wave_cleared(Asteroid arr[]);

// 5. ship asteroid collision respawn and game over
void check_ship_hits(Ship *s, Asteroid asteroids[], int w, int h);
void draw_game_over(int score, int w, int h);

// 6. draw hud (heads up display that is showing stats)
void draw_hud(int score, int lives, int wave, int w, int hud_h);


// 7. start screen 
void draw_title(int w, int h);

#endif
