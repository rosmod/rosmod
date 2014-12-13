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
	:	'Component' ros_component_name 
		'{'
			 ( ros_service 
			 | ros_pub_sub 
			 | ros_timer 
			 )*
		'}'
	;

// ROS Component Name - follows rules of an ID
ros_component_name
	:	ID 
	;

/*
 * ROS Service specification. Can be either:
 * (1) Provision - Name of the provided service
 * (2) Requirement - Name of the required service
 */
ros_service
	:	( 'Provides' service_name ';'
		| 'Requires' service_name ';'
		)
	;

// Service Name - follows the rules of an ID
service_name
	:	ID
	;

/*
 * ROS Publish/Subscribe specification. Can be either:
 * (1) Publisher - port name followed by topic
 * (2) Subscriber - port name followed by topic
 */
ros_pub_sub
	:	( 'Publisher' publisher topic ';'
		| 'Subscriber' subscriber topic ';'
		)
	; 

// Publisher Port Name - follows rules of an ID
publisher
	:	ID
	;

// Subscriber Port Name - follows rules of an ID
subscriber
	:	ID
	;

// ROS Topic - starts with the keyword 'Topic' followed by a valid topic name
topic
	:	ID
	;

/*
 * ROS Timer. Contains:
 * (1) Keyword 'Timer'
 * (2) Timer name
 * (3) Timer Properties 
 *     - Timer Period - if period is set to zero, it is a one-shot timer
 *	   - Timer Offset - offset from start of component execution
 */
ros_timer
	:	'Timer' timer_name 
		'{' 
			('period' '=' timer_period period_unit ';')
			('offset' '=' timer_offset offset_unit ';')
		'}'
	;

// Name of ROS Timer - follows rules of an ID
timer_name
	:	ID
	;

// Timer Period - If 0.0, this is a one-shot timer
timer_period
	:	DOUBLE
	;

// Unit for Timer period
period_unit
	:	('s' | 'ms' | 'us' | 'ns')
	;

// Timer Offset
timer_offset
	:	DOUBLE
	;

// Unit for Timer offset
offset_unit
	:	('s' | 'ms' | 'us' | 'ns')
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

// A Double-precision floating point number
DOUBLE
	:	(DIGIT)+ '.' (DIGIT)*
	;

// A boolean variable - must be either true or false
BOOL
	:	( 'true' | 'false' )
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
