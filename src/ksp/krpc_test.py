# KRPC Flight Controller
# Date: 2015.06.11

import krpc
import time
import math
from pid import *

def get_relative_heading(current, target):
    relative_longitude = (target.longitude - current.longitude)
    relative_latitude = (target.latitude - current.latitude)
    heading = (90.0 - (math.atan2(relative_latitude, relative_longitude))*180.0/math.pi )
    return heading

class Position:
    def __init__(self,
                 latitude = 0.0,
                 longitude = 0.0,
                 altitude = 0.0):
        self.latitude = latitude
        self.longitude = longitude
        self.altitude = altitude

class Flight_Controller:
    def __init__(self):
        self.connection = krpc.connect(name='KRPC_Flight_Controller')
        self.vessel = self.connection.space_center.active_vessel
        self.flight = self.vessel.flight()
        self.ref_frame = self.vessel.orbit.body.reference_frame
        self.control = self.vessel.control
        self.control.brakes = True
        self.cruise_altitude = 1000
        self.waypoint_list = []
        print "Ready to Take Off!"
        self.state = "Take_Off"
        print "Latitude: " + str(self.flight.latitude) + "; Longitude: " + str(self.flight.longitude)
        choice = raw_input('Start Control? [Y/n]: ') or 'Y'
        if choice == 'Y':
            self.run()

    def change_state(self, target_heading, target_altitude):
        print "Changing State!"
        # Prepare to change state
        self.control.sas = False
        self.control.pitch = 0.0
        self.control.roll = 0.0        
        self.control.yaw = 0.0

        altitude_tolerance = 10
        heading_tolerance = 2
        roll_tolerance = 2

        AoA_PID = PID()
        altitude_PID = PID()
        AoA_setting = 0
        while True:
            # want this to give us an angle between [-45,45] degrees
            temp_alt = altitude_PID.update(self.flight.surface_altitude) 
            AoA_setting = temp_alt
            AoA_PID.setPoint(AoA_setting)
            # want this to give us a pitch between [-1,1]
            temp_AoA = AoA_PID.update(self.flight.pitch)
            self.control.pitch = temp_AoA

        while (abs(self.flight.heading - target_heading) > altitude_tolerance) or (abs(self.flight.surface_altitude - target_altitude) > altitude_tolerance):

            if (abs(self.flight.heading - target_heading) > heading_tolerance):
                roll_pid = PID()
                roll_pid.setPoint(-90)
                while (abs(self.flight.roll + 90) > roll_tolerance):
                    temp_roll = roll_pid.update(self.flight.roll)
                    print "Roll: " + str(temp_roll)
                    self.control.roll = temp_roll
                    
                heading_pid = PID()
                heading_pid.setPoint(target_heading)
                while (abs(self.flight.heading - target_heading) > heading_tolerance):
                     temp_heading = heading_pid.update(self.flight.heading)
                     print "Heading: " + str(temp_heading)
                     self.control.pitch = temp_heading

            if (abs(self.flight.surface_altitude - target_altitude) > altitude_tolerance):
                roll_pid = PID()
                roll_pid.setPoint(0)
                while (abs(self.flight.roll) > roll_tolerance):
                    temp_roll = roll_pid.update(self.flight.roll)
                    print "Roll: " + str(temp_roll)
                    self.control.roll = temp_roll
                                    
                altitude_pid = PID()
                altitude_pid.setPoint(target_altitude)
                while (abs(self.flight.surface_altitude - target_altitude) > altitude_tolerance):
                    temp_heading = altitude_pid.update(self.flight.surface_altitude)/10000
                    print "Heading: " + str(temp_heading)
                    self.control.pitch = temp_heading
                

    def run(self):
        while True:
            if self.state == "Take_Off":
                self.take_off()
            elif self.state == "Cruise":
                self.cruise()

    def take_off(self):
        self.control.brakes = False
        self.control.activate_next_stage()   
        self.control.throttle = 1.0
        self.control.pitch = 0.1
        ascent_pitch = 20
        pitch_epsilon = 1
        pitch_change = 0.5
        self.change_state(self.flight.heading, self.cruise_altitude)             
            
        self.state = "Cruise"

    def cruise(self):
        cruise_altitude = 1000
        # Island hanger
        target_position = Position(-1.5308109, -71.94456, 0.0)
       
        while True:
            current_position = Position(self.flight.latitude, self.flight.longitude)
            print "Heading: " + str(self.flight.heading) + "; Relative Heading: " + str(get_relative_heading(current_position, target_position))

def main():
    #station = Position(-0.0486859, -74.7254, 0.0)
    #target = Position(-1.5308109, -71.94456, 0.0)
    #print get_relative_heading(station, target)
    controller = Flight_Controller()

if __name__ == "__main__":
    main()
