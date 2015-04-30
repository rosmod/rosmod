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
        WS_NL
        (package)+
        WS_NL
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

// Reference
reference
    :   
        ( ID ( '/' ID )* )
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
        WS_NL
        'package' WS_NL name
        WS_NL '{' WS_NL
        ( message | service | component | node )*
        WS_NL '}' WS_NL
    ;

// ROS Message
message
    :   
        WS_NL 'msg' WS_NL name
        WS_NL '{' WS_NL
        ( field )
        WS_NL '}' WS_NL
    ;

// ROS Field
field
    :   
        ( ID ( ' ' | '\n' ) )*
    ;

// ROS Service
service
    :   
        WS_NL 'srv' WS_NL name
        WS_NL '{' WS_NL
        ( WS_NL 'request' WS_NL
        WS_NL '{' WS_NL 
            ( request_field  ) 
            WS_NL '}' WS_NL
        )?
        ( WS_NL 'response' WS_NL
        WS_NL '{' WS_NL 
            ( response_field  ) 
            WS_NL '}' WS_NL
        )?
        WS_NL '}' WS_NL
    ;

// ROS Service Request
request_field
    :   
        ( ID ( WS_NL ) )*
    ;

// ROS Service Response
response_field
    :   
        ( ID ( WS_NL ) )*
    ;

// ROS Component
component
    :   
        WS_NL
        'component' WS_NL name WS_NL ':' WS_NL datatype
        WS_NL '{' WS_NL
        ( port | timer )*
        WS_NL '}' WS_NL
    ;

// ROS Component Port
port
    :   
        ( client | server | publisher | subscriber )
    ;

// ROS Client
client
    :   
        WS_NL 'client' WS_NL '<' WS_NL reference WS_NL '>' WS_NL name WS_NL ';'
    ;

// ROS Server
server
    :   
        WS_NL 'server' WS_NL '<' WS_NL reference WS_NL '>' WS_NL name
        WS_NL '{' WS_NL
        ( WS_NL 'priority' WS_NL '=' WS_NL priority WS_NL ';' WS_NL 
        | WS_NL 'deadline' WS_NL '=' WS_NL deadline WS_NL ';' WS_NL
        | WS_NL 'business_logic' WS_NL '{' WS_NL abstract_business_logic WS_NL '}' WS_NL
        )+
        WS_NL '}' WS_NL
    ;

// ROS Publisher
publisher
    :   
        WS_NL 'publisher' WS_NL '<' WS_NL reference WS_NL '>' WS_NL name WS_NL ';' WS_NL
    ;

// ROS Subscriber
subscriber
    :   
        WS_NL 'subscriber' WS_NL '<' WS_NL reference WS_NL '>' WS_NL name WS_NL
        WS_NL '{' WS_NL
        ( WS_NL 'priority' WS_NL '=' WS_NL priority WS_NL ';' WS_NL 
        | WS_NL 'deadline' WS_NL '=' WS_NL deadline WS_NL ';' WS_NL
        | WS_NL 'business_logic' WS_NL '{' WS_NL abstract_business_logic WS_NL '}' WS_NL
        )+
        WS_NL '}' WS_NL
    ;

// ROS Component Timer
timer
    :   
        WS_NL 'timer' WS_NL name WS_NL
        WS_NL '{' WS_NL
        ( WS_NL 'period' WS_NL '=' WS_NL period WS_NL ';' WS_NL
        | WS_NL 'priority' WS_NL '=' WS_NL priority WS_NL ';' WS_NL 
        | WS_NL 'deadline' WS_NL '=' WS_NL deadline WS_NL ';' WS_NL
        | WS_NL 'business_logic' WS_NL '{' WS_NL abstract_business_logic WS_NL '}' WS_NL
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

// Business Logic of Port
abstract_business_logic
    :   
        ( ID ( ' ' | '\n' ) )*
    ;

/*
 * Each ROS Node consists of:
 * (1) Node name
 * (2) One or more component instances
 */
node
    :   
        WS_NL 'node' WS_NL name WS_NL
        WS_NL '{' WS_NL
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
        WS_NL 'component' WS_NL '<' WS_NL reference WS_NL '>' WS_NL name WS_NL ';' WS_NL
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

WS_NL
    :
        ( ' ' | '\n' | '\t' )*
    ;

// White spaces and escape codes are ignored
/*
WS
    :   
        ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;
*/

// Paragraph comments are ignored
COMMENT
    :   
        WS_NL '/*' WS_NL .*? WS_NL '*/' WS_NL -> skip
    ;

// Line comments are ignored
LINE_COMMENT
    :   
        WS_NL '//' WS_NL ~[\r\n]* WS_NL -> skip
    ;

