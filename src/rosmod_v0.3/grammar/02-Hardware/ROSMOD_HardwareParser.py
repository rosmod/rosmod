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
        buf.write(u"\22Z\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\4\b\t\b\4\t\t\t\4\n\t\n\3\2\7\2\26\n\2\f\2\16\2\31\13")
        buf.write(u"\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3")
        buf.write(u"\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3")
        buf.write(u"\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3")
        buf.write(u"\5\3\5\3\5\3\5\3\5\3\5\5\5G\n\5\3\5\3\5\3\5\3\5\3\5\3")
        buf.write(u"\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\n\3\n\3\n\2")
        buf.write(u"\2\13\2\4\6\b\n\f\16\20\22\2\2R\2\27\3\2\2\2\4\34\3\2")
        buf.write(u"\2\2\6\36\3\2\2\2\b \3\2\2\2\nO\3\2\2\2\fQ\3\2\2\2\16")
        buf.write(u"S\3\2\2\2\20U\3\2\2\2\22W\3\2\2\2\24\26\5\b\5\2\25\24")
        buf.write(u"\3\2\2\2\26\31\3\2\2\2\27\25\3\2\2\2\27\30\3\2\2\2\30")
        buf.write(u"\32\3\2\2\2\31\27\3\2\2\2\32\33\7\2\2\3\33\3\3\2\2\2")
        buf.write(u"\34\35\7\17\2\2\35\5\3\2\2\2\36\37\7\17\2\2\37\7\3\2")
        buf.write(u"\2\2 !\7\3\2\2!\"\5\4\3\2\"#\7\4\2\2#$\7\5\2\2$%\7\n")
        buf.write(u"\2\2%&\7\f\2\2&\'\5\n\6\2\'(\7\f\2\2()\7\r\2\2)*\3\2")
        buf.write(u"\2\2*+\7\t\2\2+,\7\n\2\2,-\7\f\2\2-.\5\f\7\2./\7\f\2")
        buf.write(u"\2/\60\7\r\2\2\60\61\3\2\2\2\61\62\7\16\2\2\62\63\7\n")
        buf.write(u"\2\2\63\64\7\f\2\2\64\65\5\16\b\2\65\66\7\f\2\2\66\67")
        buf.write(u"\7\r\2\2\678\3\2\2\289\7\b\2\29:\7\n\2\2:;\7\f\2\2;<")
        buf.write(u"\5\20\t\2<=\7\f\2\2=>\7\r\2\2>F\3\2\2\2?@\7\6\2\2@A\7")
        buf.write(u"\n\2\2AB\7\f\2\2BC\5\22\n\2CD\7\f\2\2DE\7\r\2\2EG\3\2")
        buf.write(u"\2\2F?\3\2\2\2FG\3\2\2\2GH\3\2\2\2HI\7\7\2\2IJ\7\n\2")
        buf.write(u"\2JK\5\6\4\2KL\7\r\2\2LM\3\2\2\2MN\7\13\2\2N\t\3\2\2")
        buf.write(u"\2OP\7\17\2\2P\13\3\2\2\2QR\7\17\2\2R\r\3\2\2\2ST\7\17")
        buf.write(u"\2\2T\17\3\2\2\2UV\7\17\2\2V\21\3\2\2\2WX\7\17\2\2X\23")
        buf.write(u"\3\2\2\2\4\27F")
        return buf.getvalue()
		

