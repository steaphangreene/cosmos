#include <SDL.h>

void graphics_init(int, int, int, Uint32);

void set_sprite(int, SDL_Surface *, int own=1);
void move_sprite(int, int, int);
void update_sprite(int);
void clear_sprites(int s = 1, int e = -1);

void set_cursor(SDL_Surface *s);

void update_all();
void update(SDL_Rect *r);
void update(int x, int y, unsigned int w, unsigned int h);
void do_updates();
void toggle_fullscreen();

SDL_Surface *get_image(const char *, int, int);
SDL_Surface *get_alpha_image(const char *, int, int);

SDL_Surface *get_cursor_image();
SDL_Surface *get_select_image();

SDL_Surface *get_blank0_image();
SDL_Surface *get_blank1_image();

SDL_Surface *get_check0_image();
SDL_Surface *get_check1_image();

SDL_Surface *get_star_image();
SDL_Surface *get_gstar_image();
SDL_Surface *get_splanet_image();

void drawline(SDL_Surface *, int, int, int, int, Uint32, Uint32);
SDL_Surface *getline(int, int, int, int, Uint32, Uint32);

void show_progress(int prog, int total);

extern const unsigned int rchan;
extern const unsigned int gchan;
extern const unsigned int bchan;
extern const unsigned int achan;

unsigned int color3(int);

#define overlaps(a, b) (   (a).x < ((b).x + (b).w) && (a).y < ((b).y + (b).h) \
			&& (b).x < ((a).x + (a).w) && (b).y < ((a).y + (a).h))

