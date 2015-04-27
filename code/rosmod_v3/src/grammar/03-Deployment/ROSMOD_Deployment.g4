/* 
 * ROSMOD Deployment Model
 * Author: Pranav Srinivas Kumar
 * Date: 2015.04.23
 */

grammar ROSMOD_Deployment;

/*
 * This is the start of the deployment model
 */
start
    :   ( hardware )
        ( hardware_instance )+
        ( group )+
    ;

// Name
name
    :   ID
    ;

// Hardware model being used
hardware
    :   'using' name ';'
    ;

// Node Hardware Map
hardware_instance
    :   'hardware_instance' name
        '{'
        ( hardware_instance_properties )
        ( node_instance )+
        '}'
    ;

// Hardware Instance Properties
hardware_instance_properties
    :   'properties'
        '{'
        ( ip_address )
        ( username )
        ( sshkey )
        ( init )
        ( env_var )*
        '}'
    ;

// IP Address of Hardware Instance
ip_address
    :   'ip_address' '=' name ';'
    ;

// Username of App User
username
    :   'username' '=' name ';'
    ;

// Full path to SSH Key
sshkey
    :   'sshkey' '=' name ';'
    ;

// Full path to Init Script
init
    :   'init' '=' name ';'
    ;

// Environment Variables
env_var
    :   'env_var' '=' name ';'
    ;

// Node Instances in Hardware Instance
node_instance
    :   'node_instance' name
        '{'
        ( reference )
        ( cmd_args )
        '}'
    ;

// Reference to Node in Software Model
reference
    :   'ref' '=' name ';'
    ;

// Command line Arguments
cmd_args
    :   ( 'cmd_args' '=' name ';' )?
    ;

// Group/Associations between component ports
group
    :   'group' name
        '{'
        ( port )+
        '}'
    ;

// Reference to a component port in Software Model
port
    :   ID
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' | '/' | '~' | '.')
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | ':' )*
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










