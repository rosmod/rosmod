/*
 * ROSMOD Hardware Model
 * Author: Pranav Srinivas Kumar
 * Date: 2015.04.23
 */

grammar ROSMOD_Hardware;

/*
 * This is the start of the hardware model
 */
start
    :   (hardware)*
        EOF
    ;

// Name of Hardware
name
    :   ID
    ;

// Type of Architecture
arch_type
    :   ID
    ;

/*
 * Define a Hardware Type
 */
hardware
    :   'hardware' name
        '{'
        ( 'arch' '=' arch_type ';' )
        '}'
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' )
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' )*
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


