/*
 * Image Processor Class Definition
 * 
 * Author: Pranav Srinivas Kumar
 * Date: 2014.11.26
 */

#include "agse_package/sample_detector.hpp"

// Filter Global Variables
/*
 * YCRCB FILTER
 */
int hue_min = 184;
int hue_max = 255;
int saturation_min = 0;
int saturation_max = 255;
int value_min = 0;
int value_max = 255;

/*

int hue_min = 190;
int hue_max = 255;
int saturation_min = 190;
int saturation_max = 255;
int value_min = 190;
int value_max = 255;
*/


int min_grayscale_thresh = 190;
int max_grayscale_thresh = 255;

Size hsv_erode_size = Size(20, 20); 
Size hsv_dilate_size = Size(30, 30); 

Size grayscale_erode_size = Size(20, 20);
Size grayscale_dilate_size = Size(30, 30);

// Global callback function for sliders
void slider_update(int, void*){}

void Sample_Detector::create_filter_knobs_slider(){
  namedWindow("Filter Knobs", 0);    
  createTrackbar( "Hue_MIN", "Filter Knobs", &hue_min, hue_max, slider_update);
  createTrackbar( "Hue_MAX", "Filter Knobs", &hue_max, hue_max, slider_update);
  createTrackbar( "Sat_MIN", "Filter Knobs", &saturation_min, saturation_max, slider_update);
  createTrackbar( "Sat_MAX", "Filter Knobs", &saturation_max, saturation_max, slider_update);
  createTrackbar( "Value_MIN", "Filter Knobs", &value_min, value_max, slider_update);
  createTrackbar( "Value_MAX", "Filter Knobs", &value_max, value_max, slider_update);
}

// Setup Webcam Feed
void Sample_Detector::setup_webcam_feed(int cam_id){
  // Start up webcam
  webcam.open(cam_id);

  //set Height and Width of Feed
  webcam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  webcam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

// Show Output Windows
void Sample_Detector::show_windows(){
  imshow("Webcam", webcam_feed);
  imshow("HSV", HSV);
  imshow("Filter", filtered_output);
  imshow("Object Tracker", tracker_output);
}

// Check for User Input
void Sample_Detector::handle_user_input(){
  // Briefly wait for key press
  key_press = cvWaitKey(20);

  // If user presses ESC, quit loop
  if (key_press == 27)
    quit = true;
}

// Main Initialize Function
void Sample_Detector::init(){
}

Mat image;
Mat hsv_image;
Mat hsv_filtered_image;
Mat hsv_tracked_image;
Object_Tracker obj_tracker;

Mat grayscale_image;
Mat grayscale_filtered_image;
Mat grayscale_tracked_image;

Mat equalizeIntensity(const Mat& inputImage)
{
    if(inputImage.channels() >= 3)
    {
        Mat ycrcb;

        cvtColor(inputImage,ycrcb,CV_BGR2YCrCb);

        vector<Mat> channels;
        split(ycrcb,channels);

        equalizeHist(channels[0], channels[0]);

        Mat result;
        merge(channels,ycrcb);

        cvtColor(ycrcb,result,CV_YCrCb2BGR);

        return result;
    }
    return Mat();
}

vector<RotatedRect> hsv_method(Mat &image, Mat& imgMask) {

  //  std::cout << "SAMPLE_DETECTOR::Starting YCrCb METHOD" << std::endl;

   // Convert from RGB TO HSV space
  //  cvtColor(image, hsv_image, COLOR_BGR2HSV);

  Mat ycrcb = equalizeIntensity(image);

  // Filter HSV Image based on slider values
  inRange(ycrcb,
	  Scalar(hue_min, saturation_min, value_min), 
	  Scalar(hue_max, saturation_max, value_max),
	  hsv_filtered_image);
  
  cv::imwrite("Sample-YCrCb-Threshold.png", hsv_filtered_image);

  // Erode and Dilate
  obj_tracker.filter(hsv_filtered_image, hsv_erode_size, hsv_dilate_size);
	
  cv::imwrite("Sample-YCrCb-Filtered.png", hsv_filtered_image);

  //  std::cout << "SAMPLE_DETECTOR::Completed YCrCb METHOD" << std::endl;

  return obj_tracker.track(image,hsv_filtered_image,imgMask);
}

vector<RotatedRect> grayscale_method(Mat& image, Mat& imgMask) {

  //  std::cout << "SAMPLE_DETECTOR::Starting Grayscale METHOD" << std::endl;

  cvtColor(image, grayscale_image, CV_BGR2GRAY);
  Mat eq_gray;
  equalizeHist(grayscale_image,eq_gray);
  cv::imwrite("Sample-04-Grayscale.png", grayscale_image);
  cv::imwrite("Sample-Grayscale-equalized.png", eq_gray);

  threshold(eq_gray, grayscale_filtered_image, min_grayscale_thresh, max_grayscale_thresh, 0);
  cv::imwrite("Sample-05-Grayscale-Threshold.png", grayscale_filtered_image);

  // Erode and Dilate
  obj_tracker.filter(grayscale_filtered_image, grayscale_erode_size, grayscale_dilate_size);
	
  cv::imwrite("Sample-06-Grayscale-Filtered.png", grayscale_filtered_image);

  //  std::cout << "SAMPLE_DETECTOR::Completed Grayscale METHOD" << std::endl;

  return obj_tracker.track(image,grayscale_filtered_image,imgMask);
}

// Main Real-Time Loop
DetectedObject Sample_Detector::run( Mat& image, 
				     Mat& maskOutput,
				     const char* fname)
{
  DetectedObject object;
  object.state = HIDDEN;

  Mat hsvMask = Mat::zeros(image.size(), CV_8UC3);
  Mat grayMask = Mat::zeros(image.size(), CV_8UC3);
  vector<RotatedRect> hsv_tracked_objects = hsv_method(image, hsvMask);
  vector<RotatedRect> gray_tracked_objects = grayscale_method(image, grayMask);
  vector<RotatedRect> tracked_objects;

  if ( hsv_tracked_objects.size() > 0 && gray_tracked_objects.size() > 0 )
    {
      Mat AND_image;
      bitwise_and(hsv_filtered_image, grayscale_filtered_image, AND_image);
      cv::imwrite("Sample-07-Bitwise-AND-Filtered.png", AND_image);

      Mat bitwise_and_tracked;
      tracked_objects = obj_tracker.track(image, AND_image, maskOutput);
      cv::imwrite("Sample-08-Bitwise-AND-Tracked.png", maskOutput);
      std::cout << "OBJECT FOUND!!!!!" << std::endl;
    } else if ( hsv_tracked_objects.size() > 0 )
    {
      tracked_objects = hsv_tracked_objects;
      maskOutput = hsvMask;
    } else if ( gray_tracked_objects.size() > 0 )
    {
      tracked_objects = gray_tracked_objects;
      maskOutput = grayMask;
    }

  if ( tracked_objects.size() > 0 )
    {
      object.state = DETECTED;
      object.x = tracked_objects[0].center.x;
      object.y = tracked_objects[0].center.y;
      object.angle = tracked_objects[0].angle;
      Size objSize = tracked_objects[0].size;
      if ( objSize.height > objSize.width )
	{
	  object.angle += 90.0f;
	}
    }

  return object;
}
