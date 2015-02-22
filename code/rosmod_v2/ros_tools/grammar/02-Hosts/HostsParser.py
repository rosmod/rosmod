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
        buf.write(u"\22g\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\4\f\t\f\4\r\t\r\4")
        buf.write(u"\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22\3\2")
        buf.write(u"\3\2\6\2\'\n\2\r\2\16\2(\3\2\3\2\3\3\3\3\3\3\3\3\3\4")
        buf.write(u"\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\6")
        buf.write(u"\3\6\3\7\3\7\3\7\3\7\3\b\3\b\3\t\3\t\3\t\5\tI\n\t\3\n")
        buf.write(u"\3\n\3\13\3\13\3\13\5\13P\n\13\3\f\3\f\3\r\3\r\3\r\3")
        buf.write(u"\r\3\16\3\16\3\17\3\17\3\17\3\17\3\20\3\20\3\21\3\21")
        buf.write(u"\3\21\5\21c\n\21\3\22\3\22\3\22\2\2\23\2\4\6\b\n\f\16")
        buf.write(u"\20\22\24\26\30\32\34\36 \"\2\3\3\2\17\20Y\2$\3\2\2\2")
        buf.write(u"\4,\3\2\2\2\6\60\3\2\2\2\b\62\3\2\2\2\n=\3\2\2\2\f?\3")
        buf.write(u"\2\2\2\16C\3\2\2\2\20H\3\2\2\2\22J\3\2\2\2\24O\3\2\2")
        buf.write(u"\2\26Q\3\2\2\2\30S\3\2\2\2\32W\3\2\2\2\34Y\3\2\2\2\36")
        buf.write(u"]\3\2\2\2 b\3\2\2\2\"d\3\2\2\2$&\5\4\3\2%\'\5\b\5\2&")
        buf.write(u"%\3\2\2\2\'(\3\2\2\2(&\3\2\2\2()\3\2\2\2)*\3\2\2\2*+")
        buf.write(u"\7\2\2\3+\3\3\2\2\2,-\7\7\2\2-.\5\6\4\2./\7\f\2\2/\5")
        buf.write(u"\3\2\2\2\60\61\7\20\2\2\61\7\3\2\2\2\62\63\7\t\2\2\63")
        buf.write(u"\64\5\n\6\2\64\65\7\3\2\2\65\66\5\f\7\2\66\67\5\30\r")
        buf.write(u"\2\678\5\34\17\289\5\20\t\29:\5\24\13\2:;\5 \21\2;<\7")
        buf.write(u"\13\2\2<\t\3\2\2\2=>\7\20\2\2>\13\3\2\2\2?@\7\4\2\2@")
        buf.write(u"A\7\n\2\2AB\5\16\b\2B\r\3\2\2\2CD\7\17\2\2D\17\3\2\2")
        buf.write(u"\2EF\7\5\2\2FG\7\n\2\2GI\5\22\n\2HE\3\2\2\2HI\3\2\2\2")
        buf.write(u"I\21\3\2\2\2JK\7\20\2\2K\23\3\2\2\2LM\7\6\2\2MN\7\n\2")
        buf.write(u"\2NP\5\26\f\2OL\3\2\2\2OP\3\2\2\2P\25\3\2\2\2QR\7\21")
        buf.write(u"\2\2R\27\3\2\2\2ST\7\b\2\2TU\7\n\2\2UV\5\32\16\2V\31")
        buf.write(u"\3\2\2\2WX\t\2\2\2X\33\3\2\2\2YZ\7\16\2\2Z[\7\n\2\2[")
        buf.write(u"\\\5\36\20\2\\\35\3\2\2\2]^\t\2\2\2^\37\3\2\2\2_`\7\r")
        buf.write(u"\2\2`a\7\n\2\2ac\5\"\22\2b_\3\2\2\2bc\3\2\2\2c!\3\2\2")
        buf.write(u"\2de\7\21\2\2e#\3\2\2\2\6(HOb")
        return buf.getvalue()
		

