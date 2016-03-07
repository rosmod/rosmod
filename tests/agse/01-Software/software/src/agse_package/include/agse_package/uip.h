#ifndef UIP_H
#define UIP_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <stdarg.h>
#include <ctime>

#include <iostream>
#include <fstream>
using namespace std;
IplImage *DispImage;
IplImage *Mode_1;    
IplImage *Mode_2;    
IplImage *Mode_3;    
IplImage *Mode_4;    

void cvShowManyImages(char* title, int nArgs, ...) {

  // img - Used for getting the arguments 
  IplImage *img;

  int size;
  int i;
  int m, n;
  int x, y;

  // w - Maximum number of images in a row 
  // h - Maximum number of images in a column 
  int w, h;

  // scale - How much we have to resize the image
  float scale;
  int max;

  // If the number of arguments is lesser than 0 or greater than 12
  // return without displaying 
  if(nArgs <= 0) {
    printf("Number of arguments too small....\n");
    return;
  }
  else if(nArgs > 12) {
    printf("Number of arguments too large....\n");
    return;
  }
  
  // Determine the size of the image, 
  // and the number of rows/cols 
  // from number of arguments 
  else if (nArgs == 1) {
    w = h = 1;
    size = 300;
  }
  else if (nArgs == 2) {
    w = 2; h = 1;
    size = 300;
  }
  else if (nArgs == 3 || nArgs == 4) {
    w = 2; h = 2;
    size = 400;
  }
  else if (nArgs == 5 || nArgs == 6) {
    w = 3; h = 2;
    size = 200;
  }
  else if (nArgs == 7 || nArgs == 8) {
    w = 4; h = 2;
    size = 200;
  }
  else {
    w = 4; h = 3;
    size = 150;
  }

  // Create a new 3 channel image
  DispImage = cvCreateImage( cvSize(800, 480), 8, 3);
  //  DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, 3 );

  // Used to get the arguments passed
  va_list args;
  va_start(args, nArgs);

  // Loop for nArgs number of arguments
  for (i = 0, m = 1, n = 1; i < nArgs; i++, m += 400) {
    // Get the Pointer to the IplImage
    img = va_arg(args, IplImage*);

    // Check whether it is NULL or not
    // If it is NULL, release the image, and return
    if(img == 0) {
      printf("Invalid arguments");
      cvReleaseImage(&DispImage);
      return;
    }

    // Find the width and height of the image
    x = img->width;
    y = img->height;
  
    // Find whether height or width is greater in order to resize the image
    max = (x > y)? x: y;

    // Find the scaling factor to resize the image
    scale = (float) ( (float) max / size );

    // Used to Align the images
    if( i % w == 0 && m!= 1) {
      m = 1;
      n+= 245;
    }

    // Set the image ROI to display the current image
    cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));

    // Resize the input image and copy the it to the Single Big Image
    cvResize(img, DispImage);

    // Reset the ROI in order to display the next image
    cvResetImageROI(DispImage);
  }

  // Create a new window, and show the Single Big Image
  cvNamedWindow( title, 1 );
  cvSetWindowProperty(title, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

  
  // Add font to image
  CvFont sample_font = cvFontQt("Arial", 24, cvScalar(255, 0, 0), CV_FONT_BLACK, CV_STYLE_NORMAL, 0);
  CvFont sample_state = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_state_value = cvFontQt("Arial", 16, cvScalar(0, 0, 255), CV_FONT_BLACK, CV_STYLE_NORMAL, 0);
  CvFont sample_center = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_center_value = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_radius = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_radius_value = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_angle = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_angle_value = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_depth = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont sample_depth_value = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);

  CvFont pb_font = cvFontQt("Arial", 24, cvScalar(255, 0, 0), CV_FONT_BLACK, CV_STYLE_NORMAL, 0);
  CvFont pb_state = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont pb_state_value = cvFontQt("Arial", 16, cvScalar(0, 0, 255), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont pb_center = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont pb_radius = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont pb_angle = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);
  CvFont pb_depth = cvFontQt("Arial", 16, cvScalarAll(0), CV_FONT_BOLD, CV_STYLE_NORMAL, 0);

  // Sample Detection Text
  cvAddText (DispImage, "Sample Detection", cvPoint(410, 40), &sample_font);
  cvAddText (DispImage, "Detection State       : ", cvPoint(410, 80), &sample_state);  
  cvAddText (DispImage, "Searching...", cvPoint(650, 80), &sample_state_value);  
  cvAddText (DispImage, "Sample Angle φ      : ", cvPoint(410, 110), &sample_center);  
  cvAddText (DispImage, "Arm Radius r           : ", cvPoint(410, 140), &sample_radius);  
  cvAddText (DispImage, "Arm Orientation θ   : ", cvPoint(410, 170), &sample_angle);  
  cvAddText (DispImage, "Arm Depth z            : ", cvPoint(410, 200), &sample_depth);  

  // Payload Bay Detection Text
  cvAddText (DispImage, "Payload Bay Detection", cvPoint(410, 280), &pb_font);
  cvAddText (DispImage, "Detection State       : ", cvPoint(410, 320), &pb_state);  
  cvAddText (DispImage, "Searching...", cvPoint(650, 320), &pb_state_value);  
  cvAddText (DispImage, "Bay Center O           : ", cvPoint(410, 350), &pb_center);  
  cvAddText (DispImage, "Arm Radius r           : ", cvPoint(410, 380), &pb_radius);  
  cvAddText (DispImage, "Arm Orientation θ   : ", cvPoint(410, 410), &pb_angle);  
  cvAddText (DispImage, "Arm Depth z            : ", cvPoint(410, 440), &pb_depth);  
  /*
  unsigned int microseconds = 3000000;

  while(true) {
  cvShowManyImages("UIP", 4, img1, img2, img3, img4);
  usleep(microseconds);
  }
  */

   cvShowImage( title, DispImage);

   //   cvShowImage(title, DispImage);
   /*
    cvWaitKey();
   cvDestroyWindow(title);

  // End the number of arguments
   va_end(args);

  // Release the Image Memory
   cvReleaseImage(&DispImage);
   */
}

#endif
