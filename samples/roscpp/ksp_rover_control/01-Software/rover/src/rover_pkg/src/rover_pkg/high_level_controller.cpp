#include "rover_pkg/high_level_controller.hpp"

KRPCI krpci_client;

//# Start User Globals Marker

#include <math.h>
#define PI 3.14159265

int current_waypoint = 0;
std::vector<Waypoint>::size_type wp_size;
Waypoint dynamicWP;

double cruising_speed = 20.0;
double waypoint_latitude_tolerance = 0.005;
double waypoint_longitude_tolerance = 0.005;

uint64_t vesselID;
uint64_t controlID;
uint64_t surfaceRefFrameID;
uint64_t orbitID;
uint64_t bodyID;
uint64_t orbitalRefFrameID;

float get_relative_heading(double current_latitude, 
			   double current_longitude,
			   double target_latitude,
			   double target_longitude) {
  double relative_longitude = target_longitude - current_longitude;
  double relative_latitude = target_latitude - current_latitude;
  float heading = (90.0 - atan2(relative_latitude, relative_longitude) * 180/PI);
  if (heading < 0)
    heading = 360 + heading;
  if (heading > 360.0)
    heading = heading - 360;
  return heading;
}

bool high_level_controller::isGoalReached() {
  // Check current sensor values
  double headingDiff = abs(goal_heading - current_heading);
  if ( headingDiff < heading_tolerance ) {
    return true;
  }
  return false;
}

bool high_level_controller::state_func_INIT() {
  // Change State
  goal_heading = 90.0;
  goal_speed = 15.0;
  current_state = CRUISE; 
  std::vector<uint64_t> return_vec;
  //krpci_client.Control_ActivateNextStage(controlID, return_vec);
  return true;
}

bool high_level_controller::state_func_CRUISE() {
  // Set goals for LAND state
  // Iterate through all waypoints in cruise_waypoints
  Waypoint activeWP = cruise_waypoints[current_waypoint];    
  double wp_lat = activeWP.latitude_;
  double wp_lon = activeWP.longitude_;
  double wp_lat_tolerance = activeWP.lat_tolerance_;
  double wp_lon_tolerance = activeWP.long_tolerance_;

  // Find midpoint between current waypoint & next waypoint
  // Converge more gradually by choosing the midpoint between 2 waypoints!
  if (current_waypoint < wp_size) {

    double latDiff = abs(current_latitude - wp_lat);
    double lonDiff = abs(current_longitude - wp_lon);

    // If the dynamic midpoint is very close to next waypoint, we update dynamic and current WP
    if ( latDiff < wp_lat_tolerance && lonDiff < wp_lon_tolerance) {
      current_waypoint++;
      activeWP = cruise_waypoints[current_waypoint];    
      wp_lat = activeWP.latitude_;
      wp_lon = activeWP.longitude_;
    }
    double midpoint_latitude = (current_latitude + wp_lat)/2;
    double midpoint_longitude = (current_longitude + wp_lon)/2;
    goal_heading = get_relative_heading(current_latitude,
					current_longitude,
					midpoint_latitude,
					midpoint_longitude);
    goal_speed = activeWP.speed_; 
  }
  else
    current_waypoint = 85;
}

