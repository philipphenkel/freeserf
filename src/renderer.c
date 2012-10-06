#include "renderer.h"

#if defined(SDL_RENDERER)

// ######## SDL RENDERER ######## ######## ######## ######## ######## ######## ########
#include "renderer_sdl.h"
#include "gfx.h"

int
renderer_init()
{
	return sdl_init();
}

void
renderer_deinit()
{
    sdl_deinit();
}

int renderer_set_resolution
(int width, int height, int fullscreen)
{
	return sdl_set_resolution(width, height, fullscreen);
}

frame_t *
renderer_get_screen_frame()
{
	return sdl_get_screen_frame();
}

void
renderer_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest)
{
    sdl_frame_init(frame, x, y, width, height, dest);
}

int
renderer_frame_get_width(const frame_t *frame)
{
	return sdl_frame_get_width(frame);
}

int
renderer_frame_get_height(const frame_t *frame)
{
	return sdl_frame_get_height(frame);
}


void
renderer_draw_transp_sprite(const sprite_t *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest)
{
    sdl_draw_transp_sprite(sprite, x, y, use_off, y_off, color_off, dest);
}

void
renderer_draw_waves_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
    sdl_draw_waves_sprite(sprite, x, y, dest);
}

void
renderer_draw_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
    sdl_draw_sprite(sprite, x, y, dest);
}

void
renderer_draw_overlay_sprite(const sprite_t *sprite, int x, int y, int y_off, frame_t *dest)
{
    sdl_draw_overlay_sprite(sprite, x, y, y_off, dest);
}

surface_t *
renderer_draw_masked_sprite(const sprite_t *sprite, int x, int y, const sprite_t *mask, surface_t *surface, frame_t *dest)
{
	return sdl_draw_masked_sprite(sprite, x, y, mask, surface, dest);
}

void
renderer_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h)
{
    sdl_draw_frame(dx, dy, dest, sx, sy, src, w, h);
}

void
renderer_draw_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
    sdl_draw_rect(x, y, width, height, color, dest);
}

void
renderer_fill_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
    sdl_fill_rect(x, y, width, height, color, dest);
}

void
renderer_mark_dirty(int x, int y, int width, int height)
{
    sdl_mark_dirty(x, y, width, height);
}

void
renderer_swap_buffers()
{
    sdl_swap_buffers();
}

void
renderer_set_palette(const uint8_t *palette)
{
    sdl_set_palette(palette);
}

#elif defined(NULL_RENDERER)

// ######## NULL RENDERER ######## ######## ######## ######## ######## ######## ########

#include "renderer_null.h"
#include "gfx.h"

int
renderer_init()
{
	return null_init();
}

void
renderer_deinit()
{
    null_deinit();
}

int renderer_set_resolution
(int width, int height, int fullscreen)
{
	return null_set_resolution(width, height, fullscreen);
}

frame_t *
renderer_get_screen_frame()
{
	return null_get_screen_frame();
}

void
renderer_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest)
{
    null_frame_init(frame, x, y, width, height, dest);
}

int
renderer_frame_get_width(const frame_t *frame)
{
	return null_frame_get_width(frame);
}

int
renderer_frame_get_height(const frame_t *frame)
{
	return null_frame_get_height(frame);
}


void
renderer_draw_transp_sprite(const sprite_t *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest)
{
    null_draw_transp_sprite(sprite, x, y, use_off, y_off, color_off, dest);
}

void
renderer_draw_waves_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
    null_draw_waves_sprite(sprite, x, y, dest);
}

void
renderer_draw_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
    null_draw_sprite(sprite, x, y, dest);
}

void
renderer_draw_overlay_sprite(const sprite_t *sprite, int x, int y, int y_off, frame_t *dest)
{
    null_draw_overlay_sprite(sprite, x, y, y_off, dest);
}

surface_t *
renderer_draw_masked_sprite(const sprite_t *sprite, int x, int y, const sprite_t *mask, surface_t *surface, frame_t *dest)
{
	return null_draw_masked_sprite(sprite, x, y, mask, surface, dest);
}

void
renderer_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h)
{
    null_draw_frame(dx, dy, dest, sx, sy, src, w, h);
}

void
renderer_draw_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
    null_draw_rect(x, y, width, height, color, dest);
}

void
renderer_fill_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
    null_fill_rect(x, y, width, height, color, dest);
}

void
renderer_mark_dirty(int x, int y, int width, int height)
{
    null_mark_dirty(x, y, width, height);
}

void
renderer_swap_buffers()
{
    null_swap_buffers();
}

void
renderer_set_palette(const uint8_t *palette)
{
    null_set_palette(palette);
}
#else

#error Define a renderer: SDL_RENDERER, NULL_RENDERER
#endif








