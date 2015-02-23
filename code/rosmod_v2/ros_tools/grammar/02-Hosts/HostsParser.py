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
        buf.write(u"\17\65\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t")
        buf.write(u"\7\4\b\t\b\3\2\6\2\22\n\2\r\2\16\2\23\3\2\3\2\3\3\3\3")
        buf.write(u"\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\5\7\61\n\7\3")
        buf.write(u"\b\3\b\3\b\2\2\t\2\4\6\b\n\f\16\2\2/\2\21\3\2\2\2\4\27")
        buf.write(u"\3\2\2\2\6\36\3\2\2\2\b \3\2\2\2\n\'\3\2\2\2\f\60\3\2")
        buf.write(u"\2\2\16\62\3\2\2\2\20\22\5\4\3\2\21\20\3\2\2\2\22\23")
        buf.write(u"\3\2\2\2\23\21\3\2\2\2\23\24\3\2\2\2\24\25\3\2\2\2\25")
        buf.write(u"\26\7\2\2\3\26\3\3\2\2\2\27\30\7\6\2\2\30\31\5\6\4\2")
        buf.write(u"\31\32\7\3\2\2\32\33\5\b\5\2\33\34\5\f\7\2\34\35\7\b")
        buf.write(u"\2\2\35\5\3\2\2\2\36\37\7\f\2\2\37\7\3\2\2\2 !\7\4\2")
        buf.write(u"\2!\"\7\7\2\2\"#\7\t\2\2#$\5\n\6\2$%\7\t\2\2%&\7\n\2")
        buf.write(u"\2&\t\3\2\2\2\'(\7\13\2\2(\13\3\2\2\2)*\7\5\2\2*+\7\7")
        buf.write(u"\2\2+,\7\t\2\2,-\5\16\b\2-.\7\t\2\2./\7\n\2\2/\61\3\2")
        buf.write(u"\2\2\60)\3\2\2\2\60\61\3\2\2\2\61\r\3\2\2\2\62\63\7\f")
        buf.write(u"\2\2\63\17\3\2\2\2\4\23\60")
        return buf.getvalue()
		

class HostsParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__7=1
    T__6=2
    T__5=3
    T__4=4
    T__3=5
    T__2=6
    T__1=7
    T__0=8
    IP_ADDRESS_STRING=9
    ID=10
    WS=11
    COMMENT=12
    LINE_COMMENT=13

    tokenNames = [ u"<INVALID>", u"'{'", u"'ip_address'", u"'architecture'", 
                   u"'host'", u"'='", u"'}'", u"'\"'", u"';'", u"IP_ADDRESS_STRING", 
                   u"ID", u"WS", u"COMMENT", u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_host = 1
    RULE_host_name = 2
    RULE_ip_address = 3
    RULE_ip_address_string = 4
    RULE_architecture = 5
    RULE_architecture_string = 6

    ruleNames =  [ u"start", u"host", u"host_name", u"ip_address", u"ip_address_string", 
                   u"architecture", u"architecture_string" ]

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
            self.state = 15 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 14 
                self.host()
                self.state = 17 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==HostsParser.T__4):
                    break

            self.state = 19
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
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 21
            self.match(self.T__4)
            self.state = 22 
            self.host_name()
            self.state = 23
            self.match(self.T__7)

            self.state = 24 
            self.ip_address()

            self.state = 25 
            self.architecture()
            self.state = 26
            self.match(self.T__2)
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
            self.state = 28
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

        def ip_address_string(self):
            return self.getTypedRuleContext(HostsParser.Ip_address_stringContext,0)


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
            self.state = 30
            self.match(self.T__6)
            self.state = 31
            self.match(self.T__3)
            self.state = 32
            self.match(self.T__1)
            self.state = 33 
            self.ip_address_string()
            self.state = 34
            self.match(self.T__1)
            self.state = 35
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Ip_address_stringContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Ip_address_stringContext, self).__init__(parent, invokingState)
            self.parser = parser

        def IP_ADDRESS_STRING(self):
            return self.getToken(HostsParser.IP_ADDRESS_STRING, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_ip_address_string

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterIp_address_string(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitIp_address_string(self)




    def ip_address_string(self):

        localctx = HostsParser.Ip_address_stringContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_ip_address_string)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 37
            self.match(self.IP_ADDRESS_STRING)
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

        def architecture_string(self):
            return self.getTypedRuleContext(HostsParser.Architecture_stringContext,0)


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
        self.enterRule(localctx, 10, self.RULE_architecture)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 46
            _la = self._input.LA(1)
            if _la==HostsParser.T__5:
                self.state = 39
                self.match(self.T__5)
                self.state = 40
                self.match(self.T__3)
                self.state = 41
                self.match(self.T__1)
                self.state = 42 
                self.architecture_string()
                self.state = 43
                self.match(self.T__1)
                self.state = 44
                self.match(self.T__0)


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Architecture_stringContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Architecture_stringContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_architecture_string

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterArchitecture_string(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitArchitecture_string(self)




    def architecture_string(self):

        localctx = HostsParser.Architecture_stringContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_architecture_string)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 48
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




