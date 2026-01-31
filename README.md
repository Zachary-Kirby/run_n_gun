# Run and Gun Game

A simple 2D game based loosely on Contra and Megaman.

![Early Development Image](image.png)

# Prerequisites
 - Debian-based Linux
 - CMake >= 3.10
 - GCC >= 10
 - SDL2 >= 2.0.18
 - SDL2_image

```bash
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev
```

## Linux

## Build

This project uses Cmake and the built in package manager for Linux. VSCode is configured using [launch.json](./.vscode/launch.json) and [tasks.json](./.vscode/tasks.json) to use GDB for debugging and compiling the project every time you debug, given you do not want to run cmake and make yourself.

```bash
mkdir build && cd build
cmake ..
cd ..
make -C ./build levelEdit
```

## Running

Run the generated executable files from the top directory of the project. The executable files are named `rungun` and `levelEdit`.

To run the game, simply execute the following command in your terminal:
```bash
./build/rungun
```

To run the editor, simply execute the following command in your terminal:
```bash
./build/levelEdit
```

