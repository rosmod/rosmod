#!
sudo proot -q qemu-arm -S BBB-Root -b BBB-Boot:/boot /bin/bash compile.sh $1 $2
