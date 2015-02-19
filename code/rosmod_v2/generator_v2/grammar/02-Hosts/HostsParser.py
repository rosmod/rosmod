# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO
package = globals().get("__package__", None)
ischild = len(package)>0 if package is not None else False
if ischild:
    from .HostsListener import HostsListener
else:
    from HostsListener import HostsListener
def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3")
        buf.write(u"\f)\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\3\2\6\2\16")
        buf.write(u"\n\2\r\2\16\2\17\3\2\3\2\3\3\3\3\3\3\3\3\3\3\5\3\31\n")
        buf.write(u"\3\3\3\3\3\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3")
        buf.write(u"\6\3\6\3\6\2\2\7\2\4\6\b\n\2\2%\2\r\3\2\2\2\4\23\3\2")
        buf.write(u"\2\2\6\34\3\2\2\2\b\36\3\2\2\2\n#\3\2\2\2\f\16\5\4\3")
        buf.write(u"\2\r\f\3\2\2\2\16\17\3\2\2\2\17\r\3\2\2\2\17\20\3\2\2")
        buf.write(u"\2\20\21\3\2\2\2\21\22\7\2\2\3\22\3\3\2\2\2\23\24\7\6")
        buf.write(u"\2\2\24\25\5\6\4\2\25\26\7\3\2\2\26\30\5\b\5\2\27\31")
        buf.write(u"\5\n\6\2\30\27\3\2\2\2\30\31\3\2\2\2\31\32\3\2\2\2\32")
        buf.write(u"\33\7\b\2\2\33\5\3\2\2\2\34\35\7\13\2\2\35\7\3\2\2\2")
        buf.write(u"\36\37\7\4\2\2\37 \7\7\2\2 !\7\n\2\2!\"\7\t\2\2\"\t\3")
        buf.write(u"\2\2\2#$\7\5\2\2$%\7\7\2\2%&\7\13\2\2&\'\7\t\2\2\'\13")
        buf.write(u"\3\2\2\2\4\17\30")
        return buf.getvalue()
		

class HostsParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__6=1
    T__5=2
    T__4=3
    T__3=4
    T__2=5
    T__1=6
    T__0=7
    ADDRESS=8
    ID=9
    WS=10

    tokenNames = [ u"<INVALID>", u"'{'", u"'ip_address'", u"'architecture'", 
                   u"'host'", u"'='", u"'}'", u"';'", u"ADDRESS", u"ID", 
                   u"WS" ]

    RULE_start = 0
    RULE_host = 1
    RULE_host_name = 2
    RULE_ip_address = 3
    RULE_architecture = 4

    ruleNames =  [ u"start", u"host", u"host_name", u"ip_address", u"architecture" ]

    def __init__(self, input):
        super(HostsParser, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None



    class StartContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.StartContext, self).__init__(parent, invokingState)
            self.parser = parser

        def EOF(self):
            return self.getToken(HostsParser.EOF, 0)

        def host(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(HostsParser.HostContext)
            else:
                return self.getTypedRuleContext(HostsParser.HostContext,i)


        def getRuleIndex(self):
            return HostsParser.RULE_start

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterStart(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitStart(self)




    def start(self):

        localctx = HostsParser.StartContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_start)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 11 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 10 
                self.host()
                self.state = 13 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==HostsParser.T__3):
                    break

            self.state = 15
            self.match(self.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class HostContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.HostContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ip_address(self):
            return self.getTypedRuleContext(HostsParser.Ip_addressContext,0)


        def architecture(self):
            return self.getTypedRuleContext(HostsParser.ArchitectureContext,0)


        def host_name(self):
            return self.getTypedRuleContext(HostsParser.Host_nameContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_host

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterHost(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitHost(self)




    def host(self):

        localctx = HostsParser.HostContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_host)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 17
            self.match(self.T__3)
            self.state = 18 
            self.host_name()
            self.state = 19
            self.match(self.T__6)

            self.state = 20 
            self.ip_address()
            self.state = 22
            _la = self._input.LA(1)
            if _la==HostsParser.T__4:
                self.state = 21 
                self.architecture()


            self.state = 24
            self.match(self.T__1)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Host_nameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Host_nameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_host_name

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterHost_name(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitHost_name(self)




    def host_name(self):

        localctx = HostsParser.Host_nameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_host_name)
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

    class Ip_addressContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Ip_addressContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ADDRESS(self):
            return self.getToken(HostsParser.ADDRESS, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_ip_address

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterIp_address(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitIp_address(self)




    def ip_address(self):

        localctx = HostsParser.Ip_addressContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_ip_address)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 28
            self.match(self.T__5)
            self.state = 29
            self.match(self.T__2)
            self.state = 30
            self.match(self.ADDRESS)
            self.state = 31
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class ArchitectureContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.ArchitectureContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_architecture

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterArchitecture(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitArchitecture(self)




    def architecture(self):

        localctx = HostsParser.ArchitectureContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_architecture)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 33
            self.match(self.T__4)
            self.state = 34
            self.match(self.T__2)
            self.state = 35
            self.match(self.ID)
            self.state = 36
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




