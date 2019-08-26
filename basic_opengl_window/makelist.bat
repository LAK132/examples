set BINDIR=bin
set OBJDIR=obj
set COMDIR=..\common
set OUTPUT=BasicOpenGLWindow.exe

set INCDIRS=%COMDIR%\include %COMDIR%\include\SDL
set LIBDIR=%COMDIR%\lib
set LIBS=SDL2main.lib SDL2.lib

set CXX=cl
set CXXFLAGS=/nologo /std:c++17 /D_CRT_SECURE_NO_WARNINGS /MD /EHsc

if "%mode%"=="release" (
  set COMPFLAGS=/DNDEBUG /bigobj /O2
  set LINKFLAGS=/SUBSYSTEM:CONSOLE
)

if "%mode%"=="debug" (
  set COMPFLAGS=/Zi /bigobj /Od
  set LINKFLAGS=/SUBSYSTEM:CONSOLE /DEBUG
)

call %COMDIR%\makelist.bat

set SOURCE=src\main.cpp %GL_GL3W_C%
