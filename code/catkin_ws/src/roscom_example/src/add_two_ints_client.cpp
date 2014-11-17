#include "ros/ros.h"
#include "roscom_example/AddTwoInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_two_ints_client");
  if (argc != 3)
    {
      ROS_INFO("usage: add_two_ints_client X Y");
      return 1;
    }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<roscom_example::AddTwoInts>("add_two_ints");
  roscom_example::AddTwoInts srv;
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);
  while (!client.waitForExistence(ros::Duration(0.1))) { // -1 here means wait forever
    ROS_INFO("Waiting for server to come up");  // unnecessary as the call prints info anyway
  }
  if (client.call(srv))
    {
      ROS_INFO("Sum: %ld", (long int)srv.response.sum);
    }
  else
    {
      ROS_ERROR("Failed to call service add_two_ints");
      return 1;
    }

  return 0;
}
