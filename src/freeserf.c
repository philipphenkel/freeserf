#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <assert.h>

#include "SDL.h"

#include "core.h"
#include "freeserf_endian.h"
#include "globals.h"
#include "random.h"
#include "serf.h"
#include "flag.h"
#include "building.h"
#include "player.h"
#include "map.h"
#include "viewport.h"
#include "gfx.h"
#include "data.h"
#include "renderer.h"
#include "misc.h"
#include "debug.h"
#include "log.h"


/* TODO This file is one big of mess of all the things that should really
   be separated out into smaller files.  */

#define DEFAULT_SCREEN_WIDTH  800
#define DEFAULT_SCREEN_HEIGHT 600


static unsigned int tick;

static char *game_file = NULL;


/* Might be useful for testing. */
static int
wait_for_lmb_or_timeout()
{
	for (int i = 0; i < 214; i++) {
		SDL_Event event;
		SDL_PumpEvents();
		int r = SDL_PeepEvents(&event, 1, SDL_GETEVENT,
				       SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN));
		if (r < 0) {
			LOGE("PeepEvents error: %s.", SDL_GetError());
			exit(EXIT_FAILURE);
		} else if (r > 0) {
			return 1;
		}

		SDL_Delay(20);
	}

	return 0;
}


/* update_player_input is split into a click handler and a drag handler.
   x and y are absolute instead of relative coords.
   lmb and rmb are boolean (i.e. zero is false, non-zero is true),
   instead of 0 is true, -1 is false. */
static void
update_player_input_drag(player_t *player, int x, int y, int lmb, int rmb)
{
	if (rmb) {
		if (/*fast mapclick*/0) {
			/* TODO fast mapclick */
		} else {
			player->click |= BIT(3); /* set right click pending */
		}

		int dx = x - player->pointer_x;
		int dy = y - player->pointer_y;
		if (dx != 0 || dy != 0) {
			/* TODO drag accelerates in linux */
			viewport_move_by_pixels(get_viewport(), dx, dy);
			SDL_WarpMouse(player->pointer_x, player->pointer_y);
		}
	} else {
		player->click &= ~BIT(3); /* reset right click pending */

		if (x != player->pointer_x || y != player->pointer_y) {
			player->pointer_x = min(max(0, x), player->pointer_x_max);
			player->pointer_y = min(max(0, y), player->pointer_y_max);
		}
	}

	/* TODO more lmb(?) */
}

static void
update_player_input_click(player_t *player, int x, int y, int lmb, int rmb, int lmb_dbl_time)
{
	/*if (player->field_9E) {
		player->field_9E -= 1;
		if (player->field_9E == 0) player->click &= ~BIT(4);
	}*/

	player->pointer_x = min(max(0, x), player->pointer_x_max);
	player->pointer_y = min(max(0, y), player->pointer_y_max);

	if (lmb) {
		/* TODO ... */
		player->flags &= ~BIT(3);
		player->flags |= BIT(2);
		/* player->field_8A = 0; */
		player->pointer_x_clk = player->pointer_x;
		player->pointer_y_clk = player->pointer_y;
	} else {
		player->flags |= BIT(3);
	}

	/* Handle double click as special click */
	if (lmb && lmb_dbl_time < 600) player->click |= BIT(3);
	else player->click &= ~BIT(3);
}


/* The length of a game tick in miliseconds. */
#define TICK_LENGTH  20


/* game_loop() has been turned into a SDL based loop.
   The code for one iteration of the original game_loop is
   in game_loop_iter. */
