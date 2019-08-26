GL_GL3W_H = $(COMDIR)/include/GL/gl3w.h
GL_GL3W_C = $(COMDIR)/include/GL/gl3w.c
GL_GL3W_O = $(OBJDIR)/gl_gl3w.o

LAK_OPENGL_SHADER_CPP = $(COMDIR)/include/lak/opengl/shader.cpp
LAK_OPENGL_SHADER_HPP = $(COMDIR)/include/lak/opengl/shader.hpp
LAK_OPENGL_SHADER_O = $(OBJDIR)/lak_opengl_shader.o

LAK_OPENGL_STATE_HPP = $(COMDIR)/include/lak/opengl/state.hpp
LAK_OPENGL_STATE_CPP = $(COMDIR)/include/lak/opengl/state.cpp
LAK_OPENGL_STATE_O = $(COMDIR)/include/lak/opengl/state.o

$(GL_GL3W_O): $(GL_GL3W_C) $(GL_GL3W_H) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCFLAGS)

$(LAK_OPENGL_SHADER_O): $(LAK_OPENGL_SHADER_CPP) $(LAK_OPENGL_SHADER_HPP) $(LAK_OPENGL_STATE_O) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCFLAGS)

$(LAK_OPENGL_STATE_O): $(LAK_OPENGL_STATE_CPP) $(LAK_OPENGL_STATE_HPP) $(GL_GL3W_O) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCFLAGS)

clean:
	rm -f $(BINDIR)/*
	rm -f $(OBJDIR)/*

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)



INCFLAGS = $(foreach D,$(INCDIRS),-I$D )$(foreach D,$(LIBDIRS),-L$D )$(foreach L,$(LIBS),-l$L )

$(OUTPUT): $(SOURCE) | $(BINDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OPTIMISATION) -o $(BINDIR)/$(OUTPUT) $(SOURCE) $(INCFLAGS)
