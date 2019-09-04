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

#include <lak/opengl/shader.hpp>
#include <lak/opengl/state.hpp>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/gl3w.h>

#include <assert.h>
#include <cmath>
#include <thread>
#include <vector>

inline float target_frame_time(uint16_t targetFrameFreq)
{
  return 1.0f / (float) targetFrameFreq;
}

struct color_t
{
  float r, g, b, a;
};

struct vertex_t
{
  double pos[4];
  float uv[2];
  color_t color;
};

struct polygon_t
{
  GLuint verts[3];
};

struct program_state
{
  uint32_t screen_size_x = 760;
  uint32_t screen_size_y = 480;
  uint32_t screen_pos_x = 0;
  uint32_t screen_pos_y = 0;
  int display = 0;

  uint16_t target_frame_freq = 59; // FPS
  bool running = true;

  SDL_DisplayMode display_mode;
  SDL_Window *window = nullptr;
  SDL_GLContext opengl_context = nullptr;

  std::vector<vertex_t> vertices;
  std::vector<polygon_t> polygons;
  GLuint polygons_vert_shader;
  GLuint polygons_frag_shader;
  GLuint polygons_shader_program;
  GLuint polygons_texture;
  GLuint polygons_array_buffer;
  GLuint polygons_index_buffer;
  GLuint polygons_vertex_array;
};

void handle_opengl_message(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar *message, const void *userParam)
{
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
  {
    printf("Source: %d; Type: %d; ID: %d; Severity: %d;\n%s\n\n",
          source, type, id, severity, message);
    std::abort();
  }

}

void handle_init(program_state &state)
{
  state.polygons_vert_shader = lak::opengl::create_shader(GL_VERTEX_SHADER,
    "#version 130\n"
    "uniform mat4 uViewProj;\n"
    "in vec4 vPosition;\n"
    "in vec2 vUV;\n"
    "in vec4 vColor;\n"
    "out vec2 fUV;\n"
    "out vec4 fColor;\n"
    "void main() {\n"
    "  fUV = vUV;\n"
    "  fColor = vColor;\n"
    "  gl_Position = uViewProj * vPosition;\n"
    "}\n");

  state.polygons_frag_shader = lak::opengl::create_shader(GL_FRAGMENT_SHADER,
    "#version 130\n"
    "uniform sampler2D fTexture;\n"
    "in vec2 fUV;\n"
    "in vec4 fColor;\n"
    "out vec4 pColor;\n"
    "void main() {\n"
    // "  pColor = fColor;\n"
    // "  pColor = fColor * texture(fTexture, fUV);\n"
    "  pColor = texture(fTexture, fUV.st);\n"
    // "  pColor = vec4(fUV, 1.0, 1.0);\n"
    "}\n");

  state.polygons_shader_program = lak::opengl::create_program({
    state.polygons_vert_shader,
    state.polygons_frag_shader
  });

  glGenBuffers(1, &state.polygons_array_buffer);
  glGenBuffers(1, &state.polygons_index_buffer);

  state.vertices = {
    vertex_t{
      {100.0, 100.0, 0.0, 1.0},
      {1.0f, 0.333f},
      {1.0f, 1.0f, 0.0f, 1.0f}
    },
    vertex_t{
      {0.0, 100.0, 0.0, 1.0},
      {0.0f, 0.333f},
      {1.0f, 1.0f, 0.0f, 1.0f}
    },
    vertex_t{
      {50.0, 0.0, 0.0, 1.0},
      {0.5f, 0.0f},
      {1.0f, 1.0f, 0.0f, 1.0f}
    },
    vertex_t{
      {100.0, 300.0, 0.0, 1.0},
      {1.0f, 1.0f},
      {1.0f, 0.0f, 1.0f, 1.0f}
    },
    vertex_t{
      {0.0, 300.0, 0.0, 1.0},
      {0.0f, 1.0f},
      {1.0f, 0.0f, 1.0f, 1.0f}
    },
    vertex_t{
      {50.0, 200.0, 0.0, 1.0},
      {0.5f, 0.666f},
      {1.0f, 0.0f, 1.0f, 1.0f}
    }
  };
  state.polygons = {
    polygon_t{ 0, 1, 2 },
    polygon_t{ 0, 1, 5 },
    polygon_t{ 3, 4, 5 }
  };

  const size_t width = 16;
  const size_t height = 16;
  color_t pixels[width * height];
  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      float v = ((x + 1) * (y + 1)) / 256.0f;
      pixels[x + (y * width)] = {v, v, v, 1.0};
    }
  }

  auto old_texture = lak::opengl::get_uint(GL_TEXTURE_BINDING_2D);

  glGenTextures(1, &state.polygons_texture);
  glBindTexture(GL_TEXTURE_2D, state.polygons_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
               GL_RGBA, GL_FLOAT, pixels);

  glBindTexture(GL_TEXTURE_2D, old_texture);
}

