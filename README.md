# INTRODUCTION
A pinball game (for Windows)
# COMPILE
1. Install `OpenCV`, `OpenGL` and `Boost` library correctly. (`OpenCV` should be install in the folder of `mingw32`)
2. Enter `build` folder
3. Type command `cmake .. -DBOOST=<ROOT> -G "MinGW Makefiles"` in terminal, where `<ROOT>` is the root directory of `BOOST` library.
4. Type command `make` in terminal. 