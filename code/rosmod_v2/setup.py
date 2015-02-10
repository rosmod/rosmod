#!/usr/bin/python
import os

# Get setup path
setup_path = os.getcwd()

# Create generator alias
generator_path = os.path.join(setup_path, "generator_v2")
generator_command = "echo alias rosgen=\'python " + os.path.join(generator_path, "rosgen.py") + "\'" + " >> ~/.bashrc"
print generator_command
os.system(generator_command)

# Create editor alias
editor_path = os.path.join(setup_path, "editor_v2")
editor_command = '''echo alias rosmod=\'python ''' + os.path.join(editor_path, "rosmod.py") + "\'"  + " >> ~/.bashrc"
print editor_command
os.system(editor_command)


