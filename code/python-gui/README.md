# ROSML Editor GUI
=======================
This codebase controls the graphical editor for *.rosml files.  It allows for the creation, alteration, and deletion of elements within a ROSML model such as 

  1. The workspace
  2. packages
  3. messages
  4. services
  5. component definitions
  6. component timers
  7. component message subscribers
  8. component message publishers
  9. component service providers (servers)
  10. component service users (clients)
  11. nodes
  12. node component instances

Furthermore, the editor allows for the interpretation and analysis of the ROSML model.  Interpretation of the model involves generating component based skeleton code following the design of the model.  Analysis of the model entails either analyzing the model's network quality of service characteristics or analyzing the business logic model's timing and scheduling characteristics.  

## Using the Editor
-------------------
This section describes how to use the ROSML editor to create, open, edit, save, and interpret a ROSML model file.  Most of the user interface for the editor is based around right-click context menus which provide the relevant actions according to the given context.  When the editor is started, the main editor pane does not exist, as no model has been initialized yet.  To initialize a model, you must either [Create a new model](#creating-a-model) or [Open an existing model](#open-an-existing-model).

### Creating a Model
--------------------
To create a new model, navigate to the _File_ menu and click on _New Model_.  

![Alt text](./screenshots/initialWindow.png "Initial Window")

![Alt text](./screenshots/fileDialog.png "File Dialog")

### Opening an Existing Model
-----------------------------

### Editing a Model
-------------------

### Saving a Model
------------------

### Generating code for a Model
-------------------------------

