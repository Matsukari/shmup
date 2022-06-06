#!/usr/bin/bash
#COMPILE AND ASSEMBLE

OBJECTS=(
 settings
 visual_object
 actor
 bullet
 gun
 main)

for obj in ${OBJECTS[@]}; do
	echo "bash c $obj"
	bash c $obj
done