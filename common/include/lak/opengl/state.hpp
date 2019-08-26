/*
MIT License

Copyright (c) 2019 LAK132

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef LAK_OPENGL_STATE_HPP
#define LAK_OPENGL_STATE_HPP

#include <GL/gl3w.h>

#include <array>

namespace lak
{
  namespace opengl
  {
    /* --- Enable / Disable --- */

    GLboolean enabled(GLenum target);

    void enable(GLenum target, GLboolean enabled);

    /* --- GLboolean --- */

    GLboolean get_bool(GLenum target);

    template <size_t S>
    std::array<GLboolean, S> get_bools(GLenum target)
    {
      std::array<GLboolean, S> result;
      glGetBooleanv(target, result.data());
      return result;
    }

    GLboolean get_shader_bool(GLuint shader, GLenum target);

    GLboolean get_program_bool(GLuint program, GLenum target);

    /* --- GLint --- */

    GLint get_int(GLenum target);

    template <size_t S>
    std::array<GLint, S> get_ints(GLenum target)
    {
      std::array<GLint, S> result;
      glGetIntegerv(target, result.data());
      return result;
    }

    GLint get_shader_int(GLuint shader, GLenum target);

    GLint get_program_int(GLuint program, GLenum target);

    /* -- GLuint --- */

    GLint get_uint(GLenum target);

    template <size_t S>
    std::array<GLuint, S> get_uints(GLenum target)
    {
      std::array<GLuint, S> result;
      glGetIntegerv(target, (GLint*)result.data());
      return result;
    }

    GLuint get_shader_uint(GLuint shader, GLenum target);

    GLuint get_program_uint(GLuint program, GLenum target);

    /* --- GLenum --- */

    GLenum get_enum(GLenum target);

    template <size_t S>
    std::array<GLenum, S> get_enums(GLenum target)
    {
      std::array<GLenum, S> result;
      glGetIntegerv(target, (GLint*)result.data());
      return result;
    }

    GLenum get_shader_enum(GLuint shader, GLenum target);

    GLenum get_program_enum(GLuint program, GLenum target);

    /* --- GLint64 --- */

    GLint64 get_int64(GLenum target);

    template <size_t S>
    std::array<GLint64, S> get_int64s(GLenum target)
    {
      std::array<GLint64, S> result;
      glGetInteger64v(target, result.data());
      return result;
    }

    /* --- GLuint64 --- */

    GLuint64 get_uint64(GLenum target);

    template <size_t S>
    std::array<GLuint64, S> get_uint64s(GLenum target)
    {
      std::array<GLuint64, S> result;
      glGetInteger64v(target, (GLint64*)result.data());
      return result;
    }

    /* --- GLfloat --- */

    GLfloat get_float(GLenum target);

    template <size_t S>
    std::array<GLfloat, S> get_floats(GLenum target)
    {
      std::array<GLfloat, S> result;
      glGetFloatv(target, result.data());
      return result;
    }

    /* --- GLdouble --- */

    GLdouble get_double(GLenum target);

    template <size_t S>
    std::array<GLdouble, S> get_doubles(GLenum target)
    {
      std::array<GLdouble, S> result;
      glGetDoublev(target, result.data());
      return result;
    }
  }
}

#endif // LAK_OPENGL_STATE_HPP