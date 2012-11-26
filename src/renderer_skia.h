#ifndef _RENDERER_SKIA_H_2345862
#define _RENDERER_SKIA_H_2345862

#include <stdint.h>

struct sprite;
typedef int surface_t;

typedef struct {
    uint16_t clip_x;
    uint16_t clip_y;
    uint16_t clip_w;
    uint16_t clip_h;
    void* skCanvas;
    void* skBitmap;
} frame_t;

#ifdef __cplusplus
extern "C" {
#endif

int skia_init();
void skia_deinit();
int skia_set_resolution(int width, int height, int fullscreen);
frame_t *skia_get_screen_frame();

void skia_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest);
int skia_frame_get_width(const frame_t *frame);
int skia_frame_get_height(const frame_t *frame);

void skia_draw_transp_sprite(const struct sprite *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest);
void skia_draw_waves_sprite(const struct sprite *sprite, int x, int y, frame_t *dest);
void skia_draw_sprite(const struct sprite *sprite, int x, int y, frame_t *dest);
void skia_draw_overlay_sprite(const struct sprite *sprite, int x, int y, int y_off, frame_t *dest);
surface_t *skia_draw_masked_sprite(const struct sprite *sprite, int x, int y, const struct sprite *mask, surface_t *surface, frame_t *dest);
void skia_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h);
void skia_draw_rect(int x, int y, int width, int height, int color, frame_t *dest);
void skia_fill_rect(int x, int y, int width, int height, int color, frame_t *dest);
void skia_set_palette(const uint8_t *palette);
void skia_mark_dirty(int x, int y, int width, int height);
void skia_swap_buffers();

#ifdef __cplusplus
}
#endif

#endif // _RENDERER_SKIA_H_2345862
