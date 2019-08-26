BINDIR = bin
OBJDIR = obj
COMDIR = ../common
OUTPUT = BasicOpenGLWindowPolygons.elf

INCDIRS = $(COMDIR)/include
LIBDIRS =
LIBS = SDL2 GL dl stdc++fs

CXX = g++-8
CXXFLAGS = -no-pie -std=c++17 -Wall -Werror -Wfatal-errors -pthread `sdl2-config --cflags`

release: OPTIMISATION := -g0 -O3
release: $(OUTPUT)

debug: OPTIMISATION := -g3 -O0
debug: $(OUTPUT)

.PHONY: $(OUTPUT)

SOURCE = src/main.cpp $(GL_GL3W_O) $(LAK_OPENGL_SHADER_O) $(LAK_OPENGL_STATE_O)

include $(COMDIR)/common.mk
