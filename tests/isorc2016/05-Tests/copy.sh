#!/bin/bash
directory=$1
shift
for i in $@;
do
    #scp -i ~/.ssh/id_rsa_jetsontk1 *.snp root@node$i:/home/ubuntu/.
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/home/ubuntu/*.csv $directory
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/var/log/ROSMOD*.log $directory
done
cp ~/rosmod_log_plotter.py $1
