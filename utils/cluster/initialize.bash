./uninstall.bash
rm -rf /etc/issue*
rm -rf /etc/udev/rules.d/70*
cp rc.local /etc/.
./interfaces.bash $1
