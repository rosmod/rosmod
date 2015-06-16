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
arch
    :   ID
    ;

/*
 * Define a Hardware Type
 */
hardware
    :   'hardware' name
        '{'
        ( 'ip_address' '=' '"' ip_address '"' ';'  )
        ( 'username' '=' '"' username '"' ';' )
        ( 'sshkey' '=' '"' sshkey '"' ';'  )
        ( 'deployment_path' '=' '"' deployment_path '"' ';' )   
        ( 'install_path' '=' '"' install_path '"' ';' )?
        ( 'init' '=' '"' init '"' ';')?
        ( 'arch' '=' arch ';' )
        '}'
    ;

// IP Address of Hardware
ip_address
    :
        ID
    ;

// Username @ hardware
username
    :
        ID
    ;

// SSHKey to access hardware as username
sshkey 
    :
        ID
    ;

// Path where executables are deployed
deployment_path
    :
        ID
    ;

// Path where ROS has been installed
install_path
    :
        ID
    ;

// Path to some init script at target that is run before deploying the nodes
init
    :
        ID
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' | '/' | '~' | '.' | '-')
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | ':' | '.' | '/' )*
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


