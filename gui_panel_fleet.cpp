#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "audio.h"
#include "fonts.h"
#include "graphics.h"
#include "gui_local.h"
#include "ship.h"
#include "fleet.h"
#include "position.h"

#define SKIP 3

static int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_fleet() {
  }

void panel_init_fleet() {
  buttlist[PANEL_FLEET][BUTTON_EXIT] =		11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  }

void panel_cleanup_fleet() {
  }

void panel_draw_fleet() {
  int line = 0;
  char buf[80];
  Fleet *flt = (Fleet*)cur_object;
  int col = cur_game->players[flt->Owner()]->color;
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = min(int(screenrec.h), 24*(flt->NumShips()+SKIP));
  SDL_FillRect(screen, &screenrec, black);

  sprintf(buf, "Fleet: %s", flt->Name());
  string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);

  if(flt->Target() && flt->Distance() >= 0) {
    sprintf(buf, "Course to %s", flt->Target()->Name());
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    int eta = flt->Distance();
    if(eta > 1) sprintf(buf, "ETA: %d Days", eta);
    else if(eta == 1) sprintf(buf, "ETA: 1 Day");
    else sprintf(buf, "Would Arrive Today");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    }
  else if(flt->Target()) {
    sprintf(buf, "Invalid Course");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    int eta = -flt->Distance();
    if(eta > 1) sprintf(buf, "ETA: %d Days", eta);
    else if(eta == 1) sprintf(buf, "ETA: 1 Day");
    else sprintf(buf, "Would Arrive Today");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    }
  else if((!flt->Location()) && (!flt->Destination())) {
    sprintf(buf, "Orbiting Star");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    sprintf(buf, "%d-Day Orbit", (flt->Period()+255)/256);
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    }
  else if(!flt->Destination()) {
    sprintf(buf, "Orbiting %s", flt->Location()->Name());
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    sprintf(buf, "16 Orbits/Day");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    }
  else {
    sprintf(buf, "Going to %s", flt->Destination()->Name());
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    int eta = flt->ArriveTurn() - cur_game->turn;
    if(eta > 1) sprintf(buf, "ETA: %d Days", eta);
    else if(eta == 1) sprintf(buf, "ETA: 1 Day");
    else sprintf(buf, "Arriving Today");
    string_draw(screen, 816, 13+24*(line++), cur_font_black[col], buf);
    }

  for(int ctr=0; ctr < min(int(flt->NumShips()), 23-SKIP); ++ctr) {
    int clr = cur_game->players[flt->GetShip(ctr)->Owner()]->color;
    if(ctr == selection) clr = 8;
    sprintf(buf, "  %s: %s",
	flt->GetShip(ctr)->CName(), flt->GetShip(ctr)->Name());
    string_draw(screen, 816, 13+24*(line++), cur_font_black[clr], buf);
    }

  if(flt->NumShips() > 1) {
    buttlist[PANEL_FLEET][BUTTON_SPLIT] =		9;
    }
  else {
    buttlist[PANEL_FLEET][BUTTON_SPLIT] =		0;
    }

  if(flt->CanLand() && (!flt->Destination())) {
    buttlist[PANEL_FLEET][BUTTON_LAND] =	10;
    mo[BUTTON_LAND] = -1;
    }
  else {
    buttlist[PANEL_FLEET][BUTTON_LAND] =	0;
    SDL_Rect canrec = {800, 768-64*2, 224, 64};
    SDL_FillRect(screen, &canrec, black);
    update(&canrec);
    }
  }

void panel_clicked_fleet(int mx, int my, int mb) {
  Fleet *flt = (Fleet*)cur_object;
  if(mx >= 800 && mb == 4) {
    --panel_offset;
    panel_draw_fleet();
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = min(int(screenrec.h), 24*(flt->NumShips()));
    update(&screenrec);
    return;
    }
  if(mx >= 800 && mb == 5) {
    ++panel_offset;
    panel_draw_fleet();
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = min(int(screenrec.h), 24*(flt->NumShips()));
    update(&screenrec);
    return;
    }

  int line = my;
  line -= 12+24*SKIP;
  if(line >= 0) line /= 24;
  if(line < 0 || line > 22
        || line >= int(flt->NumShips()) - panel_offset) {
    if(selection >= 0) {
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = -1;
      panel_draw_fleet();
      }
    return;
    }

  line += panel_offset;

  if(mb == 3) {
    }
  else if(mb == 1) {
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24};
    if(selection >= 0) {
      screenrec.y = 12+24*(SKIP+selection);
      update(&screenrec);
      }
    if(selection != line) {
      selection = line;
      screenrec.y = 12+24*(SKIP+selection);
      update(&screenrec);
      panel_draw_fleet();
      }
    else {
      cur_ship = line;
      panel = PANEL_SHIP;
      panel_init();
      }
    audio_play(click2, 8, 8);
    }
  }

void mouse_released_fleet() {
  grabbed = -1;
  }

void mouse_moved_fleet(int mx, int my) {
  }

void button_clicked_fleet(int button) {
  Fleet *flt = (Fleet*)cur_object;
  if(button == BUTTON_LAND) {
    if(!flt->CanLand()) return;
    if(!flt->Location()) return;
    if(!flt->Location()->Represents()) return;
    if(((Planet*)flt->Location()->Represents())->colonies.size() < 1) {
      ((Planet*)flt->Location()->Represents())->colonies.push_back(
	new Colony(flt->Owner(), ((Planet*)flt->Location()->Represents()))
	);
      }

    int which = 0;
    while(!flt->GetShip(which)->CanLand()) ++which;
    ((Planet*)flt->Location()->Represents())->colonies[0]->LandShip(flt->GetShip(which));
    if(flt->NumShips() <= 1) clear_sprites(1, 10);
    flt->DestroyShip(which);
    panel_draw();
    page_draw();
    }
  if(button == BUTTON_SPLIT) {
/*
    if(!flt->Location()) return;
    if(!flt->Location()->Represents()) return;
    for(int shp=1; shp < flt->NumShips(); ++shp) {
      Fleet *newfleet = new Fleet(flt->Location()->Represents(), flt->Name());
      newfleet->AddShip(flt->GetShip(shp));
      newfleet->Location()->Represents()->Sys()->objects.push_back(newfleet);
      }
*/
    for(int shp=1; shp < flt->NumShips(); ++shp) {
      Fleet *newfleet = new Fleet();
      newfleet->CopyFrom(flt);
      newfleet->SetName(flt->Name());
      newfleet->AddShip(flt->GetShip(shp));
      flt->Sys()->objects.push_back(newfleet);
      }
    flt->RemoveShips(1);
    page_draw();
    panel_init();
    }
  }

void panel_update_fleet() {
  }
