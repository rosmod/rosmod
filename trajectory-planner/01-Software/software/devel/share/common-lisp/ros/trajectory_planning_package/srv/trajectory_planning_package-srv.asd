
(cl:in-package :asdf)

(defsystem "trajectory_planning_package-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "compute" :depends-on ("_package_compute"))
    (:file "_package_compute" :depends-on ("_package"))
  ))