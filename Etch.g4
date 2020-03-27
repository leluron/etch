grammar Etch;

file: globals* EOF;

scope: sc = ('public' | 'private');

globals
    : scope? ID '=' exp;

stat: ID (',' ID)* '=' explist;

retstat: explist;

exp
    // literals
    : 'true'                            #trueexp
    | 'false'                           #falseexp
    | inte                              #intexp
    | FLOAT                             #floatexp
    | STRING                            #stringexp
    | ID                                #idexp
    // object creation
    | '[' explist? ']'                  #listexp
    | 'function' '(' (ID (',' ID)*)? ')' stat* retstat #functiondef
    | '{' (exp '=' exp)* '}'                  #mapdef
    // access
    | exp '[' exp ']'                   #indexexp
    // functions
    | exp '(' explist? ')'             #funccallexp
    // unop
    | op=('-' | 'not') exp              #unaryexp
    // binop
    | exp op=('*' | '/' | '%') exp      #multiplicativeexp
    | exp op=('+' | '-') exp            #additiveexp
    | exp op=('<=' | '<' | '>' | '>=') exp #relationexp
    | exp op=('==' | '!=' ) exp            #comparisonexp
    | exp op='and' exp                     #andexp
    | exp op='or' exp                      #orexp
    // ternary
    | exp 'if' exp 'else' exp           #ternaryexp
    // parenthesis
    | '(' exp ')'                       #parenexp
    ;

inte : INT | HEX;

explist: exp (',' exp)*;

ID
    : [a-zA-Z_] [a-zA-Z_0-9]*
    ;

INT
    : [0-9]+
    ;

FLOAT
    : [0-9]+ '.' [0-9]* 
    | '.' [0-9]+ 
    ;

HEX
    : '0' [xX] [0-9a-fA-F]+;

STRING
    : '"' (~('"'))* '"'
    | '\'' (~('\''))* '\''
    ;

COMMENT
    : '//' ~[\r\n]* -> skip
    ;

SPACE
    : [ \t\r\n] -> skip
    ;

OTHER
    : . 
    ;