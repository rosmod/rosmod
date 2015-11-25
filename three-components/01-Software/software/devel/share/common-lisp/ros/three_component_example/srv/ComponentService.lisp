; Auto-generated. Do not edit!


(cl:in-package three_component_example-srv)


;//! \htmlinclude ComponentService-request.msg.html

(cl:defclass <ComponentService-request> (roslisp-msg-protocol:ros-message)
  ((operation
    :reader operation
    :initarg :operation
    :type cl:string
    :initform ""))
)

(cl:defclass ComponentService-request (<ComponentService-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ComponentService-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ComponentService-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name three_component_example-srv:<ComponentService-request> is deprecated: use three_component_example-srv:ComponentService-request instead.")))

(cl:ensure-generic-function 'operation-val :lambda-list '(m))
(cl:defmethod operation-val ((m <ComponentService-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader three_component_example-srv:operation-val is deprecated.  Use three_component_example-srv:operation instead.")
  (operation m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ComponentService-request>) ostream)
  "Serializes a message object of type '<ComponentService-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'operation))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'operation))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ComponentService-request>) istream)
  "Deserializes a message object of type '<ComponentService-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'operation) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'operation) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ComponentService-request>)))
  "Returns string type for a service object of type '<ComponentService-request>"
  "three_component_example/ComponentServiceRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComponentService-request)))
  "Returns string type for a service object of type 'ComponentService-request"
  "three_component_example/ComponentServiceRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ComponentService-request>)))
  "Returns md5sum for a message object of type '<ComponentService-request>"
  "bc470ca3ad39af1aebdc8e6af0a31999")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ComponentService-request)))
  "Returns md5sum for a message object of type 'ComponentService-request"
  "bc470ca3ad39af1aebdc8e6af0a31999")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ComponentService-request>)))
  "Returns full string definition for message of type '<ComponentService-request>"
  (cl:format cl:nil "string operation~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ComponentService-request)))
  "Returns full string definition for message of type 'ComponentService-request"
  (cl:format cl:nil "string operation~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ComponentService-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'operation))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ComponentService-request>))
  "Converts a ROS message object to a list"
  (cl:list 'ComponentService-request
    (cl:cons ':operation (operation msg))
))
;//! \htmlinclude ComponentService-response.msg.html

(cl:defclass <ComponentService-response> (roslisp-msg-protocol:ros-message)
  ((name
    :reader name
    :initarg :name
    :type cl:string
    :initform ""))
)

(cl:defclass ComponentService-response (<ComponentService-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ComponentService-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ComponentService-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name three_component_example-srv:<ComponentService-response> is deprecated: use three_component_example-srv:ComponentService-response instead.")))

(cl:ensure-generic-function 'name-val :lambda-list '(m))
(cl:defmethod name-val ((m <ComponentService-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader three_component_example-srv:name-val is deprecated.  Use three_component_example-srv:name instead.")
  (name m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ComponentService-response>) ostream)
  "Serializes a message object of type '<ComponentService-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ComponentService-response>) istream)
  "Deserializes a message object of type '<ComponentService-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ComponentService-response>)))
  "Returns string type for a service object of type '<ComponentService-response>"
  "three_component_example/ComponentServiceResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComponentService-response)))
  "Returns string type for a service object of type 'ComponentService-response"
  "three_component_example/ComponentServiceResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ComponentService-response>)))
  "Returns md5sum for a message object of type '<ComponentService-response>"
  "bc470ca3ad39af1aebdc8e6af0a31999")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ComponentService-response)))
  "Returns md5sum for a message object of type 'ComponentService-response"
  "bc470ca3ad39af1aebdc8e6af0a31999")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ComponentService-response>)))
  "Returns full string definition for message of type '<ComponentService-response>"
  (cl:format cl:nil "string name~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ComponentService-response)))
  "Returns full string definition for message of type 'ComponentService-response"
  (cl:format cl:nil "string name~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ComponentService-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ComponentService-response>))
  "Converts a ROS message object to a list"
  (cl:list 'ComponentService-response
    (cl:cons ':name (name msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'ComponentService)))
  'ComponentService-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'ComponentService)))
  'ComponentService-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComponentService)))
  "Returns string type for a service object of type '<ComponentService>"
  "three_component_example/ComponentService")