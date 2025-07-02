/**
 * @file video_gr.c
 * @brief Implementation file for video graphics functions
 */

#include "video_gr.h"

static char *video_mem;            /**< Process (virtual) address to which VRAM is mapped */
static char *first_buffer;         /**< First double buffer */
static char *second_buffer;        /**< Second double buffer */
static char *arena_buffer;         /**< Buffer for arena drawing */
static char *drawing_buffer;       /**< Current buffer used for drawing */
bool drawing_first_buffer = false; /**< Flag to track the current drawing buffer */

static unsigned h_res;           /**< Horizontal resolution in pixels */
static unsigned v_res;           /**< Vertical resolution in pixels */
static unsigned bits_per_pixel;  /**< Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /**< Number of bytes per pixel */

vbe_mode_info_t vmi_p; /**< VBE mode information */

/**
 * @brief Initializes the video graphics mode
 *
 * This function sets up the video graphics mode specified by the mode parameter,
 * maps video memory, and prepares double buffering.
 *
 * @param mode The VBE mode to set
 * @return Returns a pointer to the video memory, or NULL on failure
 */

void *(vg_init) (uint16_t mode) {
  reg86_t r86;
  vbe_get_mode_info(mode, &vmi_p);
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base;    /* VRAM's physical addresss */
  unsigned int vram_size;    /* VRAM's size, but you can use the frame-buffer size, instead */

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  int r;
  vram_base = vmi_p.PhysBasePtr;
  vram_size = 4 * vmi_p.XResolution * vmi_p.YResolution * bytes_per_pixel;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  first_buffer = video_mem;
  second_buffer = video_mem + vmi_p.XResolution * vmi_p.YResolution * bytes_per_pixel;
  arena_buffer = video_mem + vmi_p.XResolution * vmi_p.YResolution * bytes_per_pixel * 2;
  drawing_buffer = second_buffer;
  memset(&r86, 0, sizeof(r86));

  r86.ax = 0x4F02;
  r86.bx = BIT(14);
  r86.bx |= mode;
  r86.intno = 0x10;
  if (sys_int86(&r86) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }

  return NULL;
}

/**
 * @brief Draws a pixel at a specified location
 *
 * This function sets the color of the pixel at the specified (x, y) location.
 *
 * @param x The x-coordinate of the pixel
 * @param y The y-coordinate of the pixel
 * @param color The color to set the pixel
 * @return Returns 0 on success
 */

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  memcpy(&drawing_buffer[bytes_per_pixel * (vmi_p.XResolution * y + x)], &color, bytes_per_pixel);
  return 0;
}

/**
 * @brief Draws a horizontal line
 *
 * This function draws a horizontal line starting from the specified (x, y) position.
 *
 * @param x The starting x-coordinate of the line
 * @param y The starting y-coordinate of the line
 * @param len The length of the line
 * @param color The color of the line
 * @return Returns 0 on success
 */

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  uint16_t tempX = x;
  while (tempX < x + len) {
    vg_draw_pixel(tempX, y, color);
    tempX++;
  }
  return 0;
}

/**
 * @brief Draws a rectangle
 *
 * This function draws a rectangle starting from the specified (x, y) position.
 *
 * @param x The x-coordinate of the rectangle
 * @param y The y-coordinate of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * @return Returns 0 on success
 */

int(vg_draw_rectangle)(uint16_t x, uint16_t y, u_int16_t width, u_int16_t height, uint32_t color) {
  uint16_t tempY = y;
  while (tempY < y + height) {
    vg_draw_hline(x, tempY, width, color);
    tempY++;
  }
  return 0;
}

