#ifndef PAYLOADBAY_DETECTOR_HPP
#define PAYLOADBAY_DETECTOR_HPP

#include <iostream>
#include "aruco/aruco.h"
#include "aruco/cvdrawingutils.h"
#include <opencv2/highgui/highgui.hpp>

#include "agse_package/detected_object.hpp"

using namespace cv;
using namespace aruco;

class PayloadBay_Detector {
public:
  void init(float msize, const char* camParamFileName);
  DetectedObject run( Mat& image,
		      Mat& maskOutput,
		      const char* fname = "");
private:
  MarkerDetector MDetector;
  // real-world size of the markers
  float MarkerSize;
  // the read-in yml file which describes camera intrinsics
  aruco::CameraParameters CamParam;
};

#endif
