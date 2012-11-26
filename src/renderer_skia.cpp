
#include "SkCanvas.h"
#include "SkDevice.h"
#include "SkBitmap.h"
#include "SkColor.h"
#include "SkColorFilter.h"
#include <map>
#include <memory>

extern "C" {

#include "log.h"
#include "renderer_skia.h"
#include "freeserf_endian.h"
#include "gfx.h"

}

// TODO screen pointer can moved to core - it is only used there!
static frame_t screen;

static std::shared_ptr<SkColorTable> g_color_table;

typedef std::map<const sprite_t *, std::shared_ptr<SkBitmap>> bitmap_cache_t;
static bitmap_cache_t g_bitmap_cache;

// helpers
SkCanvas* getCanvas(frame_t *frame) {
	SkCanvas* res = NULL;
	if(frame->skCanvas != NULL) {
		res = (SkCanvas*)frame->skCanvas;
	}

	return res;
}



int
skia_init()
{
    if(!g_color_table) {
        g_color_table.reset(new SkColorTable(256));
    }
    g_bitmap_cache.clear();
	return 0;
}

void
skia_deinit()
{
    g_bitmap_cache.clear();
}

int
skia_set_resolution(int width, int height, int fullscreen)
{
	// no implementation needed
	/*
    LOGI("skia_set_resolution w=%d h=%d", width, height);
    screen.clip_x = 0;
    screen.clip_y = 0;
	screen.clip_w = width;
	screen.clip_h = height;
	*/
	return 0;
}

frame_t *
skia_get_screen_frame()
{
	return &screen;
}

void
skia_frame_init(frame_t *frame, int x, int y, int width, int height, frame_t *dest)
{
	LOGI("skia_frame_init x=%d y=%d w=%d h=%d", x, y, width, height);

    frame->clip_x = x;
    frame->clip_y = y;
    frame->clip_w = width;
    frame->clip_h = height;
	frame->skBitmap = NULL;
	frame->skCanvas = NULL;

	if(dest != NULL) {
		frame->skCanvas = dest->skCanvas;
	}
	else {
		LOGI("skia_frame_init screen.skCanvas=%d screen.skBitmap=%d", screen.skCanvas, screen.skBitmap);
	}
}

int
skia_frame_get_width(const frame_t *frame)
{
	return frame->clip_w;
}

int
skia_frame_get_height(const frame_t *frame)
{
	return frame->clip_h;
}


void
skia_draw_transp_sprite(const sprite_t *sprite, int x, int y, int use_off, int y_off, int color_off, frame_t *dest)
{
//    if(1) return;
//	LOGI("skia_draw_transp_sprite x=%d y=%d use_off=%d y_off=%d", x, y, use_off, y_off);

	if (use_off) {
		x += le16toh(sprite->x);
		y += le16toh(sprite->y);
	}


	void *data = (uint8_t *)sprite + sizeof(sprite_t);
	size_t width = le16toh(sprite->w);
	size_t height = le16toh(sprite->h);


   std::shared_ptr<SkBitmap> bitmap;
    bitmap_cache_t::iterator it = g_bitmap_cache.find(sprite);
    if( it != g_bitmap_cache.end() ) {
        bitmap = it->second;
    }
    else {
        LOGI("create transparent sprite = %d", sprite);

        // Create bitmap and add to cache
        bitmap.reset(new SkBitmap());
        g_bitmap_cache[sprite] = bitmap;

        bitmap->setConfig(SkBitmap::kIndex8_Config, width, height);
        bitmap->allocPixels(g_color_table.get());

        {
            SkAutoLockPixels alp(*bitmap);
            size_t unpack_size = width * height;
            gfx_unpack_transparent_sprite(bitmap->getPixels(), data, unpack_size, color_off);
        }

        bitmap->notifyPixelsChanged();
    }

	SkRect src_rect;
	src_rect.fLeft = 0;
	src_rect.fTop = y_off;
	src_rect.fRight = SkIntToScalar(width);
	src_rect.fBottom = - y_off + SkIntToScalar(height);

	SkRect dst_rect;
	dst_rect.fLeft = x;
	dst_rect.fTop = y + y_off;
	dst_rect.fRight = x + SkIntToScalar(width);
	dst_rect.fBottom = y - y_off + SkIntToScalar(height);

	((SkCanvas*)dest->skCanvas)->drawBitmapRectToRect(*bitmap, &src_rect, dst_rect, NULL);

#if 0
	/* Bounding box */
	skia_draw_rect(x, y + y_off, width, height - y_off, 72, dest);
#endif
}

