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
    :   'host' hostname
        '{'
            (nodes)+
        '}'
    ;

/*
 * Refer to a valid Host name
 */
hostname
    :   ID
    ;

/*
 * Define a node-to-host mapping
 */
nodes  
    :   'deploy' node 'as' node_alias ';'
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


// White spaces and escape codes are ignored
WS
    :   ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;
