echo 1000000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
cpufreq-set -g performance
