#!/usr/bin/python
# ROSMOD Log Plotter
# Author: Pranav Srinivas Kumar
# Date: 2015.07.20

import os
import sys
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid.anchored_artists import AnchoredText
import pylab
import argparse

class Time_Difference():
    def __init__(self):
        self.time_stamp_1 = 0.0
        self.time_stamp_2 = 0.0
        self.difference = 0.0

class ROSMOD_WCET_Finder():
    def __init__(self):
        self.time_diffs = []
        self.time_unit = ""

    def parse(self, log_contents, entry1, entry2):
        divide = 1000000.0
        for line in log_contents.split('\n'):
            if entry1 in line:
                time_diff = Time_Difference()
                time_diff.time_stamp_1 = (float(line.split('::')[2]))/divide
                self.time_diffs.append(time_diff)
            if entry2 in line:
                if self.time_diffs != []:
                    self.time_diffs[-1].time_stamp_2 = (float(line.split('::')[2]))/divide
        for diff in self.time_diffs:
            diff.difference = diff.time_stamp_2 - diff.time_stamp_1
            
        self.time_diffs = [td.difference for td in self.time_diffs if td.time_stamp_1 != 0.0 if td.time_stamp_2 != 0.0 if td.difference != 0.0]
        print "Average Execution Time: " + str(max(self.time_diffs)/len(self.time_diffs))  + "; Maximum Execution Time: " + str(max(self.time_diffs)) + " ms"

def main():

    parser = argparse.ArgumentParser(description='functionality: Calculate Worst-Case Execution Time between 2 Log entries', 
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--log', help='Name of log file to process')
    parser.add_argument('--entry1', help='Time Stamp 1 - Text to look for')
    parser.add_argument('--entry2', help='Time Stamp 2 - Text to look for')
    args = vars(parser.parse_args())

    with open(args['log'], 'r') as log:
        wcet_finder = ROSMOD_WCET_Finder()
        log_contents = log.read()
        
        # Provide the log file name and time unit
        wcet_finder.parse(log_contents, args['entry1'], args['entry2'])

if __name__ == "__main__":
    main()
