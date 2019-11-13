BINDIR = bin
OBJDIR = obj
COMDIR = ../common
OUTPUT = BasicSoftwareWindow.elf

INCDIRS = $(COMDIR)/include
LIBDIRS =
LIBS = SDL2 dl stdc++fs

CXX = g++-8
CXXFLAGS = -no-pie -std=c++17 -Wall -Werror -Wfatal-errors -pthread `sdl2-config --cflags`

release: OPTIMISATIONS := -g0 -O3
release: $(OUTPUT)

debug: OPTIMISATIONS := -g3 -O0
debug: $(OUTPUT)

.PHONY: $(OUTPUT)

SOURCE = src/main.cpp

include $(COMDIR)/common.mk