from Tkinter import * 
import tkFileDialog
import tkMessageBox
from collections import OrderedDict

canvasObjectDict = OrderedDict()

editorDialogsDict = OrderedDict()
nameDialogDict = OrderedDict()
nodeDialogDict = OrderedDict()
compDialogDict = OrderedDict()
msgDialogDict = OrderedDict()
srvDialogDict = OrderedDict()
tmrDialogDict = OrderedDict()
editorDialogsDict['name'] = nameDialogDict
editorDialogsDict['node'] = nodeDialogDict
editorDialogsDict['component'] = compDialogDict
editorDialogsDict['message'] = msgDialogDict
editorDialogsDict['service'] = srvDialogDict
editorDialogsDict['timer'] = tmrDialogDict

digitStringChars = "0987654321."
nameStringChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_-"
unitStringChars = "munpfs"
fieldStringChars = nameStringChars + digitStringChars + "[]:"

def validString(string,validChars):
    for char in string:
        if char not in validChars:
            return False
    return True

class Dialog(Toplevel):

    def __init__(self, parent, title = None, initValsList=None):

        Toplevel.__init__(self, parent)
        self.transient(parent)

        self.valsList = initValsList

        if title:
            self.title(title)

        self.parent = parent

        self.entries = []
        self.entryStrings = {}
        self.options = []
        self.stringVars = []
        self.result = None

        body = Frame(self)
        self.initial_focus = self.body(body)
        body.pack(padx=5, pady=5)

        self.buttonbox()

        self.grab_set()

        if not self.initial_focus:
            self.initial_focus = self

        self.protocol("WM_DELETE_WINDOW", self.cancel)

        self.geometry("+%d+%d" % (parent.winfo_rootx()+50,
                                  parent.winfo_rooty()+50))

        self.initial_focus.focus_set()

        self.wait_window(self)

    #
    # construction hooks
    def body(self, master):
        pass

    def buttonbox(self):
        box = Frame(self)

        w = Button(box, text="OK", width=10, command=self.ok, default=ACTIVE)
        w.pack(side=LEFT, padx=5, pady=5)
        w = Button(box, text="Cancel", width=10, command=self.cancel)
        w.pack(side=LEFT, padx=5, pady=5)

        self.bind("<Return>", self.ok)
        self.bind("<Escape>", self.cancel)

        box.pack()

    #
    # standard button semantics

    def ok(self, event=None):

        if not self.validate():
            self.initial_focus.focus_set() # put focus back
            return

        self.withdraw()
        self.update_idletasks()

        self.apply()

        self.cancel()

    def cancel(self, event=None):
        # put focus back to the parent window
        self.parent.focus_set()
        self.destroy()

    #
    # command hooks
    def validate(self):
        return 1 # override
    def apply(self):
        pass # override

class EditorDialogPopup(Dialog):
    def body(self, master):

        if self.valsList == None:
            self.valsList = [["Name:","",nameStringChars]]
        r = 0
        for vals in self.valsList:
            Label(master, text=vals[0]).grid(row=r)
            entry = Entry(master)
            self.entryStrings[entry] = vals[2]
            self.entries.append(entry)
            self.entries[-1].grid(row=r,column=1)
            self.entries[-1].insert(0,vals[1])
            r += 1

        return self.entries[0] # initial focus

    def validate(self):
        for entry in self.entries:
            val = entry.get()
            if not validString(val,self.entryStrings[entry]):
                tkMessageBox.showwarning(
                    "Bad Input",
                    "Input:\n{0}\n may only contain:\n{1}\nTry again.".format(val,self.entryStrings[entry])
                )
                return 0
        return 1

    def apply(self):
        self.result = []
        for entry in self.entries:
            val = entry.get()
            self.result.append(val)

