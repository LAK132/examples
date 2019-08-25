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

#include "shader.hpp"
#include "state.hpp"

namespace lak
{
  namespace opengl
  {
    GLuint create_shader(GLenum shader_type, const GLchar *const source)
    {
      GLuint shader = glCreateShader(shader_type);
      glShaderSource(shader, 1, &source, nullptr);
      glCompileShader(shader);
      if (!get_shader_bool(shader, GL_COMPILE_STATUS))
      {
        std::vector<char> msg;
        msg.resize(get_shader_uint(shader, GL_INFO_LOG_LENGTH) + 1);
        if (msg.size() > 1)
        {
          glGetShaderInfoLog(shader, msg.size(), nullptr, msg.data());
          msg.back() = 0;
          fprintf(stderr, "Shader failed to compile\n%s\n", msg.data());
        }
        glDeleteShader(shader);
        return 0; // 0 is reserved
      }
      return shader;
    }

    GLuint create_program(const std::vector<GLuint> &shaders)
    {
      GLuint program = glCreateProgram();
      for (const auto shader : shaders)
        if (shader != 0)
          glAttachShader(program, shader);
      glLinkProgram(program);
      if (!get_program_bool(program, GL_LINK_STATUS))
      {
        std::vector<char> msg;
        msg.resize(get_program_uint(program, GL_INFO_LOG_LENGTH) + 1);
        if (msg.size() > 1)
        {
          glGetProgramInfoLog(program, msg.size(), nullptr, msg.data());
          msg.back() = 0;
          fprintf(stderr, "Program failed to link\n%s\n", msg.data());
        }
        glDeleteProgram(program);
        return 0; // 0 is reserved
      }
      return program;
    }
  }
}
