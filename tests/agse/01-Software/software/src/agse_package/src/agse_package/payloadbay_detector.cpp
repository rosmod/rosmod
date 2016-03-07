#include "agse_package/payloadbay_detector.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"

void PayloadBay_Detector::init(float msize, const char* camParamFileName)
{
  MarkerSize = msize;
  CamParam.readFromXMLFile(camParamFileName);
}

DetectedObject PayloadBay_Detector::run( Mat& image, 
					 Mat& maskOutput,
					 const char* fname)
{
  DetectedObject object;
  object.state = HIDDEN;
  std::vector<Marker> Markers;
  std::vector<Point2f> Centers;
  Point2f center(0.0f,0.0f);
  float angle = 0.0f;
  try {
    float width = image.cols;
    float height = image.rows;
    CamParam.resize(Size(height,width));
    
    // Converting to grayscale
    //Mat grayscale;
    //cvtColor(image, grayscale, CV_BGR2GRAY);
    //Mat eq_gray;
    //equalizeHist(grayscale, eq_gray);
    //Mat eq_gray_filtered;
    //threshold(eq_gray, eq_gray_filtered, 130, 255, 0);
    //cv::imwrite("Payload-Grayscale-Thresholded.png", eq_gray_filtered);
    
    MDetector.detect(image, Markers, CamParam, MarkerSize);

    //for each marker, draw info and its boundaries in the image
    for (unsigned int i=0;i<Markers.size();i++) {
      Markers[i].draw(maskOutput,Scalar(0,0,255),4);
      CvDrawingUtils::draw3dAxis(maskOutput,Markers[i],CamParam);
    }

    if (Markers.size() >= 1) {
      for (unsigned int i=0;i<Markers.size();i++) {
	printf("MARKER XY = %f, %f\n",Markers[i].getCenter().x,Markers[i].getCenter().y);
	Centers.push_back(Markers[i].getCenter());
	center += Centers[i];
	if (i>0)
	  angle += atan2(Centers[i].y-Centers[i-1].y,Centers[i].x-Centers[i-1].x);
      }
      if (Centers.size() > 1)
	{
	  angle = angle/((float)(Centers.size()-1)) * 180.0f / M_PI;
	  center = center * (1.0f / (float)Centers.size());
	}
      else
	angle = 0;


      Scalar color = Scalar(255, 
			    255, 
			    255);
      circle(image,center,20.0f,color,-1);

      object.state = DETECTED;
      if (Markers.size() == 1)
	object.state = PARTIAL;
      //      printf("CENTER XY = %f,%f\n",center.x,center.y);
      object.x = center.x;
      object.y = center.y;
      object.angle = angle;
    }

    //cv::imwrite("Payload-THresholded.png", MDetector.getThresholdedImage());
    int nameLen = 0;
    if ( (nameLen = strlen(fname)) > 0 )
      {
	char edgeName[nameLen + 20];
	sprintf(edgeName,"%s_edges.png",fname);
	cv::imwrite(edgeName, MDetector.getThresholdedImage());
      }
  } catch (std::exception &ex)
    {
      printf("ERROR: caught exception detecting markers: %s\n",ex.what());
      object.state = HIDDEN;
    }
  return object;
}
