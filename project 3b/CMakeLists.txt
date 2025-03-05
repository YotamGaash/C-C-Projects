cmake_minimum_required(VERSION 3.10)
project(Snakes C)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)

# Add your source files
set(SOURCE_FILES
        snakes_and_ladders.c
        markov_chain.c
        linked_list.c
        # Add more source files as needed
        )

# Add your header files (if any)
set(HEADER_FILES
        markov_chain.h
        linked_list.h
        # Add more header files as needed
        test.c test.h)

add_executable(snakes_and_ladders ${SOURCE_FILES} ${HEADER_FILES})