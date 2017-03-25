cmake_minimum_required(VERSION 3.6)
project(c)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        bfsha1match
        sha1collisionchecker.c
        dictionary.txt
        hashedpassword.txt
        ReadMe.txt SHA1Crack.c passwordgenerator.c passwordgenerator.h bfsha1match.h)

add_executable(c ${SOURCE_FILES})