cmake_minimum_required(VERSION 3.14)
project(main CXX)
project(main VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL_ttf)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/glm)
add_subdirectory(tests)

add_library(RC STATIC src/Texture.cpp
					  src/Timer.cpp
					  src/Window.cpp
					  src/Render.cpp
					  src/Map.cpp
					  src/Player.cpp
					  src/RayCaster.cpp
					  src/utils.cpp
					  src/Vec.cpp)

target_compile_options(RC PRIVATE -Wall -Werror)
target_link_libraries(RC PRIVATE SDL2 SDL2_image SDL2_ttf)
target_include_directories(RC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
									  SDL2 SDL2_ttf SDL2_image)

#-Wconversion
add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE RC)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


include(CTest)
include(GoogleTest)
enable_testing()
