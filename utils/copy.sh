#!/bin/bash
directory=$1
shift
for i in $@;
do
    scp -i ~/.ssh/id_rsa_jetsontk1 tc_config_nodes.sh root@node$i:/home/debian/Scripts/.
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/var/log/*.csv $directory
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/var/log/ROSMOD*.log $directory
    ssh -i ~/.ssh/id_rsa_jetsontk1 root@node$i "/home/debian/Scripts/clearLogs.bash"
done
cp ~/rosmod_log_plotter.py $directory
