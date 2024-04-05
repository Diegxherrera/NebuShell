cmake_minimum_required(VERSION 3.26)
project(NebuShell C)

set(CMAKE_C_STANDARD 11)

add_executable(NebuShell
        main.c
        internal/parser.c
        internal/bin.c
        internal/bin.h
        internal/parser.h
        utils/nebula_tools.c
        utils/nebula_tools.h
        utils/signal_handler.c
        utils/signal_handler.h
        internal/history.c
        internal/history.h
        main.h
        internal/jobs.c
        internal/jobs.h)