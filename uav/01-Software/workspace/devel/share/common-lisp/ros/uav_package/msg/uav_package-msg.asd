
(cl:in-package :asdf)

(defsystem "uav_package-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "sensor_state" :depends-on ("_package_sensor_state"))
    (:file "_package_sensor_state" :depends-on ("_package"))
    (:file "control_command" :depends-on ("_package_control_command"))
    (:file "_package_control_command" :depends-on ("_package"))
  ))