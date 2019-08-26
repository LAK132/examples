@echo off
SetLocal EnableDelayedExpansion

set mode=%1
set target=%2

if not "%mode%"=="debug" if not "%mode%"=="release" if not "%mode%"=="clean" goto useage
if not "%target%"=="x86" if not "%target%"=="x64" if not "%mode%"=="clean" goto useage

call makelist.bat

if "%mode%"=="clean" goto clean

if exist "vcvarsall.bat" (
    call "vcvarsall.bat" %target%
    goto begin
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %target%
    goto begin
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\VC\Auxiliary\Build\vscarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\VC\Auxiliary\Build\vscarsall.bat" %target%
    goto begin
)
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %target%
    goto begin
)
set /p VCVARSALLBAT=vcvarsall.bat directory:
call "%VCVARSALLBAT%" %target%

:begin
echo Compiling in %mode% mode for %target%

title Compiler

REM some windows functions are pedantic about \
set _OBJDIR_=!OBJDIR!\%mode%\%target%
set _LIBDIR_=!LIBDIR!\%target%

if not exist %_OBJDIR_% mkdir %_OBJDIR_%
if not exist %BINDIR% mkdir %BINDIR%

if "%mode%"=="debug" goto run
if "%mode%"=="release" goto run

:useage
echo compile: "make <debug/release> <x86/x64>"
echo clean: "make clean"
goto :eof

:clean
rmdir /S /Q %OBJDIR%\
rmdir /S /Q %BINDIR%\
goto :eof

:run
set _LIBS_=
for %%L in (%LIBS%) do (set _LIBS_=!_LIBS_! %_LIBDIR_%\%%L)

set _INC_=
for %%I in (%INCDIRS%) do (set _INC_=!_INC_! /I%%I)

call %CXX% %CXXFLAGS% !COMPFLAGS! /Fo:%_OBJDIR_%\ /Fe:%BINDIR%\%OUTPUT% %SOURCE% !_LIBS_! !_INC_! /link !LINKFLAGS!

for /f %%F in ('dir /b %_LIBDIR_%') do (if "%%~xF"==".dll" echo f | xcopy /y %_LIBDIR_%\%%F %BINDIR%\%%F)

EndLocal
