#!/usr/bin/env bash
if [ $1 = ""]; then
    echo "Usage: ./create_egg.bash <path_to_save_egg>"
    echo "Example: ./create_egg.bash /home/jeb"
else
    zip -r ../rosmod.zip *
    mv ../rosmod.zip ../rosmod.egg
    mv ../rosmod.egg $1
    echo "Created rosmod.egg at path:" $1
fi
