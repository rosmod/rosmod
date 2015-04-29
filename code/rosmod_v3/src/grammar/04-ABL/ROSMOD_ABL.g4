/*
 * ROSMOD Abstract Business Logic Grammar
 * Author: Pranav Srinivas Kumar
 * Date: 2015.04.28
 */

grammar ROSMOD_ABL;

/*
 * This is the start of the ABL Model
 * ABL Consists of one or more operational steps
 */
start
    :
        ( step )+
    ;

// Name
name
    :   
        ( ID ( '/' ID )* )
    ;

/*
 * Each operation step in a component operation can be:
 * (1) Fragment of code taking integer amount of time
 * (2) Client calling a server operation
 * (3) Publisher publishing on a topic
 * (4) Control Loop
 */
step
    :   
        ( local_code_block
        | client_using_srv
        | publish_using_msg
        | loop
        )
    ;

/*
 * Local code block
 * Worst-case amount of time taken to execute local code fragment
 */
local_code_block
    :
        name '.' 'local' '(' DOUBLE ')' ';'
    ;

/*
 * Client using a server operation
 * (1) Name of client port
 * (2) 
 */
client_using_srv
    :   
        name '.' 'call' '(' ')' ';'
    ;

/*
 * Publisher publishing on a msg
 */
publish_using_msg
    :
        name '.' 'publish' '(' ')' ';' 
    ;

/*
 * Control loop in operation
 */
loop
    :
        'loop' '(' count ')'
        '{'
        ( step )+
        '}'
    ;

// Count
count
    :
        INT
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


