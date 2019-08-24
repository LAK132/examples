BINDIR = bin
OBJDIR = obj

CXX = g++-8

BINARY = BasicOpenGLWindow.elf
SOURCE = src/main.cpp

COMMDIR = ../common
INCDIRS = $(COMMDIR)/include
LIBDIRS =
LIBS = SDL2 GL dl stdc++fs

release: OPTIMISATION := -g0 -O3
release: $(BINARY)

debug: OPTIMISATION := -g3 -O0
debug: $(BINARY)

CXXFLAGS = $(OPTIMISATION) -no-pie -std=c++17 -Wall -Werror -Wfatal-errors -pthread `sdl2-config --cflags`

$(BINARY): | $(BINDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(BINARY) $(SOURCE) $(foreach D,$(INCDIRS),-I$D ) $(foreach D,$(LIBDIRS),-L$D ) $(foreach L,$(LIBS),-l$L )

.PHONY: $(BINARY)

clean:
	rm -f $(BINDIR)/*
	rm -f $(OBJDIR)/*

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
