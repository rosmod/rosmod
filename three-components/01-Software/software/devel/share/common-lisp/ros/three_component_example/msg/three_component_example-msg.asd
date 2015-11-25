
(cl:in-package :asdf)

(defsystem "three_component_example-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "ComponentName" :depends-on ("_package_ComponentName"))
    (:file "_package_ComponentName" :depends-on ("_package"))
  ))