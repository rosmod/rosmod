# rosmod
=================

This repository contains the relevant work towards implementing a component model for ROS, integrating verification for component operation service metrics, and model-driven design and development for ROS.

All design documents and discussion thus far can be found in the [docs](docs) folder and a listing of all relevant documentation with hyper-links can be found in the [Relevant Documentation Section](#relevant-documentation) of this document.

## Research:
------------


## Design:
----------
The current design uses a textual language (defined by a specific grammar with associated parsers) to define the component model.  Models written by the developer using this language can be parsed, checked by the analysis tools, and then generated into complete workspaces with code and build files.

The generated code can run on top of stock ROS, or can utilize our changes to the ROS infrastructure.  These changes, detailed in the the [Component Model Design Document](docs/ComponentModelOverview.md), provide
  1. priority-based queuing of component operations
  2. component operation deadline monitoring
  3. component operation network QoS measurement and enforcement


## Implementation:
------------------
The current state of the implementation has these features:
  1. Component implementation with multiple components per node, one thread per component, and one component operation queue for each component.
  2. Component modeling grammar specification and example
  3. Orbiter Demo implementation in ROS


## Relevant Documentation:
-----------------------
* [Component Model Design Document](docs/ComponentModelOverview.md)
* [Steps for installing and compiling ROS from source](docs/ROS Setup.md)
* [A list of tasks as next steps for development](docs/Tasks.md)

