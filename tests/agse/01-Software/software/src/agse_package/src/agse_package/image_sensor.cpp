#include "agse_package/image_sensor.hpp"

//# Start User Globals Marker
struct buffer      *buffers;
struct v4l2_buffer  buf;
int                 n_buffers;

static void xioctl(int fh, int request, void *arg)
{
  int r;
  do {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));
  if (r == -1) {
    fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
//# End User Globals Marker

// Initialization Function
//# Start Init Marker
void image_sensor::init_timer_operation(const NAMESPACE::TimerEvent& event)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_sensor::init_timer_operation");
#endif
  // Initialize Here
  paused = true;
  sprintf(videoDevice,"/dev/video0");
  width = 1920;
  height = 1080;

  struct v4l2_format              fmt;
  struct v4l2_requestbuffers      reqBufs;

  videoFD = v4l2_open(videoDevice, O_RDWR | O_NONBLOCK, 0);
  if (videoFD < 0) {
    perror("Cannot open device");
    exit(EXIT_FAILURE);
  }

  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width;
  fmt.fmt.pix.height      = height;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  xioctl(videoFD, VIDIOC_S_FMT, &fmt);
  if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_BGR24) {
    printf("Libv4l didn't accept BGR24 format. Can't proceed.\n");
    exit(EXIT_FAILURE);
  }
  if ((fmt.fmt.pix.width != width) || (fmt.fmt.pix.height != height))
    printf("Warning: driver is sending image at %dx%d\n",
	   fmt.fmt.pix.width, fmt.fmt.pix.height);

  CLEAR(reqBufs);
  reqBufs.count = 2;
  reqBufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  reqBufs.memory = V4L2_MEMORY_MMAP;
  xioctl(videoFD, VIDIOC_REQBUFS, &reqBufs);

  buffers = (buffer *)calloc(reqBufs.count, sizeof(*buffers));
  for (n_buffers = 0; n_buffers < reqBufs.count; ++n_buffers) {
    CLEAR(buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;

    xioctl(videoFD, VIDIOC_QUERYBUF, &buf);

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
					 PROT_READ | PROT_WRITE, MAP_SHARED,
					 videoFD, buf.m.offset);

    if (MAP_FAILED == buffers[n_buffers].start) {
      perror("mmap");
      exit(EXIT_FAILURE);
    }
  }

  // Command line args for servo control
  for (int i = 0; i < node_argc; i++) 
    {
      if (!strcmp(node_argv[i], "-unpaused"))
	paused = false;
    }
  // Stop Init Timer
  init_timer.stop();
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_sensor::init_timer_operation");
#endif  
}
//# End Init Marker



// Subscriber Operation - controlInputs_sub
//# Start controlInputs_sub_operation Marker
void image_sensor::controlInputs_sub_operation(const agse_package::controlInputs::ConstPtr& received_data)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_sensor::controlInputs_sub_operation");
#endif
  // Business Logic for controlInputs_sub_operation
  paused = received_data->paused;
  ROS_INFO( paused ? "Image Sensor paused!" : "Image Sensor Unpaused!" );
  
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_sensor::controlInputs_sub_operation");
#endif
}
//# End controlInputs_sub_operation Marker