/**
 * @brief Draws a pattern of rectangles
 *
 * This function draws a pattern of rectangles on the screen.
 *
 * @param no_rectangles The number of rectangles per row and column
 * @param first The color of the first rectangle
 * @param step The color step between rectangles
 * @param mode The mode of the pattern
 * @return Returns 0 on success
 */

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step, uint16_t mode) {
  u_int16_t width = vmi_p.XResolution / no_rectangles;
  u_int16_t height = vmi_p.YResolution / no_rectangles;
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t row = 0;
  uint8_t col = 0;
  uint32_t color = 0;
  uint32_t R, G, B = 0;
  for (uint8_t i = 0; i < no_rectangles; i++) {
    for (uint8_t j = 0; j < no_rectangles; j++) {
      if (mode == 0x105)
        color = (first + (row * no_rectangles + col) * step) % (1 << vmi_p.BitsPerPixel);
      else {
        R = (first >> vmi_p.RedFieldPosition) & ((1 << vmi_p.RedMaskSize) - 1);
        R = (R + col * step) % (1 << vmi_p.RedMaskSize);

        G = (first >> vmi_p.GreenFieldPosition) & ((1 << vmi_p.GreenMaskSize) - 1);
        G = (G + row * step) % (1 << vmi_p.GreenMaskSize);

        B = (first >> vmi_p.BlueFieldPosition) & ((1 << vmi_p.BlueMaskSize) - 1);
        B = (B + (col + row) * step) % (1 << vmi_p.BlueMaskSize);

        color = (R << vmi_p.RedFieldPosition) | (G << vmi_p.GreenFieldPosition) | (B << vmi_p.BlueFieldPosition);
      }
      vg_draw_rectangle(x, y, width, height, color);
      x += width;
      col++;
    }
    y += height;
    row++;
    col = 0;
    x = 0;
  }
  return 0;
}

/**
 * @brief Draws a pixmap
 *
 * This function draws a pixmap at the specified (x, y) position.
 *
 * @param xpm The XPM image to draw
 * @param x The x-coordinate of the top-left corner of the pixmap
 * @param y The y-coordinate of the top-left corner of the pixmap
 * @return Returns 0 on success
 */

int(vg_draw_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_8_8_8, &img);
  uint32_t color = 0;
  uint32_t desl = 0;
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      color = *(map + desl) | (*(map + desl + 1) << 8) | *(map + desl + 2) << 16;
      vg_draw_pixel(x + j, y + i, color);
      desl += 3;
    }
  }
  return 0;
}

/**
 * @brief Draws an arena pixmap
 *
 * This function draws a pixmap at the specified (x, y) position onto the arena buffer.
 *
 * @param xpm The XPM image to draw
 * @param x The x-coordinate of the top-left corner of the pixmap
 * @param y The y-coordinate of the top-left corner of the pixmap
 * @param ground_color The color representing the ground
 * @return Returns 0
 */

int(vg_draw_arena_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint32_t ground_color) {
  char *buffer = arena_buffer;
  xpm_image_t img;
  unsigned char *map;
  map = xpm_load(xpm, XPM_8_8_8, &img);
  uint32_t color;
  uint32_t desl;
  int color_index;
  uint16_t height = img.height;
  uint16_t width = img.width;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      desl = (i * width + j) * 3;
      color = map[desl] | (map[desl + 1] << 8) | (map[desl + 2] << 16);
      if (color != ground_color) {
      }
      if (color != xpm_transparency_color(XPM_8_8_8)) {
        color_index = (h_res * (y + i) + (x + j)) * 3;
        buffer[color_index] = color & 0xFF;
        buffer[color_index + 1] = (color >> 8) & 0xFF;
        buffer[color_index + 2] = (color >> 16) & 0xFF;
      }
    }
  }
  return 0;
}

/**
 * @brief Draws a character pixmap
 *
 * This function draws a character pixmap at the specified (x, y) position.
 *
 * @param xpm The XPM image to draw
 * @param x The x-coordinate of the top-left corner of the pixmap
 * @param y The y-coordinate of the top-left corner of the pixmap
 * @param xpm_desl The XPM image displacement
 * @param character_width The width of the character pixmap
 * @param character_height The height of the character pixmap
 * @return Returns 0 on success
 */

