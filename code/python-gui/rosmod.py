#!/usr/bin/python
'''
This Program is designed to be the user interface
for the ROSMOD modeling language.  It has the ability
to create, load, display, edit, and export ROSMOD models.

Additionally, it supports the interaction with analysis
tools through the use of toolbar buttons and subprocesses.
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

class Toolbar:
    def __init__(self, master,nameToCallbackMap):
        self.buttons=[]
        self.master=master
        self.nameToCallbackMap = nameToCallbackMap
        self.frame = Frame(self.master)
        for name,callback in self.nameToCallbackMap.iteritems():
            newButton = Button(self.frame, text=name, width=len(name), command=callback)
            self.buttons.append(newButton)
            newButton.grid(row=0,column=(len(self.buttons)-1))
        self.frame.pack(side=TOP,fill=X)

class Menubar:
    def __init__(self, master, menuDictDict):
        self.master = master
        self.menu = Menu(self.master)
        self.menus = []
        for name,menuDict in menuDictDict.iteritems():
            self.menus.append(Menu(self.menu,tearoff=0))
            for option,callback in menuDict.iteritems():
                self.menus[-1].add_command(label=option,command=callback)
            self.menu.add_cascade(label=name,menu=self.menus[-1])
        self.master.config(menu=self.menu)

class StatusBar(Frame):

    def __init__(self, master):
        Frame.__init__(self, master)
        self.label = Label(self, bd=1, relief=SUNKEN, anchor=S)
        self.label.pack(side="left",fill=X)

    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()

    def clear(self):
        self.label.config(text="")
        self.label.update_idletasks()

class EditorFrame(Frame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        Frame.__init__(self,master)
        Frame.config(self,bd=2, relief=SUNKEN)

        self.height=height
        self.width=width

        self.VScrollBar = Scrollbar(self, orient=VERTICAL)
        self.VScrollBar.pack(fill='y', side=RIGHT)
        self.HScrollBar = Scrollbar(self, orient=HORIZONTAL)
        self.HScrollBar.pack(side=BOTTOM)

        self.canvas = Canvas(
            self,
            width=width, 
            height=height, 
            scrollregion=(0,0,maxWidth,maxHeight),
            xscrollcommand=self.HScrollBar.set,
            yscrollcommand=self.VScrollBar.set
        )
        
        self.VScrollBar.config(command=self.canvas.yview)
        self.HScrollBar.config(command=self.canvas.xview)

        self.canvas.pack()

class ObjectList(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        EditorFrame.__init__(self,master,height,width,maxHeight,maxWidth)

        self.objects = []
        self.numObjects = 100
        self.selectedObject = { "object": None }

        # create a couple movable objects
        self._create_object((self.width/2, 100), self.width * 3/4, "white")
        self._create_object((self.width/2, 200), self.width * 3/4, "black")
        self._create_object((self.width/2, 1000), self.width * 3/4, "blue")

        # add bindings for clicking, dragging and releasing over
        # any object with the "token" tag
        self.canvas.tag_bind("token", "<Button-1>", self.OnObjectButtonPress)
        #self.canvas.tag_bind("token", "<ButtonRelease-1>", self.OnTokenButtonRelease)
        #self.canvas.tag_bind("token", "<B1-Motion>", self.OnTokenMotion)

    def _create_object(self, coord, size,  color):
        (x,y) = coord
        self.canvas.create_rectangle(x-size/2, y-size/2, x+size/2, y+size/2, 
                                outline=color, fill=color, tags="object")

    def OnObjectButtonPress(self, event):
        self.selectedObject["item"] = self.canvas.find_closest(event.x, event.y)[0]   
        print "Selected {0}".format(self.selectedObject["item"])
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        EditorFrame.__init__(self,master,height,width,maxHeight,maxWidth)

class App:

    def __init__(self, master):

        self.master = master

        self.fileFormats = [
            ('ROSMOD Model','*.model'),
            ('ROSMOD Component','*.comp'),
            ('ROSMOD Node','*.node')
        ]

        ''' Set up the program's menu '''
        menuDictDict = OrderedDict()
        fileDict = OrderedDict()
        fileDict['New Model']=self.menubar_New_Callback
        fileDict['Open Model']=self.menubar_Open_Callback
        fileDict['Save Model']=self.menubar_Save_Callback
        fileDict['Quit']=self.menubar_Quit_Callback
        optionsDict = OrderedDict()
        helpDict = OrderedDict()
        helpDict['Version']=self.menubar_Version_Callback
        helpDict['Author']=self.menubar_Author_Callback
        helpDict['Email']=self.menubar_Email_Callback

        menuDictDict['File'] = fileDict
        menuDictDict['Options'] = optionsDict
        menuDictDict['Help'] = helpDict

        self.menubar = Menubar(self.master,menuDictDict)


        ''' Set up the toolbar '''
        toolbarMap = OrderedDict()
        toolbarMap['interpret'] = self.toolbar_Interpret_Callback
        toolbarMap['Network QoS'] = self.toolbar_NetworkAnalysis_Callback
        toolbarMap['BL Timing'] = self.toolbar_TimingAnalysis_Callback
        self.toolbar = Toolbar(self.master,toolbarMap)

        ''' Set up the Object Browser ''' 
        self.editorHeight = 800
        self.editorWidth = 800
        self.objectWidth = 100

        ''' Set up the Editor Pane ''' 
        self.editorPane = PanedWindow(
            self.master, 
            orient = HORIZONTAL, 
            opaqueresize = True, 
            height = self.editorHeight, 
            width = self.editorWidth
        )
        ''' Set up the Object Browser ''' 
        self.objectList = ObjectList(
            self.master, 
            height=self.editorHeight, 
            width=self.objectWidth,
            maxHeight=self.editorHeight*2,
            maxWidth=self.objectWidth
        )
        ''' Set up the Model View / Editor ''' 
        self.modelViewer = ModelViewer(
            self.master, 
            height=self.editorHeight, 
            width=self.editorWidth-self.objectWidth,
            maxHeight=self.editorHeight*2,
            maxWidth=self.editorWidth*4
        )
        self.editorPane.add(self.objectList)
        self.editorPane.add(self.modelViewer)

        self.editorPane.pack(fill='both', expand=1)
        
        self.statusBar = StatusBar(self.master)
        self.statusBar.pack()

        self.master.protocol("WM_DELETE_WINDOW", self.close_Callback)

    def close_Callback(self):
        if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
            root.destroy()

    '''
    ------------------------
    Menu Callback Functions:
    ------------------------
    '''
    def menubar_Open_Callback(self):
        file = tkFileDialog.askopenfile(
            parent=self.master,
            mode='rb',
            filetypes=self.fileFormats,
            title="Select Model"
        )
        if file != None:
            data = file.read()
            print "File {0} has {1} bytes".format(file,len(data))
            file.close()

    def menubar_New_Callback(self):
        print "you pressed New"

    def menubar_Save_Callback(self):
        fileName = tkFileDialog.asksaveasfilename(
            parent=self.master,
            filetypes=self.fileFormats,
            title="Save the model as..."
        )
        if len(fileName) > 0:
            print "Now saving model as {0}".format(fileName)

    def menubar_Version_Callback(self):
        print "ROSMOD GUI Version 0.0.1"

    def menubar_Author_Callback(self):
        print "William Emfinger; Pranav Srinivas Kumar"

    def menubar_Email_Callback(self):
        print "emfinger@isis.vanderbilt.edu; pkumar@isis.vanderbilt.edu"

    def menubar_Quit_Callback(self):
        print "you pressed Quit"
        self.master.quit()

    '''
    ---------------------------
    Toolbar Callback Functions:
    ---------------------------
    '''
    def toolbar_Interpret_Callback(self):
        print "you pressed interpret!"
        
    def toolbar_NetworkAnalysis_Callback(self):
        print "Analyzing the network characteristics!"

    def toolbar_TimingAnalysis_Callback(self):
        print "Analyzing the business logic timing characteristics!"

root = Tk()

app = App(root)

root.mainloop()
#root.destroy()
