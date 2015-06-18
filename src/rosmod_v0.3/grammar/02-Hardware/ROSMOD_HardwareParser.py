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
        buf.write(u"\23g\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\3\2\7\2\30\n\2\f\2")
        buf.write(u"\16\2\33\13\2\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\5\5I\n\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\5\5\5R\n\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5")
        buf.write(u"\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\n\3\n\3\13\3\13\3")
        buf.write(u"\13\2\2\f\2\4\6\b\n\f\16\20\22\24\2\2_\2\31\3\2\2\2\4")
        buf.write(u"\36\3\2\2\2\6 \3\2\2\2\b\"\3\2\2\2\nZ\3\2\2\2\f\\\3\2")
        buf.write(u"\2\2\16^\3\2\2\2\20`\3\2\2\2\22b\3\2\2\2\24d\3\2\2\2")
        buf.write(u"\26\30\5\b\5\2\27\26\3\2\2\2\30\33\3\2\2\2\31\27\3\2")
        buf.write(u"\2\2\31\32\3\2\2\2\32\34\3\2\2\2\33\31\3\2\2\2\34\35")
        buf.write(u"\7\2\2\3\35\3\3\2\2\2\36\37\7\20\2\2\37\5\3\2\2\2 !\7")
        buf.write(u"\20\2\2!\7\3\2\2\2\"#\7\4\2\2#$\5\4\3\2$%\7\5\2\2%&\7")
        buf.write(u"\6\2\2&\'\7\13\2\2\'(\7\r\2\2()\5\n\6\2)*\7\r\2\2*+\7")
        buf.write(u"\16\2\2+,\3\2\2\2,-\7\n\2\2-.\7\13\2\2./\7\r\2\2/\60")
        buf.write(u"\5\f\7\2\60\61\7\r\2\2\61\62\7\16\2\2\62\63\3\2\2\2\63")
        buf.write(u"\64\7\17\2\2\64\65\7\13\2\2\65\66\7\r\2\2\66\67\5\16")
        buf.write(u"\b\2\678\7\r\2\289\7\16\2\29:\3\2\2\2:;\7\t\2\2;<\7\13")
        buf.write(u"\2\2<=\7\r\2\2=>\5\20\t\2>?\7\r\2\2?@\7\16\2\2@H\3\2")
        buf.write(u"\2\2AB\7\3\2\2BC\7\13\2\2CD\7\r\2\2DE\5\22\n\2EF\7\r")
        buf.write(u"\2\2FG\7\16\2\2GI\3\2\2\2HA\3\2\2\2HI\3\2\2\2IQ\3\2\2")
        buf.write(u"\2JK\7\7\2\2KL\7\13\2\2LM\7\r\2\2MN\5\24\13\2NO\7\r\2")
        buf.write(u"\2OP\7\16\2\2PR\3\2\2\2QJ\3\2\2\2QR\3\2\2\2RS\3\2\2\2")
        buf.write(u"ST\7\b\2\2TU\7\13\2\2UV\5\6\4\2VW\7\16\2\2WX\3\2\2\2")
        buf.write(u"XY\7\f\2\2Y\t\3\2\2\2Z[\7\20\2\2[\13\3\2\2\2\\]\7\20")
        buf.write(u"\2\2]\r\3\2\2\2^_\7\20\2\2_\17\3\2\2\2`a\7\20\2\2a\21")
        buf.write(u"\3\2\2\2bc\7\20\2\2c\23\3\2\2\2de\7\20\2\2e\25\3\2\2")
        buf.write(u"\2\5\31HQ")
        return buf.getvalue()
		

