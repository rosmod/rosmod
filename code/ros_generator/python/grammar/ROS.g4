/*
 * ROS Grammar
 * Author: Pranav Srinivas Kumar
 * Date: 2014.12.16
 */

grammar ROS;

/*
 * This is the start of the ROS Grammar
 */
start
    :   (define_workspace)
        (ros_packages)+
        EOF
    ;

/*
 * Define a ROS workspace
 */
define_workspace
    :   'workspace' workspace_name ';'
    ;

/*
 * Provide a name for the workspace
 */
workspace_name
    :   ID
    ;

/*
 * Define a ROS Package
 * Every package consists of 
 * (1) Package name
 * (2) Set of messages
 * (3) Set of services
 * (4) Set of component definitions
 * (5) Set of ros nodes - that use instances of defined components
 */
ros_packages
    :	'package' package_name
        '{'
            (messages)?
            (services)?
            (components)
            (nodes)
        '}'
    ; 

// Name of ROS Package
package_name
    :   ID
    ;

/*
 * Define ROS Messages
 */
messages
    :   'messages'
        '{'
            (ros_msg)+
        '}'
    ;

/*
 * Each ROS message consists of one or more fields
 * Each field consists of:
 * (1) Field Type
 * (2) Field Name
 * (3) Field Value
 */
ros_msg
    :   'msg' msg_name
        '{'
            (msg_field)+
        '}'
    ;

// Name of ROS message
msg_name
    :   ID
    ;

// ROS msg field
msg_field
    :   (msg_field_type msg_field_name ('=' msg_field_value)?  ';')
    ;

// Type of message field
msg_field_type
    :   ID
    ;

// Name of message field
msg_field_name
    :   ID
    ;

// Message Field value - the field becomes a const
msg_field_value
    :   ( ID | INT | DOUBLE | BOOL)
    ;

/*
 * Define ROS Services
 */
services
    :   'services'
        '{'
            (ros_srv)+
        '}'
    ;

/*
 * Each ROS service consists of one or more:
 * (1) Request fields
 * (2) Response fields
 */
ros_srv
    :   'srv' srv_name
        '{'
            (request)*
            (response)*
        '}'
    ;

// Name of ROS service
srv_name
    :   ID
    ;

// ROS Service request fields
request
    :   'request'
        '{'
            (req_argument)+
        '}'
    ;

// Request Argument
req_argument
    :   (req_field_type req_field_name ('=' req_field_value)? ';')
    ;

// ROS Service response fields
response
    :   'response'
        '{'
            (res_argument)+
        '}'
    ;

// Response Argument
res_argument
    :   (res_field_type res_field_name ('=' res_field_value)? ';')
    ;

// Request Field Type
req_field_type
    :   ID
    ;

// Request Field Name
req_field_name
    :   ID
    ;

// Request Field value - the field becomes a const
req_field_value
    :   ( ID | INT | DOUBLE | BOOL)
    ;

// Response Field Type
res_field_type
    :   ID
    ;

// Response Field Name
res_field_name
    :   ID
    ;

// Response Field value - the field becomes a const
res_field_value
    :   ( ID | INT | DOUBLE | BOOL)
    ;

// Group of ROS components
components
    :   'components'
        '{'
            (ros_component)+
        '}'
    ;

/*
 * Each ROS component consists of an optional set of:
 * (1) Publisher ports
 * (2) Subscriber ports
 * (3) Provided services
 * (4) Required services
 * (5) Timers
 */
ros_component
    :   'component' component_name
        '{'
            ( ros_service
            | ros_pub_sub
            | ros_timer
            )*
        '}'
    ;   

// ROS Component Name
component_name
    :   ID
    ;

/*
 * ROS Service Specification - Can be either:
 * (1) Provision - Name of the provided service
 * (2) Requirement - Name of the required service
 */
ros_service
    :   ( 'provides' service_name ';'
        | 'requires' service_name ';'
        )
    ;

// Service Name
service_name
    :   ID
    ;

/*
 * ROS Publish/Subscribe specification - can be either:
 * (1) Publisher - port name and topic
 * (2) Subscriber - port name and topic
 */
ros_pub_sub
    :   ( 'publisher''<'topic'>' publisher ';'
        | 'subscriber''<'topic'>'subscriber ';'
        )
    ;

// Publisher port name
publisher
    :   ID
    ;

// Subscriber port name
subscriber
    :   ID
    ;

// ROS Topic
topic
    :   ID
    ;

/*
 * ROS Timer. Contains:
 * (1) Timer name
 * (2) Timer Properties
 *     - Timer period
 */
ros_timer
    :   'timer' timer_name
        '{'
            ('period' '=' timer_period period_unit ';')
        '}'
    ;

// Name of ROS Timer 
timer_name
    :   ID
    ;

// Timer Period
timer_period
    :   DOUBLE
    ;

// Unit for timer period
period_unit
    :   ( 's' | 'ms' | 'us' | 'ns')
    ; 

// ROS Node
nodes
    :   'nodes'
        '{'
            (ros_node)+
        '}'
    ;

/*
 * Each ROS Node consists of:
 * (1) Node name
 * (2) One or more component instances
 */
ros_node
    :   'node' node_name
        '{'
            (component_instances)+ 
        '}'
    ;

// Name of ROS Node
node_name
    :   ID
    ;

/*
 * Each component instance consists of:
 * (1) Type of the component
 * (2) Name of the component instance
 */
component_instances
    :   'component''<'component_type'>' component_instance ';'
    ;

// Type of the component instance
// Refers to a component_name previously defined in components group
component_type
    :   ID
    ;

// Name of the component instance
component_instance
    :   ID
    ;


// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ('a'..'z' | 'A'..'Z' | '_')
        ('a'..'z' | 'A'..'Z' | '0'..'9' | '_')*
    ;

// A digit - any number between 0 and 9
fragment DIGIT
    :   '0'..'9'
    ;

// An integer - one or more digits
INT
    :   DIGIT+
    ;

// A double-precision floating point number
DOUBLE
    :   (DIGIT)+ '.' (DIGIT)*
    ;

// A boolean variable - must be either true or false
BOOL
    :   ( 'true' | 'false')
    ;

// White spaces and escape codes are ignored
WS
    :   ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;

// Paragraph comments are ignored
COMMENT
    :   '/*' .*? '*/' -> skip
    ;

// Line comments are ignored
LINE_COMMENT
    :   '//' ~[\r\n]* -> skip
    ;


