cmake_minimum_required(VERSION 3.15)
project(Proyecto C)

set(CMAKE_C_STANDARD 99)

add_executable(Proyecto main.c Graph.c Graph.h List.c List.h Queue.c Queue.h bool.h PriorityQueue.c PriorityQueue.h Set.c Set.h)