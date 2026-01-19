# Run and Gun Game

## How to build

### Linux

This project uses Cmake and the built in package manager for Linux. VSCode is configured to use GDB for debugging and compiling the project every time you debug, given you do not want to run cmake and make yourself.

Install `libsdl2-dev` and `libsdl-image-dev` with your package manager. Create a build folder and run `cmake ..` from there. Then run `make -C ./build levelEdit` or `make -C ./build rungun` from the top level folder of the project depending on the target you want. The assets folder and levels folder need to be in the folder you run the executable files from so run `./build/rungun` or `./build/levelEdit`.