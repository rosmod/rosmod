; Auto-generated. Do not edit!


(cl:in-package client_server_package-srv)


;//! \htmlinclude Power-request.msg.html

(cl:defclass <Power-request> (roslisp-msg-protocol:ros-message)
  ((base
    :reader base
    :initarg :base
    :type cl:float
    :initform 0.0)
   (exponent
    :reader exponent
    :initarg :exponent
    :type cl:float
    :initform 0.0))
)

(cl:defclass Power-request (<Power-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Power-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Power-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name client_server_package-srv:<Power-request> is deprecated: use client_server_package-srv:Power-request instead.")))

(cl:ensure-generic-function 'base-val :lambda-list '(m))
(cl:defmethod base-val ((m <Power-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader client_server_package-srv:base-val is deprecated.  Use client_server_package-srv:base instead.")
  (base m))

(cl:ensure-generic-function 'exponent-val :lambda-list '(m))
(cl:defmethod exponent-val ((m <Power-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader client_server_package-srv:exponent-val is deprecated.  Use client_server_package-srv:exponent instead.")
  (exponent m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Power-request>) ostream)
  "Serializes a message object of type '<Power-request>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'base))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'exponent))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Power-request>) istream)
  "Deserializes a message object of type '<Power-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'base) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'exponent) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Power-request>)))
  "Returns string type for a service object of type '<Power-request>"
  "client_server_package/PowerRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Power-request)))
  "Returns string type for a service object of type 'Power-request"
  "client_server_package/PowerRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Power-request>)))
  "Returns md5sum for a message object of type '<Power-request>"
  "5b953294a1eff28068639768c321cf6d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Power-request)))
  "Returns md5sum for a message object of type 'Power-request"
  "5b953294a1eff28068639768c321cf6d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Power-request>)))
  "Returns full string definition for message of type '<Power-request>"
  (cl:format cl:nil "float64 base~%float64 exponent~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Power-request)))
  "Returns full string definition for message of type 'Power-request"
  (cl:format cl:nil "float64 base~%float64 exponent~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Power-request>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Power-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Power-request
    (cl:cons ':base (base msg))
    (cl:cons ':exponent (exponent msg))
))
;//! \htmlinclude Power-response.msg.html

(cl:defclass <Power-response> (roslisp-msg-protocol:ros-message)
  ((result
    :reader result
    :initarg :result
    :type cl:float
    :initform 0.0))
)

(cl:defclass Power-response (<Power-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Power-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Power-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name client_server_package-srv:<Power-response> is deprecated: use client_server_package-srv:Power-response instead.")))

(cl:ensure-generic-function 'result-val :lambda-list '(m))
(cl:defmethod result-val ((m <Power-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader client_server_package-srv:result-val is deprecated.  Use client_server_package-srv:result instead.")
  (result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Power-response>) ostream)
  "Serializes a message object of type '<Power-response>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Power-response>) istream)
  "Deserializes a message object of type '<Power-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Power-response>)))
  "Returns string type for a service object of type '<Power-response>"
  "client_server_package/PowerResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Power-response)))
  "Returns string type for a service object of type 'Power-response"
  "client_server_package/PowerResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Power-response>)))
  "Returns md5sum for a message object of type '<Power-response>"
  "5b953294a1eff28068639768c321cf6d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Power-response)))
  "Returns md5sum for a message object of type 'Power-response"
  "5b953294a1eff28068639768c321cf6d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Power-response>)))
  "Returns full string definition for message of type '<Power-response>"
  (cl:format cl:nil "float64 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Power-response)))
  "Returns full string definition for message of type 'Power-response"
  (cl:format cl:nil "float64 result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Power-response>))
  (cl:+ 0
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Power-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Power-response
    (cl:cons ':result (result msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Power)))
  'Power-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Power)))
  'Power-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Power)))
  "Returns string type for a service object of type '<Power>"
  "client_server_package/Power")