class ReferenceDialogPopup(Dialog):
    def body(self, master):

        r = 0
        Label(master, text=self.valsList[0][0]).grid(row=r)
        entry = Entry(master)
        self.entryStrings[entry] = self.valsList[0][2]
        self.entries.append(entry)
        self.entries[-1].grid(row=r,column=1)
        self.entries[-1].insert(0,self.valsList[0][1])
        r += 1
        
        self.optionsDict = OrderedDict()

        for val in self.valsList[1:]:
            Label(master, text=val[0]).grid(row=r)
            var = StringVar()
            self.optionsDict["{0}".format(var)] = val[2]
            self.stringVars.append(var)
            self.stringVars[-1].set(val[1])
            option1 = OptionMenu(
                master,
                self.stringVars[-1], 
                *self.optionsDict["{0}".format(var)].keys()
            )
            self.options.append(option1)
            self.options[-1].grid(row=r,column=1)
            r += 1

        return self.entries[0] # initial focus

    def validate(self):
        for entry in self.entries:
            val = entry.get()
            if not validString(val,self.entryStrings[entry]):
                tkMessageBox.showwarning(
                    "Bad Input",
                    "Input:\n{0}\n may only contain:\n{1}\nTry again.".format(val,self.entryStrings[entry])
                )
                return 0
        return 1

    def apply(self):
        self.result = []
        for entry in self.entries:
            val = entry.get()
            self.result.append(val)
        for var in self.stringVars:
            self.result.append(self.optionsDict["{0}".format(var)][var.get()])


class TypeDialogPopup(Dialog):

    def buttonbox(self):
        box = Frame(self)

        w = Button(box, text="OK", width=10, command=self.ok, default=ACTIVE)
        w.pack(side=LEFT, padx=5, pady=5)
        w = Button(box, text="Cancel", width=10, command=self.cancel)
        w.pack(side=LEFT, padx=5, pady=5)

        #self.bind("<Return>", self.ok)
        self.bind("<Escape>", self.cancel)

        box.pack()

    def body(self, master):

        r = 0
        Label(master, text=self.valsList[0][0]).grid(row=r)
        entry = Entry(master)
        self.entryStrings[entry] = self.valsList[0][2]
        self.entries.append(entry)
        self.entries[-1].grid(row=r,column=1)
        self.entries[-1].insert(0,self.valsList[0][1])
        r += 1
        
        self.optionsDict = OrderedDict()

        for val in self.valsList[1:]:
            Label(master, text=val[0]).grid(row=r)
            text1 = Text(
                master
            )
            text1.insert(END,self.fieldsToString(val[1]))
            self.options.append(text1)
            self.options[-1].grid(row=r,column=1)
            r += 1

        return self.entries[0] # initial focus

    def fieldsToString(self,fieldsList):
        retStr = ""
        for fields in fieldsList:
            retStr += "{0} {1}".format(fields[0],fields[1])
            if len(fields) == 3:
                retStr += " = {0}".format(fields[2])
            retStr += ";\n"
        return retStr

    def parseFields(self,stringLine):
        fields = None
        splits = stringLine.split(' ')
        if len(splits) < 2:
            return fields
        if validString(splits[0],fieldStringChars):
            fields = []
            fields.append(splits[0])
            if len(splits) == 2 and splits[1][-1] == ';':
                if validString(splits[1][0:-1],fieldStringChars):
                    fields.append(splits[1][0:-1])
                else:
                    fields = None
            elif len(splits) == 4 and splits[2] == '=' and splits[3][-1] == ';':
                if validString(splits[1],fieldStringChars):
                    fields.append(splits[1])
                    if validString(splits[3][0:-1],fieldStringChars):
                        fields.append(splits[3][0:-1])
                    else:
                        fields = None
                else:
                    fields = None
            else:
                fields = None
        return fields

    def validate(self):
        for textBox in self.options:
            text = textBox.get(1.0,END)
            lines = text.split("\n")
            for line in lines:
                if len(line) == 0:
                    continue
                fields = self.parseFields(line)
                if fields == None:
                    tkMessageBox.showwarning(
                        "Bad Input",
                        "Input:\n{0}\nmust be ROS formatted, please try again.".format(line)
                    )
                    return 0
        return 1

    def apply(self):
        self.result = []
        for entry in self.entries:
            val = entry.get()
            self.result.append(val)        
        for textBox in self.options:
            text = textBox.get(1.0,END)
            lines = text.split("\n")
            boxFields = []
            for line in lines:
                fields = self.parseFields(line)
                if fields != None:
                    boxFields.append(fields)
            self.result.append(boxFields)

