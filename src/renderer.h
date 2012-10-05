
#ifndef _RENDERER_H_86745346
#define _RENDERER_H_86745346

#if defined(SDL_RENDERER)
# include "renderer_sdl.h"
#else
# include "renderer_null.h"
#endif


int renderer_init();
void renderer_deinit();
int renderer_set_resolution(int width, int height, int fullscreen);
frame_t *renderer_get_screen_frame();
void renderer_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest);
int renderer_frame_get_width(const frame_t *frame);
int renderer_frame_get_height(const frame_t *frame);

void renderer_draw_transp_sprite(const sprite_t *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest);
void renderer_draw_waves_sprite(const sprite_t *sprite, int x, int y, frame_t *dest);
void renderer_draw_sprite(const sprite_t *sprite, int x, int y, frame_t *dest);
void renderer_draw_overlay_sprite(const sprite_t *sprite, int x, int y, int y_off, frame_t *dest);
surface_t *renderer_draw_masked_sprite(const sprite_t *sprite, int x, int y, const sprite_t *mask, surface_t *surface, frame_t *dest);
void renderer_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h);
void renderer_draw_rect(int x, int y, int width, int height, int color, frame_t *dest);
void renderer_fill_rect(int x, int y, int width, int height, int color, frame_t *dest);
void renderer_set_palette(const uint8_t *palette);
void renderer_mark_dirty(int x, int y, int width, int height);
void renderer_swap_buffers();

#endif // ! _RENDERER_H_86745346
