# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO


def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\2")
        buf.write(u"\rW\b\1\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t")
        buf.write(u"\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\4\f\t\f\3\2\3\2")
        buf.write(u"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\4\3\4")
        buf.write(u"\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5")
        buf.write(u"\3\5\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\n\6\n")
        buf.write(u"B\n\n\r\n\16\nC\3\13\3\13\7\13H\n\13\f\13\16\13K\13\13")
        buf.write(u"\3\f\7\fN\n\f\f\f\16\fQ\13\f\3\f\5\fT\n\f\3\f\3\f\2\2")
        buf.write(u"\r\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r")
        buf.write(u"\3\2\6\6\2\60\60\62<CHch\5\2C\\aac|\6\2\62;C\\aac|\4")
        buf.write(u"\2\13\f\17\17Z\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2")
        buf.write(u"\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21")
        buf.write(u"\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3\2\2\2\3\31")
        buf.write(u"\3\2\2\2\5\33\3\2\2\2\7&\3\2\2\2\t\63\3\2\2\2\138\3\2")
        buf.write(u"\2\2\r:\3\2\2\2\17<\3\2\2\2\21>\3\2\2\2\23A\3\2\2\2\25")
        buf.write(u"E\3\2\2\2\27S\3\2\2\2\31\32\7}\2\2\32\4\3\2\2\2\33\34")
        buf.write(u"\7k\2\2\34\35\7r\2\2\35\36\7a\2\2\36\37\7c\2\2\37 \7")
        buf.write(u"f\2\2 !\7f\2\2!\"\7t\2\2\"#\7g\2\2#$\7u\2\2$%\7u\2\2")
        buf.write(u"%\6\3\2\2\2&\'\7c\2\2\'(\7t\2\2()\7e\2\2)*\7j\2\2*+\7")
        buf.write(u"k\2\2+,\7v\2\2,-\7g\2\2-.\7e\2\2./\7v\2\2/\60\7w\2\2")
        buf.write(u"\60\61\7t\2\2\61\62\7g\2\2\62\b\3\2\2\2\63\64\7j\2\2")
        buf.write(u"\64\65\7q\2\2\65\66\7u\2\2\66\67\7v\2\2\67\n\3\2\2\2")
        buf.write(u"89\7?\2\29\f\3\2\2\2:;\7\177\2\2;\16\3\2\2\2<=\7$\2\2")
        buf.write(u"=\20\3\2\2\2>?\7=\2\2?\22\3\2\2\2@B\t\2\2\2A@\3\2\2\2")
        buf.write(u"BC\3\2\2\2CA\3\2\2\2CD\3\2\2\2D\24\3\2\2\2EI\t\3\2\2")
        buf.write(u"FH\t\4\2\2GF\3\2\2\2HK\3\2\2\2IG\3\2\2\2IJ\3\2\2\2J\26")
        buf.write(u"\3\2\2\2KI\3\2\2\2LN\7\"\2\2ML\3\2\2\2NQ\3\2\2\2OM\3")
        buf.write(u"\2\2\2OP\3\2\2\2PT\3\2\2\2QO\3\2\2\2RT\t\5\2\2SO\3\2")
        buf.write(u"\2\2SR\3\2\2\2TU\3\2\2\2UV\b\f\2\2V\30\3\2\2\2\7\2CI")
        buf.write(u"OS\3\2\3\2")
        return buf.getvalue()
		

class HostsLexer(Lexer):

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    T__7 = 1
    T__6 = 2
    T__5 = 3
    T__4 = 4
    T__3 = 5
    T__2 = 6
    T__1 = 7
    T__0 = 8
    IP_ADDRESS_STRING = 9
    ID = 10
    WS = 11


    modeNames = [ u"DEFAULT_MODE" ]

    tokenNames = [ u"<INVALID>",
            u"'\\u0000'", u"'\\u0001'", u"'\\u0002'", u"'\\u0003'", u"'\\u0004'", 
            u"'\\u0005'", u"'\\u0006'", u"'\\u0007'", u"'\b'", u"'\t'", 
            u"'\n'", u"'\\u000B'" ]

    ruleNames = [ u"T__7", u"T__6", u"T__5", u"T__4", u"T__3", u"T__2", 
                  u"T__1", u"T__0", u"IP_ADDRESS_STRING", u"ID", u"WS" ]

    grammarFileName = u"Hosts.g4"

    def __init__(self, input=None):
        super(HostsLexer, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = LexerATNSimulator(self, self.atn, self.decisionsToDFA, PredictionContextCache())
        self._actions = None
        self._predicates = None


