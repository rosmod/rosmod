/*
 * ROSMOD Software Model
 * Author: Pranav Srinivas Kumar
 * Date: 2015.04.22
 */

grammar ROSMOD_Software;

/*
 * This is the start of the Software Model
 */
start
    :   
        (package)+
        EOF
    ;

// Name
name
    :   
        ( ID ( '/' ID )* )
    ;

// Data Type
datatype
    :   
        ( ID ( '/' ID )* ) ( '[' (INT)? ']' )*
    ;

// Value
value 
    :   
        ( ID | INT | DOUBLE | BOOL)
    ;

// Unit of provided metric
unit
    :   
        ( 's' | 'ms' | 'us' | 'ns' )
    ;

/*
 * ROS Package
 * Every package contains:
 * (1) Package name
 * (2) Set of messages
 * (3) Set of services
 * (4) Set of component definitions
 * (5) Set of nodes/actors
 */
package
    :   
        'package' name
        '{'
        ( message | service | component | node )*
        '}'
    ;

// ROS Message
message
    :   
        'msg' name
        '{'
        (field)*
        '}'
    ;

// ROS Field
field
    :   
        ( datatype name ( '=' value )? ';' )
    ;

// ROS Service
service
    :   
        'srv' name
        '{'
        ( request )?
        ( response )?
        '}'
    ;

// ROS Service Request
request
    :   
        'request'
        '{'
        ( field )*
        '}'
    ;

// ROS Service Response
response
    :   
        'response'
        '{'
        ( field )*
        '}'
    ;

// ROS Component
component
    :   
        'component' name ':' datatype
        '{'
        ( port | timer )*
        '}'
    ;

// ROS Component Port
port
    :   
        ( client | server | publisher | subscriber )
    ;

// ROS Client
client
    :   
        'client' '<' datatype '>' name ';'
    ;

// ROS Server
server
    :   
        'server' '<' datatype '>' name
        '{'
        ( 'priority' '=' priority ';' 
        | 'deadline' '=' deadline ';'
        )+
        '}'
    ;

// ROS Publisher
publisher
    :   
        'publisher' '<' datatype '>' name ';'
    ;

// ROS Subscriber
subscriber
    :   
        'subscriber' '<' datatype '>' name
        '{'
        ( 'priority' '=' priority ';' 
        | 'deadline' '=' deadline ';'
        )+
        '}'
    ;

// ROS Component Timer
timer
    :   
        'timer' name
        '{'
        ( 'period' '=' period ';'
        | 'priority' '=' priority ';' 
        | 'deadline' '=' deadline ';'
        )+
        '}'
    ;

// Timer Period
period
    :   
        DOUBLE
    ;

// ROS Port Callback Priority
priority
    :   
        INT
    ;

// ROS Port Callback Deadline
deadline
    :   
        DOUBLE
    ;

/*
 * Each ROS Node consists of:
 * (1) Node name
 * (2) One or more component instances
 */
node
    :   
        'node' name
        '{'
        ( component_instance )+
        '}'
    ;

/*
 * Each component instance consists of:
 * (1) Type of the component
 * (2) Name of the component instance
 */
component_instance
    :   
        'component' '<' datatype '>' name ';'
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   
        ('a'..'z' | 'A'..'Z' | '_')
        ('a'..'z' | 'A'..'Z' | '0'..'9' | '_' )*
    ;

// A digit - any number between 0 and 9
fragment DIGIT
    :   
        '0'..'9'
    ;

// An integer - one or more digits
INT
    :   
        DIGIT+
    ;

// A double-precision floating point number
DOUBLE
    :   
        (DIGIT)+ '.' (DIGIT)*
    ;

// A boolean variable - must be either true or false
BOOL
    :   
        ( 'true' | 'false')
    ;

// White spaces and escape codes are ignored
WS
    :   
        ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;

// Paragraph comments are ignored
COMMENT
    :   
        '/*' .*? '*/' -> skip
    ;

// Line comments are ignored
LINE_COMMENT
    :   
        '//' ~[\r\n]* -> skip
    ;