void handle_event(program_state &state, SDL_Event &event)
{
  switch (event.type)
  {
    case SDL_QUIT:
    {
      state.running = false;
    } break;

    case SDL_WINDOWEVENT: switch (event.window.event)
    {
      case SDL_WINDOWEVENT_RESIZED:
      case SDL_WINDOWEVENT_SIZE_CHANGED:
      {
        state.screen_size_x = event.window.data1;
        state.screen_size_y = event.window.data2;
      } break;
    } break;

    case SDL_DROPFILE:
    {
      if (event.drop.file != nullptr)
      {
        SDL_free(event.drop.file);
      }
    } break;

    // Your code here
  }
}

void handle_update(program_state &state)
{

}

void handle_draw(program_state &state)
{
  auto old_program = lak::opengl::get_uint(GL_CURRENT_PROGRAM);
  auto old_active_texture = lak::opengl::get_uint(GL_ACTIVE_TEXTURE);
  auto old_vertex_array = lak::opengl::get_uint(GL_VERTEX_ARRAY_BINDING);
  auto old_array_buffer = lak::opengl::get_uint(GL_ARRAY_BUFFER_BINDING);
  auto old_index_buffer =
    lak::opengl::get_uint(GL_ELEMENT_ARRAY_BUFFER_BINDING);
  auto old_blend_enabled = lak::opengl::enabled(GL_BLEND);
  auto old_cull_face_enabled = lak::opengl::enabled(GL_CULL_FACE);
  auto old_depth_test_enabled = lak::opengl::enabled(GL_DEPTH_TEST);
  auto old_scissor_test_enabled = lak::opengl::enabled(GL_SCISSOR_TEST);
  auto old_scissor = lak::opengl::get_ints<4>(GL_SCISSOR_BOX);

  glGenVertexArrays(1, &state.polygons_vertex_array);

  glUseProgram(state.polygons_shader_program);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, state.polygons_texture);
  glBindVertexArray(state.polygons_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, state.polygons_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.polygons_index_buffer);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                      GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);

  {
    const float W = state.screen_size_x;
    const float H = state.screen_size_y;
    const float O1 = ((state.screen_pos_x * 2.0f) + W) / -W;
    const float O2 = ((state.screen_pos_y * 2.0f) + H) / H;
    const float uViewProj[] =
    {
      2.0f / W,   0.0f,       0.0f,     0.0f,
      0.0f,       2.0f / -H,  0.0f,     0.0f,
      0.0f,       0.0f,       -1.0f,    0.0f,
      O1,         O2,         0.0f,     1.0f
    };
    glUniformMatrix4fv(
      glGetUniformLocation(state.polygons_shader_program, "uViewProj"),
      1, GL_FALSE, uViewProj);
  }
  auto uniform_tex = glGetUniformLocation(state.polygons_shader_program,
                                           "fTexture");
  glUniform1i(uniform_tex, 0);

  auto attrib_pos =
    glGetAttribLocation(state.polygons_shader_program, "vPosition");
  if (attrib_pos >= 0)
  {
    glEnableVertexAttribArray(attrib_pos);
    glVertexAttribPointer(attrib_pos, 4, GL_DOUBLE, GL_FALSE,
                          sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, pos));
  }

  auto attrib_uv = glGetAttribLocation(state.polygons_shader_program, "vUV");
  if (attrib_uv >= 0)
  {
    glEnableVertexAttribArray(attrib_uv);
    glVertexAttribPointer(attrib_uv, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (GLvoid*)offsetof(vertex_t, uv));
  }

  auto attrib_color =
    glGetAttribLocation(state.polygons_shader_program, "vColor");
  if (attrib_color >= 0)
  {
    glEnableVertexAttribArray(attrib_color);
    glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t),
                          (GLvoid*)offsetof(vertex_t, color));
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * state.vertices.size(),
               state.vertices.data(), GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(polygon_t) * state.polygons.size(),
               state.polygons.data(), GL_STREAM_DRAW);
  glDrawElements(GL_TRIANGLES, 3 * state.polygons.size(), GL_UNSIGNED_INT, 0);

  glDeleteVertexArrays(1, &state.polygons_vertex_array);

  glUseProgram(old_program);
  glActiveTexture(old_active_texture);
  glBindVertexArray(old_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, old_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, old_index_buffer);
  lak::opengl::enable(GL_BLEND, old_blend_enabled);
  lak::opengl::enable(GL_CULL_FACE, old_cull_face_enabled);
  lak::opengl::enable(GL_DEPTH_TEST, old_depth_test_enabled);
  lak::opengl::enable(GL_SCISSOR_TEST, old_scissor_test_enabled);
  glScissor(old_scissor[0], old_scissor[1],
            old_scissor[2], old_scissor[3]);
}