void
skia_draw_waves_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
//	LOGI("skia_draw_waves_sprite x=%d y=%d", x, y);

    skia_draw_transp_sprite(sprite, x, y, 0, 0, 0, dest);
}

void
skia_draw_sprite(const sprite_t *sprite, int x, int y, frame_t *dest)
{
	LOGI("skia_draw_sprite x=%d y=%d", x, y);

	x += le16toh(sprite->x);
	y += le16toh(sprite->y);

	size_t width = le16toh(sprite->w);
	size_t height = le16toh(sprite->h);

	void *s_data = (uint8_t *)sprite + sizeof(sprite_t);

	SkBitmap bitmap;
	bitmap.setConfig(SkBitmap::kIndex8_Config, width, height);
	bitmap.setPixels(s_data, g_color_table.get());

	SkRect src_rect;
	src_rect.fLeft = 0;
	src_rect.fTop = 0;
	src_rect.fRight = SkIntToScalar(width);
	src_rect.fBottom = SkIntToScalar(height);

	SkRect dst_rect;
	dst_rect.fLeft = x;
	dst_rect.fTop = y;
	dst_rect.fRight = x + SkIntToScalar(width);
	dst_rect.fBottom = y + SkIntToScalar(height);

	((SkCanvas*)dest->skCanvas)->drawBitmapRectToRect(bitmap, &src_rect, dst_rect, NULL);

#if 1
	/* Bounding box */
	skia_draw_rect(x, y, width, height, 68, dest);
#endif
}

void
skia_draw_overlay_sprite(const sprite_t *sprite, int x, int y, int y_off, frame_t *dest)
{
//    if(1) return;
//	LOGI("skia_draw_overlay_sprite x=%d y=%d, y_off=%d", x, y, y_off);

	size_t width = le16toh(sprite->w);
	size_t height = le16toh(sprite->h);

	x += le16toh(sprite->x);
	y += le16toh(sprite->y);

    std::shared_ptr<SkBitmap> bitmap;
    bitmap_cache_t::iterator it = g_bitmap_cache.find(sprite);

    if( it != g_bitmap_cache.end() ) {
        bitmap = it->second;
    }
    else {
        LOGI("create overlay sprite = %d", sprite);

        // Create bitmap and add to cache
        bitmap.reset(new SkBitmap());
        g_bitmap_cache[sprite] = bitmap;

        bitmap->setConfig(SkBitmap::kA8_Config, width, height);
        bitmap->allocPixels();

        {
            // Fill bitmap with sprite data
            SkAutoLockPixels alp(*bitmap);
            void *data = (uint8_t *)sprite + sizeof(sprite_t);
            gfx_unpack_overlay_sprite(bitmap->getPixels(), data, width * height);
        }

        bitmap->notifyPixelsChanged();
    }

	SkRect src_rect;
	src_rect.fLeft = 0;
	src_rect.fTop = y_off;
	src_rect.fRight = SkIntToScalar(width);
	src_rect.fBottom = - y_off + SkIntToScalar(height);

	SkRect dst_rect;
	dst_rect.fLeft = x;
	dst_rect.fTop = y + y_off;
	dst_rect.fRight = x + SkIntToScalar(width);
	dst_rect.fBottom = y - y_off + SkIntToScalar(height);

	((SkCanvas*)dest->skCanvas)->drawBitmapRectToRect(*bitmap, &src_rect, dst_rect, NULL);

#if 0
	/* Bounding box */
	skia_draw_rect(x, y + y_off, width, height - y_off, 1, dest);
#endif
}

