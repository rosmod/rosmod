
(cl:in-package :asdf)

(defsystem "trajectory_planning_package-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "sensor_reading" :depends-on ("_package_sensor_reading"))
    (:file "_package_sensor_reading" :depends-on ("_package"))
  ))