cmake_minimum_required(VERSION 3.26)
project(NebuShell C)

set(CMAKE_C_STANDARD 11)

add_executable(NebuShell
        main.c
        internal/parser.c
        internal/bin.c
        internal/bin.h
        internal/parser.h
        tools/nebula_tools.c
        tools/nebula_tools.h
        tools/signal_handler.c
        tools/signal_handler.h
        internal/history.c
        internal/history.h
        internal/jobs.c
        internal/jobs.h
        internal/file_handler.c
        internal/file_handler.h
)