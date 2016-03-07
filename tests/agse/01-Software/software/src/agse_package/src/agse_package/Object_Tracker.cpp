/*
 * Object Tracker Class Definition
 * 
 * Author: Pranav Srinivas Kumar
 * Date: 2014.11.26
 */

#include "agse_package/Object_Tracker.h"

float minArea = 150*150;
float maxArea = 200000;
float minRatio = 1.5f;
float maxRatio = 5.0f;
float minAreaRatio = 1.0f;
float maxAreaRatio = 1.3f;

RNG rng(12345);

// Constructor
Object_Tracker::Object_Tracker(){
  max_objects = 20;
  min_area = minArea;
  max_area = maxArea;
  object_angle = 0.0;
}

// Getters
float Object_Tracker::get_object_angle(){
  return object_angle;
}

// Setters
void Object_Tracker::set_min_area(int this_area){
  min_area = this_area;
}

void Object_Tracker::set_max_area(int this_area){
  max_area = this_area;
}

void Object_Tracker::set_max_objects(int this_number){
  max_objects = this_number;
}


// Helper Function INT to STRING
string intToString(int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Filter the input image matrix using erode and dilate functions
void Object_Tracker::filter(Mat &filtered_output, Size erode_size, Size dilate_size){
  // Erode Element - 3px*3px rectangle
  Mat erodeElement = getStructuringElement( MORPH_RECT, erode_size);
  // Dilate with larger element
  Mat dilateElement = getStructuringElement( MORPH_RECT, dilate_size);

  // ERODE
  erode(filtered_output, filtered_output, erodeElement);
  erode(filtered_output, filtered_output, erodeElement);

  // DILATE
  dilate(filtered_output, filtered_output, dilateElement);
  dilate(filtered_output, filtered_output, dilateElement);
}

Mat Object_Tracker::PolygonApproximate(Mat filtered_output)
{
}

// Track Object
vector<RotatedRect> Object_Tracker::track(Mat& raw_image, Mat& filtered_output, Mat& tracked_output){

  // Center of Object
  int x = 0, y  = 0;

  // Copy to temporary matrix
  Mat temp;
  filtered_output.copyTo(temp);

  // Vectors needed by findContours
  vector< vector<Point> > contours;
  vector< vector<Point> > contours_filtered;
  vector<Vec4i> hierarchy;

  // Find Contour
  findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

  // Use moments to find object
  double refArea = 0;
  bool objectFound = false;

  std::cout << "Hierarchy Size: " << hierarchy.size() << std::endl;

  if (hierarchy.size() > 0){
    int numObjects = hierarchy.size();

    //	if(numObjects < max_objects){
    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
      Moments moment = moments((cv::Mat)contours[index]);
      double area = moment.m00;
      if(area > min_area && area< max_area && area > refArea){
	x = moment.m10/area;
	y = moment.m01/area;
	objectFound = true;
	refArea = area;
	contours_filtered.push_back(contours[index]);
      }
      else 
	objectFound = false;
    }
    //}
  }

  contours_filtered = contours;

  vector<Vec4i> hierarchy_filtered;

  /*
   * TIME TO DRAW THE OBJECT CONTOUR
   */
  //findContours(temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  // Approximate contour to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly(contours_filtered.size());
  vector<RotatedRect> boundRect(contours_filtered.size());

  vector<vector<Point>> tracked_contours;
  vector<RotatedRect> tracked_BB;
  vector<float> objectAngle;
	
  for(int i = 0; i < contours_filtered.size(); i++){
    approxPolyDP(Mat(contours_filtered[i]), contours_poly[i], 3, true);
    boundRect[i] = minAreaRect(Mat (contours_poly[i]));

    float imgwidth = raw_image.cols;
    float imgheight = raw_image.rows;
    bool onImageEdge = false;
    Point2f rectPoints[4];
    boundRect[i].points(rectPoints);
    float epsilon = 5.0f;
    for (int j=0;j<4;j++)
      {
	if ( rectPoints[j].x <= epsilon || rectPoints[j].x >= (imgwidth-epsilon) ||
	     rectPoints[j].y <= epsilon || rectPoints[j].y >= (imgheight-epsilon) )
	  {
	    onImageEdge = true;
	    break;
	  }
      }

    float area, cArea, width, height, aspectRatio, areaRatio;
    width = max(boundRect[i].size.width,boundRect[i].size.height);
    height = min(boundRect[i].size.width,boundRect[i].size.height);
    aspectRatio = width/height;
    area = width * height;
    cArea = contourArea(contours_poly[i]);
    areaRatio = area/cArea;
    //    printf("area: %f, cArea: %f, areaRatio: %f,aspectRatio: %f, h: %f, w: %f, angle: %f\n",area,cArea,areaRatio,aspectRatio,height,width,boundRect[i].angle);
    if ( area > minArea && area < maxArea && 
	 aspectRatio > minRatio && aspectRatio < maxRatio &&
	 areaRatio > minAreaRatio && areaRatio < maxAreaRatio &&
      	 !onImageEdge )
      {
	tracked_contours.push_back(contours_poly[i]);
	tracked_BB.push_back(boundRect[i]);
	objectAngle.push_back(boundRect[i].angle);
      } else 
      {
      }
  }

  // Draw polygonal contour + bounding rects + circles
  for (int i = 0; i < tracked_contours.size(); i++){
    Scalar color = Scalar(rng.uniform(0, 255), 
			  rng.uniform(0, 255), 
			  rng.uniform(0,255));
    drawContours(tracked_output, tracked_contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
    Point2f rectPoints[4];
    tracked_BB[i].points(rectPoints);
    for (int j=0;j<4;j++) {
      line(tracked_output, rectPoints[j], rectPoints[(j+1)%4], color, 2, 8, 0);
    }
  }
  return tracked_BB;
}