void handle_shutdown(program_state &state)
{
  glDeleteProgram(state.polygons_shader_program);
  glDeleteShader(state.polygons_vert_shader);
  glDeleteShader(state.polygons_frag_shader);

  glDeleteBuffers(1, &state.polygons_array_buffer);
  glDeleteBuffers(1, &state.polygons_index_buffer);

  glDeleteTextures(1, &state.polygons_texture);
}

int main()
{
  /* --- Config variables --- */
  const char * const title = "Basic OpenGL Window with Textured Polygons";
  const bool doubleBuffered = true;
  const uint8_t depthSize = 8;
  const uint8_t colorSize = 8;
  const uint8_t stencilSize = 8;

  program_state state;

  /* --- Start SDL and get OpenGL context --- */
  SDL_SetMainReady();
  assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

  assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                             SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                             SDL_GL_CONTEXT_PROFILE_CORE) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffered) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   depthSize) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     colorSize) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   colorSize) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    colorSize) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilSize) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == 0);
  assert(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) == 0);

  SDL_GetCurrentDisplayMode(state.display, &state.display_mode);

  state.window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  state.screen_size_x,
                                  state.screen_size_y,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  state.opengl_context = SDL_GL_CreateContext(state.window);

  // context must be created before calling this
  assert(gl3wInit() == GL3W_OK);

  SDL_GL_MakeCurrent(state.window, state.opengl_context);

  if (SDL_GL_SetSwapInterval(-1) == -1)
      assert(SDL_GL_SetSwapInterval(1) == 0);

  const uint64_t perfFreq = SDL_GetPerformanceFrequency();
  uint64_t perfCount = SDL_GetPerformanceCounter();
  // start non-zero
  float frameTime = target_frame_time(state.target_frame_freq);

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(handle_opengl_message, nullptr);

  glViewport(0, 0, state.screen_size_x, state.screen_size_y);
  glClearColor(0.0f, 0.3125f, 0.3125f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  handle_init(state);

  while (state.running)
  {
    /* --- BEGIN EVENTS --- */

    for (SDL_Event event; SDL_PollEvent(&event); )
      handle_event(state, event);

    /* --- END EVENTS --- */

    /* --- BEGIN UPDATE --- */

    handle_update(state);

    /* --- END UPDATE --- */

    glViewport(0, 0, state.screen_size_x, state.screen_size_y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* --- BEGIN DRAW --- */

    handle_draw(state);

    /* --- END DRAW --- */

    SDL_GL_SwapWindow(state.window);

    const float targetFrameTime = target_frame_time(state.target_frame_freq);
    float frameTimeError = frameTime - targetFrameTime;
    if (frameTimeError < 0.0f)
      frameTimeError = 0.0f;
    else if (frameTimeError > 0.0f)
      frameTimeError = std::fmod(frameTimeError, targetFrameTime);
    const uint64_t prevPerfCount = perfCount;
    do {
      perfCount = SDL_GetPerformanceCounter();
      frameTime =
        (float)(frameTimeError + ((double)(perfCount - prevPerfCount) /
                                  (double)perfFreq));
      std::this_thread::yield();
    } while (frameTime < targetFrameTime);
  }

  handle_shutdown(state);

  assert(state.opengl_context != nullptr);
  SDL_GL_DeleteContext(state.opengl_context);
  SDL_DestroyWindow(state.window);
  SDL_Quit();

  return 0;
}
