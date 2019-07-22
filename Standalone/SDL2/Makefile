ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

ifeq ($(detected_OS),Windows)
CORE=$(wildcard ./src/core/windows/*.c)
MAIN=$(wildcard ./src/main/windows/*.c)
PLATVID=$(wildcard ./src/video/windows/*.c)
PLATTHREAD=$(wildcard ./src/thread/windows/*.c)
PLATPOWER=$(wildcard ./src/power/windows/*.c)
PLATTIMER=$(wildcard ./src/timer/windows/*.c)
PLATFS=$(wildcard ./src/filesystem/windows/*.c)
PLATSENSOR=$(wildcard ./src/sensor/windows/*.c)
PLATAUDIO= ./src/audio/winmm/SDL_winmm.c ./src/audio/directsound/SDL_directsound.c ./src/audio/wasapi/SDL_wasapi.c ./src/audio/wasapi/SDL_wasapi_win32.c
LOADSO=$(wildcard ./src/loadso/windows/*.c)
HIDAPI=$(wildcard ./src/hidapi/windows/*.c)
JOYSTICKHIDAPI=$(wildcard ./src/joystick/hidapi/*.c)
PLATHAPTIC=$(wildcard ./src/haptic/windows/*.c)
PLATJOYSTICK=$(wildcard ./src/joystick/windows/*.c)
DEFINES += -D__WIN32__
endif

ifeq ($(detected_OS),Linux)
endif

SDL=$(wildcard ./src/*.c)
ATOMIC=$(wildcard ./src/atomic/*.c)
FILE=$(wildcard ./src/file/*.c)
DYNAPI=$(wildcard ./src/dynapi/*.c)
CPUINFO=$(wildcard ./src/cpuinfo/*.c)
AUDIO=$(wildcard ./src/audio/*.c)
DISKAUDIO=$(wildcard ./src/audio/disk/*.c)
AUDIODUMMY=$(wildcard ./src/audio/dummy/*.c)
JOYSTICK=$(wildcard ./src/joystick/*.c)
JOYSTICKDUMMY=$(wildcard ./src/joystick/dummy/*.c)
EVENTS=$(wildcard ./src/events/*.c)
LIBM=$(wildcard ./src/libm/*.c)
RENDER=$(wildcard ./src/render/*.c)
DIRECTX=$(wildcard ./src/render/direct3d/*.c)
DIRECTX11=$(wildcard ./src/render/direct3d11/*.c)
OPENGL=$(wildcard ./src/render/opengl/*.c)
GLES=$(wildcard ./src/render/opengles/*.c)
GLES2=$(wildcard ./src/render/opengles2/*.c)
PSP=$(wildcard ./src/render/psp/*.c)
RENSOFT=$(wildcard ./src/render/software/*.c)
STDLIB=$(wildcard ./src/stdlib/*.c)
THREAD=$(wildcard ./src/thread/*.c)
TIMER=$(wildcard ./src/timer/*.c)
VID=$(wildcard ./src/video/*.c)
VIDDUMMY=$(wildcard ./src/video/dummy/*.c)
YUV2RGB=$(wildcard ./src/video/yuv2rgb/*.c)
HAPTIC=$(wildcard ./src/haptic/*.c)
HAPTICDUMMY=$(wildcard ./src/haptic/dummy/*.c)
SENSOR=$(wildcard ./src/sensor/*.c)
POWER=$(wildcard ./src/power/*.c)
SENSORDUMMY=$(wildcard ./src/sensor/dummy/*.c)
THREADGENERIC=$(wildcard ./src/thread/generic/*.c)

OBJS=$(SDL:.c=.o)
OBJS2=$(ATOMIC:.c=.o)
OBJS3=$(FILE:.c=.o)
OBJS4=$(DYNAPI:.c=.o)
OBJS5=$(CPUINFO:.c=.o)
OBJS6=$(EVENTS:.c=.o)
OBJS7=$(LIBM:.c=.o)
OBJS8=$(RENDER:.c=.o)
OBJS9=$(DIRECTX11:.c=.o)
OBJS10=$(OPENGL:.c=.o)
OBJS11=$(GLES:.c=.o)
OBJS12=$(GLES2:.c=.o)
OBJS13=$(PSP:.c=.o)
OBJS14=$(RENSOFT:.c=.o)
OBJS16=$(STDLIB:.c=.o)
OBJS17=$(THREAD:.c=.o)
OBJS18=$(TIMER:.c=.o)
OBJS19=$(VID:.c=.o)
OBJS20=$(VIDDUMMY:.c=.o)
OBJS21=$(YUV2RGB:.c=.o)
OBJS22=$(HAPTIC:.c=.o)
OBJS23=$(SENSOR:.c=.o)
OBJS24=$(POWER:.c=.o)
OBJS33=$(AUDIO:.c=.o)
OBJS36=$(JOYSTICK:.c=.o)
OBJS38=$(SENSORDUMMY:.c=.o)
OBJS39=$(THREADGENERIC:.c=.o)
OBJS40=$(JOYSTICKDUMMY:.c=.o)
OBJS41=$(HAPTICDUMMY:.c=.o)
OBJS42=$(DIRECTX:.c=.o)
OBJS44=$(DISKAUDIO:.c=.o)
OBJS46=$(AUDIODUMMY:.c=.o)

OBJS25=$(CORE:.c=.o)
OBJS32=$(MAIN:.c=.o)
OBJS26=$(PLATVID:.c=.o)
OBJS27=$(PLATTHREAD:.c=.o)
OBJS28=$(PLATPOWER:.c=.o)
OBJS29=$(PLATTIMER:.c=.o)
OBJS30=$(PLATFS:.c=.o)
OBJS31=$(PLATSENSOR:.c=.o)
OBJS34=$(PLATAUDIO:.c=.o)
OBJS35=$(LOADSO:.c=.o)
OBJS37=$(HIDAPI:.c=.o)
OBJS43=$(PLATHAPTIC:.c=.o)
OBJS45=$(PLATJOYSTICK:.c=.o)
OBJS47=$(JOYSTICKHIDAPI:.c=.o)

CC = gcc
LIBNAME = sdl
LIBPATH = ../lib
FF = -mfpmath=387 -msse -m3dnow -mmmx -Wshadow -fvisibility=hidden -Wdeclaration-after-statement -Werror=declaration-after-statement -fno-strict-aliasing
CFLAGS = $(SHARED_FLAGS) -w -fPIC -I.. -I ./include  -I ./src/video/khronos $(FF) $(DEFINES)

OBJSS = $(OBJS) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11)
OBJSS += $(OBJS12) $(OBJS13) $(OBJS14) $(OBJS15) $(OBJS16) $(OBJS17) $(OBJS18) $(OBJS19) $(OBJS20) $(OBJS21)
OBJSS += $(OBJS22) $(OBJS23) $(OBJS24) $(OBJS25) $(OBJS26) $(OBJS27) $(OBJS28) $(OBJS29) $(OBJS30) $(OBJS31)
OBJSS += $(OBJS32) $(OBJS33) $(OBJS34) $(OBJS35) $(OBJS36) $(OBJS37) $(OBJS38) $(OBJS39) $(OBJS40) $(OBJS41)
OBJSS += $(OBJS42) $(OBJS43) $(OBJS44) $(OBJS45) $(OBJS46) $(OBJS47)

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJSS) lib$(LIBNAME).a
	rm -rf ../lib/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJSS)
	ar -cvr lib$(LIBNAME).a $(OBJSS)
	gcc-ranlib lib$(LIBNAME).a

.PHONY $(LIBPATH):
	mkdir -p ../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../lib