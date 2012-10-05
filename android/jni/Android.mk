LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Freeserf

LOCAL_C_INCLUDES :=

LOCAL_SRC_FILES := \
	../../src/building.c \
	../../src/flag.c \
	../../src/gfx.c \
	../../src/list.c \
	../../src/map.c \
	../../src/player.c \
	../../src/random.c \
	../../src/renderer_null.c \
	../../src/renderer.c \
	../../src/serf.c \
	../../src/version.c \
	../../src/viewport.c \
	../../src/core.c \

#LOCAL_STATIC_LIBRARIES := SDL

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog
LOCAL_CFLAGS := -std=c99 -DNULL_RENDERER
#LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)
