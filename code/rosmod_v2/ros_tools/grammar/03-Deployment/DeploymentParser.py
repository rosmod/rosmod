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
        buf.write(u"\13\'\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\3\2\3\2\3\2\3\3\6\3\23\n\3\r\3\16\3\24\3\4\3\4\3\4\3")
        buf.write(u"\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\6\3\6\3\7\3\7\3")
        buf.write(u"\7\2\2\b\2\4\6\b\n\f\2\2!\2\16\3\2\2\2\4\22\3\2\2\2\6")
        buf.write(u"\26\3\2\2\2\b \3\2\2\2\n\"\3\2\2\2\f$\3\2\2\2\16\17\5")
        buf.write(u"\4\3\2\17\20\7\2\2\3\20\3\3\2\2\2\21\23\5\6\4\2\22\21")
        buf.write(u"\3\2\2\2\23\24\3\2\2\2\24\22\3\2\2\2\24\25\3\2\2\2\25")
        buf.write(u"\5\3\2\2\2\26\27\7\3\2\2\27\30\7\7\2\2\30\31\5\b\5\2")
        buf.write(u"\31\32\7\6\2\2\32\33\5\n\6\2\33\34\7\4\2\2\34\35\7\5")
        buf.write(u"\2\2\35\36\5\f\7\2\36\37\7\b\2\2\37\7\3\2\2\2 !\7\t\2")
        buf.write(u"\2!\t\3\2\2\2\"#\7\n\2\2#\13\3\2\2\2$%\7\n\2\2%\r\3\2")
        buf.write(u"\2\2\3\24")
        return buf.getvalue()
		

class DeploymentParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__5=1
    T__4=2
    T__3=3
    T__2=4
    T__1=5
    T__0=6
    ROS_NODE=7
    ID=8
    WS=9

    tokenNames = [ u"<INVALID>", u"'deploy'", u"'>'", u"'on'", u"','", u"'<'", 
                   u"';'", u"ROS_NODE", u"ID", u"WS" ]

    RULE_start = 0
    RULE_deployment = 1
    RULE_node_host_mapping = 2
    RULE_node = 3
    RULE_node_alias = 4
    RULE_host = 5

    ruleNames =  [ u"start", u"deployment", u"node_host_mapping", u"node", 
                   u"node_alias", u"host" ]

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
            self.state = 12 
            self.deployment()
            self.state = 13
            self.match(self.EOF)
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
        self.enterRule(localctx, 2, self.RULE_deployment)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 16 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 15 
                self.node_host_mapping()
                self.state = 18 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==DeploymentParser.T__5):
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

        def node_alias(self):
            return self.getTypedRuleContext(DeploymentParser.Node_aliasContext,0)


        def host(self):
            return self.getTypedRuleContext(DeploymentParser.HostContext,0)


        def node(self):
            return self.getTypedRuleContext(DeploymentParser.NodeContext,0)


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
        self.enterRule(localctx, 4, self.RULE_node_host_mapping)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 20
            self.match(self.T__5)
            self.state = 21
            self.match(self.T__1)
            self.state = 22 
            self.node()
            self.state = 23
            self.match(self.T__2)
            self.state = 24 
            self.node_alias()
            self.state = 25
            self.match(self.T__4)
            self.state = 26
            self.match(self.T__3)
            self.state = 27 
            self.host()
            self.state = 28
            self.match(self.T__0)
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

        def ROS_NODE(self):
            return self.getToken(DeploymentParser.ROS_NODE, 0)

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
        self.enterRule(localctx, 6, self.RULE_node)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 30
            self.match(self.ROS_NODE)
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
        self.enterRule(localctx, 8, self.RULE_node_alias)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 32
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class HostContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(DeploymentParser.HostContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(DeploymentParser.ID, 0)

        def getRuleIndex(self):
            return DeploymentParser.RULE_host

        def enterRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.enterHost(self)

        def exitRule(self, listener):
            if isinstance( listener, DeploymentListener ):
                listener.exitHost(self)




    def host(self):

        localctx = DeploymentParser.HostContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_host)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 34
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




