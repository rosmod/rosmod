#
echo "Image: " $1
sudo umount BBB-Boot BBB-Root
lsblk
sudo kpartx -d $1
lsblk


