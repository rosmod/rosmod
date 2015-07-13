#include "ros/ros.h"
#include "sequential_leds_app/led_state.h"

#include <sstream>

unsigned int ledPin;
bool ledState;
bool invert;

ros::Publisher led_state_pub;
ros::Subscriber led_state_sub;

void led_state_sub_OnOneData(const sequential_leds_app::led_state::ConstPtr& received_data)
{
  // Business Logic for led_state_sub Subscriber
  ledState = received_data->state;
  ledPin = received_data->pin;
  printf("Received new state %d for pin %d\n",ledState,ledPin);

  ros::Duration(0.5).sleep();

  sequential_leds_app::led_state nextLEDState;
  if (invert)
    ledState = !ledState;
  printf("Publishing new state %d for pin %d\n",ledState,ledPin);
  nextLEDState.pin = ledPin;
  nextLEDState.state = ledState;
  led_state_pub.publish(nextLEDState);
}

int main(int argc, char **argv)
{
  ledPin = 0;  // if using USR LEDs, use 0-3, else use real pin values
  ledState = false;
  invert = false;
 
  bool start = false;

  std::string nodeName = "simplePubSubNode";
  std::string pubName;
  std::string subName;
  
  for (int i = 0; i < argc; i++)
    {
      if (!strcmp(argv[i], "--nodename"))
	{
	  nodeName = argv[i+1];
	}
      if (!strcmp(argv[i], "--pin"))
	{
	  ledPin = atoi(argv[i+1]);
	}
      if (!strcmp(argv[i], "--invert"))
	{
	  invert = true;
	}
      if (!strcmp(argv[i], "--start"))
	{
	  start = true;
	}
      if (!strcmp(argv[i], "--pubname"))
	{
	  pubName = argv[i+1];
	}
      if (!strcmp(argv[i], "--subname"))
	{
	  subName = argv[i+1];
	}
    }

  ros::init(argc, argv, nodeName.c_str());
  ros::NodeHandle nh;

  std::string advertiseName;

  // Component Subscriber - led_state_sub
  advertiseName = "led_state" + subName;
  led_state_sub = nh.subscribe(advertiseName.c_str(), 1000, led_state_sub_OnOneData);

  // Component Publisher - led_state_pub
  advertiseName = "led_state" + pubName;
  led_state_pub = nh.advertise<sequential_leds_app::led_state>(advertiseName.c_str(), 1000);

  if (start)
    {
      ledState = true;
      printf("Publishing new state %d for pin %d\n",ledState,ledPin);
      sequential_leds_app::led_state nextLEDState;
      nextLEDState.pin = ledPin;
      nextLEDState.state = ledState;
      led_state_pub.publish(nextLEDState);
    }

  ros::Rate loop_rate(10);

  while (ros::ok())
    {
      ros::spinOnce();

      loop_rate.sleep();
    }


  return 0;
}
