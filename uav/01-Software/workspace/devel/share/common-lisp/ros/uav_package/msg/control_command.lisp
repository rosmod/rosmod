; Auto-generated. Do not edit!


(cl:in-package uav_package-msg)


;//! \htmlinclude control_command.msg.html

(cl:defclass <control_command> (roslisp-msg-protocol:ros-message)
  ((newHeading
    :reader newHeading
    :initarg :newHeading
    :type cl:float
    :initform 0.0)
   (newAltitude
    :reader newAltitude
    :initarg :newAltitude
    :type cl:float
    :initform 0.0)
   (command
    :reader command
    :initarg :command
    :type cl:string
    :initform ""))
)

(cl:defclass control_command (<control_command>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <control_command>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'control_command)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name uav_package-msg:<control_command> is deprecated: use uav_package-msg:control_command instead.")))

(cl:ensure-generic-function 'newHeading-val :lambda-list '(m))
(cl:defmethod newHeading-val ((m <control_command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:newHeading-val is deprecated.  Use uav_package-msg:newHeading instead.")
  (newHeading m))

(cl:ensure-generic-function 'newAltitude-val :lambda-list '(m))
(cl:defmethod newAltitude-val ((m <control_command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:newAltitude-val is deprecated.  Use uav_package-msg:newAltitude instead.")
  (newAltitude m))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <control_command>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader uav_package-msg:command-val is deprecated.  Use uav_package-msg:command instead.")
  (command m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <control_command>) ostream)
  "Serializes a message object of type '<control_command>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'newHeading))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'newAltitude))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'command))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'command))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <control_command>) istream)
  "Deserializes a message object of type '<control_command>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'newHeading) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'newAltitude) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'command) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<control_command>)))
  "Returns string type for a message object of type '<control_command>"
  "uav_package/control_command")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'control_command)))
  "Returns string type for a message object of type 'control_command"
  "uav_package/control_command")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<control_command>)))
  "Returns md5sum for a message object of type '<control_command>"
  "60e397f8b340625273e563eebda2ee59")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'control_command)))
  "Returns md5sum for a message object of type 'control_command"
  "60e397f8b340625273e563eebda2ee59")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<control_command>)))
  "Returns full string definition for message of type '<control_command>"
  (cl:format cl:nil "float32 newHeading~%float32 newAltitude ~%string command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'control_command)))
  "Returns full string definition for message of type 'control_command"
  (cl:format cl:nil "float32 newHeading~%float32 newAltitude ~%string command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <control_command>))
  (cl:+ 0
     4
     4
     4 (cl:length (cl:slot-value msg 'command))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <control_command>))
  "Converts a ROS message object to a list"
  (cl:list 'control_command
    (cl:cons ':newHeading (newHeading msg))
    (cl:cons ':newAltitude (newAltitude msg))
    (cl:cons ':command (command msg))
))