// Server Operation - captureImage_server
//# Start captureImage_operation Marker
bool image_sensor::captureImage_operation(agse_package::captureImage::Request  &req,
  agse_package::captureImage::Response &res)
{
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Entering image_sensor::captureImage_operation");
#endif
  // Business Logic for captureImage_server_operation
    if (!paused)
    {
      // Business Logic for captureImage_server Server providing captureImage Service
      fd_set                          fds;
      struct timeval                  tv;
      enum v4l2_buf_type              type;
      int                             r;

      for (int i = 0; i < n_buffers; ++i) {
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = i;
	xioctl(videoFD, VIDIOC_QBUF, &buf);
      }
      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      xioctl(videoFD, VIDIOC_STREAMON, &type);
      do {
	FD_ZERO(&fds);
	FD_SET(videoFD, &fds);

	/* Timeout. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	r = select(videoFD + 1, &fds, NULL, NULL, &tv);
      } while ((r == -1 && (errno = EINTR)));
      if (r == -1) {
	perror("select");
	return false;
      }

      CLEAR(buf);
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      xioctl(videoFD, VIDIOC_DQBUF, &buf);

      //fwrite(buffers[buf.index].start, buf.bytesused, 1, fout);

      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      xioctl(videoFD, VIDIOC_STREAMOFF, &type);

      res.imgVector.reserve(buf.bytesused);
      std::copy(&((unsigned char *)buffers[buf.index].start)[0],
		&((unsigned char *)buffers[buf.index].start)[0] + buf.bytesused,
		back_inserter(res.imgVector));
      res.width = width;
      res.height = height;
		
      return true;
    }
  return false;
#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->log("DEBUG", "Exiting image_sensor::captureImage_operation");
#endif
  return true;
}
//# End captureImage_operation Marker


// Destructor - Cleanup Ports & Timers
image_sensor::~image_sensor()
{
  controlInputs_sub.shutdown();
  captureImage_server.shutdown();
  //# Start Destructor Marker
  //# End Destructor Marker
}

// Startup - Setup Component Ports & Timers
void image_sensor::startUp()
{
  NAMESPACE::NodeHandle nh;
  std::string advertiseName;

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
  std::string log_file_path = pwd + config.nodeName + "." + config.compName + ".log"; 

  logger->create_file("/var/log/" + config.nodeName + "." + config.compName + ".log");
  logger->set_is_periodic(config.is_periodic_logging);
  logger->set_max_log_unit(config.periodic_log_unit);

#ifdef USE_ROSMOD
  comp_queue.ROSMOD_LOGGER->create_file("/var/log/ROSMOD_DEBUG." + config.nodeName + "." + config.compName + ".log");
  comp_queue.ROSMOD_LOGGER->set_is_periodic(config.is_periodic_logging);
  comp_queue.ROSMOD_LOGGER->set_max_log_unit(config.periodic_log_unit);
#endif    
  
#ifdef USE_ROSMOD 
  this->comp_queue.scheduling_scheme = config.schedulingScheme;
  rosmod::ROSMOD_Callback_Options callback_options;
#endif  
  // Configure all provided services associated with this component
#ifdef USE_ROSMOD  
  callback_options.alias = "captureImage_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec =0;
  callback_options.deadline.nsec = 100000000;
#endif    
  // Component Server - captureImage_server
  advertiseName = "captureImage";
  if (config.portGroupMap.find("captureImage_server") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["captureImage_server"];
  NAMESPACE::AdvertiseServiceOptions captureImage_server_server_options;
  captureImage_server_server_options = NAMESPACE::AdvertiseServiceOptions::create<agse_package::captureImage>
      (advertiseName.c_str(),
       boost::bind(&image_sensor::captureImage_operation, this, _1, _2),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD       
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif
  this->captureImage_server = nh.advertiseService(captureImage_server_server_options);

  if (config.num_comps_to_sync > 1 )
    {
      // Synchronize components now that all publishers and servers have been initialized
      this->comp_sync_pub = nh.advertise<std_msgs::Bool>("component_synchronization", 1000);
  
#ifdef USE_ROSMOD  
      rosmod::SubscribeOptions comp_sync_sub_options;
      rosmod::ROSMOD_Callback_Options sync_callback_options;
#else
      ros::SubscribeOptions comp_sync_sub_options;
#endif
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      comp_sync_sub_options = NAMESPACE::SubscribeOptions::create<std_msgs::Bool>
	("component_synchronization",
	 1000,
	 boost::bind(&image_sensor::component_sync_operation, this, _1),
	 NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD     
	 &this->comp_queue,
	 sync_callback_options);
#else
         &this->comp_queue);
#endif
      this->comp_sync_sub = nh.subscribe(comp_sync_sub_options);

      ros::Time now = ros::Time::now();
      while ( this->comp_sync_sub.getNumPublishers() < this->config.num_comps_to_sync &&
	      (ros::Time::now() - now) < ros::Duration(config.comp_sync_timeout))
	ros::Duration(0.1).sleep();
      ros::Duration(config.comp_sync_timeout/2.0).sleep();
      this->comp_sync_sub.shutdown();  
      this->comp_sync_pub.shutdown();
    }

  // Configure all subscribers associated with this component
#ifdef USE_ROSMOD 
  callback_options.alias = "controlInputs_sub_operation";
  callback_options.priority = 50;
  callback_options.deadline.sec = 0;
  callback_options.deadline.nsec = 100000000;
#endif  
  // Component Subscriber - controlInputs_sub
  advertiseName = "controlInputs";
  if (config.portGroupMap.find("controlInputs_sub") != config.portGroupMap.end())
    advertiseName += "_" + config.portGroupMap["controlInputs_sub"];
  NAMESPACE::SubscribeOptions controlInputs_sub_options;
  controlInputs_sub_options = NAMESPACE::SubscribeOptions::create<agse_package::controlInputs>
      (advertiseName.c_str(),
       1000,
       boost::bind(&image_sensor::controlInputs_sub_operation, this, _1),
       NAMESPACE::VoidPtr(),
#ifdef USE_ROSMOD
       &this->comp_queue,
       callback_options);
#else
       &this->comp_queue);
#endif 
  controlInputs_sub_options.transport_hints = NAMESPACE::TransportHints().udp();
  this->controlInputs_sub = nh.subscribe(controlInputs_sub_options);

  // Init Timer
#ifdef USE_ROSMOD    
  callback_options.alias = "init_timer_operation";
  callback_options.priority = 99;
  callback_options.deadline.sec = 1;
  callback_options.deadline.nsec = 0;
#endif
  NAMESPACE::TimerOptions timer_options;
  timer_options = 
    NAMESPACE::TimerOptions
    (ros::Duration(-1),
     boost::bind(&image_sensor::init_timer_operation, this, _1),
     &this->comp_queue,
#ifdef USE_ROSMOD     
     callback_options,
#endif     
     true,
     false); 
  this->init_timer = nh.createTimer(timer_options);
  this->init_timer.stop();


  this->init_timer.start();
}

extern "C" {
  Component *maker(ComponentConfig &config, int argc, char **argv) {
    return new image_sensor(config,argc,argv);
  }
}
