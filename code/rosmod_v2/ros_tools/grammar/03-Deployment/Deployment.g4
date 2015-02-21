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
    :   (deployment)
        EOF
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
 * Define a node-to-host mapping
 */
node_host_mapping    
    :   'deploy' '<' node ',' node_alias '>' 'on' host ';'
    ;

/*
 * Refer to a valid ROS Node
 */
node
    :   ROS_NODE
    ;

/*
 * Define an alias to ROS Node
 */
node_alias
    :   ID
    ;

/*
 * Refer to a valid Host name
 */
host
    :   ID
    ;

/*
 * Valid ROS Node reference
 */
ROS_NODE
    :   ('a'..'z' | 'A'..'Z' | '_')
        ('a'..'z' | 'A'..'Z' | '0'..'9' | '_' | '/')*
    ;   

/*
 * Valid ID
 */
ID
    :   ('a'..'z' | 'A'..'Z' | '_')
        ('a'..'z' | 'A'..'Z' | '0'..'9' | '_')*
    ;


// White spaces and escape codes are ignored
WS
    :   ( (' ')*
        | '\t'
        | '\r'
        | '\n'
        ) -> channel(HIDDEN)
    ;
