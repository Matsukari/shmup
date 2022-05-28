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
 json.o
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

echo "g++ $OBJECTS $LINKERS -o shmup"
g++ $OBJECTS $LINKERS -o shmup