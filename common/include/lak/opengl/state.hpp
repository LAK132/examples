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

    inline GLboolean enabled(GLenum target)
    {
      return glIsEnabled(target);
    }

    inline void enable(GLenum target, GLboolean enabled)
    {
      if (enabled)  glEnable(target);
      else          glDisable(target);
    }

    /* --- GLboolean --- */

    inline GLboolean get_bool(GLenum target)
    {
      GLboolean result;
      glGetBooleanv(target, &result);
      return result;
    }

    template <size_t S>
    inline std::array<GLboolean, S> get_bools(GLenum target)
    {
      std::array<GLboolean, S> result;
      glGetBooleanv(target, result.data());
      return result;
    }

    inline GLboolean get_shader_bool(GLuint shader, GLenum target)
    {
      GLboolean result;
      glGetShaderiv(shader, target, (GLint*)&result);
      return result;
    }

    inline GLboolean get_program_bool(GLuint program, GLenum target)
    {
      GLboolean result;
      glGetProgramiv(program, target, (GLint*)&result);
      return result;
    }

    /* --- GLint --- */

    inline GLint get_int(GLenum target)
    {
      GLint result;
      glGetIntegerv(target, &result);
      return result;
    }

    template <size_t S>
    inline std::array<GLint, S> get_ints(GLenum target)
    {
      std::array<GLint, S> result;
      glGetIntegerv(target, result.data());
      return result;
    }

    inline GLint get_shader_int(GLuint shader, GLenum target)
    {
      GLint result;
      glGetShaderiv(shader, target, &result);
      return result;
    }

    inline GLint get_program_int(GLuint program, GLenum target)
    {
      GLint result;
      glGetProgramiv(program, target, &result);
      return result;
    }

    /* -- GLuint --- */

    inline GLint get_uint(GLenum target)
    {
      GLuint result;
      glGetIntegerv(target, (GLint*)&result);
      return result;
    }

    template <size_t S>
    inline std::array<GLuint, S> get_uints(GLenum target)
    {
      std::array<GLuint, S> result;
      glGetIntegerv(target, (GLint*)result.data());
      return result;
    }

    inline GLuint get_shader_uint(GLuint shader, GLenum target)
    {
      GLuint result;
      glGetShaderiv(shader, target, (GLint*)&result);
      return result;
    }

    inline GLuint get_program_uint(GLuint program, GLenum target)
    {
      GLuint result;
      glGetProgramiv(program, target, (GLint*)&result);
      return result;
    }

    /* --- GLenum --- */

    inline GLenum get_enum(GLenum target)
    {
      GLenum result;
      glGetIntegerv(target, (GLint*)&result);
      return result;
    }

    template <size_t S>
    inline std::array<GLenum, S> get_enums(GLenum target)
    {
      std::array<GLenum, S> result;
      glGetIntegerv(target, (GLint*)result.data());
      return result;
    }

    inline GLenum get_shader_enum(GLuint shader, GLenum target)
    {
      GLenum result;
      glGetShaderiv(shader, target, (GLint*)&result);
      return result;
    }

    inline GLenum get_program_enum(GLuint program, GLenum target)
    {
      GLenum result;
      glGetProgramiv(program, target, (GLint*)&result);
      return result;
    }

    /* --- GLint64 --- */

    inline GLint64 get_int64(GLenum target)
    {
      GLint64 result;
      glGetInteger64v(target, &result);
      return result;
    }

    template <size_t S>
    inline std::array<GLint64, S> get_int64s(GLenum target)
    {
      std::array<GLint64, S> result;
      glGetInteger64v(target, result.data());
      return result;
    }

    /* --- GLuint64 --- */

    inline GLuint64 get_uint64(GLenum target)
    {
      GLuint64 result;
      glGetInteger64v(target, (GLint64*)&result);
      return result;
    }

    template <size_t S>
    inline std::array<GLuint64, S> get_uint64s(GLenum target)
    {
      std::array<GLuint64, S> result;
      glGetInteger64v(target, (GLint64*)result.data());
      return result;
    }

    /* --- GLfloat --- */

    inline GLfloat get_float(GLenum target)
    {
      GLfloat result;
      glGetFloatv(target, &result);
      return result;
    }

    template <size_t S>
    inline std::array<GLfloat, S> get_floats(GLenum target)
    {
      std::array<GLfloat, S> result;
      glGetFloatv(target, result.data());
      return result;
    }

    /* --- GLdouble --- */

    inline GLdouble get_double(GLenum target)
    {
      GLdouble result;
      glGetDoublev(target, &result);
      return result;
    }

    template <size_t S>
    inline std::array<GLdouble, S> get_doubles(GLenum target)
    {
      std::array<GLdouble, S> result;
      glGetDoublev(target, result.data());
      return result;
    }
  }
}

#endif // LAK_OPENGL_STATE_HPP