class ROSMOD_HardwareParser ( Parser ):

    grammarFileName = "java-escape"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    EOF = Token.EOF
    T__12=1
    T__11=2
    T__10=3
    T__9=4
    T__8=5
    T__7=6
    T__6=7
    T__5=8
    T__4=9
    T__3=10
    T__2=11
    T__1=12
    T__0=13
    ID=14
    WS=15
    COMMENT=16
    LINE_COMMENT=17

    tokenNames = [ u"<INVALID>", u"'install_path'", u"'hardware'", u"'{'", 
                   u"'ip_address'", u"'init'", u"'arch'", u"'deployment_path'", 
                   u"'username'", u"'='", u"'}'", u"'\"'", u"';'", u"'sshkey'", 
                   u"ID", u"WS", u"COMMENT", u"LINE_COMMENT" ]

    RULE_start = 0
    RULE_name = 1
    RULE_arch = 2
    RULE_hardware = 3
    RULE_ip_address = 4
    RULE_username = 5
    RULE_sshkey = 6
    RULE_deployment_path = 7
    RULE_install_path = 8
    RULE_init = 9

    ruleNames =  [ u"start", u"name", u"arch", u"hardware", u"ip_address", 
                   u"username", u"sshkey", u"deployment_path", u"install_path", 
                   u"init" ]

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
            self.state = 23
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==ROSMOD_HardwareParser.T__11:
                self.state = 20 
                self.hardware()
                self.state = 25
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 26
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
            self.state = 28
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
            self.state = 30
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


        def install_path(self):
            return self.getTypedRuleContext(ROSMOD_HardwareParser.Install_pathContext,0)


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
            self.state = 32
            self.match(self.T__11)
            self.state = 33 
            self.name()
            self.state = 34
            self.match(self.T__10)

            self.state = 35
            self.match(self.T__9)
            self.state = 36
            self.match(self.T__4)
            self.state = 37
            self.match(self.T__2)
            self.state = 38 
            self.ip_address()
            self.state = 39
            self.match(self.T__2)
            self.state = 40
            self.match(self.T__1)

            self.state = 42
            self.match(self.T__5)
            self.state = 43
            self.match(self.T__4)
            self.state = 44
            self.match(self.T__2)
            self.state = 45 
            self.username()
            self.state = 46
            self.match(self.T__2)
            self.state = 47
            self.match(self.T__1)

            self.state = 49
            self.match(self.T__0)
            self.state = 50
            self.match(self.T__4)
            self.state = 51
            self.match(self.T__2)
            self.state = 52 
            self.sshkey()
            self.state = 53
            self.match(self.T__2)
            self.state = 54
            self.match(self.T__1)

            self.state = 56
            self.match(self.T__6)
            self.state = 57
            self.match(self.T__4)
            self.state = 58
            self.match(self.T__2)
            self.state = 59 
            self.deployment_path()
            self.state = 60
            self.match(self.T__2)
            self.state = 61
            self.match(self.T__1)
            self.state = 70
            _la = self._input.LA(1)
            if _la==ROSMOD_HardwareParser.T__12:
                self.state = 63
                self.match(self.T__12)
                self.state = 64
                self.match(self.T__4)
                self.state = 65
                self.match(self.T__2)
                self.state = 66 
                self.install_path()
                self.state = 67
                self.match(self.T__2)
                self.state = 68
                self.match(self.T__1)


            self.state = 79
            _la = self._input.LA(1)
            if _la==ROSMOD_HardwareParser.T__8:
                self.state = 72
                self.match(self.T__8)
                self.state = 73
                self.match(self.T__4)
                self.state = 74
                self.match(self.T__2)
                self.state = 75 
                self.init()
                self.state = 76
                self.match(self.T__2)
                self.state = 77
                self.match(self.T__1)


            self.state = 81
            self.match(self.T__7)
            self.state = 82
            self.match(self.T__4)
            self.state = 83 
            self.arch()
            self.state = 84
            self.match(self.T__1)
            self.state = 86
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
            self.state = 88
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
            self.state = 90
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
            self.state = 92
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
            self.state = 94
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Install_pathContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(ROSMOD_HardwareParser.Install_pathContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(ROSMOD_HardwareParser.ID, 0)

        def getRuleIndex(self):
            return ROSMOD_HardwareParser.RULE_install_path

        def enterRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.enterInstall_path(self)

        def exitRule(self, listener):
            if isinstance( listener, ROSMOD_HardwareListener ):
                listener.exitInstall_path(self)




    def install_path(self):

        localctx = ROSMOD_HardwareParser.Install_pathContext(self, self._ctx, self.state)
        self.enterRule(localctx, 16, self.RULE_install_path)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 96
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
        self.enterRule(localctx, 18, self.RULE_init)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 98
            self.match(self.ID)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




