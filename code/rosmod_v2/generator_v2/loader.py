# Loader
# Author: Pranav Srinivas Kumar
# Date: 2015.02.06

import sys
import os

class Loader:

    def load_business_logic(self, listener, path):
        
        self.workspace_dir = path + "/" + listener.workspace.properties["name"]
        
