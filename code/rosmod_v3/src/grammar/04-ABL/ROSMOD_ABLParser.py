# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO
package = globals().get("__package__", None)
ischild = len(package)>0 if package is not None else False
if ischild:
    from .ROSMOD_ABLListener import ROSMOD_ABLListener
else:
    from ROSMOD_ABLListener import ROSMOD_ABLListener
def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3")
        buf.write(u"\23J\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\4\b\t\b\4\t\t\t\3\2\6\2\24\n\2\r\2\16\2\25\3\3\3\3\3")
        buf.write(u"\3\7\3\33\n\3\f\3\16\3\36\13\3\3\4\3\4\3\4\3\4\5\4$\n")
        buf.write(u"\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3")
        buf.write(u"\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\b\3\b\3\b\3")
        buf.write(u"\b\3\b\3\b\6\bB\n\b\r\b\16\bC\3\b\3\b\3\t\3\t\3\t\2\2")
        buf.write(u"\n\2\4\6\b\n\f\16\20\2\2G\2\23\3\2\2\2\4\27\3\2\2\2\6")
        buf.write(u"#\3\2\2\2\b%\3\2\2\2\n-\3\2\2\2\f\64\3\2\2\2\16;\3\2")
        buf.write(u"\2\2\20G\3\2\2\2\22\24\5\6\4\2\23\22\3\2\2\2\24\25\3")
        buf.write(u"\2\2\2\25\23\3\2\2\2\25\26\3\2\2\2\26\3\3\2\2\2\27\34")
        buf.write(u"\7\16\2\2\30\31\7\13\2\2\31\33\7\16\2\2\32\30\3\2\2\2")
        buf.write(u"\33\36\3\2\2\2\34\32\3\2\2\2\34\35\3\2\2\2\35\5\3\2\2")
        buf.write(u"\2\36\34\3\2\2\2\37$\5\b\5\2 $\5\n\6\2!$\5\f\7\2\"$\5")
        buf.write(u"\16\b\2#\37\3\2\2\2# \3\2\2\2#!\3\2\2\2#\"\3\2\2\2$\7")
        buf.write(u"\3\2\2\2%&\5\4\3\2&\'\7\6\2\2\'(\7\t\2\2()\7\n\2\2)*")
        buf.write(u"\7\20\2\2*+\7\7\2\2+,\7\r\2\2,\t\3\2\2\2-.\5\4\3\2./")
        buf.write(u"\7\6\2\2/\60\7\b\2\2\60\61\7\n\2\2\61\62\7\7\2\2\62\63")
        buf.write(u"\7\r\2\2\63\13\3\2\2\2\64\65\5\4\3\2\65\66\7\6\2\2\66")
        buf.write(u"\67\7\3\2\2\678\7\n\2\289\7\7\2\29:\7\r\2\2:\r\3\2\2")
        buf.write(u"\2;<\7\4\2\2<=\7\n\2\2=>\5\20\t\2>?\7\7\2\2?A\7\5\2\2")
        buf.write(u"@B\5\6\4\2A@\3\2\2\2BC\3\2\2\2CA\3\2\2\2CD\3\2\2\2DE")
        buf.write(u"\3\2\2\2EF\7\f\2\2F\17\3\2\2\2GH\7\17\2\2H\21\3\2\2\2")
        buf.write(u"\6\25\34#C")
        return buf.getvalue()
		

