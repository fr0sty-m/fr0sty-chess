cmake_minimum_required(VERSION 3.30)
project(chess VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(${PROJECT_NAME}
  src/main.cpp

  src/Board.cpp
  src/Board.h

  src/Game.cpp
  src/Game.h

  src/Assets.cpp
  src/Assets.h

  src/Piece.h
  src/Colors.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
  sfml-window
  sfml-system
)
