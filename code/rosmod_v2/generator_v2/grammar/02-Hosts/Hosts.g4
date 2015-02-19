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
    :   (host)+
        EOF
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
            (architecture)?
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
    :   'ip_address' '=' ADDRESS ';'
    ;

/*
 * Architecture
 */
architecture
    :   'architecture' '=' ID ';'
    ;

/*
 * Valid IP Address
 */
ADDRESS
    :    ('0'..'9' | 'a'..'f' | 'A'..'f' | ':' | '.')+
    ;

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
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

