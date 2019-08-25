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

struct program_state
{
  uint32_t screen_size_x = 760;
  uint32_t screen_size_y = 480;
  int display = 0;

  uint16_t target_frame_freq = 59; // FPS
  bool running = true;

  SDL_DisplayMode display_mode;
  SDL_Window *window = nullptr;
  SDL_GLContext opengl_context = nullptr;

  std::vector<SDL_Cursor*> cursors;
  size_t cursor_index = 0;
};

void handle_init(program_state &state)
{
  state.cursors = {
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE),
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)
  };
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

    case SDL_MOUSEBUTTONDOWN: switch (event.button.button)
    {
      case SDL_BUTTON_LEFT:
      {
        if (state.cursor_index >= state.cursors.size())
          state.cursor_index = 0;
        if (state.cursor_index < state.cursors.size())
          SDL_SetCursor(state.cursors[state.cursor_index++]);
      } break;

      case SDL_BUTTON_RIGHT:
      {
        SDL_ShowCursor(!SDL_ShowCursor(-1));
      } break;
    } break;

    // Your code here
  }
}

void handle_update(program_state &state)
{

}

void handle_draw(program_state &state)
{

}

void handle_shutdown(program_state &state)
{
  for (auto *cursor : state.cursors)
    SDL_FreeCursor(cursor);
  state.cursors.clear();
}

int main()
{
  /* --- Config variables --- */
  const char * const title = "Basic OpenGL Window with Cursors";
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

#include <GL/gl3w.c>