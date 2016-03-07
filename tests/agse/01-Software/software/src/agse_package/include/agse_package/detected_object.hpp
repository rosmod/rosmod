#ifndef DETECTED_OBJECT_HPP
#define DETECTED_OBJECT_HPP

enum ObjectState {
  HIDDEN,    // Object was not found
  DETECTED,  // Object was completely found
  PARTIAL,   // Part of the object was found
};


class DetectedObject
{
public:
  ObjectState state;  // did the function find the object? or part of it?
  float x,y;          // object center coordinates in image space
  float angle;        // image-space angle of the object
};

#endif
