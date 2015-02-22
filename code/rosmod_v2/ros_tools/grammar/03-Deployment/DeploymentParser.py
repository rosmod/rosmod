# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO
package = globals().get("__package__", None)
ischild = len(package)>0 if package is not None else False
if ischild:
    from .DeploymentListener import DeploymentListener
else:
    from DeploymentListener import DeploymentListener
def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3")
        buf.write(u"\r?\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4")
        buf.write(u"\b\t\b\4\t\t\t\4\n\t\n\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3")
        buf.write(u"\3\3\4\3\4\3\5\6\5 \n\5\r\5\16\5!\3\6\3\6\3\6\3\6\6\6")
        buf.write(u"(\n\6\r\6\16\6)\3\6\3\6\3\7\3\7\3\b\3\b\3\b\3\b\3\b\3")
        buf.write(u"\b\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\n\3\n\3\n\2\2\13\2\4")
        buf.write(u"\6\b\n\f\16\20\22\2\2\67\2\24\3\2\2\2\4\30\3\2\2\2\6")
        buf.write(u"\34\3\2\2\2\b\37\3\2\2\2\n#\3\2\2\2\f-\3\2\2\2\16/\3")
        buf.write(u"\2\2\2\20:\3\2\2\2\22<\3\2\2\2\24\25\5\4\3\2\25\26\5")
        buf.write(u"\b\5\2\26\27\7\2\2\3\27\3\3\2\2\2\30\31\7\5\2\2\31\32")
        buf.write(u"\5\6\4\2\32\33\7\n\2\2\33\5\3\2\2\2\34\35\7\f\2\2\35")
        buf.write(u"\7\3\2\2\2\36 \5\n\6\2\37\36\3\2\2\2 !\3\2\2\2!\37\3")
        buf.write(u"\2\2\2!\"\3\2\2\2\"\t\3\2\2\2#$\7\7\2\2$%\5\f\7\2%\'")
        buf.write(u"\7\4\2\2&(\5\16\b\2\'&\3\2\2\2()\3\2\2\2)\'\3\2\2\2)")
        buf.write(u"*\3\2\2\2*+\3\2\2\2+,\7\t\2\2,\13\3\2\2\2-.\7\f\2\2.")
        buf.write(u"\r\3\2\2\2/\60\7\6\2\2\60\61\5\20\t\2\61\62\7\4\2\2\62")
        buf.write(u"\63\7\3\2\2\63\64\7\b\2\2\64\65\7\13\2\2\65\66\5\22\n")
        buf.write(u"\2\66\67\7\13\2\2\678\7\n\2\289\7\t\2\29\17\3\2\2\2:")
        buf.write(u";\7\f\2\2;\21\3\2\2\2<=\7\f\2\2=\23\3\2\2\2\4!)")
        return buf.getvalue()
		

class DeploymentParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__8=1
    T__7=2
    T__6=3
    T__5=4
    T__4=5
    T__3=6
    T__2=7
    T__1=8
    T__0=9
    ID=10
    WS=11

    tokenNames = [ u"<INVALID>", u"'alias'", u"'{'", u"'deployment'", u"'node'", 
                   u"'host'", u"'='", u"'}'", u"';'", u"'\"'", u"ID", u"WS" ]

    RULE_start = 0
    RULE_define_deployment = 1
    RULE_deployment_name = 2
    RULE_deployment = 3
    RULE_node_host_mapping = 4
    RULE_hostname = 5
    RULE_nodes = 6
    RULE_node = 7
    RULE_node_alias = 8

    ruleNames =  [ u"start", u"define_deployment", u"deployment_name", u"deployment", 
                   u"node_host_mapping", u"hostname", u"nodes", u"node", 
                   u"node_alias" ]

    def __init__(self, input):
        super(DeploymentParser, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None



    class StartContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.StartContext, self).__init__(parent, invokingState)
            self.parser = parser

        def EOF(self):
            return self.getToken(DeploymentParser.EOF, 0)

        def define_deployment(self):
            return self.getTypedRuleContext(DeploymentParser.Define_deploymentContext,0)


        def deployment(self):
            return self.getTypedRuleContext(DeploymentParser.DeploymentContext,0)


        def getRuleIndex(self):
            return DeploymentParser.RULE_start

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterStart(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitStart(self)




    def start(self):

        localctx = DeploymentParser.StartContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_start)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 18 
            self.define_deployment()

            self.state = 19 
            self.deployment()
            self.state = 20
            self.match(self.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Define_deploymentContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.Define_deploymentContext, self).__init__(parent, invokingState)
            self.parser = parser

        def deployment_name(self):
            return self.getTypedRuleContext(DeploymentParser.Deployment_nameContext,0)


        def getRuleIndex(self):
            return DeploymentParser.RULE_define_deployment

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterDefine_deployment(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitDefine_deployment(self)




    def define_deployment(self):

        localctx = DeploymentParser.Define_deploymentContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_define_deployment)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 22
            self.match(self.T__6)
            self.state = 23 
            self.deployment_name()
            self.state = 24
            self.match(self.T__1)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Deployment_nameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.Deployment_nameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(DeploymentParser.ID, 0)

        def getRuleIndex(self):
            return DeploymentParser.RULE_deployment_name

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterDeployment_name(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitDeployment_name(self)




    def deployment_name(self):

        localctx = DeploymentParser.Deployment_nameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_deployment_name)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 26
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class DeploymentContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.DeploymentContext, self).__init__(parent, invokingState)
            self.parser = parser

        def node_host_mapping(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(DeploymentParser.Node_host_mappingContext)
            else:
                return self.getTypedRuleContext(DeploymentParser.Node_host_mappingContext,i)


        def getRuleIndex(self):
            return DeploymentParser.RULE_deployment

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterDeployment(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitDeployment(self)




    def deployment(self):

        localctx = DeploymentParser.DeploymentContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_deployment)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 29 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 28 
                self.node_host_mapping()
                self.state = 31 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==DeploymentParser.T__4):
                    break

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Node_host_mappingContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.Node_host_mappingContext, self).__init__(parent, invokingState)
            self.parser = parser

        def nodes(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(DeploymentParser.NodesContext)
            else:
                return self.getTypedRuleContext(DeploymentParser.NodesContext,i)


        def hostname(self):
            return self.getTypedRuleContext(DeploymentParser.HostnameContext,0)


        def getRuleIndex(self):
            return DeploymentParser.RULE_node_host_mapping

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterNode_host_mapping(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitNode_host_mapping(self)




    def node_host_mapping(self):

        localctx = DeploymentParser.Node_host_mappingContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_node_host_mapping)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 33
            self.match(self.T__4)
            self.state = 34 
            self.hostname()
            self.state = 35
            self.match(self.T__7)
            self.state = 37 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 36 
                self.nodes()
                self.state = 39 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==DeploymentParser.T__5):
                    break

            self.state = 41
            self.match(self.T__2)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class HostnameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.HostnameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(DeploymentParser.ID, 0)

        def getRuleIndex(self):
            return DeploymentParser.RULE_hostname

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterHostname(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitHostname(self)




    def hostname(self):

        localctx = DeploymentParser.HostnameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_hostname)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 43
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class NodesContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.NodesContext, self).__init__(parent, invokingState)
            self.parser = parser

        def node_alias(self):
            return self.getTypedRuleContext(DeploymentParser.Node_aliasContext,0)


        def node(self):
            return self.getTypedRuleContext(DeploymentParser.NodeContext,0)


        def getRuleIndex(self):
            return DeploymentParser.RULE_nodes

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterNodes(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitNodes(self)




    def nodes(self):

        localctx = DeploymentParser.NodesContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_nodes)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 45
            self.match(self.T__5)
            self.state = 46 
            self.node()
            self.state = 47
            self.match(self.T__7)
            self.state = 48
            self.match(self.T__8)
            self.state = 49
            self.match(self.T__3)
            self.state = 50
            self.match(self.T__0)
            self.state = 51 
            self.node_alias()
            self.state = 52
            self.match(self.T__0)
            self.state = 53
            self.match(self.T__1)
            self.state = 54
            self.match(self.T__2)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class NodeContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.NodeContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(DeploymentParser.ID, 0)

        def getRuleIndex(self):
            return DeploymentParser.RULE_node

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterNode(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitNode(self)




    def node(self):

        localctx = DeploymentParser.NodeContext(self, self._ctx, self.state)
        self.enterRule(localctx, 14, self.RULE_node)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 56
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Node_aliasContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.Node_aliasContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(DeploymentParser.ID, 0)

        def getRuleIndex(self):
            return DeploymentParser.RULE_node_alias

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterNode_alias(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitNode_alias(self)




    def node_alias(self):

        localctx = DeploymentParser.Node_aliasContext(self, self._ctx, self.state)
        self.enterRule(localctx, 16, self.RULE_node_alias)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 58
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




