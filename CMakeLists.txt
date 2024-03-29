cmake_minimum_required(VERSION 3.26)
project(NebuShell C)

set(CMAKE_C_STANDARD 11)

add_executable(NebuShell
        main.c
        utils/parser.c
        utils/processHandler.c
        utils/formatter.c)
