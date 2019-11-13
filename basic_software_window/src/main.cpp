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

#include <assert.h>
#include <cmath>
#include <thread>

inline float target_frame_time(uint16_t target)
{
  return 1.0f / (float) target;
}

struct program_state
{
  bool running = true;
  uint16_t target_frame_freq = 59; // FPS

  uint32_t screen_size_x = 760;
  uint32_t screen_size_y = 480;
  int display = 0;

  SDL_DisplayMode display_mode;
  SDL_Window *window = nullptr;

  SDL_Surface *back_buffer = nullptr;
};

void set_window_size(program_state &state, uint32_t x, uint32_t y)
{
  if (state.back_buffer != nullptr)
  {
    SDL_FreeSurface(state.back_buffer);
    state.back_buffer = nullptr;
  }

  state.back_buffer = SDL_CreateRGBSurfaceWithFormat(
    0, x, y, 32, SDL_PIXELFORMAT_RGBA32);

  state.screen_size_x = x;
  state.screen_size_y = y;
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
        set_window_size(state, event.window.data1, event.window.data2);
      } break;
    } break;

    case SDL_DROPFILE:
    {
      if (event.drop.file != nullptr)
      {
        SDL_free(event.drop.file);
      }
    } break;
  }
}

void handle_update(program_state &state)
{

}

void handle_draw(program_state &state)
{
  assert(state.back_buffer != nullptr);
  assert(state.back_buffer->w >= 0);
  assert(state.back_buffer->h >= 0);

  // Safe to directly access state.back_buffer->pixels after lock
  SDL_LockSurface(state.back_buffer);

  const uint32_t w = state.back_buffer->w;
  const uint32_t h = state.back_buffer->h;

  const uint32_t b = 0xFF;
  for (uint32_t y = 0; y < h; ++y)
  {
    const uint32_t g = (255 * y) / h;
    for (uint32_t x = 0; x < w; ++x)
    {
      const uint32_t r = ((255 * x) / w);
      auto &pixel =
        reinterpret_cast<uint32_t*>(state.back_buffer->pixels)[x + (y * w)];
      pixel = (r << 24) | (g << 16) | (b << 8) | 0xFF;
    }
  }

  SDL_UnlockSurface(state.back_buffer);
}

int main()
{
  const char * const title = "Basic Software Rendered Window";

  program_state state;

  SDL_SetMainReady();
  assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

  SDL_GetCurrentDisplayMode(state.display, &state.display_mode);

  state.window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  state.screen_size_x,
                                  state.screen_size_y,
                                  SDL_WINDOW_RESIZABLE);

  // Program crashes if this isn't called here. Not sure why.
  SDL_GetWindowSurface(state.window);

  // Init the back buffer
  set_window_size(state, state.screen_size_x, state.screen_size_y);

  const uint64_t perfFreq = SDL_GetPerformanceFrequency();
  uint64_t perfCount = SDL_GetPerformanceCounter();
  // start non-zero
  float frameTime = target_frame_time(state.target_frame_freq);

  while (state.running)
  {
    for (SDL_Event event; SDL_PollEvent(&event); )
      handle_event(state, event);

    handle_update(state);

    handle_draw(state);

    SDL_Surface *window = SDL_GetWindowSurface(state.window);
    assert(window != nullptr);
    // Clear the screen to a flat colour
    SDL_Rect clip;
    SDL_GetClipRect(window, &clip);
    SDL_FillRect(window, &clip,
                 SDL_MapRGBA(window->format, 0x00, 0x80, 0x80, 0xFF));
    // Blit the back buffer to the screen
    if (SDL_BlitSurface(state.back_buffer, nullptr, window, nullptr) != 0)
    {
      printf("%s\n", SDL_GetError());
      std::abort();
    }

    assert(SDL_UpdateWindowSurface(state.window) == 0);

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

  SDL_FreeSurface(state.back_buffer);

  SDL_DestroyWindow(state.window);
  SDL_Quit();

  return EXIT_SUCCESS;
}