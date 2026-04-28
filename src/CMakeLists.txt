find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(${PROJECT_NAME}
  main.cpp

  Utils/Colors.h
  Utils/Definitions.h
  Utils/Functions.cpp
  Utils/Functions.h
  Utils/Piece.h
  Utils/Utils.h

  Board/Board.cpp
  Board/Board.h

  Game/Game.cpp
  Game/Game.h

  Game/MoveValidator.cpp
  Game/MoveValidator.h

  Game/GameManager.cpp
  Game/GameManager.h

  Assets/Assets.cpp
  Assets/Assets.h

  Config/Config.cpp
  Config/Config.h

)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
  sfml-window
  sfml-system
  sfml-audio
)
