cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(rungun)


#Required Dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)

#Optional Dependencies
#Check if Aseprite and Python is available to auto-rebuild sprite definitions
find_program(
  ASEPRITE_PATH
  aseprite
)
find_package(Python3 COMPONENTS Interpreter)
if (ASEPRITE_PATH AND Python3_Interpreter_FOUND)
  message(STATUS "auto-build sprites ON")
  message(STATUS "  Aseprite found in PATH at " ${ASEPRITE_PATH})
  message(STATUS "  Python3 found in PATH at " ${Python3_EXECUTABLE})
  set(AUTO_BUILD_SPRITES true)
else()
  #TODO change this error to actually say if it is Aseprite or python missing
  message(STATUS "Aseprite not found in PATH: auto-build sprites OFF")
  message(STATUS "  To enable: cmake -DASEPRITE_PATH=/path/to/aseprite")
  set(AUTO_BUILD_SPRITES false)
endif()



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
  game/helper/animations.cpp
  game/levelObjects/bird.cpp
  game/core/renderer.cpp
)
target_link_libraries(rungun rungun_shared OpenGL::GL ${CMAKE_SOURCE_DIR}/libs/libglad.a)
target_include_directories(rungun PRIVATE game/core game/helper game/levelObjects libs/glad)
if (AUTO_BUILD_SPRITES) 
  add_dependencies(rungun update_sprites)
endif()



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



#Update Assets
if (AUTO_BUILD_SPRITES)
  set(SPRITE_NAMES
    Player
    EnergyBar
    Bird
  )

  list(TRANSFORM SPRITE_NAMES APPEND ".aseprite")
  list(TRANSFORM SPRITE_NAMES PREPEND "${CMAKE_SOURCE_DIR}/Assets/sprites/")
  
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/Assets/sprites.png ${CMAKE_SOURCE_DIR}/Assets/frames.json
    COMMAND ${CMAKE_SOURCE_DIR}/Assets/sprites/generate.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Assets/sprites
    DEPENDS ${SPRITE_NAMES}
  )
  
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/Assets/spriteData.bin
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/SpriteAnimationConverter.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${CMAKE_SOURCE_DIR}/Assets/frames.json
  )

  add_custom_target(update_sprites ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/Assets/sprites.png
    DEPENDS ${CMAKE_SOURCE_DIR}/Assets/spriteData.bin
  )
endif()
