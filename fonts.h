#include <SDL.h>

struct font;

font *font_init();
font *font_colored(font *f, unsigned long c);
int string_length(const char *str, font *f);
int string_height(const char *str, font *f);
SDL_Surface *get_string(font *f, const char *t);
void string_draw(SDL_Surface *s, int x, int y, font *f, const char *t);
void string_drawr(SDL_Surface *s, int x, int y, font *f, const char *t);
void string_drawc(SDL_Surface *s, int x, int y, font *f, const char *t);
