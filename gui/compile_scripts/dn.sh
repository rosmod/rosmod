#
echo "Image: " $1
sudo umount BBB-Root
rm -rf BBB-Root
lsblk
sudo kpartx -d $1
lsblk


