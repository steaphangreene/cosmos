#include <SDL.h>

struct font;

font *font_init();
font *font_colored(font *f, unsigned long c);
int string_len(const char *str, font *f);
void string_draw(SDL_Surface *s, int x, int y, font *f, const char *t);
