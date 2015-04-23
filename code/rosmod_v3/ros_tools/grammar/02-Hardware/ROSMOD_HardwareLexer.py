# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO


def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\2")
        buf.write(u"\fX\b\1\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t")
        buf.write(u"\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\3\2\3\2\3\2\3\2")
        buf.write(u"\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\4\3\4\3\4\3\4\3\4\3\5")
        buf.write(u"\3\5\3\6\3\6\3\7\3\7\3\b\3\b\7\b\60\n\b\f\b\16\b\63\13")
        buf.write(u"\b\3\t\7\t\66\n\t\f\t\16\t9\13\t\3\t\5\t<\n\t\3\t\3\t")
        buf.write(u"\3\n\3\n\3\n\3\n\7\nD\n\n\f\n\16\nG\13\n\3\n\3\n\3\n")
        buf.write(u"\3\n\3\n\3\13\3\13\3\13\3\13\7\13R\n\13\f\13\16\13U\13")
        buf.write(u"\13\3\13\3\13\3E\2\f\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21")
        buf.write(u"\n\23\13\25\f\3\2\6\5\2C\\aac|\6\2\62;C\\aac|\4\2\13")
        buf.write(u"\f\17\17\4\2\f\f\17\17\\\2\3\3\2\2\2\2\5\3\2\2\2\2\7")
        buf.write(u"\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3")
        buf.write(u"\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\3\27\3")
        buf.write(u"\2\2\2\5 \3\2\2\2\7\"\3\2\2\2\t\'\3\2\2\2\13)\3\2\2\2")
        buf.write(u"\r+\3\2\2\2\17-\3\2\2\2\21;\3\2\2\2\23?\3\2\2\2\25M\3")
        buf.write(u"\2\2\2\27\30\7j\2\2\30\31\7c\2\2\31\32\7t\2\2\32\33\7")
        buf.write(u"f\2\2\33\34\7y\2\2\34\35\7c\2\2\35\36\7t\2\2\36\37\7")
        buf.write(u"g\2\2\37\4\3\2\2\2 !\7}\2\2!\6\3\2\2\2\"#\7c\2\2#$\7")
        buf.write(u"t\2\2$%\7e\2\2%&\7j\2\2&\b\3\2\2\2\'(\7?\2\2(\n\3\2\2")
        buf.write(u"\2)*\7\177\2\2*\f\3\2\2\2+,\7=\2\2,\16\3\2\2\2-\61\t")
        buf.write(u"\2\2\2.\60\t\3\2\2/.\3\2\2\2\60\63\3\2\2\2\61/\3\2\2")
        buf.write(u"\2\61\62\3\2\2\2\62\20\3\2\2\2\63\61\3\2\2\2\64\66\7")
        buf.write(u"\"\2\2\65\64\3\2\2\2\669\3\2\2\2\67\65\3\2\2\2\678\3")
        buf.write(u"\2\2\28<\3\2\2\29\67\3\2\2\2:<\t\4\2\2;\67\3\2\2\2;:")
        buf.write(u"\3\2\2\2<=\3\2\2\2=>\b\t\2\2>\22\3\2\2\2?@\7\61\2\2@")
        buf.write(u"A\7,\2\2AE\3\2\2\2BD\13\2\2\2CB\3\2\2\2DG\3\2\2\2EF\3")
        buf.write(u"\2\2\2EC\3\2\2\2FH\3\2\2\2GE\3\2\2\2HI\7,\2\2IJ\7\61")
        buf.write(u"\2\2JK\3\2\2\2KL\b\n\3\2L\24\3\2\2\2MN\7\61\2\2NO\7\61")
        buf.write(u"\2\2OS\3\2\2\2PR\n\5\2\2QP\3\2\2\2RU\3\2\2\2SQ\3\2\2")
        buf.write(u"\2ST\3\2\2\2TV\3\2\2\2US\3\2\2\2VW\b\13\3\2W\26\3\2\2")
        buf.write(u"\2\b\2\61\67;ES\4\2\3\2\b\2\2")
        return buf.getvalue()
		

class ROSMOD_HardwareLexer(Lexer):

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    T__5 = 1
    T__4 = 2
    T__3 = 3
    T__2 = 4
    T__1 = 5
    T__0 = 6
    ID = 7
    WS = 8
    COMMENT = 9
    LINE_COMMENT = 10


    modeNames = [ u"DEFAULT_MODE" ]

    tokenNames = [ u"<INVALID>",
            u"'\\u0000'", u"'\\u0001'", u"'\\u0002'", u"'\\u0003'", u"'\\u0004'", 
            u"'\\u0005'", u"'\\u0006'", u"'\\u0007'", u"'\b'", u"'\t'", 
            u"'\n'" ]

    ruleNames = [ u"T__5", u"T__4", u"T__3", u"T__2", u"T__1", u"T__0", 
                  u"ID", u"WS", u"COMMENT", u"LINE_COMMENT" ]

    grammarFileName = u"ROSMOD_Hardware.g4"

    def __init__(self, input=None):
        super(ROSMOD_HardwareLexer, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = LexerATNSimulator(self, self.atn, self.decisionsToDFA, PredictionContextCache())
        self._actions = None
        self._predicates = None


