#!/usr/bin/env bash
zip -r ../rosmod.zip *
mv ../rosmod.zip ../rosmod.egg
sudo mv ../rosmod.egg /opt/rosmod.egg
echo 'alias rosmod="python /opt/rosmod.egg"' > ~/.bashrc
echo "Created rosmod.egg at path:"

