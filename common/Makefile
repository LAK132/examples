
GL_GL3W_H = $(COMMDIR)/include/GL/gl3w.h
GL_GL3W_C = $(COMMDIR)/include/GL/gl3w.c
GL_GL3W_O = $(OBJDIR)/gl_gl3w.o

LAK_OPENGL_SHADER_CPP = $(COMMDIR)/include/lak/opengl/shader.cpp
LAK_OPENGL_SHADER_HPP = $(COMMDIR)/include/lak/opengl/shader.hpp
LAK_OPENGL_SHADER_O = $(OBJDIR)/lak_opengl_shader.o

LAK_OPENGL_STATE_HPP = $(COMMDIR)/include/lak/opengl/state.hpp

$(GL_GL3W_O): $(GL_GL3W_C) $(GL_GL3W_H) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCFLAGS)

$(LAK_OPENGL_SHADER_O): $(LAK_OPENGL_SHADER_CPP) $(LAK_OPENGL_SHADER_HPP) $(LAK_OPENGL_STATE_HPP) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCFLAGS)
