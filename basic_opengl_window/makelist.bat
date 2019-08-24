set BINDIR=bin
set OBJDIR=obj

if "%mode%"=="release" goto release
if "%mode%"=="debug" goto debug
goto :eof

set CXX=cl

set BINARY=explorer.exe
set SOURCE=src\main.cpp

set COMMDIR=..\common
set INCDIRS=%COMMDIR%\include %COMMDIR%\include\SDL
set LIBDIR=lib
set LIBS=SDL2main.lib SDL2.lib

set CXXFLAGS=/nologo /std:c++17 /D_CRT_SECURE_NO_WARNINGS /MD /EHsc

:debug
set COMPFLAGS=/Zi /bigobj /O2
set LINKFLAGS=/SUBSYSTEM:CONSOLE /DEBUG
goto :eof

:release
set COMPFLAGS=/DNDEBUG /bigobj /O2
set LINKFLAGS=/SUBSYSTEM:CONSOLE
