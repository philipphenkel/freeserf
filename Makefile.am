
# freeserf
bin_PROGRAMS = freeserf

freeserf_SOURCES = \
	src/freeserf.c \
	src/core.c \
	src/globals.h \
	src/serf.c src/serf.h \
	src/flag.c src/flag.h \
	src/building.c src/building.h \
	src/random.c src/random.h \
	src/gfx.c src/gfx.h \
	src/viewport.c src/viewport.h \
	src/map.c src/map.h \
	src/player.c src/player.h \
	src/renderer.c src/renderer.h \
	src/renderer_sdl.c \
	src/list.c src/list.h \
	src/log.h \
	src/misc.h \
	src/debug.h \
	src/freeserf_endian.h \
	src/version.c src/version.h src/version_git.h

AM_CFLAGS = -DSDL_RENDERER $(SDL_CFLAGS) -I$(top_builddir)/src
freeserf_LDADD = $(SDL_LIBS) $(SDL_CFLAGS) -lm

VCS_VERSION_FILE = src/version_vcs.h

CLEANFILES = $(VCS_VERSION_FILE)


all: gitversion

.PHONY: gitversion
gitversion:
	@( FREESERF_VERSION="" ; \
	[ -e "$(VCS_VERSION_FILE)" ] && FREESERF_VERSION=$$(grep "define VERSION_VCS " "$(VCS_VERSION_FILE)" | cut -f3 -d" " | sed 's/"//g') ; \
	VCS_VERSION=$$(git describe --always --tags --dirty) ; \
	echo "FREESERF_VERSION = $$FREESERF_VERSION" ; \
	echo "VCS_VERSION = $$VCS_VERSION" ; \
	if [ "$${VCS_VERSION}" != "$${FREESERF_VERSION}" ] ; then \
		$(MKDIR_P) $(@D) ; \
		echo "Updating version file." ; \
		printf "#ifndef VERSION_VCS_H\n#define VERSION_VCS_H\n\n#define VERSION_VCS \"$${VCS_VERSION}\"\n\n#endif /* VERSION_VCS_H */\n" > $(VCS_VERSION_FILE) ; \
	fi )