class ROSMOD_ABLParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__10=1
    T__9=2
    T__8=3
    T__7=4
    T__6=5
    T__5=6
    T__4=7
    T__3=8
    T__2=9
    T__1=10
    T__0=11
    ID=12
    INT=13
    DOUBLE=14
    WS=15
    COMMENT=16
    LINE_COMMENT=17

    tokenNames = [ u"<INVALID>", u"'publish'", u"'loop'", u"'{'", u"'.'", 
                   u"')'", u"'call'", u"'local'", u"'('", u"'/'", u"'}'", 
                   u"';'", u"ID", u"INT", u"DOUBLE", u"WS", u"COMMENT", 
                   u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_name = 1
    RULE_step = 2
    RULE_local_code_block = 3
    RULE_client_using_srv = 4
    RULE_publish_using_msg = 5
    RULE_loop = 6
    RULE_count = 7

    ruleNames =  [ u"start", u"name", u"step", u"local_code_block", u"client_using_srv", 
                   u"publish_using_msg", u"loop", u"count" ]

    def __init__(self, input):
        super(ROSMOD_ABLParser, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None



    class StartContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.StartContext, self).__init__(parent, invokingState)
            self.parser = parser

        def step(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(ROSMOD_ABLParser.StepContext)
            else:
                return self.getTypedRuleContext(ROSMOD_ABLParser.StepContext,i)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_start

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterStart(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitStart(self)




    def start(self):

        localctx = ROSMOD_ABLParser.StartContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_start)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 17 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 16 
                self.step()
                self.state = 19 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==ROSMOD_ABLParser.T__9 or _la==ROSMOD_ABLParser.ID):
                    break

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class NameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.NameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self, i=None):
            if i is None:
                return self.getTokens(ROSMOD_ABLParser.ID)
            else:
                return self.getToken(ROSMOD_ABLParser.ID, i)

        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_name

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterName(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitName(self)




    def name(self):

        localctx = ROSMOD_ABLParser.NameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_name)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 21
            self.match(self.ID)
            self.state = 26
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==ROSMOD_ABLParser.T__2:
                self.state = 22
                self.match(self.T__2)
                self.state = 23
                self.match(self.ID)
                self.state = 28
                self._errHandler.sync(self)
                _la = self._input.LA(1)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class StepContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.StepContext, self).__init__(parent, invokingState)
            self.parser = parser

        def loop(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.LoopContext,0)


        def local_code_block(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.Local_code_blockContext,0)


        def publish_using_msg(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.Publish_using_msgContext,0)


        def client_using_srv(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.Client_using_srvContext,0)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_step

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterStep(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitStep(self)




    def step(self):

        localctx = ROSMOD_ABLParser.StepContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_step)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 33
            la_ = self._interp.adaptivePredict(self._input,2,self._ctx)
            if la_ == 1:
                self.state = 29 
                self.local_code_block()
                pass

            elif la_ == 2:
                self.state = 30 
                self.client_using_srv()
                pass

            elif la_ == 3:
                self.state = 31 
                self.publish_using_msg()
                pass

            elif la_ == 4:
                self.state = 32 
                self.loop()
                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Local_code_blockContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.Local_code_blockContext, self).__init__(parent, invokingState)
            self.parser = parser

        def DOUBLE(self):
            return self.getToken(ROSMOD_ABLParser.DOUBLE, 0)

        def name(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.NameContext,0)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_local_code_block

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterLocal_code_block(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitLocal_code_block(self)




    def local_code_block(self):

        localctx = ROSMOD_ABLParser.Local_code_blockContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_local_code_block)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 35 
            self.name()
            self.state = 36
            self.match(self.T__7)
            self.state = 37
            self.match(self.T__4)
            self.state = 38
            self.match(self.T__3)
            self.state = 39
            self.match(self.DOUBLE)
            self.state = 40
            self.match(self.T__6)
            self.state = 41
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Client_using_srvContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.Client_using_srvContext, self).__init__(parent, invokingState)
            self.parser = parser

        def name(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.NameContext,0)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_client_using_srv

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterClient_using_srv(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitClient_using_srv(self)




    def client_using_srv(self):

        localctx = ROSMOD_ABLParser.Client_using_srvContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_client_using_srv)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 43 
            self.name()
            self.state = 44
            self.match(self.T__7)
            self.state = 45
            self.match(self.T__5)
            self.state = 46
            self.match(self.T__3)
            self.state = 47
            self.match(self.T__6)
            self.state = 48
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Publish_using_msgContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.Publish_using_msgContext, self).__init__(parent, invokingState)
            self.parser = parser

        def name(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.NameContext,0)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_publish_using_msg

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterPublish_using_msg(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitPublish_using_msg(self)




    def publish_using_msg(self):

        localctx = ROSMOD_ABLParser.Publish_using_msgContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_publish_using_msg)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 50 
            self.name()
            self.state = 51
            self.match(self.T__7)
            self.state = 52
            self.match(self.T__10)
            self.state = 53
            self.match(self.T__3)
            self.state = 54
            self.match(self.T__6)
            self.state = 55
            self.match(self.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class LoopContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.LoopContext, self).__init__(parent, invokingState)
            self.parser = parser

        def count(self):
            return self.getTypedRuleContext(ROSMOD_ABLParser.CountContext,0)


        def step(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(ROSMOD_ABLParser.StepContext)
            else:
                return self.getTypedRuleContext(ROSMOD_ABLParser.StepContext,i)


        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_loop

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterLoop(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitLoop(self)




    def loop(self):

        localctx = ROSMOD_ABLParser.LoopContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_loop)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 57
            self.match(self.T__9)
            self.state = 58
            self.match(self.T__3)
            self.state = 59 
            self.count()
            self.state = 60
            self.match(self.T__6)
            self.state = 61
            self.match(self.T__8)
            self.state = 63 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 62 
                self.step()
                self.state = 65 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==ROSMOD_ABLParser.T__9 or _la==ROSMOD_ABLParser.ID):
                    break

            self.state = 67
            self.match(self.T__1)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class CountContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_ABLParser.CountContext, self).__init__(parent, invokingState)
            self.parser = parser

        def INT(self):
            return self.getToken(ROSMOD_ABLParser.INT, 0)

        def getRuleIndex(self):
            return ROSMOD_ABLParser.RULE_count

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.enterCount(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_ABLListener ):
                listener.exitCount(self)




    def count(self):

        localctx = ROSMOD_ABLParser.CountContext(self, self._ctx, self.state)
        self.enterRule(localctx, 14, self.RULE_count)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 69
            self.match(self.INT)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




