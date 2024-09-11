@echo off

REM Compile the program
gcc -I ./include\SDL2 -L ./lib ^
    -o maze src/1-texture.c src/rotate.c src/essential.c src/init_sdl.c ^
    src/init-texture_path.c src/move.c ^
    -lmingw32 -lSDL2main -lSDL2

REM Check if the compilation was successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

REM Run the compiled program
texture.exe

pause