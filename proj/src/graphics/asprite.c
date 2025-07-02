/**
 * @file asprite.c
 * @brief Implementation file for animated sprites management
 */


#include "asprite.h"

/**
 * @brief Creates an animated sprite
 *
 * This function creates an animated sprite with the provided parameters.
 * It initializes the sprite with the first pixmap and allocates memory
 * for an array of pointers to store subsequent pixmaps.
 *
 * @param aspeed The animation speed
 * @param no_pic The number of pixmap frames
 * @param pic1 The first pixmap frame
 * @param ... Additional pixmap frames (variadic arguments)
 * @return A pointer to the created animated sprite, or NULL if creation fails
 */


// Adapted from the lecture slides(https://web.fe.up.pt/~pfs/aulas/lcom2324/at/9sprites.pdf)
AnimSprite *create_asprite(int aspeed, uint8_t no_pic, const char *pic1[], ...) {
  AnimSprite *asp = malloc(sizeof(AnimSprite));
  // create a standard sprite with first pixmap
  asp->sp = create_sprite((const char**)pic1, 0, 0, 0, 0);
  // allocate array of pointers to pixmaps
  asp->map = malloc((no_pic) * sizeof(char *));
  // initialize the first pixmap
  asp->map[0] = asp->sp->map;

  asp->num_fig = no_pic;
  asp->cur_fig = 0;
  asp->aspeed = aspeed;
  asp->cur_aspeed = 0;


  // initialize the remainder with the variable arguments
  // iterate over the list of arguments
  va_list ap;
  va_start(ap, pic1);
  for (int i = 1; i < no_pic; i++) {
    const char **tmp = va_arg(ap, const char **);
    xpm_image_t img;
    asp->map[i] = xpm_load(tmp, XPM_8_8_8, &img);
    if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height) { // failure: release allocated memory
      for (int j = 1; j < i; j++)
        free(asp->map[i]);
      free(asp->map);
      destroy_sprite(asp->sp);
      free(asp);
      va_end(ap);
      return NULL;
    }
  }
  va_end(ap);

  return asp;
}

/**
 * @brief Animates an animated sprite
 *
 * This function updates the animated sprite's current frame according to
 * its animation speed. If the animation speed allows for it, the sprite's
 * pixmap is updated to the next frame.
 *
 * @param asp The animated sprite to animate
 * @param destroy Flag indicating whether the sprite should be destroyed
 *                after reaching the last frame
 * @return 1 if the last frame is reached and destroy flag is set, 0 otherwise
 */


int animate_asprite(AnimSprite *asp, bool destroy) {

  if (asp->cur_aspeed >= asp->aspeed) {
    asp->cur_aspeed = 0;
    asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
    asp->sp->map = asp->map[asp->cur_fig];
    if (destroy && asp->cur_fig == asp->num_fig - 1) {
            return 1;
        }
  }
  else {
    asp->cur_aspeed++;
  }
  return 0;
}

/**
 * @brief Destroys an animated sprite
 *
 * This function frees the memory allocated for an animated sprite,
 * including its pixmaps and the sprite structure itself.
 *
 * @param asp The animated sprite to destroy
 */

void destroy_asprite(AnimSprite *asp) {
    if (asp == NULL) return;

    // Free all the pixmaps
    for (int i = 0; i < asp->num_fig; i++) {
        free(asp->map[i]);
    }

    // Free the pixmaps array
    free(asp->map);

    // Destroy the main sprite
    destroy_sprite(asp->sp);

    // Free the AnimSprite structure
    free(asp);
}
