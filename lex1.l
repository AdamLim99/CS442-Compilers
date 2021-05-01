%{
//#include "yaccExample.h"
#include "IOMngr.h"
#include "y.tab.h"
#include "Semantics.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = getNextSourceChar(); \
          result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
        }

%}


letter [A-Za-z]
digit [0-9]

%%
if					{return IF;}
else					{return ELSE;}
while					{return WHILE;}
for					{return FOR;}
void					{return Void;}
int					{return Int;}
bool					{return Bool;}
return					{return Ret;}
read					{return Read;}
print					{return Write;}
print\(					{return Print;}
printBool				{return printB;}
printlines				{return printL;}
printspaces				{return printS;}
true					{return True;}
false					{return False;}
{letter}({letter}|{digit})*		{return Ident;}
{digit}{digit}*				{return IntLit;}
\=\=					{return EQ;}
\!\=					{return EQ;}
\=					{return '=';}
\+					{return '+';}
\-					{return '-';}
\*					{return '*';}
\/					{return '/';}
\%					{return '%';}
\^					{return '^';}
\,					{return ',';}
\;					{return ';';}
\{					{return '{';}
\}					{return '}';}
\[					{return '[';}
\]					{return ']';}
\(					{return '(';}
\)					{return ')';}
\&\&					{return BitAnd;}
\|\|					{return BitOr;}
\<					{return '<';}
\>					{return '>';}
\<\=					{return setLE;}
\>\=					{return setGE;}
\!					{return '!';}
[ \t\r\n]				{}
.					{writeIndicator(getCurrentColumnNum());
				   	 writeMessage("Illegal Character in lex");}

%%

	
int yywrap () {
    return 1;
}