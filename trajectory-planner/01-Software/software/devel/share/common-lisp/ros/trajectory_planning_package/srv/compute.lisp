; Auto-generated. Do not edit!


(cl:in-package trajectory_planning_package-srv)


;//! \htmlinclude compute-request.msg.html

(cl:defclass <compute-request> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (z
    :reader z
    :initarg :z
    :type cl:float
    :initform 0.0))
)

(cl:defclass compute-request (<compute-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <compute-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'compute-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name trajectory_planning_package-srv:<compute-request> is deprecated: use trajectory_planning_package-srv:compute-request instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <compute-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader trajectory_planning_package-srv:x-val is deprecated.  Use trajectory_planning_package-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <compute-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader trajectory_planning_package-srv:y-val is deprecated.  Use trajectory_planning_package-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'z-val :lambda-list '(m))
(cl:defmethod z-val ((m <compute-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader trajectory_planning_package-srv:z-val is deprecated.  Use trajectory_planning_package-srv:z instead.")
  (z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <compute-request>) ostream)
  "Serializes a message object of type '<compute-request>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <compute-request>) istream)
  "Deserializes a message object of type '<compute-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'z) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<compute-request>)))
  "Returns string type for a service object of type '<compute-request>"
  "trajectory_planning_package/computeRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'compute-request)))
  "Returns string type for a service object of type 'compute-request"
  "trajectory_planning_package/computeRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<compute-request>)))
  "Returns md5sum for a message object of type '<compute-request>"
  "db6eac0cd8499c2f814eda61ea5f8077")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'compute-request)))
  "Returns md5sum for a message object of type 'compute-request"
  "db6eac0cd8499c2f814eda61ea5f8077")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<compute-request>)))
  "Returns full string definition for message of type '<compute-request>"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'compute-request)))
  "Returns full string definition for message of type 'compute-request"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <compute-request>))
  (cl:+ 0
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <compute-request>))
  "Converts a ROS message object to a list"
  (cl:list 'compute-request
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':z (z msg))
))
;//! \htmlinclude compute-response.msg.html

(cl:defclass <compute-response> (roslisp-msg-protocol:ros-message)
  ((result
    :reader result
    :initarg :result
    :type cl:float
    :initform 0.0))
)

(cl:defclass compute-response (<compute-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <compute-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'compute-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name trajectory_planning_package-srv:<compute-response> is deprecated: use trajectory_planning_package-srv:compute-response instead.")))

(cl:ensure-generic-function 'result-val :lambda-list '(m))
(cl:defmethod result-val ((m <compute-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader trajectory_planning_package-srv:result-val is deprecated.  Use trajectory_planning_package-srv:result instead.")
  (result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <compute-response>) ostream)
  "Serializes a message object of type '<compute-response>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'result))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <compute-response>) istream)
  "Deserializes a message object of type '<compute-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'result) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<compute-response>)))
  "Returns string type for a service object of type '<compute-response>"
  "trajectory_planning_package/computeResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'compute-response)))
  "Returns string type for a service object of type 'compute-response"
  "trajectory_planning_package/computeResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<compute-response>)))
  "Returns md5sum for a message object of type '<compute-response>"
  "db6eac0cd8499c2f814eda61ea5f8077")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'compute-response)))
  "Returns md5sum for a message object of type 'compute-response"
  "db6eac0cd8499c2f814eda61ea5f8077")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<compute-response>)))
  "Returns full string definition for message of type '<compute-response>"
  (cl:format cl:nil "float64 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'compute-response)))
  "Returns full string definition for message of type 'compute-response"
  (cl:format cl:nil "float64 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <compute-response>))
  (cl:+ 0
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <compute-response>))
  "Converts a ROS message object to a list"
  (cl:list 'compute-response
    (cl:cons ':result (result msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'compute)))
  'compute-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'compute)))
  'compute-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'compute)))
  "Returns string type for a service object of type '<compute>"
  "trajectory_planning_package/compute")