surface_t *
skia_draw_masked_sprite(const sprite_t *sprite, int x, int y, const sprite_t *mask, surface_t *surface, frame_t *dest)
{
//	LOGI("skia_draw_masked_sprite x=%d y=%d", x, y);

	if (dest == NULL) {
		LOGE("skia_draw_masked_sprite: dest is NULL");
		return 0;
	}

	if (dest->skCanvas == NULL) {
		LOGE("skia_draw_masked_sprite: dest->skCanvas is NULL");
		return 0;
	}

	size_t m_width = le16toh(mask->w);
	size_t m_height = le16toh(mask->h);

	size_t s_width = le16toh(sprite->w);
	size_t s_height = le16toh(sprite->h);

    SkAutoMalloc auto_s_copy (m_width * m_height);
    uint8_t* s_copy = static_cast<uint8_t*>(auto_s_copy.get());
    if (s_copy == NULL) {
        LOGE("skia_draw_masked_sprite: out of memory");
        return 0;
    }

    void *s_data = (uint8_t *)sprite + sizeof(sprite_t);

    size_t to_copy = m_width * m_height;
    size_t s_size = s_width * s_height;
    uint8_t *copy_dest = s_copy;
    while (to_copy) {
        size_t s = to_copy < s_size ? to_copy : s_size;
        memcpy(copy_dest, s_data, s * sizeof(uint8_t));  // copy from beginning again
        to_copy -= s;
        copy_dest += s;
    }


    // Mask
	void *m_data = (uint8_t *)mask + sizeof(sprite_t);

	/* Unpack mask */
	size_t unpack_size = m_width * m_height;
    SkAutoMalloc auto_m_unpack (unpack_size);
    uint8_t* m_unpack = static_cast<uint8_t*>(auto_m_unpack.get());
	if (m_unpack == NULL) {
		LOGE("m_unpack is NULL");
		return 0;
	}

	gfx_unpack_mask_sprite(m_unpack, m_data, unpack_size);

	// Fill alpha value from mask data
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			if (m_unpack[y*m_width+x] == 0) {
			    s_copy[y*m_width+x] = 0;
			}
		}
	}

	SkBitmap bitmap;
	bitmap.setConfig(SkBitmap::kIndex8_Config, m_width, m_height);
	bitmap.setPixels(s_copy, g_color_table.get());

	x += le16toh(mask->x);
	y += le16toh(mask->y);

	SkRect src_rect;
	src_rect.fLeft = 0;
	src_rect.fTop = 0;
	src_rect.fRight = SkIntToScalar(m_width);
	src_rect.fBottom = SkIntToScalar(m_height);

	SkRect dst_rect;
	dst_rect.fLeft = x;
	dst_rect.fTop = y;
	dst_rect.fRight = x + SkIntToScalar(m_width);
	dst_rect.fBottom = y + SkIntToScalar(m_height);

	((SkCanvas*)dest->skCanvas)->drawBitmapRectToRect(bitmap, &src_rect, dst_rect, NULL);

	return 0;
}

void
skia_draw_frame(int dx, int dy, frame_t *dest, int sx, int sy, frame_t *src, int w, int h)
{
//	LOGI("skia_draw_frame dx=%d dy=%d sx=%d sy=%d w=%d h=%d", dx, dy, sx, sy, w, h);

	SkRect src_rect;
	src_rect.fLeft = sx;
	src_rect.fTop = sy;
	src_rect.fRight = sx + SkIntToScalar(w);
	src_rect.fBottom = sy + SkIntToScalar(h);

	SkRect dst_rect;
	dst_rect.fLeft = dx;
	dst_rect.fTop = dy;
	dst_rect.fRight = dx + SkIntToScalar(w);
	dst_rect.fBottom = dy + SkIntToScalar(h);

    const SkBitmap & bitmap = ((SkCanvas*)src->skCanvas)->getDevice()->accessBitmap(false);

	((SkCanvas*)dest->skCanvas)->drawBitmapRectToRect(bitmap, &src_rect, dst_rect, NULL);
}

void
skia_draw_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
	LOGI("skia_draw_rect x=%d y=%d w=%d h=%d, color=%d", x, y, width, height, color);

    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(SkIntToScalar(1));
    paint.setColor((*g_color_table)[color]);
    ((SkCanvas*)dest->skCanvas)->drawRectCoords(SkIntToScalar(x), SkIntToScalar(y), SkIntToScalar(x+width), SkIntToScalar(y+height), paint);
}

void
skia_fill_rect(int x, int y, int width, int height, int color, frame_t *dest)
{
	LOGI("skia_fill_rect x=%d y=%d w=%d h=%d", x, y, width, height);

    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor((*g_color_table)[color]);
    ((SkCanvas*)dest->skCanvas)->drawRectCoords(SkIntToScalar(x), SkIntToScalar(y), SkIntToScalar(x+width), SkIntToScalar(y+height), paint);
}

void
skia_mark_dirty(int x, int y, int width, int height)
{
	// no implementation needed
}

void
skia_swap_buffers()
{
	// no implementation needed
}

void
skia_set_palette(const uint8_t *palette)
{
	LOGI("skia_set_palette");

    if(!g_color_table) {
        g_color_table.reset(new SkColorTable(256));
    }

    SkPMColor *color_table = g_color_table->lockColors();
	color_table[0] = SkPreMultiplyARGB(0, 0, 0, 0);
	for (int i = 1; i < 256; i++) {
		color_table[i] = SkPreMultiplyARGB(255, palette[i*3], palette[i*3+1], palette[i*3+2]);
	}
    g_color_table->unlockColors(true);
}

