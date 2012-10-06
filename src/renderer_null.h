#ifndef _RENDERER_NULL_H_456792343
#define _RENDERER_NULL_H_456792343

#include <stdint.h>

struct sprite;
typedef int surface_t;

typedef struct {
	uint16_t w;
	uint16_t h;
} frame_t;

int null_init();
void null_deinit();
int null_set_resolution(int width, int height, int fullscreen);
frame_t *null_get_screen_frame();
void null_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest);
int null_frame_get_width(const frame_t *frame);
int null_frame_get_height(const frame_t *frame);

void null_draw_transp_sprite(const struct sprite *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest);
void null_draw_waves_sprite(const struct sprite *sprite, int x, int y, frame_t *dest);
void null_draw_sprite(const struct sprite *sprite, int x, int y, frame_t *dest);
void null_draw_overlay_sprite(const struct sprite *sprite, int x, int y, int y_off, frame_t *dest);
surface_t *null_draw_masked_sprite(const struct sprite *sprite, int x, int y, const struct sprite *mask, surface_t *surface, frame_t *dest);
void null_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h);
void null_draw_rect(int x, int y, int width, int height, int color, frame_t *dest);
void null_fill_rect(int x, int y, int width, int height, int color, frame_t *dest);
void null_set_palette(const uint8_t *palette);
void null_mark_dirty(int x, int y, int width, int height);
void null_swap_buffers();


#endif // _RENDERER_NULL_H_456792343
