# Chip8_Emulator
An Emulator for the __CHIP-8__ Language written in __C++__ using __SDL Library__ 

__CHIP-8__ was created by RCA engineer Joe Weisbecker in 1977 for the COSMAC VIP microcomputer. It was intended as a simpler way to make small programs and games for the computer. Instead of using machine language for the VIP’s CDP1802 processor, you could type in hexadecimal instructions (with the VIP’s hex keypad) that resembled machine code, but which were more high-level, and interpreted on the fly by a small program (the CHIP-8 emulator/interpreter).

## Build

1. Setup SDL2 using the link [Here](https://wiki.libsdl.org/SDL3/Installation)

2. Clone the project 

```bash
git clone https://github.com/AmrMohsen122/Chip8_Emulator.git
```

2. Open a terminal and navigate to the folder to which you cloned the project

```bash
cd <LOCATION_OF_PROJECT\Chip8_Emulator>
```
3. Create build directory and cd inside it
```bash
mkdir build
cd build
```
4. Generate make files using cmake
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=<path_to_sdl>
```
- if you are using linux you might want to change "MinGW Makefiles" to "Unix Makefiles"
- the path to sdl should look something like this "<Directory_which_contains_sdl>\SDL2-2.30.9"
5. Run make
```bash
make
```
6. Copy SDL.dll inside your build directory

## Running 
To Run the Emulator you will find the resulting executable in the build directory of your project

```bash
cd build
```
```bash
./Emulator.exe -f <ROM FILE NAME> -s <EXECUTION SPEED>
```

### Execution Flags

|    Flag        | Function |
|:------------|:-----------|
|       -s        |      (Optional)  The Emulator execution speed you might want to play around with execution speed as some games expect different speeds than others      |
|       -f        |       The file name which contains the binary ROM     |



### Common Errors during running the emulator
- "The application was unable to start correctly (0xc000007b)"
you forgot to copy the SDL.dll to build directory or you simply have the wrong dll
the dll is usually inside "\SDL2-2.30.9\lib\<x64 , x86>"
depending on your platform you should choose either x64 or x86 if you don't know simply try both
- "[ERROR] rom file name can't be empty"
you haven't specified a rom file name
- "[ROM] file name not found"
you specified an invalid rom file name, make sure you spelled the file name correctly and you have the full path not a relative one

## Examples
Here are some examples of games played on the emulator

# Tetris.ch8

![Tetris GIF](https://github.com/AmrMohsen122/Chip8_Emulator/blob/main/gifs/tetris.gif?raw=true)

# Snake.ch8
![Snake GIF](https://github.com/AmrMohsen122/Chip8_Emulator/blob/main/gifs/snake.gif?raw=true)













