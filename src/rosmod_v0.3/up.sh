#
echo "Image: " $1
sudo kpartx -a -v $1
sudo fdisk -l /dev/loop0
sudo mount /dev/mapper/loop0p1 BBB-Boot
sudo mount /dev/mapper/loop0p2 BBB-Root
