import os
i = 7
while (i < 32):
    os.system('scp -i ~/.ssh/id_rsa_jetsontk1 ubuntu@node' + str(i) + ':/home/ubuntu/ROSMOD*Periodic_Timer*.log .')
    i = i + 1
