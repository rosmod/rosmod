#!/usr/bin/python
# CPN Analysis Log Plotter
# Author: Pranav Srinivas Kumar
# Date: 2015.08.12

import os
import sys
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid.anchored_artists import AnchoredText
import pylab
import argparse

class CPN_Callback():
    def __init__(self):
        self.node = ""
        self.component_instance = ""
        self.name = ""
        self.enqueue_time = 0.0
        self.completion_time = 0.0
        self.deadline = 0.0
        self.exec_time = 0.0

class CPN_Log_Plotter():
    def __init__(self):
        self.node = ""
        self.component_instance = ""
        self.callbacks = []
        self.unique_callbacks = []
        self.colors = ['b', 'g', 'r', 'k']
        self.opn_max = 0
        self.time_unit = ""

    def parse(self, log_contents, unit):
        if unit == 's':
            divide = 1000000000.0
        elif unit == 'ms':
            divide = 1000000.0
        elif unit == 'us':
            divide = 1000.0
        elif unit == 'ns':
            divide = 1.0
        else:
            print "ERROR: Provided unit is invalid! Correct Values = (s, ms, us, ns)"
            return
        self.time_unit = unit

        for line in log_contents.split('\n'):
            split_line = line.split('::')
            if len(split_line) == 7:
                self.node = split_line[0].split('=')[-1]
                self.component_instance = split_line[1].split('=')[-1]
                new_callback = CPN_Callback()
                alias = split_line[2].split('=')[-1]            
                enqueue_time = int(split_line[3].split('=')[-1]) * 1000
                completion_time = int(split_line[4].split('=')[-1]) * 1000
                deadline = int(split_line[5].split('=')[-1]) * 1000
                exec_time = int(split_line[6].split('=')[-1]) * 1000
                new_callback.name = alias
                new_callback.node = self.node
                new_callback.component_instance = self.component_instance
                if alias not in self.unique_callbacks:
                    self.unique_callbacks.append(alias)
                new_callback.deadline = float(deadline)/divide
                new_callback.enqueue_time = float(enqueue_time)/divide
                new_callback.completion_time = float(completion_time)/divide
                new_callback.exec_time = float(exec_time)/divide
                self.callbacks.append(new_callback)

    def plot(self, opn_max, combined, directory):
        index = 0
        subnum = 0
        fig = plt.figure()
        x_min = 0.0
        x_max = 0
        for call in self.unique_callbacks:
            count = 1
            calls = [c for c in self.callbacks if c.name == call]
            x_axis_lim_start = 0.0
            x_axis_lim_end = 0.0
            for instance in calls:
                if opn_max == 'all' or count < int(opn_max):
                    if x_axis_lim_start == 0.0:
                        x_axis_lim_start = instance.enqueue_time
                    count = count + 1
                    x_axis_lim_end = instance.completion_time
                else:
                    x_axis_lim_end = instance.completion_time
                    break   
            if x_min == 0.0:
                x_min = x_axis_lim_start
            elif x_axis_lim_start <= x_min:
                x_min = x_axis_lim_start
            if x_axis_lim_end > x_max:
                x_max = x_axis_lim_end

        for call in self.unique_callbacks:
            if subnum == 0:
                ax = fig.add_subplot(110)
            if subnum > 0:
                n = len(fig.axes)
                for i in range(n):
                    fig.axes[i].change_geometry(n+1, 1, i+1)
                ax = fig.add_subplot(n+1, 1, n+1)
            color = self.colors[index % len(self.colors)]
            index = index + 1
            calls = [c for c in self.callbacks if c.name == call]
            x_axis = []
            y_axis = []
            deadline = []
            count = 1
            for instance in calls:
                if opn_max == 'all' or count <= int(opn_max):
                    x_axis.extend([instance.enqueue_time, instance.enqueue_time, instance.completion_time, instance.completion_time])
                    y_axis.extend([0.0, instance.exec_time, instance.exec_time, 0.0])
                    deadline.extend([instance.deadline, instance.deadline, instance.deadline, instance.deadline])
                    count = count + 1
                else:
                    break
            exec_plot = plt.plot(x_axis, y_axis, color, label=call)
            deadline_plot = plt.plot(x_axis, deadline, 'r', label='Deadline')
            at = AnchoredText("Node: " + calls[0].node + "\nComponent Instance: " + calls[0].component_instance +\
                              "\nOperation Deadline:" + str(instance.deadline) + ' ' + self.time_unit\
                              + "\nMaximum Execution Time: " + str(max([instance.exec_time for instance in calls])) + ' ' + self.time_unit +\
                              "\nAverage Execution Time: " + str(sum([instance.exec_time for instance in calls])/len([instance.exec_time for instance in calls])) +\
                              ' ' + self.time_unit,
                              prop=dict(size=12), frameon=True,
                              loc=2,
                          )
            at.patch.set_boxstyle("round,pad=0.,rounding_size=0.5")
            ax.add_artist(at)
            subnum += 1
        
            plt.xlabel('Simulation Time (' + self.time_unit + ')')
            plt.ylabel('Execution Time (' + self.time_unit + ')')
            plt.title('CPN Analysis - Operation Execution Plot - ' + call)
            plt.grid(True)
            plt.legend(loc=1,prop={'size':15})
            pylab.xlim([x_min, 100000])
            pylab.ylim([0, instance.deadline*3])
        fig = plt.gcf()
        fig.set_size_inches(18.5, 10.5)
        fig.subplots_adjust(hspace=.5)
        if combined == False:
            if directory != '.':
                fig.savefig(os.path.join(directory, call + '_cpn.png'), dpi=100)
                print "Plot: " + call + '_cpn.png saved at: ' + directory
            else:
                fig.savefig(call + '_cpn.png', dpi=100)
        else:
            if directory != '.':
                fig.savefig(os.path.join(directory, 'combined_cpn.png'), dpi=100)
                print "Plot: " + 'combined_cpn.png saved at: ' + directory
            else:
                fig.savefig('combined_cpn.png', dpi=100)
    
def main():

    parser = argparse.ArgumentParser(description='functionality: Plot CPN Timing Analysis - Component Operation Execution Time', 
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--log', nargs='?', default='all', help='Name of log file to process')
    parser.add_argument('--num_opn', nargs='?', default='all', help='Number of operation instances to plot')
    parser.add_argument('--unit', nargs='?', default='ms', choices=['s', 'ms', 'us', 'ns'], help='Time unit of the plot')
    parser.add_argument('--save', nargs='?', default='.', help='Path where plots should be saved')
    args = vars(parser.parse_args())

    if args['log'] == 'all':
        logs = [f for f in os.listdir('.') if os.path.isfile(f)]
        log_contents = ""
        for log in logs:
            if "completed_operations" in log:
                with open (log, "r") as logs:
                    log_contents += logs.read()

        plotter = CPN_Log_Plotter()
        # Provide the log file name and time unit
        plotter.parse(log_contents, args['unit'])
        # Provide the number of operation instances to plot
        plotter.plot(args['num_opn'], 
                     True,
                     args['save'])

    else:
        with open(args['log'], 'r') as log:
            plotter = CPN_Log_Plotter()
            log_contents = ""
            log_contents += log.read()

            # Provide the log file name and time unit
            plotter.parse(log_contents, args['unit'])
            # Provide the number of operation instances to plot
            plotter.plot(args['num_opn'], 
                         False,
                         args['save'])

if __name__ == "__main__":
    main()
