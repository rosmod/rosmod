
(cl:in-package :asdf)

(defsystem "three_component_example-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "ComponentService" :depends-on ("_package_ComponentService"))
    (:file "_package_ComponentService" :depends-on ("_package"))
  ))