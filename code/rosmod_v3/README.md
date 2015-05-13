ROSMOD V3
=========

Description
-----------
This directory contains code relevant to version 3 of ROSML editor and generator. The editor is a GUI application which can be used to create applications based on ROSML [Component Model](../../docs/ComponentModelOverview.md) discussed earlier. The generator is a tool to generate ROS packages from ROSML models created in the editor.

Setting up ROSML editor
-----------------------
The ROSML editor and generator can be setup by executing the `setup.py` script, which after executing -
  * Installs external python packages required for editor application, such as -
    - [wxPython](http://www.wxpython.org/)
    - [TermEmulator](https://pypi.python.org/pypi/TermEmulator)
  * Installs external python packages required for generator tool, such as -
    - [Antlr4](http://www.antlr.org/)
    - [Cheetah template engine](http://www.cheetahtemplate.org/)
    - [Fabric](http://www.fabfile.org/)