class HostsParser ( Parser ):

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
    IP_ADDRESS_STRING=13
    ID=14
    ABSOLUTE_PATH=15
    WS=16

    tokenNames = [ u"<INVALID>", u"'{'", u"'ip_address'", u"'architecture'", 
                   u"'init'", u"'hardware_configuration'", u"'username'", 
                   u"'host'", u"'='", u"'}'", u"';'", u"'sshkey'", u"'password'", 
                   u"IP_ADDRESS_STRING", u"ID", u"ABSOLUTE_PATH", u"WS" ]

    RULE_start = 0
    RULE_define_hardware_configuration = 1
    RULE_configuration_name = 2
    RULE_host = 3
    RULE_host_name = 4
    RULE_ip_address = 5
    RULE_ip_address_string = 6
    RULE_architecture = 7
    RULE_architecture_string = 8
    RULE_init = 9
    RULE_init_path = 10
    RULE_username = 11
    RULE_username_string = 12
    RULE_password = 13
    RULE_password_string = 14
    RULE_local_sshkey = 15
    RULE_sshkey_path = 16

    ruleNames =  [ u"start", u"define_hardware_configuration", u"configuration_name", 
                   u"host", u"host_name", u"ip_address", u"ip_address_string", 
                   u"architecture", u"architecture_string", u"init", u"init_path", 
                   u"username", u"username_string", u"password", u"password_string", 
                   u"local_sshkey", u"sshkey_path" ]

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


        def define_hardware_configuration(self):
            return self.getTypedRuleContext(HostsParser.Define_hardware_configurationContext,0)


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
            self.state = 34 
            self.define_hardware_configuration()
            self.state = 36 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 35 
                self.host()
                self.state = 38 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==HostsParser.T__5):
                    break

            self.state = 40
            self.match(self.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Define_hardware_configurationContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Define_hardware_configurationContext, self).__init__(parent, invokingState)
            self.parser = parser

        def configuration_name(self):
            return self.getTypedRuleContext(HostsParser.Configuration_nameContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_define_hardware_configuration

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterDefine_hardware_configuration(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitDefine_hardware_configuration(self)




    def define_hardware_configuration(self):

        localctx = HostsParser.Define_hardware_configurationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_define_hardware_configuration)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 42
            self.match(self.T__7)
            self.state = 43 
            self.configuration_name()
            self.state = 44
            self.match(self.T__2)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Configuration_nameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Configuration_nameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_configuration_name

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterConfiguration_name(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitConfiguration_name(self)




    def configuration_name(self):

        localctx = HostsParser.Configuration_nameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_configuration_name)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 46
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
            super(HostsParser.HostContext, self).__init__(parent, invokingState)
            self.parser = parser

        def username(self):
            return self.getTypedRuleContext(HostsParser.UsernameContext,0)


        def ip_address(self):
            return self.getTypedRuleContext(HostsParser.Ip_addressContext,0)


        def local_sshkey(self):
            return self.getTypedRuleContext(HostsParser.Local_sshkeyContext,0)


        def password(self):
            return self.getTypedRuleContext(HostsParser.PasswordContext,0)


        def init(self):
            return self.getTypedRuleContext(HostsParser.InitContext,0)


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
        self.enterRule(localctx, 6, self.RULE_host)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 48
            self.match(self.T__5)
            self.state = 49 
            self.host_name()
            self.state = 50
            self.match(self.T__11)

            self.state = 51 
            self.ip_address()

            self.state = 52 
            self.username()

            self.state = 53 
            self.password()

            self.state = 54 
            self.architecture()

            self.state = 55 
            self.init()

            self.state = 56 
            self.local_sshkey()
            self.state = 57
            self.match(self.T__3)
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
        self.enterRule(localctx, 8, self.RULE_host_name)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 59
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
        self.enterRule(localctx, 10, self.RULE_ip_address)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 61
            self.match(self.T__10)
            self.state = 62
            self.match(self.T__4)
            self.state = 63 
            self.ip_address_string()
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
        self.enterRule(localctx, 12, self.RULE_ip_address_string)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 65
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
        self.enterRule(localctx, 14, self.RULE_architecture)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 70
            _la = self._input.LA(1)
            if _la==HostsParser.T__9:
                self.state = 67
                self.match(self.T__9)
                self.state = 68
                self.match(self.T__4)
                self.state = 69 
                self.architecture_string()


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
        self.enterRule(localctx, 16, self.RULE_architecture_string)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 72
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
            super(HostsParser.InitContext, self).__init__(parent, invokingState)
            self.parser = parser

        def init_path(self):
            return self.getTypedRuleContext(HostsParser.Init_pathContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_init

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterInit(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitInit(self)




    def init(self):

        localctx = HostsParser.InitContext(self, self._ctx, self.state)
        self.enterRule(localctx, 18, self.RULE_init)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 77
            _la = self._input.LA(1)
            if _la==HostsParser.T__8:
                self.state = 74
                self.match(self.T__8)
                self.state = 75
                self.match(self.T__4)
                self.state = 76 
                self.init_path()


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Init_pathContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Init_pathContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ABSOLUTE_PATH(self):
            return self.getToken(HostsParser.ABSOLUTE_PATH, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_init_path

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterInit_path(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitInit_path(self)




    def init_path(self):

        localctx = HostsParser.Init_pathContext(self, self._ctx, self.state)
        self.enterRule(localctx, 20, self.RULE_init_path)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 79
            self.match(self.ABSOLUTE_PATH)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class UsernameContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.UsernameContext, self).__init__(parent, invokingState)
            self.parser = parser

        def username_string(self):
            return self.getTypedRuleContext(HostsParser.Username_stringContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_username

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterUsername(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitUsername(self)




    def username(self):

        localctx = HostsParser.UsernameContext(self, self._ctx, self.state)
        self.enterRule(localctx, 22, self.RULE_username)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 81
            self.match(self.T__6)
            self.state = 82
            self.match(self.T__4)
            self.state = 83 
            self.username_string()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Username_stringContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Username_stringContext, self).__init__(parent, invokingState)
            self.parser = parser

        def IP_ADDRESS_STRING(self):
            return self.getToken(HostsParser.IP_ADDRESS_STRING, 0)

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_username_string

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterUsername_string(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitUsername_string(self)




    def username_string(self):

        localctx = HostsParser.Username_stringContext(self, self._ctx, self.state)
        self.enterRule(localctx, 24, self.RULE_username_string)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 85
            _la = self._input.LA(1)
            if not(_la==HostsParser.IP_ADDRESS_STRING or _la==HostsParser.ID):
                self._errHandler.recoverInline(self)
            self.consume()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class PasswordContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.PasswordContext, self).__init__(parent, invokingState)
            self.parser = parser

        def password_string(self):
            return self.getTypedRuleContext(HostsParser.Password_stringContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_password

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterPassword(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitPassword(self)




    def password(self):

        localctx = HostsParser.PasswordContext(self, self._ctx, self.state)
        self.enterRule(localctx, 26, self.RULE_password)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 87
            self.match(self.T__0)
            self.state = 88
            self.match(self.T__4)
            self.state = 89 
            self.password_string()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Password_stringContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Password_stringContext, self).__init__(parent, invokingState)
            self.parser = parser

        def IP_ADDRESS_STRING(self):
            return self.getToken(HostsParser.IP_ADDRESS_STRING, 0)

        def ID(self):
            return self.getToken(HostsParser.ID, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_password_string

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterPassword_string(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitPassword_string(self)




    def password_string(self):

        localctx = HostsParser.Password_stringContext(self, self._ctx, self.state)
        self.enterRule(localctx, 28, self.RULE_password_string)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 91
            _la = self._input.LA(1)
            if not(_la==HostsParser.IP_ADDRESS_STRING or _la==HostsParser.ID):
                self._errHandler.recoverInline(self)
            self.consume()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Local_sshkeyContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Local_sshkeyContext, self).__init__(parent, invokingState)
            self.parser = parser

        def sshkey_path(self):
            return self.getTypedRuleContext(HostsParser.Sshkey_pathContext,0)


        def getRuleIndex(self):
            return HostsParser.RULE_local_sshkey

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterLocal_sshkey(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitLocal_sshkey(self)




    def local_sshkey(self):

        localctx = HostsParser.Local_sshkeyContext(self, self._ctx, self.state)
        self.enterRule(localctx, 30, self.RULE_local_sshkey)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 96
            _la = self._input.LA(1)
            if _la==HostsParser.T__1:
                self.state = 93
                self.match(self.T__1)
                self.state = 94
                self.match(self.T__4)
                self.state = 95 
                self.sshkey_path()


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class Sshkey_pathContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(HostsParser.Sshkey_pathContext, self).__init__(parent, invokingState)
            self.parser = parser

        def ABSOLUTE_PATH(self):
            return self.getToken(HostsParser.ABSOLUTE_PATH, 0)

        def getRuleIndex(self):
            return HostsParser.RULE_sshkey_path

        def enterRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.enterSshkey_path(self)

        def exitRule(self, listener):
            if isinstance( listener, HostsListener ):
                listener.exitSshkey_path(self)




    def sshkey_path(self):

        localctx = HostsParser.Sshkey_pathContext(self, self._ctx, self.state)
        self.enterRule(localctx, 32, self.RULE_sshkey_path)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 98
            self.match(self.ABSOLUTE_PATH)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx




