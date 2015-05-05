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
        ID 
    ;

// Reference
reference
    :   
        ( ID ( '/' ID )* )
    ;

/*
 * ROS Package
 * Every package contains:
 * (1) Package name
 * (2) Set of component definitions
 * (3) Set of nodes/actors
 */
package
    :   
        'package' name
        '{'
        ( component )*
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
        'client' '<' reference '>' name ';'
    ;

// ROS Server
server
    :   
        'server' '<' reference '>' name
        '{'
        ( 'priority' '=' priority ';' 
        | 'deadline' '=' deadline ';'
        )+
        '}'
    ;

// ROS Publisher
publisher
    :   
        'publisher' '<' reference '>' name ';'
    ;

// ROS Subscriber
subscriber
    :   
        'subscriber' '<' reference '>' name
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
