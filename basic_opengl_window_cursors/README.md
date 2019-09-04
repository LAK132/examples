# basic_opengl_window_cursors

Opens a window with SDL2 and initialises an OpenGL context for it.

Left clicking will change to a different system cursor.

Right clicking will toggle cursor visibility.

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