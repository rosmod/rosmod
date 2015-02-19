# Generated from java-escape by ANTLR 4.4
from __future__ import print_function
from antlr4 import *
from io import StringIO


def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\2")
        buf.write(u"\fS\b\1\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t")
        buf.write(u"\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\3\2\3\2\3\3\3\3")
        buf.write(u"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\4\3\4")
        buf.write(u"\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5")
        buf.write(u"\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\6\t>\n\t\r\t\16\t?\3")
        buf.write(u"\n\3\n\7\nD\n\n\f\n\16\nG\13\n\3\13\7\13J\n\13\f\13\16")
        buf.write(u"\13M\13\13\3\13\5\13P\n\13\3\13\3\13\2\2\f\3\3\5\4\7")
        buf.write(u"\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\3\2\6\5\2\60\60")
        buf.write(u"\62<Ch\5\2C\\aac|\6\2\62;C\\aac|\4\2\13\f\17\17V\2\3")
        buf.write(u"\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2")
        buf.write(u"\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2")
        buf.write(u"\2\2\25\3\2\2\2\3\27\3\2\2\2\5\31\3\2\2\2\7$\3\2\2\2")
        buf.write(u"\t\61\3\2\2\2\13\66\3\2\2\2\r8\3\2\2\2\17:\3\2\2\2\21")
        buf.write(u"=\3\2\2\2\23A\3\2\2\2\25O\3\2\2\2\27\30\7}\2\2\30\4\3")
        buf.write(u"\2\2\2\31\32\7k\2\2\32\33\7r\2\2\33\34\7a\2\2\34\35\7")
        buf.write(u"c\2\2\35\36\7f\2\2\36\37\7f\2\2\37 \7t\2\2 !\7g\2\2!")
        buf.write(u"\"\7u\2\2\"#\7u\2\2#\6\3\2\2\2$%\7c\2\2%&\7t\2\2&\'\7")
        buf.write(u"e\2\2\'(\7j\2\2()\7k\2\2)*\7v\2\2*+\7g\2\2+,\7e\2\2,")
        buf.write(u"-\7v\2\2-.\7w\2\2./\7t\2\2/\60\7g\2\2\60\b\3\2\2\2\61")
        buf.write(u"\62\7j\2\2\62\63\7q\2\2\63\64\7u\2\2\64\65\7v\2\2\65")
        buf.write(u"\n\3\2\2\2\66\67\7?\2\2\67\f\3\2\2\289\7\177\2\29\16")
        buf.write(u"\3\2\2\2:;\7=\2\2;\20\3\2\2\2<>\t\2\2\2=<\3\2\2\2>?\3")
        buf.write(u"\2\2\2?=\3\2\2\2?@\3\2\2\2@\22\3\2\2\2AE\t\3\2\2BD\t")
        buf.write(u"\4\2\2CB\3\2\2\2DG\3\2\2\2EC\3\2\2\2EF\3\2\2\2F\24\3")
        buf.write(u"\2\2\2GE\3\2\2\2HJ\7\"\2\2IH\3\2\2\2JM\3\2\2\2KI\3\2")
        buf.write(u"\2\2KL\3\2\2\2LP\3\2\2\2MK\3\2\2\2NP\t\5\2\2OK\3\2\2")
        buf.write(u"\2ON\3\2\2\2PQ\3\2\2\2QR\b\13\2\2R\26\3\2\2\2\7\2?EK")
        buf.write(u"O\3\2\3\2")
        return buf.getvalue()
		

class HostsLexer(Lexer):

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    T__6 = 1
    T__5 = 2
    T__4 = 3
    T__3 = 4
    T__2 = 5
    T__1 = 6
    T__0 = 7
    ADDRESS = 8
    ID = 9
    WS = 10


    modeNames = [ u"DEFAULT_MODE" ]

    tokenNames = [ u"<INVALID>",
            u"'\\u0000'", u"'\\u0001'", u"'\\u0002'", u"'\\u0003'", u"'\\u0004'", 
            u"'\\u0005'", u"'\\u0006'", u"'\\u0007'", u"'\b'", u"'\t'", 
            u"'\n'" ]

    ruleNames = [ u"T__6", u"T__5", u"T__4", u"T__3", u"T__2", u"T__1", 
                  u"T__0", u"ADDRESS", u"ID", u"WS" ]

    grammarFileName = u"Hosts.g4"

    def __init__(self, input=None):
        super(HostsLexer, self).__init__(input)
        self.checkVersion("4.4")
        self._interp = LexerATNSimulator(self, self.atn, self.decisionsToDFA, PredictionContextCache())
        self._actions = None
        self._predicates = None


