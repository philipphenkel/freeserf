/* viewport.h */

#ifndef _VIEWPORT_H
#define _VIEWPORT_H


#include "renderer.h"
#include "map.h"


typedef enum {
	VIEWPORT_LAYER_LANDSCAPE = 1<<0,
	VIEWPORT_LAYER_PATHS = 1<<1,
	VIEWPORT_LAYER_OBJECTS = 1<<2,
	VIEWPORT_LAYER_SERFS = 1<<3,
	VIEWPORT_LAYER_CURSOR = 1<<4,
	VIEWPORT_LAYER_ALL = (1<<5)-1,

	VIEWPORT_LAYER_GRID = 1<<5
} viewport_layer_t;

typedef struct {
	int x, y;
	int width, height;
	int offset_x, offset_y;
	viewport_layer_t layers;
} viewport_t;


void viewport_draw(viewport_t *viewport, frame_t *frame);
void viewport_move_to_map_pos(viewport_t *viewport, int col, int row);
void viewport_move_by_pixels(viewport_t *viewport, int x, int y);
void viewport_get_current_map_pos(viewport_t *viewport, int *col, int *row);

void viewport_screen_pix_from_map_pix(viewport_t *viewport, int mx, int my, int *sx, int *sy);
void viewport_map_pix_from_map_coord(viewport_t *viewport, int col, int row, int h, int *mx, int *my);
map_pos_t viewport_map_pos_from_screen_pix(viewport_t *viewport, int x, int y);

void viewport_redraw_map_pos(map_pos_t pos);

frame_t *viewport_get_landscape_frame();

#endif /* ! _VIEWPORT_H */
