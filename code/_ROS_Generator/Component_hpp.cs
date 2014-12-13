﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 11.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace ROS_Generator
{
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "11.0.0.0")]
    public partial class Component_hpp : Component_hppBase
    {
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            this.Write("#ifndef ");
            
            #line 1 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(define_guard);
            
            #line default
            #line hidden
            this.Write("_HPP\r\n#define ");
            
            #line 2 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(define_guard);
            
            #line default
            #line hidden
            this.Write("_HPP\r\n\r\n#include \"ros/ros.h\"\r\n\r\n#include \"Component.hpp\"\r\n\r\n");
            
            #line 8 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var topic in topics)
{ 
            
            #line default
            #line hidden
            this.Write("#include \"");
            
            #line 10 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(project_name);
            
            #line default
            #line hidden
            this.Write("/");
            
            #line 10 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(topic);
            
            #line default
            #line hidden
            this.Write(".h\"\r\n");
            
            #line 11 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 12 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var srv in services)
{ 
            
            #line default
            #line hidden
            this.Write("#include \"");
            
            #line 14 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(project_name);
            
            #line default
            #line hidden
            this.Write("/");
            
            #line 14 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(srv);
            
            #line default
            #line hidden
            this.Write(".h\"\r\n");
            
            #line 15 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\n// --------------------------------\r\n//      USER INCLUDES GO HERE\r\n// --------" +
                    "------------------------\r\n\r\nnamespace ");
            
            #line 21 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(node_name);
            
            #line default
            #line hidden
            this.Write("\r\n{\r\n");
            
            #line 24 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("    ");
            
            #line default
            #line hidden
            this.Write("class ");
            
            #line 24 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(comp_name);
            
            #line default
            #line hidden
            this.Write(" : public Component\r\n");
            
            #line 25 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("    ");
            
            #line default
            #line hidden
            this.Write("{\r\n");
            
            #line 26 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("        ");
            
            #line default
            #line hidden
            this.Write("public:\r\n\r\n");
            
            #line 28 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// These functions\' business logic will be filled in by the user:\r\n\r\n");
            
            #line 30 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// Init() is always generated\r\n");
            
            #line 31 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("void Init(const ros::TimerEvent& event);\r\n");
            
            #line 32 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 if (subscribers.Count > 0) { 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 33 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var sub in subscribers) { 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 35 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// OnOneData Subscription handler for ");
            
            #line 35 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(sub.name);
            
            #line default
            #line hidden
            this.Write(" subscriber \r\n");
            
            #line 36 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("void OnOneData(const ");
            
            #line 36 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(project_name);
            
            #line default
            #line hidden
            this.Write("::");
            
            #line 36 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(sub.topic);
            
            #line default
            #line hidden
            this.Write("::ConstPtr& received_data);  \r\n\t");
            
            #line 37 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 38 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 39 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 if (provided_services.Count > 0) { 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 40 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var provided in provided_services) { 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 42 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// Component Service Callback\r\n");
            
            #line 43 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("bool ");
            
            #line 43 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(provided.name);
            
            #line default
            #line hidden
            this.Write("Callback(");
            
            #line 43 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(project_name);
            
            #line default
            #line hidden
            this.Write("::");
            
            #line 43 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(provided.name);
            
            #line default
            #line hidden
            this.Write("::Request  &req,\r\n");
            
            #line 44 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 for (int i=0; i<=provided.name.Length + 12; i++) 
            
            #line default
            #line hidden
            this.Write(" ");
            
            #line 44 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(" ");
            
            #line default
            #line hidden
            
            #line 45 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            
            #line 45 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(project_name);
            
            #line default
            #line hidden
            this.Write("::");
            
            #line 45 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(provided.name);
            
            #line default
            #line hidden
            this.Write("::Response &res)\r\n\t");
            
            #line 46 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 47 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 48 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 if (timers.Count > 0) { 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 49 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var timer in timers) { 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 51 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// Callback for ");
            
            #line 51 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(timer.name);
            
            #line default
            #line hidden
            this.Write(" timer\r\n");
            
            #line 52 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("void ");
            
            #line 52 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(timer.name);
            
            #line default
            #line hidden
            this.Write("Callback()(const ros::TimerEvent& event);\r\n\t");
            
            #line 53 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            
            #line 54 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write(" \r\n");
            
            #line 55 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// these functions\' business logic will be auto-generated:\r\n\r\n");
            
            #line 57 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// startUp() is used to configure timers, publishers, & service providers\r\n");
            
            #line 58 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("void startUp();\r\n\r\n");
            
            #line 60 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// required for clean shutdown\r\n");
            
            #line 61 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("~");
            
            #line 61 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(comp_name);
            
            #line default
            #line hidden
            this.Write("();\r\n\r\n");
            
            #line 63 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("        ");
            
            #line default
            #line hidden
            this.Write("private:\r\n\t");
            
            #line 64 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var timer in timers)
	{ 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 67 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// ROS Timer - ");
            
            #line 67 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(timer.name);
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 69 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("ros::Timer ");
            
            #line 69 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(timer.name);
            
            #line default
            #line hidden
            this.Write(";\r\n\t");
            
            #line 70 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 71 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var sub in subscribers)
	{ 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 74 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// ROS Subscriber - ");
            
            #line 74 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(sub.name);
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 76 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("ros::Subscriber ");
            
            #line 76 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(sub.name);
            
            #line default
            #line hidden
            this.Write(";\r\n\t");
            
            #line 77 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 78 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var pub in publishers)
	{ 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 81 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// ROS Publisher - ");
            
            #line 81 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(pub.name);
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 83 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("ros::Publisher ");
            
            #line 83 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(pub.name);
            
            #line default
            #line hidden
            this.Write(";\r\n\t");
            
            #line 84 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 85 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var provided in provided_services)
	{ 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 88 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// ROS Service Server - ");
            
            #line 88 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(provided.name);
            
            #line default
            #line hidden
            this.Write("_Server\r\n");
            
            #line 89 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("ros::ServiceServer ");
            
            #line 89 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(provided.name);
            
            #line default
            #line hidden
            this.Write("_Server;\r\n\t");
            
            #line 90 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\t");
            
            #line 91 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 foreach (var required in required_services)
	{ 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 94 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("// ROS Service Client - ");
            
            #line 94 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(required.name);
            
            #line default
            #line hidden
            this.Write("_Client\r\n");
            
            #line 95 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("            ");
            
            #line default
            #line hidden
            this.Write("ros::ServiceClient ");
            
            #line 95 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write(required.name);
            
            #line default
            #line hidden
            this.Write("_Client;\r\n\t");
            
            #line 96 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 98 "C:\Users\Pranav\Documents\GitHub\rosmod\code\_ROS_Generator\Component_hpp.tt"
Write("    ");
            
            #line default
            #line hidden
            this.Write("};\r\n};\r\n\r\n#endif");
            return this.GenerationEnvironment.ToString();
        }
    }
    
    #line default
    #line hidden
    #region Base class
    /// <summary>
    /// Base class for this transformation
    /// </summary>
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "11.0.0.0")]
    public class Component_hppBase
    {
        #region Fields
        private global::System.Text.StringBuilder generationEnvironmentField;
        private global::System.CodeDom.Compiler.CompilerErrorCollection errorsField;
        private global::System.Collections.Generic.List<int> indentLengthsField;
        private string currentIndentField = "";
        private bool endsWithNewline;
        private global::System.Collections.Generic.IDictionary<string, object> sessionField;
        #endregion
        #region Properties
        /// <summary>
        /// The string builder that generation-time code is using to assemble generated output
        /// </summary>
        protected System.Text.StringBuilder GenerationEnvironment
        {
            get
            {
                if ((this.generationEnvironmentField == null))
                {
                    this.generationEnvironmentField = new global::System.Text.StringBuilder();
                }
                return this.generationEnvironmentField;
            }
            set
            {
                this.generationEnvironmentField = value;
            }
        }
        /// <summary>
        /// The error collection for the generation process
        /// </summary>
        public System.CodeDom.Compiler.CompilerErrorCollection Errors
        {
            get
            {
                if ((this.errorsField == null))
                {
                    this.errorsField = new global::System.CodeDom.Compiler.CompilerErrorCollection();
                }
                return this.errorsField;
            }
        }
        /// <summary>
        /// A list of the lengths of each indent that was added with PushIndent
        /// </summary>
        private System.Collections.Generic.List<int> indentLengths
        {
            get
            {
                if ((this.indentLengthsField == null))
                {
                    this.indentLengthsField = new global::System.Collections.Generic.List<int>();
                }
                return this.indentLengthsField;
            }
        }
        /// <summary>
        /// Gets the current indent we use when adding lines to the output
        /// </summary>
        public string CurrentIndent
        {
            get
            {
                return this.currentIndentField;
            }
        }
        /// <summary>
        /// Current transformation session
        /// </summary>
        public virtual global::System.Collections.Generic.IDictionary<string, object> Session
        {
            get
            {
                return this.sessionField;
            }
            set
            {
                this.sessionField = value;
            }
        }
        #endregion
        #region Transform-time helpers
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void Write(string textToAppend)
        {
            if (string.IsNullOrEmpty(textToAppend))
            {
                return;
            }
            // If we're starting off, or if the previous text ended with a newline,
            // we have to append the current indent first.
            if (((this.GenerationEnvironment.Length == 0) 
                        || this.endsWithNewline))
            {
                this.GenerationEnvironment.Append(this.currentIndentField);
                this.endsWithNewline = false;
            }
            // Check if the current text ends with a newline
            if (textToAppend.EndsWith(global::System.Environment.NewLine, global::System.StringComparison.CurrentCulture))
            {
                this.endsWithNewline = true;
            }
            // This is an optimization. If the current indent is "", then we don't have to do any
            // of the more complex stuff further down.
            if ((this.currentIndentField.Length == 0))
            {
                this.GenerationEnvironment.Append(textToAppend);
                return;
            }
            // Everywhere there is a newline in the text, add an indent after it
            textToAppend = textToAppend.Replace(global::System.Environment.NewLine, (global::System.Environment.NewLine + this.currentIndentField));
            // If the text ends with a newline, then we should strip off the indent added at the very end
            // because the appropriate indent will be added when the next time Write() is called
            if (this.endsWithNewline)
            {
                this.GenerationEnvironment.Append(textToAppend, 0, (textToAppend.Length - this.currentIndentField.Length));
            }
            else
            {
                this.GenerationEnvironment.Append(textToAppend);
            }
        }
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void WriteLine(string textToAppend)
        {
            this.Write(textToAppend);
            this.GenerationEnvironment.AppendLine();
            this.endsWithNewline = true;
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void Write(string format, params object[] args)
        {
            this.Write(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void WriteLine(string format, params object[] args)
        {
            this.WriteLine(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Raise an error
        /// </summary>
        public void Error(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Raise a warning
        /// </summary>
        public void Warning(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            error.IsWarning = true;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Increase the indent
        /// </summary>
        public void PushIndent(string indent)
        {
            if ((indent == null))
            {
                throw new global::System.ArgumentNullException("indent");
            }
            this.currentIndentField = (this.currentIndentField + indent);
            this.indentLengths.Add(indent.Length);
        }
        /// <summary>
        /// Remove the last indent that was added with PushIndent
        /// </summary>
        public string PopIndent()
        {
            string returnValue = "";
            if ((this.indentLengths.Count > 0))
            {
                int indentLength = this.indentLengths[(this.indentLengths.Count - 1)];
                this.indentLengths.RemoveAt((this.indentLengths.Count - 1));
                if ((indentLength > 0))
                {
                    returnValue = this.currentIndentField.Substring((this.currentIndentField.Length - indentLength));
                    this.currentIndentField = this.currentIndentField.Remove((this.currentIndentField.Length - indentLength));
                }
            }
            return returnValue;
        }
        /// <summary>
        /// Remove any indentation
        /// </summary>
        public void ClearIndent()
        {
            this.indentLengths.Clear();
            this.currentIndentField = "";
        }
        #endregion
        #region ToString Helpers
        /// <summary>
        /// Utility class to produce culture-oriented representation of an object as a string.
        /// </summary>
        public class ToStringInstanceHelper
        {
            private System.IFormatProvider formatProviderField  = global::System.Globalization.CultureInfo.InvariantCulture;
            /// <summary>
            /// Gets or sets format provider to be used by ToStringWithCulture method.
            /// </summary>
            public System.IFormatProvider FormatProvider
            {
                get
                {
                    return this.formatProviderField ;
                }
                set
                {
                    if ((value != null))
                    {
                        this.formatProviderField  = value;
                    }
                }
            }
            /// <summary>
            /// This is called from the compile/run appdomain to convert objects within an expression block to a string
            /// </summary>
            public string ToStringWithCulture(object objectToConvert)
            {
                if ((objectToConvert == null))
                {
                    throw new global::System.ArgumentNullException("objectToConvert");
                }
                System.Type t = objectToConvert.GetType();
                System.Reflection.MethodInfo method = t.GetMethod("ToString", new System.Type[] {
                            typeof(System.IFormatProvider)});
                if ((method == null))
                {
                    return objectToConvert.ToString();
                }
                else
                {
                    return ((string)(method.Invoke(objectToConvert, new object[] {
                                this.formatProviderField })));
                }
            }
        }
        private ToStringInstanceHelper toStringHelperField = new ToStringInstanceHelper();
        /// <summary>
        /// Helper to produce culture-oriented representation of an object as a string
        /// </summary>
        public ToStringInstanceHelper ToStringHelper
        {
            get
            {
                return this.toStringHelperField;
            }
        }
        #endregion
    }
    #endregion
}
