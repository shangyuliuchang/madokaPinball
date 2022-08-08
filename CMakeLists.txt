cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

project(Pinball)

aux_source_directory(src DIR_SRCS)
include_directories(inc ${BOOST})
link_directories(${BOOST}/stage/lib)
link_libraries(libopencv_core451 glut32 glu32 opengl32 libopencv_imgproc451 libopencv_imgcodecs451 gdi32 wsock32)
add_executable(Pinball ${DIR_SRCS})