static void
game_loop()
{
    LOGI("game_loop");
	/* FPS */
	int fps = 0;
	int fps_ema = 0;
	int fps_target = 100;
	const float ema_alpha = 0.003;

	int lmb_state = 0;
	int rmb_state = 0;
	unsigned int last_lmb = 0;

	unsigned int current_ticks = SDL_GetTicks();
	unsigned int accum = 0;
	unsigned int accum_frames = 0;

	int do_loop = 1;
	SDL_Event event;
	while (do_loop) {
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) lmb_state = 0;
				else if (event.button.button == SDL_BUTTON_RIGHT) rmb_state = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) lmb_state = 1;
				else if (event.button.button == SDL_BUTTON_RIGHT) rmb_state = 1;
				update_player_input_click(globals.player[0], event.button.x, event.button.y, lmb_state, rmb_state, current_ticks - last_lmb);
				if (event.button.button == SDL_BUTTON_LEFT) last_lmb = current_ticks;
				break;
			case SDL_MOUSEMOTION:
				update_player_input_drag(globals.player[0], event.motion.x, event.motion.y,
							 event.motion.state & SDL_BUTTON(1), event.motion.state & SDL_BUTTON(3));
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_q &&
				    (event.key.keysym.mod & KMOD_CTRL)) {
					do_loop = 0;
					break;
				}

				switch (event.key.keysym.sym) {
					/* Map scroll */
				case SDLK_UP:
					viewport_move_by_pixels(get_viewport(), 0, -1);
					break;
				case SDLK_DOWN:
					viewport_move_by_pixels(get_viewport(), 0, 1);
					break;
				case SDLK_LEFT:
					viewport_move_by_pixels(get_viewport(), -1, 0);
					break;
				case SDLK_RIGHT:
					viewport_move_by_pixels(get_viewport(), 1, 0);
					break;

					/* Panel click shortcuts */
				case SDLK_1:
					handle_panel_btn_click(globals.player[0], 0);
					break;
				case SDLK_2:
					handle_panel_btn_click(globals.player[0], 1);
					break;
				case SDLK_3:
					handle_panel_btn_click(globals.player[0], 2);
					break;
				case SDLK_4:
					handle_panel_btn_click(globals.player[0], 3);
					break;
				case SDLK_5:
					handle_panel_btn_click(globals.player[0], 4);
					break;

					/* Game speed */
				case SDLK_PLUS:
				case SDLK_KP_PLUS:
					if (globals.game_speed < 0xffff0000) globals.game_speed += 0x10000;
					LOGI("Game speed: %u", globals.game_speed >> 16);
					break;
				case SDLK_MINUS:
				case SDLK_KP_MINUS:
					if (globals.game_speed >= 0x10000) globals.game_speed -= 0x10000;
					LOGI("Game speed: %u", globals.game_speed >> 16);
					break;
				case SDLK_0:
					globals.game_speed = 0x20000;
					LOGI("Game speed: %u", globals.game_speed >> 16);
					break;
				case SDLK_p:
					if (globals.game_speed == 0) game_unpause();
					else game_pause();
					break;

					/* Misc */
				case SDLK_g:
					get_viewport()->layers ^= VIEWPORT_LAYER_GRID;
					break;

				default:
					break;
				}
				break;
			case SDL_QUIT:
				do_loop = 0;
				break;
			}
		}

		unsigned int new_ticks = SDL_GetTicks();
		int delta_ticks = new_ticks - current_ticks;
		current_ticks = new_ticks;

		accum += delta_ticks;
		while (accum >= TICK_LENGTH) {
			/* This is main_timer_cb */
			tick += 1;

			/* In original, deep_tree is called which will call update_game.
			   Here, update_game is just called directly. */
			update_game();

			/* FPS */
			fps = 1000*((float)accum_frames / accum);
			if (fps_ema > 0) fps_ema = ema_alpha*fps + (1-ema_alpha)*fps_ema;
			else if (fps > 0) fps_ema = fps;

			accum -= TICK_LENGTH;
			accum_frames = 0;
		}

		game_loop_iter();
#if 0
		draw_green_string(6, 10, renderer_get_screen_frame(), "FPS");
		draw_green_number(10, 10, renderer_get_screen_frame(), fps_ema);
#endif

		accum_frames += 1;

		/* Reduce framerate to target */
		if (fps_target > 0) {
			int delay = 0;
			if (fps_ema > 0) delay = (1000/fps_target) - (1000/fps_ema);
			if (delay > 0) SDL_Delay(delay);
		}
	}
}


typedef struct {
	void *data;
	size_t size;
	size_t played;
} audio_clip_t;

static void
audio_request_data_cb(void *user, uint8_t *stream, int len)
{
	audio_clip_t *clip = user;
	size_t left = clip->size - clip->played;

	memset(stream, '\0', len);

	if (left > 0) {
		memcpy(stream, ((uint8_t *)clip->data) + clip->played, min(left, len));
		clip->played += min(left, len);
	}
}

audio_clip_t audio_clip;

