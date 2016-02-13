scp -i ~/.ssh/id_rsa_jetsontk1 root@node19:/root/*.csv $1
scp -i ~/.ssh/id_rsa_jetsontk1 root@node19:/home/ubuntu/*.log $1
scp -i ~/.ssh/id_rsa_jetsontk1 root@node20:/root/*.csv $1
scp -i ~/.ssh/id_rsa_jetsontk1 root@node20:/home/ubuntu/*.log $1
cp ~/rosmod_log_plotter.py $1