//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void high_level_controller::Init(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::Init");
  // Initialize Here
  goal_heading = 90.0;
  current_state = INIT;
  
  // Set the history capacity
  previous_states.set_capacity(10);

  // Set Goal tolerances
  heading_tolerance = 1.0;
  speed_tolerance = 1.0;
  
  for (int i=0;i<node_argc;i++)
    {
      if (!strcmp(node_argv[i],"--krpc_ip"))
	{
	  krpci_client.SetIP(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--krpc_port"))
	{
	  krpci_client.SetPort(atoi(node_argv[i+1]));	  
	}
      if (!strcmp(node_argv[i],"--cruising_speed"))
	{
	  cruising_speed = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--lat_tolerance"))
	{
	  waypoint_latitude_tolerance = atof(node_argv[i+1]);
	}
      if (!strcmp(node_argv[i],"--long_tolerance"))
	{
	  waypoint_longitude_tolerance = atof(node_argv[i+1]);
	}
    }

  // Starting point of CRUISE mode
  // Latitude, Longitude, Speed, Lat. Tolerance, Long. Tolerance
  // Rover Path:
  Waypoint wp1(-0.048637, -74.724318, 
	       0.876231,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  dynamicWP = wp1;
  cruise_waypoints.push_back(wp1);
  Waypoint wp2(-0.048638, -74.724231, 
	       2.538103,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp2);
  Waypoint wp3(-0.048640, -74.723959, 
	       4.746317,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp3);
  Waypoint wp4(-0.048644, -74.723551, 
	       6.589972,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp4);
  Waypoint wp5(-0.048649, -74.723008, 
	       8.220935,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp5);
  Waypoint wp6(-0.048654, -74.722392, 
	       9.559317,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp6);
  Waypoint wp7(-0.048659, -74.721893, 
	       10.418212,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp7);
  Waypoint wp8(-0.048664, -74.721181, 
	       11.422857,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp8);
  Waypoint wp9(-0.048672, -74.720217, 
	       12.503069,
	       waypoint_latitude_tolerance,
	       waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp9);
  Waypoint wp10(-0.048679, -74.719225, 
		13.390821,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp10);
  Waypoint wp11(-0.048686, -74.718304, 
		14.070995,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp11);
  Waypoint wp12(-0.048691, -74.717398, 
		14.640635,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp12);
  Waypoint wp13(-0.048695, -74.716774, 
		14.985990,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp13);
  Waypoint wp14(-0.048702, -74.715576, 
		15.564116,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp14);
  Waypoint wp15(-0.048709, -74.714398, 
		16.044606,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp15);
  Waypoint wp16(-0.048714, -74.713217, 
		16.455965,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp16);
  Waypoint wp17(-0.048719, -74.712073, 
		16.800694,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp17);
  Waypoint wp18(-0.048722, -74.711362, 
		16.991914,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp18);
  Waypoint wp19(-0.048726, -74.710414, 
		17.231345,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp19);
  Waypoint wp20(-0.048730, -74.709320, 
		17.462358,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp20);
  Waypoint wp21(-0.048734, -74.707976, 
		17.719093,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp21);
  Waypoint wp22(-0.048738, -74.706715, 
		17.928716,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp22);
  Waypoint wp23(-0.048741, -74.705303, 
		18.133542,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp23);
  Waypoint wp24(-0.048743, -74.704469, 
		18.241562,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp24);
  Waypoint wp25(-0.048745, -74.703315, 
		18.377100,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp25);
  Waypoint wp26(-0.048747, -74.702117, 
		18.502363,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp26);
  Waypoint wp27(-0.048748, -74.700876, 
		18.618055,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp27);
  Waypoint wp28(-0.048749, -74.699521, 
		18.729451,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp28);
  Waypoint wp29(-0.048750, -74.698265, 
		18.821134,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp29);
  Waypoint wp30(-0.048760, -74.697007, 
		18.857951,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp30);
  Waypoint wp31(-0.048786, -74.696069, 
		18.915141,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp31);
  Waypoint wp32(-0.048834, -74.694476, 
		19.004481,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp32);
  Waypoint wp33(-0.048883, -74.692877, 
		19.082237,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp33);
  Waypoint wp34(-0.048933, -74.691271, 
		19.149541,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp34);
  Waypoint wp35(-0.048966, -74.690210, 
		19.190077,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp35);
  Waypoint wp36(-0.049002, -74.689000, 
		19.229559,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp36);
  Waypoint wp37(-0.049048, -74.687493, 
		19.275593,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp37);
  Waypoint wp38(-0.049092, -74.685983, 
		19.313462,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp38);
  Waypoint wp39(-0.049136, -74.684470, 
		19.347702,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp39);
  Waypoint wp40(-0.049180, -74.682954, 
		19.377858,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp40);
  Waypoint wp41(-0.049203, -74.682140, 
		19.392668,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp41);
  Waypoint wp42(-0.049249, -74.680510, 
		19.418683,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp42);
  Waypoint wp43(-0.049291, -74.678989, 
		19.439975,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp43);
  Waypoint wp44(-0.049335, -74.677393, 
		19.459032,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp44);
  Waypoint wp45(-0.049376, -74.675869, 
		19.475246,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp45);
  Waypoint wp46(-0.049408, -74.674642, 
		19.486516,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp46);
  Waypoint wp47(-0.049445, -74.673265, 
		19.497902,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp47);
  Waypoint wp48(-0.049485, -74.671516, 
		19.466096,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp48);
  Waypoint wp49(-0.049495, -74.669809, 
		19.383625,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp49);
  Waypoint wp50(-0.049528, -74.668150, 
		19.253871,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp50);
  Waypoint wp51(-0.049484, -74.667047, 
		19.244473,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp51);
  Waypoint wp52(-0.049450, -74.665356, 
		19.278428,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp52);
  Waypoint wp53(-0.049433, -74.663845, 
		19.313927,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp53);
  Waypoint wp54(-0.049417, -74.662331, 
		19.345445,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp54);
  Waypoint wp55(-0.049403, -74.660964, 
		19.370581,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp55);
  Waypoint wp56(-0.049391, -74.659890, 
		19.388075,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp56);
  Waypoint wp57(-0.049387, -74.658300, 
		19.316989,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp57);
  Waypoint wp58(-0.049451, -74.656751, 
		19.344722,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp58);
  Waypoint wp59(-0.049515, -74.655311, 
		19.370174,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp59);
  Waypoint wp60(-0.049584, -74.653758, 
		19.395487,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp60);
  Waypoint wp61(-0.049625, -74.652832, 
		19.408241,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp61);
  Waypoint wp62(-0.049694, -74.651276, 
		19.427489,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp62);
  Waypoint wp63(-0.049760, -74.649756, 
		19.444679,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp63);
  Waypoint wp64(-0.049827, -74.648197, 
		19.458703,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp64);
  Waypoint wp65(-0.049892, -74.646675, 
		19.471726,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp65);
  Waypoint wp66(-0.049929, -74.645783, 
		19.478597,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp66);
  Waypoint wp67(-0.049997, -74.644184, 
		19.488181,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp67);
  Waypoint wp68(-0.050115, -74.642521, 
		19.346449,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp68);
  Waypoint wp69(-0.050390, -74.640850, 
		19.286633,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp69);
  Waypoint wp70(-0.050606, -74.639768, 
		19.216922,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp70);
  Waypoint wp71(-0.051009, -74.638246, 
		19.165293,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp71);
  Waypoint wp72(-0.051446, -74.636851, 
		19.116566,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp72);
  Waypoint wp73(-0.052044, -74.635538, 
		18.790628,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp73);
  Waypoint wp74(-0.052853, -74.634358, 
		18.678344,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp74);
  Waypoint wp75(-0.053321, -74.633779, 
		18.551516,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp75);
  Waypoint wp76(-0.054357, -74.632773, 
		18.319190,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp76);
  Waypoint wp77(-0.055543, -74.631961, 
		18.398171,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp77);
  Waypoint wp78(-0.056791, -74.631161, 
		18.537552,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp78);
  Waypoint wp79(-0.057991, -74.630401, 
		18.657782,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp79);
  Waypoint wp80(-0.058896, -74.629827, 
		18.739766,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp80);
  Waypoint wp81(-0.059995, -74.629155, 
		18.767466,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp81);
  Waypoint wp82(-0.061386, -74.628330, 
		18.870061,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp82);
  Waypoint wp83(-0.062817, -74.627484, 
		18.963429,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp83);
  Waypoint wp84(-0.064223, -74.626653, 
		19.041527,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp84);
  Waypoint wp85(-0.065163, -74.626097, 
		19.087225,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp85);
  Waypoint wp86(-0.066557, -74.625305, 
		19.102855,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp86);
  Waypoint wp87(-0.067772, -74.624633, 
		19.154604,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp87);
  Waypoint wp88(-0.069095, -74.623930, 
		19.157147,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp88);
  Waypoint wp89(-0.070438, -74.623264, 
		19.158635,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp89);
  Waypoint wp90(-0.071230, -74.622884, 
		19.088339,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp90);
  Waypoint wp91(-0.072723, -74.622518, 
		18.026358,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp91);
  Waypoint wp92(-0.074153, -74.622453, 
		17.608006,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp92);
  Waypoint wp93(-0.075547, -74.622614, 
		16.648583,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp93);
  Waypoint wp94(-0.076421, -74.623117, 
		15.719677,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp94);
  Waypoint wp95(-0.076972, -74.623690, 
		15.213845,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp95);
  Waypoint wp96(-0.077620, -74.624664, 
		15.575329,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp96);
  Waypoint wp97(-0.078275, -74.625691, 
		15.458374,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp97);
  Waypoint wp98(-0.078905, -74.626678, 
		15.074060,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp98);
  Waypoint wp99(-0.079680, -74.627473, 
		14.165148,
		waypoint_latitude_tolerance,
		waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp99);
  Waypoint wp100(-0.080134, -74.627687, 
		 13.632370,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp100);
  Waypoint wp101(-0.081172, -74.627799, 
		 12.652635,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp101);
  Waypoint wp102(-0.082101, -74.627438, 
		 11.995713,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp102);
  Waypoint wp103(-0.082950, -74.627050, 
		 11.311309,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp103);
  Waypoint wp104(-0.083737, -74.627083, 
		 10.283727,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp104);
  Waypoint wp105(-0.084052, -74.627278, 
		 10.329741,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp105);
  Waypoint wp106(-0.084569, -74.627839, 
		 11.162494,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp106);
  Waypoint wp107(-0.085106, -74.628584, 
		 12.218390,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp107);
  Waypoint wp108(-0.085691, -74.629422, 
		 13.171708,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp108);
  Waypoint wp109(-0.086284, -74.630275, 
		 13.956630,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp109);
  Waypoint wp110(-0.086815, -74.631035, 
		 14.548886,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp110);
  Waypoint wp111(-0.087204, -74.631589, 
		 14.924830,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp111);
  Waypoint wp112(-0.087943, -74.632542, 
		 15.036244,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp112);
  Waypoint wp113(-0.088912, -74.633137, 
		 14.982063,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp113);
  Waypoint wp114(-0.090050, -74.633464, 
		 15.247194,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp114);
  Waypoint wp115(-0.091190, -74.633620, 
		 15.641270,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp115);
  Waypoint wp116(-0.092032, -74.633690, 
		 15.955066,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp116);
  Waypoint wp117(-0.093451, -74.633812, 
		 16.440320,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp117);
  Waypoint wp118(-0.094774, -74.633923, 
		 16.660509,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp118);
  Waypoint wp119(-0.096055, -74.634020, 
		 16.964772,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp119);
  Waypoint wp120(-0.097493, -74.634107, 
		 17.297502,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp120);
  Waypoint wp121(-0.098422, -74.634161, 
		 17.486004,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp121);
  Waypoint wp122(-0.099968, -74.634250, 
		 17.758058,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp122);
  Waypoint wp123(-0.101531, -74.634334, 
		 17.649656,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp123);
  Waypoint wp124(-0.102991, -74.634165, 
		 16.642196,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp124);
  Waypoint wp125(-0.103644, -74.633866, 
		 16.120403,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp125);
  Waypoint wp126(-0.104588, -74.633062, 
		 15.134138,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp126);
  Waypoint wp127(-0.105279, -74.632041, 
		 15.655159,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp127);
  Waypoint wp128(-0.105960, -74.630997, 
		 16.135814,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp128);
  Waypoint wp129(-0.106677, -74.629897, 
		 16.565211,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp129);
  Waypoint wp130(-0.107058, -74.629276, 
		 16.670949,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp130);
  Waypoint wp131(-0.107686, -74.628155, 
		 16.909833,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp131);
  Waypoint wp132(-0.108248, -74.626917, 
		 16.906823,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp132);
  Waypoint wp133(-0.108613, -74.625530, 
		 17.212141,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp133);
  Waypoint wp134(-0.108985, -74.624084, 
		 17.526615,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp134);
  Waypoint wp135(-0.109194, -74.623270, 
		 17.673057,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp135);
  Waypoint wp136(-0.109549, -74.621887, 
		 17.905529,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp136);
  Waypoint wp137(-0.109925, -74.620420, 
		 18.115758,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp137);
  Waypoint wp138(-0.110287, -74.619005, 
		 18.290841,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp138);
  Waypoint wp139(-0.110661, -74.617543, 
		 18.447133,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp139);
  Waypoint wp140(-0.110932, -74.616559, 
		 18.441364,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp140);
  Waypoint wp141(-0.111433, -74.615132, 
		 18.442266,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp141);
  Waypoint wp142(-0.112050, -74.613753, 
		 18.427640,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp142);
  Waypoint wp143(-0.112754, -74.612409, 
		 18.518372,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp143);
  Waypoint wp144(-0.113474, -74.611104, 
		 18.640400,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp144);
  Waypoint wp145(-0.113894, -74.610360, 
		 18.655494,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp145);
  Waypoint wp146(-0.114713, -74.608976, 
		 18.766302,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp146);
  Waypoint wp147(-0.115558, -74.607555, 
		 18.862233,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp147);
  Waypoint wp148(-0.116407, -74.606128, 
		 18.939531,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp148);
  Waypoint wp149(-0.117054, -74.605038, 
		 18.984363,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp149);
  Waypoint wp150(-0.117740, -74.603883, 
		 19.040405,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp150);
  Waypoint wp151(-0.118643, -74.602437, 
		 18.950136,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp151);
  Waypoint wp152(-0.119657, -74.601073, 
		 18.873625,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp152);
  Waypoint wp153(-0.120796, -74.599882, 
		 18.619659,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp153);
  Waypoint wp154(-0.121755, -74.599318, 
		 18.058884,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp154);
  Waypoint wp155(-0.123290, -74.599123, 
		 17.649807,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp155);
  Waypoint wp156(-0.124912, -74.599157, 
		 17.808383,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp156);
  Waypoint wp157(-0.126522, -74.599144, 
		 18.038636,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp157);
  Waypoint wp158(-0.127490, -74.599134, 
		 18.160290,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp158);
  Waypoint wp159(-0.129164, -74.599117, 
		 18.345691,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp159);
  Waypoint wp160(-0.130851, -74.599102, 
		 18.373201,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp160);
  Waypoint wp161(-0.132456, -74.599360, 
		 17.966382,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp161);
  Waypoint wp162(-0.133398, -74.599827, 
		 17.647872,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp162);
  Waypoint wp163(-0.134479, -74.600859, 
		 17.336532,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp163);
  Waypoint wp164(-0.135118, -74.602138, 
		 16.989474,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp164);
  Waypoint wp165(-0.135277, -74.603502, 
		 16.669205,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp165);
  Waypoint wp166(-0.135101, -74.604367, 
		 16.423266,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp166);
  Waypoint wp167(-0.134637, -74.605253, 
		 16.423589,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp167);
  Waypoint wp168(-0.133953, -74.606317, 
		 16.669673,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp168);
  Waypoint wp169(-0.133246, -74.607508, 
		 17.033097,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp169);
  Waypoint wp170(-0.132552, -74.608735, 
		 17.304478,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp170);
  Waypoint wp171(-0.132076, -74.609611, 
		 17.461970,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp171);
  Waypoint wp172(-0.131308, -74.610867, 
		 17.629562,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp172);
  Waypoint wp173(-0.130503, -74.612079, 
		 17.683093,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp173);
  Waypoint wp174(-0.129623, -74.612998, 
		 17.417826,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp174);
  Waypoint wp175(-0.128471, -74.613727, 
		 17.377980,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp175);
  Waypoint wp176(-0.127803, -74.614011, 
		 17.225546,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp176);
  Waypoint wp177(-0.126487, -74.614292, 
		 17.189464,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp177);
  Waypoint wp178(-0.125073, -74.614353, 
		 17.326209,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp178);
  Waypoint wp179(-0.123605, -74.614333, 
		 17.613762,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp179);
  Waypoint wp180(-0.122079, -74.614311, 
		 17.869949,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp180);
  Waypoint wp181(-0.121223, -74.614298, 
		 17.997807,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp181);
  Waypoint wp182(-0.119840, -74.614279, 
		 18.179837,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp182);
  Waypoint wp183(-0.118446, -74.614259, 
		 18.250554,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp183);
  Waypoint wp184(-0.117023, -74.614358, 
		 18.168846,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp184);
  Waypoint wp185(-0.115705, -74.614789, 
		 17.434230,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp185);
  Waypoint wp186(-0.115079, -74.615214, 
		 17.138068,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp186);
  Waypoint wp187(-0.114306, -74.616308, 
		 16.685203,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp187);
  Waypoint wp188(-0.113764, -74.617535, 
		 16.852586,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp188);
  Waypoint wp189(-0.113091, -74.618752, 
		 17.076768,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp189);
  Waypoint wp190(-0.112381, -74.619973, 
		 17.347630,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp190);
  Waypoint wp191(-0.111876, -74.620837, 
		 17.545821,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp191);
  Waypoint wp192(-0.111161, -74.622061, 
		 17.784095,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp192);
  Waypoint wp193(-0.110420, -74.623330, 
		 17.992276,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp193);
  Waypoint wp194(-0.109672, -74.624613, 
		 18.184600,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp194);
  Waypoint wp195(-0.109180, -74.625456, 
		 18.296640,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp195);
  Waypoint wp196(-0.108510, -74.626609, 
		 18.431604,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp196);
  Waypoint wp197(-0.107728, -74.627953, 
		 18.570289,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp197);
  Waypoint wp198(-0.106977, -74.629245, 
		 18.630045,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp198);
  Waypoint wp199(-0.106277, -74.630640, 
		 18.551750,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp199);
  Waypoint wp200(-0.105920, -74.631528, 
		 18.537751,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp200);
  Waypoint wp201(-0.105475, -74.632872, 
		 18.544353,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp201);
  Waypoint wp202(-0.105095, -74.634275, 
		 18.562946,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp202);
  Waypoint wp203(-0.104841, -74.635734, 
		 18.361388,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp203);
  Waypoint wp204(-0.104831, -74.637137, 
		 18.332933,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp204);
  Waypoint wp205(-0.104835, -74.637936, 
		 18.048410,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp205);
  Waypoint wp206(-0.104849, -74.639275, 
		 16.904036,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp206);
  Waypoint wp207(-0.105248, -74.640487, 
		 14.777904,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp207);
  Waypoint wp208(-0.106093, -74.641302, 
		 12.931356,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp208);
  Waypoint wp209(-0.107065, -74.641507, 
		 12.913265,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp209);
  Waypoint wp210(-0.107835, -74.641371, 
		 13.507459,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp210);
  Waypoint wp211(-0.109023, -74.641287, 
		 13.707683,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp211);
  Waypoint wp212(-0.110240, -74.641616, 
		 14.176969,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp212);
  Waypoint wp213(-0.111085, -74.642281, 
		 14.386013,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp213);
  Waypoint wp214(-0.111551, -74.642761, 
		 14.732901,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp214);
  Waypoint wp215(-0.112388, -74.643674, 
		 15.379688,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp215);
  Waypoint wp216(-0.113294, -74.644674, 
		 15.966452,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp216);
  Waypoint wp217(-0.114187, -74.645663, 
		 16.453652,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp217);
  Waypoint wp218(-0.115155, -74.646717, 
		 16.788646,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp218);
  Waypoint wp219(-0.115614, -74.647171, 
		 16.968331,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp219);
  Waypoint wp220(-0.116617, -74.647984, 
		 16.955541,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp220);
  Waypoint wp221(-0.117910, -74.648505, 
		 16.705434,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp221);
  Waypoint wp222(-0.119373, -74.648717, 
		 16.963030,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp222);
  Waypoint wp223(-0.120773, -74.648833, 
		 16.334831,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp223);
  Waypoint wp224(-0.121445, -74.648888, 
		 15.768582,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp224);
  Waypoint wp225(-0.122736, -74.648922, 
		 14.351594,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp225);
  Waypoint wp226(-0.123909, -74.648926, 
		 13.235854,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp226);
  Waypoint wp227(-0.124821, -74.649449, 
		 12.237007,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp227);
  Waypoint wp228(-0.125412, -74.650225, 
		 11.786604,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp228);
  Waypoint wp229(-0.125697, -74.650650, 
		 11.472193,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp229);
  Waypoint wp230(-0.126408, -74.651173, 
		 10.510006,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp230);
  Waypoint wp231(-0.127244, -74.651253, 
		 9.463448,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp231);
  Waypoint wp232(-0.127858, -74.650721, 
		 9.845550,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp232);
  Waypoint wp233(-0.128232, -74.649991, 
		 10.984689,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp233);
  Waypoint wp234(-0.128592, -74.649363, 
		 11.898885,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp234);
  Waypoint wp235(-0.129200, -74.648431, 
		 12.917724,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp235);
  Waypoint wp236(-0.129872, -74.647433, 
		 13.780366,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp236);
  Waypoint wp237(-0.130512, -74.646369, 
		 14.199869,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp237);
  Waypoint wp238(-0.130953, -74.645627, 
		 14.061833,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp238);
  Waypoint wp239(-0.131460, -74.644540, 
		 13.306325,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp239);
  Waypoint wp240(-0.131533, -74.643474, 
		 12.377300,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp240);
  Waypoint wp241(-0.131193, -74.642479, 
		 11.897372,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp241);
  Waypoint wp242(-0.130929, -74.641904, 
		 12.595172,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp242);
  Waypoint wp243(-0.130475, -74.640927, 
		 13.550310,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp243);
  Waypoint wp244(-0.129978, -74.639860, 
		 14.390247,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp244);
  Waypoint wp245(-0.129432, -74.638750, 
		 14.998137,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp245);
  Waypoint wp246(-0.129082, -74.638114, 
		 15.371134,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp246);
  Waypoint wp247(-0.128472, -74.637014, 
		 15.932813,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp247);
  Waypoint wp248(-0.127839, -74.635879, 
		 16.424510,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp248);
  Waypoint wp249(-0.127203, -74.634741, 
		 16.843239,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp249);
  Waypoint wp250(-0.126536, -74.633548, 
		 17.115442,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp250);
  Waypoint wp251(-0.126094, -74.632757, 
		 16.717617,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp251);
  Waypoint wp252(-0.125429, -74.631570, 
		 15.832316,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp252);
  Waypoint wp253(-0.124662, -74.630532, 
		 15.013072,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp253);
  Waypoint wp254(-0.123777, -74.629839, 
		 14.399524,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp254);
  Waypoint wp255(-0.122897, -74.629555, 
		 13.602757,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp255);
  Waypoint wp256(-0.122152, -74.629477, 
		 13.474830,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp256);
  Waypoint wp257(-0.120991, -74.629366, 
		 13.929104,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp257);
  Waypoint wp258(-0.119738, -74.629277, 
		 14.554473,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp258);
  Waypoint wp259(-0.118427, -74.629304, 
		 15.204074,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp259);
  Waypoint wp260(-0.117811, -74.629318, 
		 15.499021,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp260);
  Waypoint wp261(-0.116544, -74.629326, 
		 16.041405,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp261);
  Waypoint wp262(-0.115237, -74.629328, 
		 16.514927,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp262);
  Waypoint wp263(-0.113894, -74.629329, 
		 16.929282,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp263);
  Waypoint wp264(-0.112520, -74.629330, 
		 17.291094,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp264);
  Waypoint wp265(-0.111623, -74.629330, 
		 17.498927,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp265);
  Waypoint wp266(-0.110117, -74.629330, 
		 17.352531,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp266);
  Waypoint wp267(-0.108730, -74.629553, 
		 16.280947,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp267);
  Waypoint wp268(-0.107648, -74.630350, 
		 15.696593,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp268);
  Waypoint wp269(-0.107071, -74.630932, 
		 16.008254,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp269);
  Waypoint wp270(-0.106143, -74.631796, 
		 16.373332,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp270);
  Waypoint wp271(-0.105154, -74.632562, 
		 16.399434,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp271);
  Waypoint wp272(-0.103978, -74.633042, 
		 16.352216,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp272);
  Waypoint wp273(-0.102592, -74.633380, 
		 16.805858,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp273);
  Waypoint wp274(-0.101522, -74.633638, 
		 16.994079,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp274);
  Waypoint wp275(-0.100514, -74.633998, 
		 16.950052,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp275);
  Waypoint wp276(-0.099484, -74.634838, 
		 16.606692,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp276);
  Waypoint wp277(-0.098702, -74.635889, 
		 16.816466,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp277);
  Waypoint wp278(-0.097954, -74.637106, 
		 17.034811,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp278);
  Waypoint wp279(-0.097617, -74.637843, 
		 16.921007,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp279);
  Waypoint wp280(-0.097455, -74.639135, 
		 16.598112,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp280);
  Waypoint wp281(-0.097772, -74.640413, 
		 16.521053,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp281);
  Waypoint wp282(-0.098350, -74.641652, 
		 16.830257,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp282);
  Waypoint wp283(-0.099082, -74.642905, 
		 17.064517,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp283);
  Waypoint wp284(-0.099592, -74.643710, 
		 17.302551,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp284);
  Waypoint wp285(-0.100309, -74.644835, 
		 17.596548,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp285);
  Waypoint wp286(-0.100989, -74.645995, 
		 17.607704,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp286);
  Waypoint wp287(-0.101369, -74.647289, 
		 17.086176,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp287);
  Waypoint wp288(-0.101194, -74.648586, 
		 16.645059,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp288);
  Waypoint wp289(-0.100812, -74.649350, 
		 16.565482,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp289);
  Waypoint wp290(-0.100001, -74.650503, 
		 16.963867,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp290);
  Waypoint wp291(-0.099144, -74.651703, 
		 17.254543,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp291);
  Waypoint wp292(-0.098363, -74.653012, 
		 17.410077,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp292);
  Waypoint wp293(-0.097649, -74.654271, 
		 17.095866,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp293);
  Waypoint wp294(-0.097349, -74.654810, 
		 16.927223,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp294);
  Waypoint wp295(-0.096709, -74.656030, 
		 16.659258,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp295);
  Waypoint wp296(-0.096247, -74.657176, 
		 15.744064,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp296);
  Waypoint wp297(-0.096384, -74.658475, 
		 14.482229,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp297);
  Waypoint wp298(-0.096906, -74.659323, 
		 14.005090,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp298);
  Waypoint wp299(-0.097293, -74.659921, 
		 13.846596,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp299);
  Waypoint wp300(-0.097865, -74.660940, 
		 14.384781,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp300);
  Waypoint wp301(-0.098134, -74.662050, 
		 14.304815,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp301);
  Waypoint wp302(-0.097828, -74.663180, 
		 14.500834,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp302);
  Waypoint wp303(-0.097327, -74.664220, 
		 14.865351,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp303);
  Waypoint wp304(-0.096931, -74.664734, 
		 14.798409,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp304);
  Waypoint wp305(-0.095968, -74.665424, 
		 14.789754,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp305);
  Waypoint wp306(-0.094801, -74.665625, 
		 15.027438,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp306);
  Waypoint wp307(-0.093579, -74.665662, 
		 15.523569,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp307);
  Waypoint wp308(-0.092310, -74.665669, 
		 16.061053,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp308);
  Waypoint wp309(-0.091662, -74.665673, 
		 16.252858,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp309);
  Waypoint wp310(-0.090244, -74.665721, 
		 16.721451,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp310);
  Waypoint wp311(-0.088858, -74.665765, 
		 16.958542,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp311);
  Waypoint wp312(-0.087390, -74.665674, 
		 17.163692,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp312);
  Waypoint wp313(-0.086057, -74.665331, 
		 16.319686,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp313);
  Waypoint wp314(-0.085448, -74.664882, 
		 15.582527,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp314);
  Waypoint wp315(-0.084711, -74.663782, 
		 14.945189,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp315);
  Waypoint wp316(-0.084211, -74.662778, 
		 14.344506,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp316);
  Waypoint wp317(-0.084121, -74.661740, 
		 13.298300,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp317);
  Waypoint wp318(-0.084432, -74.660895, 
		 13.375389,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp318);
  Waypoint wp319(-0.084641, -74.660422, 
		 13.673071,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp319);
  Waypoint wp320(-0.085029, -74.659419, 
		 14.420673,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp320);
  Waypoint wp321(-0.085396, -74.658443, 
		 15.032370,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp321);
  Waypoint wp322(-0.085726, -74.657309, 
		 15.058582,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp322);
  Waypoint wp323(-0.085592, -74.656129, 
		 14.932761,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp323);
  Waypoint wp324(-0.085250, -74.655382, 
		 14.866908,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp324);
  Waypoint wp325(-0.084546, -74.654435, 
		 15.234709,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp325);
  Waypoint wp326(-0.083831, -74.653270, 
		 15.856679,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp326);
  Waypoint wp327(-0.083081, -74.652028, 
		 16.410530,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp327);
  Waypoint wp328(-0.082408, -74.650913, 
		 16.830933,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp328);
  Waypoint wp329(-0.081808, -74.649916, 
		 17.057285,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp329);
  Waypoint wp330(-0.081061, -74.648674, 
		 17.338885,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp330);
  Waypoint wp331(-0.079976, -74.647635, 
		 16.611399,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp331);
  Waypoint wp332(-0.078747, -74.647415, 
		 15.896628,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp332);
  Waypoint wp333(-0.078016, -74.647473, 
		 16.136365,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp333);
  Waypoint wp334(-0.076674, -74.647544, 
		 16.488879,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp334);
  Waypoint wp335(-0.075303, -74.647534, 
		 16.897636,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp335);
  Waypoint wp336(-0.073900, -74.647514, 
		 17.258528,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp336);
  Waypoint wp337(-0.072440, -74.647469, 
		 17.482452,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp337);
  Waypoint wp338(-0.071502, -74.647398, 
		 17.682128,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp338);
  Waypoint wp339(-0.069907, -74.647270, 
		 17.882583,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp339);
  Waypoint wp340(-0.068306, -74.647055, 
		 18.044406,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp340);
  Waypoint wp341(-0.066712, -74.646733, 
		 18.161655,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp341);
  Waypoint wp342(-0.065741, -74.646467, 
		 18.118368,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp342);
  Waypoint wp343(-0.064354, -74.645676, 
		 17.780612,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp343);
  Waypoint wp344(-0.063289, -74.644719, 
		 17.886796,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp344);
  Waypoint wp345(-0.062270, -74.643611, 
		 17.967367,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp345);
  Waypoint wp346(-0.061332, -74.642416, 
		 18.159444,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp346);
  Waypoint wp347(-0.060835, -74.641745, 
		 18.231352,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp347);
  Waypoint wp348(-0.059997, -74.640537, 
		 18.383642,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp348);
  Waypoint wp349(-0.059171, -74.639265, 
		 18.556101,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp349);
  Waypoint wp350(-0.058406, -74.638064, 
		 18.706580,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp350);
  Waypoint wp351(-0.057662, -74.636805, 
		 18.677711,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp351);
  Waypoint wp352(-0.057272, -74.635967, 
		 18.607730,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp352);
  Waypoint wp353(-0.056855, -74.634622, 
		 18.302553,
		 waypoint_latitude_tolerance,
		 waypoint_longitude_tolerance);
  cruise_waypoints.push_back(wp353);
  wp_size = cruise_waypoints.size();

  // Connect to kRPC Server and obtain the vessel & control ID
  if (krpci_client.Connect()) {
    krpci_client.get_ActiveVessel(vesselID);
    krpci_client.Vessel_get_Control(vesselID, controlID);
    krpci_client.Vessel_get_SurfaceReferenceFrame(vesselID, surfaceRefFrameID);
    krpci_client.Vessel_get_Orbit(vesselID, orbitID);
    krpci_client.Orbit_get_Body(orbitID, bodyID);
    krpci_client.CelestialBody_get_ReferenceFrame(bodyID, orbitalRefFrameID);
  }

  // Stop Init Timer
  initOneShotTimer.stop();
  LOGGER.DEBUG("Exiting high_level_controller::Init");
}
//# End Init Marker

// Subscriber Callback - vessel_state_sub
//# Start vessel_state_sub_OnOneData Marker
void high_level_controller::vessel_state_sub_OnOneData(const rover_pkg::vessel_state::ConstPtr& received_data)
{
  LOGGER.DEBUG("Entering high_level_controller::vessel_state_sub_OnOneData");
  // Business Logic for vessel_state_sub Subscriber
  current_heading = received_data->heading;
  current_latitude = received_data->latitude;
  current_longitude = received_data->longitude;
  current_speed = received_data->speed;
  current_landing_gear = received_data->landing_gear;
  current_brakes = received_data->brakes;
  Save_State new_state(current_heading, current_speed);
  previous_states.push_back(new_state);

  LOGGER.DEBUG("Exiting high_level_controller::vessel_state_sub_OnOneData");
}
//# End vessel_state_sub_OnOneData Marker

// Timer Callback - state_timer
//# Start state_timerCallback Marker
void high_level_controller::state_timerCallback(const ros::TimerEvent& event)
{
  LOGGER.DEBUG("Entering high_level_controller::state_timerCallback");
  // Business Logic for state_timer Timer
  switch(current_state) {
  case INIT :
    state_func_INIT();
    break;
  case CRUISE :
    state_func_CRUISE();
    break;
  }

  // Green heading marker
  krpci_client.ClearDirections();
  double x=0, y, z;
  z = sin(goal_heading * PI/180.0);
  y = cos(goal_heading * PI/180.0);
  krpci_client.DrawDirection(x,y,z,surfaceRefFrameID,0,1,0,30);

  // Publish newly set goals
  rover_pkg::goal_state new_state;
  new_state.goal_speed = goal_speed;
  new_state.goal_heading = goal_heading;
  goal_state_pub.publish(new_state);

  LOGGER.DEBUG("Exiting high_level_controller::state_timerCallback");
}
//# End state_timerCallback Marker


// Destructor - Cleanup Ports & Timers
high_level_controller::~high_level_controller()
{
  state_timer.stop();
  goal_state_pub.shutdown();
  vessel_state_sub.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void high_level_controller::startUp()
{
  LOGGER.DEBUG("Entering high_level_controller::startUp");
  ros::NodeHandle nh;
  std::string advertiseName;

  // Component Subscriber - vessel_state_sub
  advertiseName = "vessel_state";
  if (portGroupMap.find("vessel_state_sub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["vessel_state_sub"];
  ros::SubscribeOptions vessel_state_sub_options;
  vessel_state_sub_options = ros::SubscribeOptions::create<rover_pkg::vessel_state>
      (advertiseName.c_str(),
       1000,
       boost::bind(&high_level_controller::vessel_state_sub_OnOneData, this, _1),
       ros::VoidPtr(),
       &this->compQueue);
  this->vessel_state_sub = nh.subscribe(vessel_state_sub_options);

  // Component Publisher - goal_state_pub
  advertiseName = "goal_state";
  if (portGroupMap.find("goal_state_pub") != portGroupMap.end())
    advertiseName += "_" + portGroupMap["goal_state_pub"];
  this->goal_state_pub = nh.advertise<rover_pkg::goal_state>(advertiseName.c_str(), 1000);

  // Init Timer
  ros::TimerOptions timer_options;
  timer_options = 
    ros::TimerOptions
    (ros::Duration(-1),
     boost::bind(&high_level_controller::Init, this, _1),
     &this->compQueue,
     true);
  this->initOneShotTimer = nh.createTimer(timer_options);  
  
  // Component Timer - state_timer
  timer_options = 
    ros::TimerOptions
    (ros::Duration(0.5),
     boost::bind(&high_level_controller::state_timerCallback, this, _1),
     &this->compQueue);
  this->state_timer = nh.createTimer(timer_options);

  // Identify the pwd of Node Executable
  std::string s = node_argv[0];
  std::string exec_path = s;
  std::string delimiter = "/";
  std::string exec, pwd;
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    exec = s.substr(0, pos);
    s.erase(0, pos + delimiter.length());
  }
  exec = s.substr(0, pos);
  pwd = exec_path.erase(exec_path.find(exec), exec.length());
  std::string log_file_path = pwd + nodeName + "." + compName + ".log"; 
  
  // Create the log file & open file stream
  LOGGER.CREATE_FILE(log_file_path);
  
  // Establish log levels of LOGGER
  LOGGER.SET_LOG_LEVELS(logLevels);

  krpci_client.SetName(nodeName + "_" + compName);
  LOGGER.DEBUG("Exiting high_level_controller::startUp");
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new high_level_controller(config,argc,argv);
  }
}