/* Initialize audio (toy implementation). */
static int
sdl_audio_init()
{
	audio_clip.data = NULL;
	audio_clip.size = 0;
	audio_clip.played = 0;

	SDL_AudioSpec desired = {
		.freq = 8000,
		.format = AUDIO_S8,
		.channels = 1,
		.samples = 256,
		.userdata = &audio_clip,
		.callback = audio_request_data_cb
	};

	int r = SDL_OpenAudio(&desired, NULL);
	if (r < 0) {
		LOGE("Could not open audio device: %s\n", SDL_GetError());
		return -1;
	}

	SDL_PauseAudio(0);

	return 0;
}

static void
sdl_audio_play_sound(int index)
{
	SDL_LockAudio();

	audio_clip.data = gfx_get_data_object(DATA_SFX_BASE + index, &audio_clip.size);
	audio_clip.played = 0;

	SDL_UnlockAudio();
}


#define USAGE					\
	"Usage: %s [-g DATA-FILE]\n"
#define HELP							\
	USAGE							\
	" -f\t\tFullscreen mode (CTRL-q to exit)\n"		\
	" -g DATA-FILE\tUse specified data file\n"		\
	" -h\t\tShow this help text\n"				\
	" -l FILE\tLoad saved game\n"				\
	" -m MAP\t\tSelect world map (1-3)\n"			\
	" -p\t\tPreserve map bugs of the original game\n"	\
	" -r RES\t\tSet display resolution (e.g. 800x600)\n"	\
	" -t GEN\t\tMap generator (0 or 1)\n"

int
main(int argc, char *argv[])
{
	int r;

	char *data_file = NULL;

	int screen_width = DEFAULT_SCREEN_WIDTH;
	int screen_height = DEFAULT_SCREEN_HEIGHT;
	int fullscreen = 0;
	int game_map = 1;
	int map_generator = 0;
	int preserve_map_bugs = 0;

	int opt;
	while (1) {
		opt = getopt(argc, argv, "fg:hl:m:pr:t:");
		if (opt < 0) break;

		switch (opt) {
		case 'f':
			fullscreen = 1;
			break;
		case 'g':
			data_file = malloc(strlen(optarg)+1);
			if (data_file == NULL) exit(EXIT_FAILURE);
			strcpy(data_file, optarg);
			break;
		case 'h':
			LOGI(HELP, argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'l':
			game_file = malloc(strlen(optarg)+1);
			if (game_file == NULL) exit(EXIT_FAILURE);
			strcpy(game_file, optarg);
			break;
		case 'm':
			game_map = atoi(optarg);
			break;
		case 'p':
			preserve_map_bugs = 1;
			break;
		case 'r':
		{
			char *hstr = strchr(optarg, 'x');
			if (hstr == NULL) LOGI(USAGE, argv[0]);
			screen_width = atoi(optarg);
			screen_height = atoi(hstr+1);
		}
			break;
		case 't':
			map_generator = atoi(optarg);
			break;
		default:
			LOGE(USAGE, argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	/* Use default data file if none was specified. */
	if (data_file == NULL) {
		data_file = malloc(strlen("SPAE.PA")+1);
		if (data_file == NULL) exit(EXIT_FAILURE);
		strcpy(data_file, "SPAE.PA");
	}

	LOGE("Loading game data from `%s'...", data_file);

	r = gfx_load_file(data_file);
	if (r < 0) {
		LOGE("Could not load game data.");
		exit(EXIT_FAILURE);
	}

	free(data_file);

	gfx_data_fixup();

	LOGE("SDL init...");

	r = renderer_init();
	if (r < 0) exit(EXIT_FAILURE);

	/*gfx_set_palette(DATA_PALETTE_INTRO);*/
	gfx_set_palette(DATA_PALETTE_GAME);

	LOGE("SDL resolution %ix%i...\n", screen_width, screen_height);

	r = renderer_set_resolution(screen_width, screen_height, fullscreen);
	if (r < 0) exit(EXIT_FAILURE);

	globals.svga |= BIT(7); /* set svga mode */

	globals.mission_level = game_map - 1; /* set game map */
	globals.map_generator = map_generator;
	globals.map_preserve_bugs = preserve_map_bugs;

	/*sdl_audio_init();*/

	deep_tree(game_file);
	game_loop();

	/* Clean up */
	renderer_deinit();
	gfx_unload();

	return EXIT_SUCCESS;
}
