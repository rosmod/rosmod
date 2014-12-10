grammar ROS;

@parser::members
{
	protected const int EOF = Eof;
}

@lexer::members
{
	protected const int EOF = Eof;
	protected const int HIDDEN = Hidden;
}

compileUnit
	:	EOF
	;

/*
 * ROS Node
 * A node consists of one or more components
 */
ros_node
	:	(ros_component)*
	;

/*
 * A ROS Component
 * (1) Starts with the keyword 'Component' followed by a component name
 * (2) Within curly braces, the component contains zero or more ports/timers
 */
ros_component
	:	'Component' ros_component_name '{'
				(ros_ports | ros_timer)*
		'}'
	;

// ROS Component Name - follows rules of an ID
ros_component_name
	:	ID 
	;

/*
 * ROS Ports
 * (1) Facet - Provides an interface
 * (2) Receptacle - Requires an interface
 * (3) Publisher - Publishes on a topic
 * (4) Subscriber - Subscribed to a topic
 */
ros_ports
	:	( 'Facet' port_name '{' (service)* '}'
		| 'Receptacle' port_name '{' (service)* '}'
		| 'Publisher' port_name topic ';'
		| 'Subscriber' port_name topic ';'
		)
	; 

// Port Name - follows rules of an ID
port_name
	:	ID
	;

// ROS Service - starts with the keyword 'Service' followed by a valid service name
service
	:	'Service' service_name ';'
	;

// Name of ROS Service - follows rules of an ID
service_name
	:	ID
	;

// ROS Topic - starts with the keyword 'Topic' followed by a valid topic name
topic
	:	ID
	;

// Name of ROS Topic - follows rules of an ID
topic_name
	:	ID
	;

// ROS Timer - more needed here (timer options)
ros_timer
	:	'Timer' timer_name ';'
	;

// Name of ROS Timer - follows rules of an ID
timer_name
	:	ID
	;

// An ID - One or more alphanumeric characters that must start with either an alphabet/underscore.
ID
	:	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
	;

// A digit - numeric characters - any number between 0 and 9
fragment DIGIT
	:	'0'..'9' 
	;

// An Integer - one or more digits
INT 
	:	DIGIT + 
	;

// A floating point number
FLOAT
	:	(DIGIT)+ '.' (DIGIT)*
	;

// A boolean variable - must be either true or false
BOOL
	:	('true' | 'false')
	;

// White spaces and escape codes are ignored
WS  
	:	( (' ')*
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
