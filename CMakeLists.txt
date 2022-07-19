cmake_minimum_required(VERSION 3.22)
project(prova_finale_API_2022 C)

set(CMAKE_C_STANDARD 11)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEVAL -Wall -Werror -std=gnu11")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -static-libsan")

add_executable(prova_finale_API_2022
        main.c)