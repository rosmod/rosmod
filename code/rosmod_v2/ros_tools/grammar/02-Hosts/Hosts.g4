/*
 * Hosts Grammar
 * Author: Pranav Srinivas Kumar
 * Date: 2015.02.19
 */
grammar Hosts;

/*
 * This is the start of the Hosts Grammar
 */
start
    :   (define_hardware_configuration)
    	(host)+
        EOF
    ;

/*
 * Define a Hardware Configuration
 * E.g. hardware_configuration 3-Sat-Cluster
 */
define_hardware_configuration
    :   'hardware_configuration' configuration_name ';'
    ;

/*
 * Name of the hardware configuration
 */
configuration_name
    :   ID
    ;

/*
 * Define a Host for ROS Nodes
 * Every host consists of
 * (1) Host name
 * (2) IP address
 * (3) (Optional) Architecture
 */
host
    :   'host' host_name 
        '{'
            (ip_address)
            (username)
            (password)
            (architecture)
            (init)
            (local_sshkey)
        '}'
    ;

/*
 * Name of Host Machine
 */
host_name
    :   ID
    ;

/*
 * IP address of host
 */
ip_address
    :   'ip_address' '=' ip_address_string
    ;

/*
 * IP Address String
 */
ip_address_string
    :   IP_ADDRESS_STRING
    ;

/*
 * Architecture
 */
architecture
    :   ( 'architecture' '=' architecture_string )?
    ;

architecture_string
    :   ID
    ;

/*
 * Absolute path to some Init Script
 */
init
    :   ( 'init' '=' init_path )?
    ;

init_path
    :   ABSOLUTE_PATH
    ;

/*
 * Username of host machine
 */
username
    :   'username' '=' username_string
    ;

username_string
    :   ( ID | IP_ADDRESS_STRING )
    ;

/* 
 * Password of host machine
 */
password    
    :   'password' '=' password_string
    ;

password_string
    :   ( ID | IP_ADDRESS_STRING )
    ;

/*
 * Absolute Path to local ssh key
 */
local_sshkey
    :   ( 'sshkey' '=' sshkey_path )?
    ;

sshkey_path
    :   ABSOLUTE_PATH
    ;

/*
 * Valid IP Address
 */
IP_ADDRESS_STRING
    :    ( '0'..'9' | 'a'..'f' | 'A'..'F' | ':' | '.' )+
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' )
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' )*
    ;

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

