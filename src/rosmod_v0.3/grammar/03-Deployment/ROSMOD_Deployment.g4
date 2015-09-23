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
    :   ( 'using' (' ')* reference (' ')* ';' )
        ( node )*
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

// Node Instances in Hardware Instance
node
    :   'node' (' ')* name (' ')*
        (' ')* '{' (' ')*
            'properties'
            (' ')* '{' (' ')*
                ( 'ref' (' ')* '=' (' ')* '"' reference '"' (' ')* ';' )
                ( 'priority' (' ')* '=' (' ')* priority (' ')* ';' )
                ( 'username' (' ')* '=' (' ')* '"' username '"' (' ')* ';' )?
                ( 'sshkey' (' ')* '=' (' ')* '"' sshkey '"' (' ')* ';'  )?
                ( 'deployment_path' (' ')* '=' (' ')* '"' deployment_path '"' (' ')* ';' )?   
                ( 'init' (' ')* '=' (' ')* '"' init '"' (' ')* ';')?
                ( 'cmd_args' (' ')* '=' (' ')* '"' cmd_args '"' (' ')* ';' )?
            '}'

            ( component_instance )*  

        '}'
    ;

// Reference to Node in Software Model
reference
    :   
        ID ( '/' ID )*
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

// Path to some init script at target that is run before deploying the nodes
init
    :
        ID
    ;

// Command line Arguments
cmd_args
    :   
        ID ((' ')* (ID | INT) )*
    ;

// Component Scheduling Scheme
scheduling_scheme
    :
        ( 'FIFO' | 'PFIFO' | 'EDF' )
    ;

// Component logging parameters
is_periodic_logging
    :
        BOOL
    ;

periodic_log_unit
    :
        INT
    ;

// Component Log levels
logging_debug
    :
        BOOL
    ;

logging_info
    :
        BOOL
    ;

logging_warning
    :
        BOOL
    ;

logging_error
    :
        BOOL
    ;

logging_critical
    :
        BOOL
    ;

priority
    :   
        INT
    ;

component_instance
    :
        'component_instance' (' ')* name (' ')*
        (' ')* '{' (' ')*
            'properties'
            (' ')* '{' (' ')*
                ( 'ref' (' ')* '=' (' ')* '"' reference '"' (' ')* ';')
		('scheduling_scheme' (' ')* '=' (' ')* scheduling_scheme (' ')* ';' ) 
                (
                  'logging'
                  (' ')* '{' (' ')*
                      'Is Periodic?' (' ')* '=' (' ')* is_periodic_logging (' ')* ';'
                      'Periodic Log Unit' (' ')* '=' (' ')* periodic_log_unit (' ')* ';'
                      'DEBUG' (' ')* '=' (' ')* logging_debug (' ')* ';'
                      'INFO' (' ')* '=' (' ')* logging_info (' ')* ';'
                      'WARNING' (' ')* '=' (' ')* logging_warning (' ')* ';'
                      'ERROR' (' ')* '=' (' ')* logging_error (' ')* ';'
                      'CRITICAL' (' ')* '=' (' ')* logging_critical (' ')* ';' 
                   '}'
		 )
             '}'

             ( port_instance )*
        '}'
    ;


// Reference to a component port in Software Model
port_instance 
    :   
        'port_instance' (' ')* name
        (' ')* '{' (' ')*
            'ref' (' ')* '=' (' ')* '"' reference '"' (' ')* ';'
            'group' (' ')* '=' (' ')* '"' group '"' (' ')* ';' 
        '}'
    ;

// A boolean variable - must be either true or false
BOOL
    :   
        ( 'true' | 'false')
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

// An ID - one or more alphanumeric characters that must start with either an alphabet/underscore
ID
    :   ( 'a'..'z' | 'A'..'Z' | '_' | '0'..'9' | '/' | '~' | '.' | '-')
        ( 'a'..'z' | 'A'..'Z' | '0'..'9' | '_' | ':' | '.' | '/' | '-' )*
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










