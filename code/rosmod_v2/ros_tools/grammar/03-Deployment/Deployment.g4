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
    :   (define_deployment)
        (deployment)
        EOF
    ;

/*
 * Define a Deployment
 */
define_deployment
    :   'deployment' deployment_name ';'
    ;

/*
 * Name the deployment
 */
deployment_name
    :   ID
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
    :   'host_instance' hostname
        '{'
            (properties)
	        (node_instances)
        '}'
    ;

/*
 * Refer to a valid Host name
 */
hostname
    :   ID
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
    :   'username' '=' '"' username_string '"' ';'
    ;

username_string
    :   ID
    ;

/*
 * Absolute Path to local ssh key
 */
local_sshkey
    :   ( 'sshkey' '=' '"' sshkey_path '"' ';')?
    ;

sshkey_path
    :   ABSOLUTE_PATH
    ;

/*
 * Absolute path to some Init Script
 */
init
    :   ( 'init' '=' '"' init_path '"' ';')?
    ;

init_path
    :   ABSOLUTE_PATH
    ;

/*
 * Environment Variables
 */
env_variables
    :   ('ENV' env_name '=' env_value ';')*
    ;

/*
 * Name of an environment variable
 */
env_name
    :   ID
    ;

/*
 * Value of an environment variable
 */
env_value
    :   ID
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
    :   'node_instance' node_alias
        '{'
            'reference' '=' '"' node '"' ';'
        '}'
    ;

/*
 * Refer to a valid ROS Node
 */
node
    :   ID
    ;

/*
 * Define an alias to ROS Node
 */
node_alias
    :   ID
    ;

/*
 * Valid ID
 */
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' )
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | '/' )*
    ;

/*
 * Valid Absolute Path
 */
ABSOLUTE_PATH
    :   ( '/' | '~' )
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | '.' | '/' )*
    ;


// White spaces and escape codes are ignored
WS
    :   ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;
