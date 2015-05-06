# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO
package = globals().get("__package__", None)
ischild = len(package)>0 if package is not None else False
if ischild:
    from .ROSMOD_HardwareListener import ROSMOD_HardwareListener
else:
    from ROSMOD_HardwareListener import ROSMOD_HardwareListener
def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3")
        buf.write(u"\16,\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\3\2\7\2")
        buf.write(u"\16\n\2\f\2\16\2\21\13\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\6\3\6\3\6\2\2\7\2\4\6\b\n\2\2\'\2\17\3")
        buf.write(u"\2\2\2\4\24\3\2\2\2\6\26\3\2\2\2\b\30\3\2\2\2\n)\3\2")
        buf.write(u"\2\2\f\16\5\b\5\2\r\f\3\2\2\2\16\21\3\2\2\2\17\r\3\2")
        buf.write(u"\2\2\17\20\3\2\2\2\20\22\3\2\2\2\21\17\3\2\2\2\22\23")
        buf.write(u"\7\2\2\3\23\3\3\2\2\2\24\25\7\13\2\2\25\5\3\2\2\2\26")
        buf.write(u"\27\7\13\2\2\27\7\3\2\2\2\30\31\7\3\2\2\31\32\5\4\3\2")
        buf.write(u"\32\33\7\4\2\2\33\34\7\5\2\2\34\35\7\7\2\2\35\36\7\t")
        buf.write(u"\2\2\36\37\5\n\6\2\37 \7\t\2\2 !\7\n\2\2!\"\3\2\2\2\"")
        buf.write(u"#\7\6\2\2#$\7\7\2\2$%\5\6\4\2%&\7\n\2\2&\'\3\2\2\2\'")
        buf.write(u"(\7\b\2\2(\t\3\2\2\2)*\7\13\2\2*\13\3\2\2\2\3\17")
        return buf.getvalue()
		

class ROSMOD_HardwareParser ( Parser ):

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
    ID=9
    WS=10
    COMMENT=11
    LINE_COMMENT=12

    tokenNames = [ u"<INVALID>", u"'hardware'", u"'{'", u"'ip_address'", 
                   u"'arch'", u"'='", u"'}'", u"'\"'", u"';'", u"ID", u"WS", 
                   u"COMMENT", u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_name = 1
    RULE_arch = 2
    RULE_hardware = 3
    RULE_ip_address = 4

    ruleNames =  [ u"start", u"name", u"arch", u"hardware", u"ip_address" ]

    def __init__(self, input):
        super(ROSMOD_HardwareParser, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None



    class StartContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.StartContext, self).__init__(parent, invokingState)
            self.parser = parser

        def EOF(self):
            return self.getToken(ROSMOD_HardwareParser.EOF, 0)

        def hardware(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(ROSMOD_HardwareParser.HardwareContext)
            else:
                return self.getTypedRuleContext(ROSMOD_HardwareParser.HardwareContext,i)


        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_start

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterStart(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitStart(self)




    def start(self):

        localctx = ROSMOD_HardwareParser.StartContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_start)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 13
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==ROSMOD_HardwareParser.T__7:
                self.state = 10 
                self.hardware()
                self.state = 15
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 16
            self.match(self.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class NameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.NameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_name

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterName(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitName(self)




    def name(self):

        localctx = ROSMOD_HardwareParser.NameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_name)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 18
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class ArchContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.ArchContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_arch

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterArch(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitArch(self)




    def arch(self):

        localctx = ROSMOD_HardwareParser.ArchContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_arch)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 20
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class HardwareContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.HardwareContext, self).__init__(parent, invokingState)
            self.parser = parser

        def arch(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.ArchContext,0)


        def ip_address(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.Ip_addressContext,0)


        def name(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.NameContext,0)


        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_hardware

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterHardware(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitHardware(self)




    def hardware(self):

        localctx = ROSMOD_HardwareParser.HardwareContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_hardware)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 22
            self.match(self.T__7)
            self.state = 23 
            self.name()
            self.state = 24
            self.match(self.T__6)

            self.state = 25
            self.match(self.T__5)
            self.state = 26
            self.match(self.T__3)
            self.state = 27
            self.match(self.T__1)
            self.state = 28 
            self.ip_address()
            self.state = 29
            self.match(self.T__1)
            self.state = 30
            self.match(self.T__0)

            self.state = 32
            self.match(self.T__4)
            self.state = 33
            self.match(self.T__3)
            self.state = 34 
            self.arch()
            self.state = 35
            self.match(self.T__0)
            self.state = 37
            self.match(self.T__2)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Ip_addressContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.Ip_addressContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_ip_address

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterIp_address(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitIp_address(self)




    def ip_address(self):

        localctx = ROSMOD_HardwareParser.Ip_addressContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_ip_address)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 39
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




