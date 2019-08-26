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

#include "state.hpp"

namespace lak
{
  namespace opengl
  {
    /* --- Enable / Disable --- */

    GLboolean enabled(GLenum target)
    {
      return glIsEnabled(target);
    }

    void enable(GLenum target, GLboolean enabled)
    {
      if (enabled)  glEnable(target);
      else          glDisable(target);
    }

    /* --- GLboolean --- */

    GLboolean get_bool(GLenum target)
    {
      GLboolean result;
      glGetBooleanv(target, &result);
      return result;
    }

    GLboolean get_shader_bool(GLuint shader, GLenum target)
    {
      GLint result;
      glGetShaderiv(shader, target, &result);
      return (GLboolean)result;
    }

    GLboolean get_program_bool(GLuint program, GLenum target)
    {
      GLint result;
      glGetProgramiv(program, target, &result);
      return result;
    }

    /* --- GLint --- */

    GLint get_int(GLenum target)
    {
      GLint result;
      glGetIntegerv(target, &result);
      return result;
    }

    GLint get_shader_int(GLuint shader, GLenum target)
    {
      GLint result;
      glGetShaderiv(shader, target, &result);
      return result;
    }

    GLint get_program_int(GLuint program, GLenum target)
    {
      GLint result;
      glGetProgramiv(program, target, &result);
      return result;
    }

    /* -- GLuint --- */

    GLint get_uint(GLenum target)
    {
      GLuint result;
      glGetIntegerv(target, (GLint*)&result);
      return result;
    }

    GLuint get_shader_uint(GLuint shader, GLenum target)
    {
      GLuint result;
      glGetShaderiv(shader, target, (GLint*)&result);
      return result;
    }

    GLuint get_program_uint(GLuint program, GLenum target)
    {
      GLuint result;
      glGetProgramiv(program, target, (GLint*)&result);
      return result;
    }

    /* --- GLenum --- */

    GLenum get_enum(GLenum target)
    {
      GLenum result;
      glGetIntegerv(target, (GLint*)&result);
      return result;
    }

    GLenum get_shader_enum(GLuint shader, GLenum target)
    {
      GLint result;
      glGetShaderiv(shader, target, &result);
      return (GLenum)result;
    }

    GLenum get_program_enum(GLuint program, GLenum target)
    {
      GLint result;
      glGetProgramiv(program, target, &result);
      return (GLenum)result;
    }

    /* --- GLint64 --- */

    GLint64 get_int64(GLenum target)
    {
      GLint64 result;
      glGetInteger64v(target, &result);
      return result;
    }

    /* --- GLuint64 --- */

    GLuint64 get_uint64(GLenum target)
    {
      GLuint64 result;
      glGetInteger64v(target, (GLint64*)&result);
      return result;
    }

    /* --- GLfloat --- */

    GLfloat get_float(GLenum target)
    {
      GLfloat result;
      glGetFloatv(target, &result);
      return result;
    }

    /* --- GLdouble --- */

    GLdouble get_double(GLenum target)
    {
      GLdouble result;
      glGetDoublev(target, &result);
      return result;
    }
  }
}
