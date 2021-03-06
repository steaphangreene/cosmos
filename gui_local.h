#include "sobject.h"
#include "system.h"
#include "galaxy.h"

void gui_init_galaxy();
void page_draw_galaxy();
void page_init_galaxy();
void page_cleanup_galaxy();
void page_update_galaxy();
void page_clicked_galaxy(int, int, int);
void mouse_moved_galaxy(int, int);
void mouse_released_galaxy();

void gui_init_system();
void page_draw_system();
void page_init_system();
void page_cleanup_system();
void page_update_system();
void page_clicked_system(int, int, int);
void mouse_moved_system(int, int);
void mouse_released_system();

void gui_init_planet();
void page_draw_planet();
void page_init_planet();
void page_cleanup_planet();
void page_update_planet();
void page_clicked_planet(int, int, int);
void mouse_moved_planet(int, int);
void mouse_released_planet();

char *popstr(int, int mpop=0);

void gui_init_colony();
void panel_draw_colony();
void panel_init_colony();
void panel_cleanup_colony();
void panel_update_colony();
void panel_clicked_colony(int, int, int);
void button_clicked_colony(int);
void mouse_moved_colony(int, int);
void mouse_released_colony();

void gui_init_fleet();
void panel_draw_fleet();
void panel_init_fleet();
void panel_cleanup_fleet();
void panel_update_fleet();
void panel_clicked_fleet(int, int, int);
void button_clicked_fleet(int);
void mouse_moved_fleet(int, int);
void mouse_released_fleet();

void gui_init_ship();
void panel_draw_ship();
void panel_init_ship();
void panel_cleanup_ship();
void panel_update_ship();
void panel_clicked_ship(int, int, int);
void button_clicked_ship(int);
void mouse_moved_ship(int, int);
void mouse_released_ship();

void panel_init();
void page_init();
void panel_draw();
void page_draw();

extern SDL_Surface *screen;
extern SDL_Surface *selectg;
extern unsigned long black;
extern font *cur_font[9];
extern font *cur_font_black[9];
extern int click1, click2;
extern int buttlist[PANEL_MAX][BUTTON_MAX];
extern int pagemap[PAGE_MAX][BUTTON_MAX];
extern int mo[BUTTON_MAX];
extern int page, panel;

//#define FLEET_SIZE	4
//#define FLEET_SPACE	6
//#define FLEET_SQOFF	9

#define FLEET_SIZE	6
#define FLEET_SPACE	8
#define FLEET_SQOFF	16

#define PLANET_SIZE	9
#define PLANET_SPACE	10
#define PLANET_SQOFF	36

#define SYSTEM_SIZE	9
#define SYSTEM_SPACE	10
#define SYSTEM_SQOFF	36
