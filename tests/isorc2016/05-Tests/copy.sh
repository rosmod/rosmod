#!/bin/bash
directory=$1
shift
for i in $@;
do
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/home/ubuntu/*.csv $directory
    scp -i ~/.ssh/id_rsa_jetsontk1 root@node$i:/home/ubuntu/*.log $directory
done
cp ~/rosmod_log_plotter.py $1
