BINDIR = bin
OBJDIR = obj

CXX = g++-8

OUTPUT = BasicOpenGLWindow.elf

release: OPTIMISATION := -g0 -O3
release: $(OUTPUT)

debug: OPTIMISATION := -g3 -O0
debug: $(OUTPUT)

CXXFLAGS = $(OPTIMISATION) -no-pie -std=c++17 -Wall -Werror -Wfatal-errors -pthread `sdl2-config --cflags`

COMMDIR = ../common
INCDIRS = $(COMMDIR)/include
LIBDIRS = $(BINDIR)
LIBS = SDL2 GL dl stdc++fs

$(OUTPUT): | $(BINDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(OUTPUT) src/main.cpp $(foreach D,$(INCDIRS),-I$D ) $(foreach D,$(LIBDIRS),-L$D ) $(foreach L,$(LIBS),-l$L )

.PHONY: $(OUTPUT)

clean:
	rm -f $(BINDIR)/*
	rm -f $(OBJDIR)/*

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
