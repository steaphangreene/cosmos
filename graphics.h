SDL_Surface *get_image(const char *, int, int);
SDL_Surface *get_alpha_image(const char *, int, int);

SDL_Surface *get_cursor_image();

SDL_Surface *build_button0(const char *);
SDL_Surface *build_button1(const char *);

SDL_Surface *get_star_image();

extern const unsigned int rchan;
extern const unsigned int gchan;
extern const unsigned int bchan;
extern const unsigned int achan;

#define overlaps(a, b) (   (a).x < ((b).x + (b).w) && (a).y < ((b).y + (b).h) \
			&& (b).x < ((a).x + (a).w) && (b).y < ((a).y + (a).h))

