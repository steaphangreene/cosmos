#include <SDL.h>

void fonts_init();

int string_len(const char *str);
void string_draw(SDL_Surface *s, int x, int y, unsigned long c, const char *t);
