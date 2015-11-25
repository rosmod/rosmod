; Auto-generated. Do not edit!


(cl:in-package three_component_example-msg)


;//! \htmlinclude ComponentName.msg.html

(cl:defclass <ComponentName> (roslisp-msg-protocol:ros-message)
  ((name
    :reader name
    :initarg :name
    :type cl:string
    :initform ""))
)

(cl:defclass ComponentName (<ComponentName>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ComponentName>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ComponentName)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name three_component_example-msg:<ComponentName> is deprecated: use three_component_example-msg:ComponentName instead.")))

(cl:ensure-generic-function 'name-val :lambda-list '(m))
(cl:defmethod name-val ((m <ComponentName>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader three_component_example-msg:name-val is deprecated.  Use three_component_example-msg:name instead.")
  (name m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ComponentName>) ostream)
  "Serializes a message object of type '<ComponentName>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ComponentName>) istream)
  "Deserializes a message object of type '<ComponentName>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ComponentName>)))
  "Returns string type for a message object of type '<ComponentName>"
  "three_component_example/ComponentName")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComponentName)))
  "Returns string type for a message object of type 'ComponentName"
  "three_component_example/ComponentName")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ComponentName>)))
  "Returns md5sum for a message object of type '<ComponentName>"
  "c1f3d28f1b044c871e6eff2e9fc3c667")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ComponentName)))
  "Returns md5sum for a message object of type 'ComponentName"
  "c1f3d28f1b044c871e6eff2e9fc3c667")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ComponentName>)))
  "Returns full string definition for message of type '<ComponentName>"
  (cl:format cl:nil "string name~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ComponentName)))
  "Returns full string definition for message of type 'ComponentName"
  (cl:format cl:nil "string name~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ComponentName>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ComponentName>))
  "Converts a ROS message object to a list"
  (cl:list 'ComponentName
    (cl:cons ':name (name msg))
))
