Guide to using rosgen
======================

Setup Antlr4
------------
Note: This step is optional as the lexer, parser and listener are included with ROS.g4
$ cd /usr/local/lib
$ sudo curl -O http://www.antlr.org/download/antlr-4.4-complete.jar
$ export CLASSPATH=".:/usr/local/lib/antlr-4.4-complete.jar:$CLASSPATH"
$ alias antlr4='java -jar /usr/local/lib/antlr-4.4-complete.jar'
$ alias grun='java org.antlr.v4.runtime.misc.TestRig'

Generate ROS Parser, Lexer and Listener:
---------------------------------------
Note: This step is optional as the lexer, parser and listener are included with ROS.g4
$ antlr4 -Dlanguage=Python2 ROS.g4

Install the Antlr4 Python Target:
--------------------------------
Note: This step is optional as the antlr4 python target is included with ros_generator
(1) Download from: https://pypi.python.org/pypi/antlr4-python2-runtime/
(2) Extract antlr4-python2-runtime-4.4.1.tar.gz
(3) $ cd antlr4-python2-runtime-4.4.1.tar.gz
(4) $ sudo python setup.py install

Install the Cheetah Templating Engine: 
-------------------------------------
(1) Download from: https://pypi.python.org/pypi/Cheetah/2.4.4
(2) Extract Cheetah-2.4.4.tar.gz
(3) $ cd Cheetah-2.4.4
(3) $ sudo python setup.py install

Setup alias for rosgen.py
-------------------------
alias rosgen='python <PATH_TO_ROS_GENERATOR_DIRECTORY>/rosgen.py'

Using rosgen:
------------
$ cd <DIRECTORY_CONTAINING_MODEL_FILE>
$ rosgen <filename>.rosml


