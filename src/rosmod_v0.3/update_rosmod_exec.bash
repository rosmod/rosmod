#!/usr/bin/env bash
if [ $1 = ""]; then
    echo "Usage: ./update_rosmod_exec.bash <path>"
    echo "Example: ./update_rosmod_exec.bash /home/jeb"
else
    zip -r ../rosmod.zip *
    mv ../rosmod.zip ../rosmod.egg
    mv ../rosmod.egg $1
    echo "Created rosmod.egg at" $1
fi 
