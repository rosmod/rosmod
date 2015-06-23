# KRPC Flight Controller for 
# Date: 2015.06.11

import krpc
import time
import math
from pid import *

def clamp(n, minn, maxn):
    return max(min(maxn, n), minn)

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
        self.cruise_altitude = 10000
        self.gear_altitude = 100
        self.waypoint_list = []
        print "Ready to Take Off!"
        self.state = "Take_Off"
        print "Latitude: " + str(self.flight.latitude) + "; Longitude: " + str(self.flight.longitude)
        choice = raw_input('Start Control? [Y/n]: ') or 'Y'
        if choice == 'Y':
            self.run()

    def change_state(self, target_heading, target_altitude):
        # Prepare to change state
        self.control.sas = False
        self.control.pitch = 0.0
        self.control.roll = 0.0        
        self.control.yaw = 0.0

        # Roll PID 
        roll_PID = PID(P=0.05, D=0.0)
        roll_PID.setPoint(0.0)

        # Pitch PID
        pitch_PID = PID(P=0.1, I=0.01, Integrator_max=75, Integrator_min = -75)
        pitch_PID_setting = 0

        # Altitude PID
        gain = abs(self.flight.surface_altitude - target_altitude) * 1/10000.0
        altitude_PID = PID(P=gain*5, D=gain*500.0, I=gain, Integrator_max=75, Integrator_min=-75)
        #altitude_PID = PID(P=0.05, D=5.0, I=0.01, Integrator_max=75, Integrator_min=-75)
        altitude_PID.setPoint(target_altitude)

        while True:

            if self.flight.surface_altitude < self.gear_altitude and self.control.gear == False:
                self.control.gear = True
            elif self.flight.surface_altitude > self.gear_altitude and self.control.gear == True:
                self.control.gear = False
 
            # Altitude PID Control
            altitude_output = altitude_PID.update(self.flight.surface_altitude)
            #altitude_output = clamp(altitude_output, -40, 40) 
            pitch_setting = altitude_output
            # Set desired pitch
            pitch_PID.setPoint(pitch_setting)

            # Pitch PID Control - Set new pitch
            # want this to give us a pitch between [-1,1]
            pitch_output = pitch_PID.update(self.flight.pitch)
            self.control.pitch = pitch_output

            # Roll PID Control
            roll_output = roll_PID.update(self.flight.roll)
            self.control.roll = roll_output

            time.sleep(0.05)                

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
