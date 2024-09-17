@echo off

REM Compile the program
gcc -I ./include\SDL2 -L ./lib ^
    -o maze src/maze.c src/rotate.c src/floor_casting.c src/init_sdl.c ^
    src/init-texture_path.c src/move.c src/dda.c src/wall_casting.c src/ceilTexture.c ^
    -lmingw32 -lSDL2main -lSDL2  -lSDL2_image

REM Check if the compilation was successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

REM Run the compiled program
maze.exe

pause