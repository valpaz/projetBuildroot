# Spécifier le compilateur C++
set(CMAKE_CXX_COMPILER /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++)

cmake_minimum_required(VERSION 3.10)
project(moveBank_Projet)

set(CMAKE_CXX_STANDARD 17)

add_executable(launcher main.cpp authentification.cpp myFunctions.cpp request.cpp)
add_executable(updateEvent updateEvent.cpp myFunctions.cpp request.cpp)
add_executable(graph graph.cpp myFunctions.cpp)

target_link_libraries(launcher curl cryptopp gd)
target_link_libraries(updateEvent curl cryptopp gd)
target_link_libraries(graph gd)


