; Auto-generated. Do not edit!


(cl:in-package uav_package-msg)


;//! \htmlinclude sensor_state.msg.html

(cl:defclass <sensor_state> (roslisp-msg-protocol:ros-message)
  ((accX
    :reader accX
    :initarg :accX
    :type cl:integer
    :initform 0)
   (accY
    :reader accY
    :initarg :accY
    :type cl:integer
    :initform 0)
   (accZ
    :reader accZ
    :initarg :accZ
    :type cl:integer
    :initform 0)
   (latitude
    :reader latitude
    :initarg :latitude
    :type cl:float
    :initform 0.0)
   (longitude
    :reader longitude
    :initarg :longitude
    :type cl:float
    :initform 0.0)
   (heading
    :reader heading
    :initarg :heading
    :type cl:float
    :initform 0.0)
   (speed
    :reader speed
    :initarg :speed
    :type cl:float
    :initform 0.0)
   (altitude
    :reader altitude
    :initarg :altitude
    :type cl:float
    :initform 0.0))
)

(cl:defclass sensor_state (<sensor_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <sensor_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'sensor_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name uav_package-msg:<sensor_state> is deprecated: use uav_package-msg:sensor_state instead.")))

(cl:ensure-generic-function 'accX-val :lambda-list '(m))
(cl:defmethod accX-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:accX-val is deprecated.  Use uav_package-msg:accX instead.")
  (accX m))

(cl:ensure-generic-function 'accY-val :lambda-list '(m))
(cl:defmethod accY-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:accY-val is deprecated.  Use uav_package-msg:accY instead.")
  (accY m))

(cl:ensure-generic-function 'accZ-val :lambda-list '(m))
(cl:defmethod accZ-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:accZ-val is deprecated.  Use uav_package-msg:accZ instead.")
  (accZ m))

(cl:ensure-generic-function 'latitude-val :lambda-list '(m))
(cl:defmethod latitude-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:latitude-val is deprecated.  Use uav_package-msg:latitude instead.")
  (latitude m))

(cl:ensure-generic-function 'longitude-val :lambda-list '(m))
(cl:defmethod longitude-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:longitude-val is deprecated.  Use uav_package-msg:longitude instead.")
  (longitude m))

(cl:ensure-generic-function 'heading-val :lambda-list '(m))
(cl:defmethod heading-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:heading-val is deprecated.  Use uav_package-msg:heading instead.")
  (heading m))

(cl:ensure-generic-function 'speed-val :lambda-list '(m))
(cl:defmethod speed-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:speed-val is deprecated.  Use uav_package-msg:speed instead.")
  (speed m))

(cl:ensure-generic-function 'altitude-val :lambda-list '(m))
(cl:defmethod altitude-val ((m <sensor_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:altitude-val is deprecated.  Use uav_package-msg:altitude instead.")
  (altitude m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <sensor_state>) ostream)
  "Serializes a message object of type '<sensor_state>"
  (cl:let* ((signed (cl:slot-value msg 'accX)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'accY)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'accZ)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'latitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'longitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'heading))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'speed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'altitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <sensor_state>) istream)
  "Deserializes a message object of type '<sensor_state>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'accX) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'accY) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'accZ) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'latitude) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'longitude) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'heading) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'speed) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'altitude) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<sensor_state>)))
  "Returns string type for a message object of type '<sensor_state>"
  "uav_package/sensor_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'sensor_state)))
  "Returns string type for a message object of type 'sensor_state"
  "uav_package/sensor_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<sensor_state>)))
  "Returns md5sum for a message object of type '<sensor_state>"
  "ab3b1612f4b09421b105895e2bd4ce5d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'sensor_state)))
  "Returns md5sum for a message object of type 'sensor_state"
  "ab3b1612f4b09421b105895e2bd4ce5d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<sensor_state>)))
  "Returns full string definition for message of type '<sensor_state>"
  (cl:format cl:nil "int32 accX~%int32 accY~%int32 accZ~%float32 latitude~%float32 longitude~%float32 heading~%float32 speed~%float32 altitude~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'sensor_state)))
  "Returns full string definition for message of type 'sensor_state"
  (cl:format cl:nil "int32 accX~%int32 accY~%int32 accZ~%float32 latitude~%float32 longitude~%float32 heading~%float32 speed~%float32 altitude~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <sensor_state>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <sensor_state>))
  "Converts a ROS message object to a list"
  (cl:list 'sensor_state
    (cl:cons ':accX (accX msg))
    (cl:cons ':accY (accY msg))
    (cl:cons ':accZ (accZ msg))
    (cl:cons ':latitude (latitude msg))
    (cl:cons ':longitude (longitude msg))
    (cl:cons ':heading (heading msg))
    (cl:cons ':speed (speed msg))
    (cl:cons ':altitude (altitude msg))
))