class ROSMOD_HardwareParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__11=1
    T__10=2
    T__9=3
    T__8=4
    T__7=5
    T__6=6
    T__5=7
    T__4=8
    T__3=9
    T__2=10
    T__1=11
    T__0=12
    ID=13
    WS=14
    COMMENT=15
    LINE_COMMENT=16

    tokenNames = [ u"<INVALID>", u"'hardware'", u"'{'", u"'ip_address'", 
                   u"'init'", u"'arch'", u"'deployment_path'", u"'username'", 
                   u"'='", u"'}'", u"'\"'", u"';'", u"'sshkey'", u"ID", 
                   u"WS", u"COMMENT", u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_name = 1
    RULE_arch = 2
    RULE_hardware = 3
    RULE_ip_address = 4
    RULE_username = 5
    RULE_sshkey = 6
    RULE_deployment_path = 7
    RULE_init = 8

    ruleNames =  [ u"start", u"name", u"arch", u"hardware", u"ip_address", 
                   u"username", u"sshkey", u"deployment_path", u"init" ]

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
            self.state = 21
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==ROSMOD_HardwareParser.T__11:
                self.state = 18 
                self.hardware()
                self.state = 23
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 24
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
            self.state = 26
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
            self.state = 28
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

        def username(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.UsernameContext,0)


        def deployment_path(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.Deployment_pathContext,0)


        def sshkey(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.SshkeyContext,0)


        def arch(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.ArchContext,0)


        def ip_address(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.Ip_addressContext,0)


        def init(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.InitContext,0)


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
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 30
            self.match(self.T__11)
            self.state = 31 
            self.name()
            self.state = 32
            self.match(self.T__10)

            self.state = 33
            self.match(self.T__9)
            self.state = 34
            self.match(self.T__4)
            self.state = 35
            self.match(self.T__2)
            self.state = 36 
            self.ip_address()
            self.state = 37
            self.match(self.T__2)
            self.state = 38
            self.match(self.T__1)

            self.state = 40
            self.match(self.T__5)
            self.state = 41
            self.match(self.T__4)
            self.state = 42
            self.match(self.T__2)
            self.state = 43 
            self.username()
            self.state = 44
            self.match(self.T__2)
            self.state = 45
            self.match(self.T__1)

            self.state = 47
            self.match(self.T__0)
            self.state = 48
            self.match(self.T__4)
            self.state = 49
            self.match(self.T__2)
            self.state = 50 
            self.sshkey()
            self.state = 51
            self.match(self.T__2)
            self.state = 52
            self.match(self.T__1)

            self.state = 54
            self.match(self.T__6)
            self.state = 55
            self.match(self.T__4)
            self.state = 56
            self.match(self.T__2)
            self.state = 57 
            self.deployment_path()
            self.state = 58
            self.match(self.T__2)
            self.state = 59
            self.match(self.T__1)
            self.state = 68
            _la = self._input.LA(1)
            if _la==ROSMOD_HardwareParser.T__8:
                self.state = 61
                self.match(self.T__8)
                self.state = 62
                self.match(self.T__4)
                self.state = 63
                self.match(self.T__2)
                self.state = 64 
                self.init()
                self.state = 65
                self.match(self.T__2)
                self.state = 66
                self.match(self.T__1)


            self.state = 70
            self.match(self.T__7)
            self.state = 71
            self.match(self.T__4)
            self.state = 72 
            self.arch()
            self.state = 73
            self.match(self.T__1)
            self.state = 75
            self.match(self.T__3)
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
            self.state = 77
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class UsernameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.UsernameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_username

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterUsername(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitUsername(self)




    def username(self):

        localctx = ROSMOD_HardwareParser.UsernameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_username)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 79
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class SshkeyContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.SshkeyContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_sshkey

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterSshkey(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitSshkey(self)




    def sshkey(self):

        localctx = ROSMOD_HardwareParser.SshkeyContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_sshkey)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 81
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Deployment_pathContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.Deployment_pathContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_deployment_path

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterDeployment_path(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitDeployment_path(self)




    def deployment_path(self):

        localctx = ROSMOD_HardwareParser.Deployment_pathContext(self, self._ctx, self.state)
        self.enterRule(localctx, 14, self.RULE_deployment_path)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 83
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class InitContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.InitContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_init

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterInit(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitInit(self)




    def init(self):

        localctx = ROSMOD_HardwareParser.InitContext(self, self._ctx, self.state)
        self.enterRule(localctx, 16, self.RULE_init)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 85
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




