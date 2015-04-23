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
        buf.write(u"\f!\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\3\2\7\2\f\n\2\f\2")
        buf.write(u"\16\2\17\13\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\3\5\3\5\3\5\2\2\6\2\4\6\b\2\2\35\2\r")
        buf.write(u"\3\2\2\2\4\22\3\2\2\2\6\24\3\2\2\2\b\26\3\2\2\2\n\f\5")
        buf.write(u"\b\5\2\13\n\3\2\2\2\f\17\3\2\2\2\r\13\3\2\2\2\r\16\3")
        buf.write(u"\2\2\2\16\20\3\2\2\2\17\r\3\2\2\2\20\21\7\2\2\3\21\3")
        buf.write(u"\3\2\2\2\22\23\7\t\2\2\23\5\3\2\2\2\24\25\7\t\2\2\25")
        buf.write(u"\7\3\2\2\2\26\27\7\3\2\2\27\30\5\4\3\2\30\31\7\4\2\2")
        buf.write(u"\31\32\7\5\2\2\32\33\7\6\2\2\33\34\5\6\4\2\34\35\7\b")
        buf.write(u"\2\2\35\36\3\2\2\2\36\37\7\7\2\2\37\t\3\2\2\2\3\r")
        return buf.getvalue()
		

class ROSMOD_HardwareParser ( Parser ):

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
    ID=7
    WS=8
    COMMENT=9
    LINE_COMMENT=10

    tokenNames = [ u"<INVALID>", u"'hardware'", u"'{'", u"'arch'", u"'='", 
                   u"'}'", u"';'", u"ID", u"WS", u"COMMENT", u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_name = 1
    RULE_arch_type = 2
    RULE_hardware = 3

    ruleNames =  [ u"start", u"name", u"arch_type", u"hardware" ]

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
            self.state = 11
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==ROSMOD_HardwareParser.T__5:
                self.state = 8 
                self.hardware()
                self.state = 13
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 14
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
            self.state = 16
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Arch_typeContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.Arch_typeContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_arch_type

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterArch_type(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitArch_type(self)




    def arch_type(self):

        localctx = ROSMOD_HardwareParser.Arch_typeContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_arch_type)
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

    class HardwareContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.HardwareContext, self).__init__(parent, invokingState)
            self.parser = parser

        def arch_type(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.Arch_typeContext,0)


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
            self.state = 20
            self.match(self.T__5)
            self.state = 21 
            self.name()
            self.state = 22
            self.match(self.T__4)

            self.state = 23
            self.match(self.T__3)
            self.state = 24
            self.match(self.T__2)
            self.state = 25 
            self.arch_type()
            self.state = 26
            self.match(self.T__0)
            self.state = 28
            self.match(self.T__1)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




