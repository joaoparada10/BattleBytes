#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_

#include <lcom/lcf.h>
#include "../graphics/sprite.h"
#include "../graphics/asprite.h"
#include "../graphics/video_gr.h"
#include "../model/game_model.h"
#include "../view/constants.h"
#include "../logic/game_logic.h"
#include "../model/arena.h"
#include "../menu/menu.h"
#include "../../assets/xpm/objects/tank1.xpm"
#include "../../assets/xpm/objects/tank2.xpm"
#include "../../assets/xpm/objects/tank3.xpm"
#include "../../assets/xpm/objects/tank4.xpm"
#include "../../assets/xpm/objects/tank5.xpm"
#include "../../assets/xpm/objects/tank6.xpm"
#include "../../assets/xpm/objects/tank7.xpm"
#include "../../assets/xpm/objects/tank8.xpm"
#include "../../assets/xpm/objects/tank9.xpm"
#include "../../assets/xpm/objects/tank10.xpm"
#include "../../assets/xpm/objects/tank11.xpm"
#include "../../assets/xpm/objects/tank12.xpm"
#include "../../assets/xpm/crosshair.xpm"
#include "../../assets/xpm/cursor.xpm"
#include "../../assets/xpm/arenas/arena1.xpm"
#include "../../assets/xpm/arenas/arena2.xpm"
#include "../../assets/xpm/arenas/arena3.xpm"
#include "../../assets/xpm/arenas/arena4.xpm"
#include "../../assets/xpm/arenas/arena5.xpm"
#include "../../assets/xpm/objects/virus40.xpm"
#include "../../assets/xpm/objects/virus50_1.xpm"
#include "../../assets/xpm/objects/virus50_2.xpm"
#include "../../assets/xpm/objects/virus50_3.xpm"
#include "../../assets/xpm/objects/virus50_4.xpm"
#include "../../assets/xpm/objects/virus50_5.xpm"
#include "../../assets/xpm/explosion/explosion1.xpm"
#include "../../assets/xpm/explosion/explosion2.xpm"
#include "../../assets/xpm/explosion/explosion3.xpm"
#include "../../assets/xpm/explosion/explosion4.xpm"
#include "../../assets/xpm/explosion/explosion5.xpm"
#include "../../assets/xpm/explosion/explosion6.xpm"
#include "../../assets/xpm/explosion/explosion7.xpm"
#include "../../assets/xpm/explosion/explosion8.xpm"
#include "../../assets/xpm/explosion/explosion9.xpm"
#include "../../assets/xpm/explosion/explosion10.xpm"
#include "../../assets/xpm/explosion/explosion11.xpm"
#include "../../assets/xpm/explosion/explosion12.xpm"
#include "../../assets/xpm/explosion/explosion13.xpm"
#include "../../assets/xpm/explosion/explosion14.xpm"
#include "../../assets/xpm/explosion/explosion15.xpm"
#include "../../assets/xpm/explosion/explosion16.xpm"
#include "../../assets/xpm/explosion/explosion17.xpm"
#include "../../assets/xpm/explosion/explosion18.xpm"
#include "../../assets/xpm/explosion/explosion19.xpm"
#include "../../assets/xpm/explosion/explosion20.xpm"
#include "../../assets/xpm/text/game_letters.xpm"
#include "../../assets/xpm/text/game_numbers.xpm"


typedef struct Explosion {
    AnimSprite *explosion_asp;
    struct Explosion *next;
} Explosion;


int load_game_sprites();

void free_game_sprites();

void load_game_fonts();

void free_game_fonts();


Sprite* create_virus1_sprite(int x, int y);

AnimSprite *create_virus2_asprite(int x, int y);

Explosion* create_explosion(int x, int y);

void destroy_explosion(Explosion *explosion);

int draw_tank();

int draw_enemies();

int draw_explosions();

int draw_crosshair();

int draw_cursor();

int draw_game();

int draw_header();

int draw_elements();

int draw_footer();

int draw_arena();

int draw_timer();

Sprite* get_tank_sprite();

Sprite* get_crosshair();

Sprite* get_cursor();



#endif