int(vg_draw_character)(xpm_map_t xpm, uint16_t x, uint16_t y, uint16_t xpm_desl,
                       uint16_t character_width, uint16_t character_height) {
  return 0;
}

/**
 * @brief Moves a pixmap
 *
 * This function moves a pixmap from its current position to the specified (xf, yf) position with a given speed.
 *
 * @param xpm The XPM image to move
 * @param current_x A pointer to the current x-coordinate of the pixmap
 * @param current_y A pointer to the current y-coordinate of the pixmap
 * @param xf The x-coordinate of the final position
 * @param yf The y-coordinate of the final position
 * @param speed The speed of movement
 * @param draw Indicates whether to draw the pixmap while moving
 * @return Returns 0 on success
 */

int(vg_move_pixmap)(xpm_map_t xpm, uint16_t *current_x, uint16_t *current_y, uint16_t xf, uint16_t yf, int16_t speed, bool draw) {
  int16_t x_distance = xf - *current_x;
  int16_t y_distance = yf - *current_y;
  if (speed > 0) {

    if ((abs(*current_x - xf) <= speed) && (abs(*current_y - yf) <= speed)) {
      if (draw)
        vg_draw_pixmap(xpm, xf, yf);
      return 0;
    }
    if (draw)
      vg_draw_pixmap(xpm, *current_x, *current_y);
    if (*current_x != xf) {
      if (x_distance < 0)
        *current_x -= speed;
      else
        *current_x += speed;
    }
    if (*current_y != yf) {
      if (y_distance < 0)
        *current_y -= speed;
      else
        *current_y += speed;
    }
  }
  else {
  }
  return 0;
}

/**
 * @brief Flips the display buffers
 *
 * This function flips the display buffers to show the buffer that was just drawn to.
 *
 * @return Returns 0 on success, -1 on failure
 */
int vg_flip_buffers() {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ax = 0x4F07;
  r86.bh = 0;
  r86.bl = 0x80;
  r86.cx = 0;
  r86.dx = drawing_first_buffer ? 0 : v_res; // set the display to the buffer that was just drawn to
  if (sys_int86(&r86) != OK) {
    printf("set_display_start: sys_int86() failed \n");
    return -1;
  }
  drawing_first_buffer = !drawing_first_buffer;                         // flip buffers
  drawing_buffer = drawing_first_buffer ? first_buffer : second_buffer; // flip buffers
  memcpy(drawing_buffer, arena_buffer, h_res * v_res * bytes_per_pixel);

  return 0;
}

/**
 * @brief Clears the buffer
 *
 * This function clears the specified buffer.
 *
 * @param buffer The buffer to clear
 */

void vg_clear_buffer(char *buffer) {
  memset(buffer, 0, h_res * v_res * bytes_per_pixel);
}

/**
 * @brief Gets the first buffer
 *
 * This function returns a pointer to the first buffer.
 *
 * @return Pointer to the first buffer
 */

char *get_first_buffer() {
  return first_buffer;
}

/**
 * @brief Gets the second buffer
 *
 * This function returns a pointer to the second buffer.
 *
 * @return Pointer to the second buffer
 */
char *get_second_buffer() {
  return second_buffer;
}

/**
 * @brief Gets the drawing buffer
 *
 * This function returns a pointer to the current drawing buffer.
 *
 * @return Pointer to the drawing buffer
 */
char *get_drawing_buffer() {
  return drawing_buffer;
}

/**
 * @brief Gets the arena buffer
 *
 * This function returns a pointer to the arena buffer.
 *
 * @return Pointer to the arena buffer
 */
char *get_arena_buffer() {
  return arena_buffer;
}

/**
 * @brief Gets the horizontal resolution
 *
 * This function returns the horizontal resolution.
 *
 * @return Horizontal resolution
 */
unsigned get_h_res() {
  return h_res;
}
