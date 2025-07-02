/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>
#include "video_gr.h"
#include "../view/constants.h"

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  uint16_t x,y;             /**< current sprite position */
  uint16_t width, height;   /**< sprite dimensions */
  int xspeed, yspeed;  /**< current speeds in the x and y direction */
  unsigned char *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(const char *pic[], int x, int y,
                      int xspeed, int yspeed);

/** Animate the sprite "fig" according to its attributes in memory,
 * whose address is "base".  The animation detects the screen borders
 * and change the speed according; it also detects collision with
 * other objects, including the player pad. Collisions with the screen
 * border generates a perfect reflection, while collision with other
 * objects generates a random perturbation in the object speed. Other
 * strategies can be devised: use quasi-elastic collision based on the
 * objects "mass" and speed, generate spin effect based on amount of
 * tangential speed direction relative to the object center of
 * "mass"...  Returns the kind of collision detected, RIGHT_HIT or
 * LEFT_HIT, if a collision with the players pad (WHITE colored!) is
 * detected.
 */
int animate_sprite(Sprite *sp);

int draw_sprite(Sprite *sp);
int draw_sprite_to_buffer(Sprite *sp, char* buffer);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *fig);


/** @} end of sprite */

#endif
