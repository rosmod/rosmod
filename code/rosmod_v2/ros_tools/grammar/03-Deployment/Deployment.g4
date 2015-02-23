/*
 * Deployment Grammar
 * Author: Pranav Srinivas Kumar
 * Date: 2015.02.19
 */
grammar Deployment;

/*
 * This is the start of the Deployment Grammar
 */
start
    :   (use_hardware_config)
        (deployment)
        EOF
    ;

/*
 * Use a specific hardware configuration
 */
use_hardware_config
    :   'using ' hardware ';'
    ;

/*
 * Hardware Config
 */
hardware
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Define a deployment
 * Every deployment: 
 * (1) Maps a ROS Node to a Hardware Host
 */
deployment
    :   (node_host_mapping)+
    ;

/*
 * Node Host Mapping
 */
node_host_mapping
    :   'host_instance ' hostname
        '{'
            (properties)
	        (node_instances)
        '}'
    ;

/*
 * Refer to a valid Host name
 */
hostname
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Host instance properties
 */
properties
    :   'properties'
        '{'
            (username)
            (local_sshkey)
            (init)
            (env_variables)
        '}'
    ;

/*
 * Username of host machine
 */
username
    :   'username '(' ')* '='(' ')* '"' username_string '"'(' ')* ';'
    ;

username_string
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Absolute Path to local ssh key
 */
local_sshkey
    :   ( 'sshkey '(' ')* '='(' ')* '"' sshkey_path '"'(' ')* ';')
    ;

sshkey_path
    :   ABSOLUTE_PATH
    ;

/*
 * Absolute path to some Init Script
 */
init
    :   ( 'init '(' ')* '='(' ')* '"' init_path '"'(' ')* ';')?
    ;

init_path
    :   ABSOLUTE_PATH
    ;

/*
 * Environment Variables
 */
env_variables
    :   ('ENV ' env_name(' ')* '='(' ')* '"' env_value '"'(' ')* ';')*
    ;

/*
 * Name of an environment variable
 */
env_name
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Value of an environment variable
 */
env_value
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Node Instances deployed on host
 */
node_instances
    :   'nodes'
        '{'
            (nodes)+
        '}'
    ;

/*
 * Define a node-to-host mapping
 */
nodes  
    :   'node_instance ' node_alias
        '{'
            'reference ' (' ')* '=' (' ')* '"' node '"' (' ')* ';'
            ('cmdline_arguments '(' ')* '='(' ')* '"' arguments '"'(' ')* ';')?
        '}'
    ;

/*
 * Refer to a valid ROS Node
 */
node
    :   (('/')* IDENT ('/')*)* 
    ;

/*
 * Command Line Arguments
 */
arguments
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Define an alias to ROS Node
 */
node_alias
    :   (' ')* (IDENT (' ')* )*
    ;

/*
 * Valid ID
 */ 
IDENT
    :   ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' | '-' | '/' | '.' )
    ;

/*
 * Valid Absolute Path
 */
ABSOLUTE_PATH
    :   ( '/' | '~' )
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | '.' | '/' )*
    ;

// Ignore whitespaces & escape sequences
WS
    : ( ' ' | '\t' | '\n' | '\r' )+ -> channel(HIDDEN) 
    ;

