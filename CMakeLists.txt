cmake_minimum_required(VERSION 3.26)
project(NebuShell C)

set(CMAKE_C_STANDARD 11)

add_executable(NebuShell
        main.c
        utils/parser.c
        utils/bin.c
        utils/bin.h
        utils/parser.h
        utils/nebula_tools.c
        utils/nebula_tools.h
        utils/signal_handler.c
        utils/signal_handler.h
        utils/history.c
        utils/history.h
        main.h)