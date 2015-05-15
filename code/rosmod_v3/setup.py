#!/usr/bin/python
import os
import subprocess

user =  os.getenv("USER")
sudo_user = os.getenv("SUDO_USER")
if user != "root" and sudo_user == None:
    print "Please run this script as root!"
else:
    # Get setup path
    setup_path = os.getcwd()
    path = os.path.join(setup_path, "ext")

    # Setup ANTLR4
    print "SETUP::Setting up Antlr4"
    os.chdir("/usr/local/lib")
    p = subprocess.Popen(['wget', 'http://www.antlr.org/download/antlr-4.4-complete.jar'])
    p.wait()
    os.system("export CLASSPATH=\".:/usr/local/lib/antlr-4.4-complete.jar:$CLASSPATH\"")
    os.system("alias antlr4=\'java -jar /usr/local/lib/antlr-4.4-complete.jar\'")
    os.system("alias grun=\'java org.antlr.v4.runtime.misc.TestRig\'")
    os.chdir(setup_path)

    print "SETUP::Installing Python Packages at " + path
    for tar in os.listdir(path):
        if tar.endswith(".tar.gz"):
            os.chdir(path)
            if "antlr4-python2-runtime-4.4.1.tar.gz" in tar:
                print "SETUP::Installing Antlr4 Python Runtime"
                p = subprocess.Popen(['tar', '-xf', tar])
                p.wait()
                os.chdir(path + "/antlr4-python2-runtime-4.4.1")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(path)
                p = subprocess.Popen(['rm', '-rf', 'antlr4-python2-runtime-4.4.1'])
                p.wait()
                os.chdir(setup_path)
            elif "Cheetah-2.4.4.tar.gz" in tar:
                print "SETUP::Installing Cheetah Templating Engine"
                p = subprocess.Popen(['tar', '-xvzf', tar])
                p.wait()
                os.chdir(path + "/Cheetah-2.4.4")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(path)
                p = subprocess.Popen(['rm', '-rf', 'Cheetah-2.4.4'])
                p.wait()
                os.chdir(setup_path)
            elif "Fabric-1.10.1.tar.gz" in tar:
                print "SETUP::Installing Fabric "
                p = subprocess.Popen(['tar', '-xvzf', tar])
                p.wait()
                os.chdir(path + "/Fabric-1.10.1")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(path)
                p = subprocess.Popen(['rm', '-rf', 'Fabric-1.10.1'])
                p.wait()
                os.chdir(setup_path)

    for tar in os.listdir(path):
        if tar.endswith(".tar.gz") and "Pygments" not in tar:
            os.chdir(path)
            if "TermEmulator-1.0.tar.gz" in tar:
                print "SETUP::Instaling TermEmulator"
                p = subprocess.Popen(['tar', '-xf', tar])
                p.wait()
                os.chdir(path + "/TermEmulator-1.0")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(path)
                p = subprocess.Popen(['rm', '-rf', 'TermEmulator-1.0'])
                p.wait()
                os.chdir(setup_path)

    print "SETUP::Installing wxPython Tools"
    p = subprocess.Popen(['apt-get', 'install', 'python-wxtools'])
    p.wait()

    print "SETUP::Installing dtach"
    p = subprocess.Popen(['apt-get', 'install', 'dtach'])
    p.wait()




