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
    :   ( 'using' rdp_hardware ';' )
        ( hardware_instance )+
    ;

// Name
name
    :   ID
    ;

// Datatype
datatype
    :   ID
    ;

// Name of Group that contains the port
group
    :   ID
    ;

// Hardware model being used
rdp_hardware
    :   
        ID
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
        ( 'ip_address' '=' '"' ip_address '"' ';'  )
        ( 'username' '=' '"' username '"' ';' )
        ( 'sshkey' '=' '"' sshkey '"' ';'  )
        ( 'init' '=' '"' init '"' ';')?
//        ( 'env_var' '=' env_var )*
        '}'
    ;

// IP Address of Hardware Instance
ip_address
    :   
        ID
    ;

// Username of App User
username
    :   
        ID
    ;

// Full path to SSH Key
sshkey
    :   
        ID
    ;

// Full path to Init Script
init
    :   
        ID
    ;

// Environment Variables
env_var
    :   
        ID
    ;

// Node Instances in Hardware Instance
node_instance
    :   'node_instance' name
        '{'
        ( 'ref' '=' '"' ref '"' ';' )
        ( port_instance )*
        ( 'cmd_args' '=' '"' cmd_args '"' ';' )?
        '}'
    ;

// Reference to Node in Software Model
ref
    :   
        ID
    ;

// Command line Arguments
cmd_args
    :   
        ID
    ;

// Reference to a component port in Software Model
port_instance 
    :   
        'port_instance' name
        '{'
        'ref' '=' '"' datatype '"' ';'
        'group' '=' '"' group '"' ';' 
        '}'
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










