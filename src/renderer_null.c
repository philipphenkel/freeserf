#include "log.h"
#include "renderer_null.h"
#include "gfx.h"

static frame_t screen;

int
null_init()
{
	return 0;
}

void
null_deinit()
{
}

int
null_set_resolution(int width, int height, int fullscreen)
{
    LOGI("null_set_resolution w=%d h=%d", width, height);
	screen.w = width;
	screen.h = height;
	return 0;
}

frame_t *
null_get_screen_frame()
{
	return &screen;
}

void
null_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest)
{
    frame->w = width;
    frame->h = height;
}

int
null_frame_get_width(const frame_t *frame)
{
	return frame->w;
}

int
null_frame_get_height(const frame_t *frame)
{
	return frame->h;
}


void
null_draw_transp_sprite(const sprite_t *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest)
{
}

void
null_draw_waves_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
}

void
null_draw_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
}

void
null_draw_overlay_sprite(const sprite_t *sprite, int x, int y, int y_off, frame_t *dest)
{
}

surface_t *
null_draw_masked_sprite(const sprite_t *sprite, int x, int y, const sprite_t *mask, surface_t *surface, frame_t *dest)
{
	return 0;
}

void
null_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h)
{
}

void
null_draw_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
}

void
null_fill_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
}

void
null_mark_dirty(int x, int y, int width, int height)
{
}

void
null_swap_buffers()
{
}

void
null_set_palette(const uint8_t *palette)
{
}

