# basic_opengl_window_polygons_textured

Opens a window with SDL2, initialises an OpenGL context for it and renders some
textured polygons.

Uses index based polygon rendering

## Building

`<required>` `[optional]`

### Linux

```
make [debug/release] [CXX=g++-8]
```

### Windows

```
make <debug/release> <x86/x64>
```

## Dependencies

### From `common/`

SDL2 (Uses system libs/headers on Linux)

gl3w

lak/opengl/shader

lak/opengl/state
