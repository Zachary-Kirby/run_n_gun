cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(rungun)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

#Shared Library for Game and Editor
add_library(rungun_shared STATIC
  shared/levelObjects.cpp
)
#It makes it easier to share SDL with the rest of the project
target_link_libraries(rungun_shared PUBLIC SDL2::SDL2 SDL2_image::SDL2_image)
target_include_directories(rungun_shared PUBLIC shared)
target_include_directories(rungun_shared PUBLIC libs/glm)


#Main Game
add_executable(rungun 
  game/core/engine.cpp
  game/core/level.cpp
  game/core/main.cpp
  game/core/player.cpp
  game/core/bullet.cpp
  game/helper/sprite.cpp
  game/levelObjects/bird.cpp
)
target_link_libraries(rungun rungun_shared)
target_include_directories(rungun PRIVATE game/core game/helper game/levelObjects)


#Level Editor
add_executable(levelEdit 
  editor/engine.cpp
  editor/level.cpp
  editor/main.cpp
  editor/pallete.cpp
  editor/text.cpp
  editor/inputBox.cpp
)
target_link_libraries(levelEdit rungun_shared)