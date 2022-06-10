#!/usr/bin/bash

OBJECTS="
 lib/obj/collisions.o
 lib/obj/color.o
 lib/obj/logger.o
 lib/obj/sdlut.o
 lib/obj/shapes.o
 lib/obj/texture.o
 lib/obj/timer.o
 lib/obj/ttf.o
 lib/obj/window.o
 lib/obj/resource_manager.o
 lib/obj/math.o
 lib/obj/event.o
 lib/obj/json.o

 src/obj/main.o"
LINKERS="
 -lSDL2main
 -lSDL2
 -lSDL2_image
 -lSDL2_ttf
 -lSDL2_mixer"
CCFLAGS="
 -std=c++17
"

EXECUTABLE="shmup"

if [[ $1 == "-g" ]]; then
    echo "g++ $OBJECTS $LINKERS -g"
    g++ $OBJECTS $LINKERS -g

    exit
fi


echo "g++ $OBJECTS $LINKERS -o $EXECUTABLE"
g++ $OBJECTS $LINKERS -o $EXECUTABLE

# NO SUPPLEMENT ARGUMENT
if [[ $1 == "-r" || $1 == "--run" ]]; then
    ./$EXECUTABLE
fi
