#!/usr/bin/python
from pygments.lexer import RegexLexer, words, bygroups
from pygments.token import *

class MsgSrvLexer(RegexLexer):
    name = 'MsgSrv'
    aliases = ['msg','srv']
    filenames = ['*.msg','*.srv']

    tokens = {
        'root' : [
            (r'^bool|^int8|^uint8|^int16|^uint16|^int32|^uint32|^int64|^uint64|^float32|^float64|^string|^time|^duration',Keyword),
            (r'(\[\d*\])+', Operator),
            (r'([ ]\S+)(([ ]=[ ])(\S+))?$',bygroups(Text,None,Operator,String))
        ]
    }
