#!
sudo proot -q qemu-arm -S BBB-Root -b BBB-Boot:/boot /bin/bash $1 $2 $3 $USER
