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

    # Create generator alias
    generator_path = os.path.join(setup_path, "generator_v2")
    generator_command = "echo \"alias rosgen='python " + os.path.join(generator_path, "rosgen.py") + "'\"" + " >> ~/.bashrc"
    os.system(generator_command)
    print "SETUP::Created Alias: rosgen = " + generator_path 

    # Create editor alias
    editor_path = os.path.join(setup_path, "editor_v2")
    editor_command = "echo \"alias rosmod='python " + os.path.join(editor_path, "rosmod.py") + "'\""  + " >> ~/.bashrc"
    os.system(editor_command)
    print "SETUP::Created Alias: rosmod = " + editor_path

    # Setup ANTLR4
    print "SETUP::Setting up Antlr4"
    os.chdir("/usr/local/lib")
    p = subprocess.Popen(['curl', '-O', 'http://www.antlr.org/download/antlr-4.4-complete.jar'])
    p.wait()
    os.system("export CLASSPATH=\".:/usr/local/lib/antlr-4.4-complete.jar:$CLASSPATH\"")
    os.system("alias antlr4=\'java -jar /usr/local/lib/antlr-4.4-complete.jar\'")
    os.system("alias grun=\'java org.antlr.v4.runtime.misc.TestRig\'")
    os.chdir(setup_path)

    generator_ext_packages = os.path.join(generator_path, "python_packages")
    print "SETUP::Installing Python Packages at " + generator_ext_packages
    for tar in os.listdir(generator_ext_packages):
        if tar.endswith(".tar.gz"):
            os.chdir(generator_ext_packages)
            if "antlr4-python2-runtime-4.4.1.tar.gz" in tar:
                p = subprocess.Popen(['tar', '-xf', tar])
                p.wait()
                os.chdir(generator_ext_packages + "/antlr4-python2-runtime-4.4.1")
                print "ANTLR CURRENT DIRECTORY: " + os.getcwd()
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(generator_ext_packages)
                p = subprocess.Popen(['rm', '-rf', 'antlr4-python2-runtime-4.4.1'])
                p.wait()
                os.chdir(setup_path)
            elif "Cheetah-2.4.4.tar.gz" in tar:
                p = subprocess.Popen(['tar', '-xvzf', tar])
                p.wait()
                os.chdir(generator_ext_packages + "/Cheetah-2.4.4")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(generator_ext_packages)
                p = subprocess.Popen(['rm', '-rf', 'Cheetah-2.4.4'])
                p.wait()
                os.chdir(setup_path)

    editor_ext_packages = os.path.join(editor_path, "python_packages")
    print "SETUP::Installing Python Packages at " + editor_ext_packages
    for tar in os.listdir(editor_ext_packages):
        if tar.endswith(".tar.gz"):
            os.chdir(editor_ext_packages)
            if "TermEmulator-1.0.tar.gz" in tar:
                p = subprocess.Popen(['tar', '-xf', tar])
                p.wait()
                os.chdir(editor_ext_packages + "/TermEmulator-1.0")
                p = subprocess.Popen(['python', 'setup.py', 'install'])
                p.wait()
                os.chdir(editor_ext_packages)
                p = subprocess.Popen(['rm', '-rf', 'TermEmulator-1.0'])
                p.wait()
                os.chdir(setup_path)





