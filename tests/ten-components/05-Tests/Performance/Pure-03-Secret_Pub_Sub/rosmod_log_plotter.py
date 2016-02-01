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
import matplotlib as mpl

class ROSMOD_Callback():
    def __init__(self):
        self.node = ""
        self.component_instance = ""
        self.name = ""
        self.enqueue_time = 0.0
        self.completion_time = 0.0
        self.deadline = 0.0
        self.exec_time = 0.0

class ROSMOD_Log_Plotter():
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
            if 'NODE' in line and '::COMPONENT::' in line:
                split_line = line.split('::')
                self.node = split_line[1]
                self.component_instance = split_line[3]
            if 'ENQUEUE' in line:
                new_callback = ROSMOD_Callback()
                split_line = line.split('::')
                enqueue_time = float(int(split_line[2])/divide) 
                time_values = split_line[4]
                alias = time_values.split(';')[0].split('=')[1]
                deadline = time_values.split(';')[2]
                deadline_sec = deadline.split(',')[0].split('=')[-1]
                deadline_nsec = deadline.split(',')[1].split('=')[-1]                
                new_callback.name = alias
                if alias not in self.unique_callbacks:
                    self.unique_callbacks.append(alias)
                new_callback.deadline = float(float(deadline_sec)*1000000000.0 +\
                                              float(deadline_nsec))/divide
                new_callback.enqueue_time = enqueue_time
                new_callback.node = self.node
                new_callback.component_instance = self.component_instance
                self.callbacks.append(new_callback)
            if 'Execution Time' in line:
                if self.callbacks != []:
                    stack = list(reversed(self.callbacks))
                    for cbk in stack:
                        if cbk.name == line.split('::')[4].split(';')[0].split('=')[-1]:
                            if cbk.exec_time == 0.0:
                                split_line = line.split('::')
                                completion_time = float(int(split_line[2])/divide)
                                time_values = split_line[4]
                                exec_time = time_values.split(';')[2]
                                exec_time_sec = exec_time.split(',')[0].split('=')[-1]
                                exec_time_nsec = exec_time.split(',')[1].split('=')[-1]
                                cbk.exec_time = float(float(exec_time_sec)*1000000000.0 +\
                                                      float(exec_time_nsec))/divide
                                cbk.completion_time = completion_time               
                                break;
        self.callbacks = [call for call in self.callbacks if call.enqueue_time != 0.0\
                          if call.completion_time != 0.0 if call.deadline != 0.0\
                          if call.exec_time != 0.0]

    def plot(self, opn_max, combined, directory):
        index = 0
        subnum = 0
        fig = plt.figure()
        x_min = 0.0
        x_max = 0
        for call in self.unique_callbacks:
            count = 0
            calls = [c for c in self.callbacks if c.name == call]
            x_axis_lim_start = 0.0
            x_axis_lim_end = 0.0
            for instance in calls:
                if opn_max == 'all' or count <= int(opn_max):
                    if x_axis_lim_start == 0.0:
                        x_axis_lim_start = instance.enqueue_time
                    count = count + 1
                    x_axis_lim_end = instance.completion_time
                else:
                    x_axis_lim_end = instance.completion_time
                    break   
            if x_min == 0.0:
                x_min = x_axis_lim_start
            elif x_axis_lim_start < x_min:
                x_min = x_axis_lim_start
            if x_axis_lim_end > x_max:
                x_max = x_axis_lim_end

        for call in self.unique_callbacks:
            if subnum == 0:
                ax = fig.add_subplot(111)
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
            for instance in calls:
                if opn_max == 'all' or count <= int(opn_max):
                    x_axis.extend([instance.enqueue_time, instance.enqueue_time,
                                   instance.completion_time, instance.completion_time])
                    y_axis.extend([0.0, instance.exec_time, instance.exec_time, 0.0])
                    deadline.extend([instance.deadline, instance.deadline, instance.deadline,
                                     instance.deadline])
                else:
                    break
            exec_plot = plt.plot(x_axis, y_axis, color, label=call)
            deadline_plot = plt.plot(x_axis, deadline, 'r', label='Deadline')
            at = AnchoredText("Node: " + calls[0].node + "\nComponent Instance: " +\
                              calls[0].component_instance +\
                              "\nOperation Deadline:" + str(instance.deadline) + ' ' +\
                              self.time_unit\
                              + "\nMaximum Execution Time: " +\
                              str(max([instance.exec_time for instance in calls])) + ' ' +\
                              self.time_unit +\
                              "\nAverage Execution Time: " +\
                              str(sum([instance.exec_time\
                                       for instance in calls])/\
                                  len([instance.exec_time for instance in calls])) +\
                              ' ' + self.time_unit,
                              prop=dict(size=8), frameon=True,
                              loc=2,
                          )
            at.patch.set_boxstyle("round,pad=0.,rounding_size=0.5")
            ax.add_artist(at)
            subnum += 1
        
            plt.xlabel('Test Timestamp (' + self.time_unit + ')', fontsize=12)
            plt.ylabel('Execution Time (' + self.time_unit + ')', fontsize=12)
            plt.title('Operation Execution Plot - ' + call, fontsize=12)
            plt.grid(True)
            plt.legend(loc=1,prop={'size':8})
            pylab.xlim([x_min, x_max])
            plt.rcParams.update({'axes.labelsize': 'small'})
        fig = plt.gcf()
        fig.set_size_inches(18.5, 10.5)
        fig.subplots_adjust(hspace=1.0)
        if combined == False:
            if directory != '.':
                fig.savefig(os.path.join(directory, call + '.png'), dpi=100)
                fig_path = str(os.path.join(directory, call + '.png'))
                os.system('gnome-open ' + fig_path)
                print "Plot: " + call + '.png saved at: ' + directory
            else:
                fig.savefig(call + '.png', dpi=100)
                fig_path = str(call + '.png')
                os.system('gnome-open ' + fig_path)                
        else:
            if directory != '.':
                fig.savefig(os.path.join(directory, 'combined.png'), dpi=100)
                fig_path = str(os.path.join(directory, 'combined.png'))
                os.system('gnome-open ' + fig_path)                
                print "Plot: " + 'combined.png saved at: ' + directory
            else:
                fig.savefig('combined.png', dpi=100)
                fig_path = 'combined.png'
                os.system('gnome-open ' + fig_path)                                
    
        # Plot Histogram!
        fig2 = plt.figure()
        for call in self.unique_callbacks:
            if subnum == 0:
                ax = fig2.add_subplot(120)
            if subnum > 0:
                n = len(fig2.axes)
                for i in range(n):
                    fig2.axes[i].change_geometry(n+1, 1, i+1)
                ax = fig2.add_subplot(n+1, 1, n+1)
            clr = self.colors[index % len(self.colors)]
            index = index + 1
            calls = [c for c in self.callbacks if c.name == call]
            x_axis = []
            y_axis = []
            for instance in calls:
                if opn_max == 'all' or count <= int(opn_max):
                    x_axis.extend([instance.exec_time])
                else:
                    break
            plt.hist(x_axis, bins=100, color=clr, label=call, align='mid')
            at = AnchoredText("Node: " + calls[0].node + "\nComponent Instance: " +\
                              calls[0].component_instance +\
                              "\nMaximum Execution Time: " +\
                              str(max([instance.exec_time for instance in calls])) + ' ' +\
                              self.time_unit +\
                              "\nAverage Execution Time: " +\
                              str(sum([instance.exec_time for instance in calls])/\
                                  len([instance.exec_time for instance in calls])) +\
                              ' ' + self.time_unit,
                              prop=dict(size=8), frameon=True,
                              loc=4,
                          )
            at.patch.set_boxstyle("round,pad=0.,rounding_size=0.5")
            ax.add_artist(at)
            subnum += 1

            plt.title(call + " - Execution Times Histogram", fontsize=12)
            plt.xlabel("Operation Execution Time (" + self.time_unit + ")", fontsize=12)
            plt.ylabel("Number of Operation Instances", fontsize=12)
            plt.grid(True)
            plt.legend(loc=1,prop={'size':8})
            plt.rcParams.update({'axes.labelsize': 'small'})
        fig2 = plt.gcf()
        fig2.set_size_inches(18.5, 10.5)
        fig2.subplots_adjust(hspace=1.0)
        if combined == False:
            if directory != '.':
                fig2.savefig(os.path.join(directory, call + '_histogram.png'), dpi=100)
                print "Histogram Plot: " + call + '.png saved at: ' + directory
            else:
                fig2.savefig(call + '_histogram.png', dpi=100)
        else:
            if directory != '.':
                fig2.savefig(os.path.join(directory, 'combined_histogram.png'), dpi=100)
                print "Plot: " + 'combined_histogram.png saved at: ' + directory
            else:
                fig2.savefig('combined_histogram.png', dpi=100)

