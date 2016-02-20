#!
sudo proot -q qemu-arm -S BBB-Root -b BBB-Root:/boot /bin/bash $1 $2 $3 $USER
