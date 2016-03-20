#!/bin/bash
END=32
for ((i=1;i<=END;i++)); do
    # ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -i ~/.ssh/id_rsa_jetsontk1 root@10.1.1.$i ${@}
    ssh -o StrictHostKeyChecking=no -i ~/.ssh/id_rsa_jetsontk1 root@10.1.1.$i ${@}
done