def main():

    parser = argparse.ArgumentParser(description=\
                                     'functionality: Plot Component Operation Execution Behavior', 
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--log', nargs='?', default='all', help='Name of log file to process')
    parser.add_argument('--num_opn', nargs='?', default='all',
                        help='Number of operation instances to plot')
    parser.add_argument('--unit', nargs='?', default='ms', choices=['s', 'ms', 'us', 'ns'],
                        help='Time unit of the plot')
    parser.add_argument('--save', nargs='?', default='.', help='Path where plots should be saved')
    args = vars(parser.parse_args())

    if args['log'] == 'all':
        logs = [f for f in os.listdir('.') if os.path.isfile(f)]
        log_contents = ""
        for log in logs:
            if "ROSMOD_DEBUG." in log:
                with open (log, "r") as logs:
                    log_contents += "\nNODE::" + log.split('.')[1] + "::COMPONENT::" +\
                                    log.split('.')[2] + '\n'
                    log_contents += logs.read()

        plotter = ROSMOD_Log_Plotter()
        # Provide the log file name and time unit
        plotter.parse(log_contents, args['unit'])
        # Provide the number of operation instances to plot
        plotter.plot(args['num_opn'], 
                     True,
                     args['save'])

    else:
        with open(args['log'], 'r') as log:
            plotter = ROSMOD_Log_Plotter()
            log_contents = ""
            log_contents += "\nNODE::" + args['log'].split('.')[1] + "::COMPONENT::" +\
                            args['log'].split('.')[2] + '\n'
            log_contents += log.read()

            # Provide the log file name and time unit
            plotter.parse(log_contents, args['unit'])
            # Provide the number of operation instances to plot
            plotter.plot(args['num_opn'], 
                         False,
                         args['save'])

if __name__ == "__main__":
    main()
