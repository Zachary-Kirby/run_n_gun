cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(rungun)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)



#Main Game
add_executable(rungun 
  game/core/engine.cpp
  game/core/level.cpp
  game/core/main.cpp
  game/core/player.cpp
  game/helper/sprite.cpp
)
target_link_libraries(rungun SDL2::SDL2 SDL2_image::SDL2_image)
target_include_directories(rungun PRIVATE libs/glm)
target_include_directories(rungun PRIVATE game/core game/helper)

#Level Editor
add_executable(levelEdit 
  editor/engine.cpp
  editor/level.cpp
  editor/main.cpp
  editor/pallete.cpp
  editor/text.cpp
)
target_link_libraries(levelEdit SDL2::SDL2 SDL2_image::SDL2_image)
target_include_directories(levelEdit PRIVATE libs/glm editor)