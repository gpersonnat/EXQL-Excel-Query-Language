alpha [a-zA-Z]
digit [0-9]


%{


  #include "language.h"
  
  #include "parser.tab.h"

  #include <stdbool.h>


%}


%%
"choose" { return CHOOSE; }
"sheet" { return SHEET; }
"select" { return SELECT; }
"all"   { return ALL; }
"where" { return WHERE; }
"or"    { return OR_OP; }
{alpha}({alpha}|{digit})* { yylval.strval = strdup(yytext); return WORD; }
"="    { return EQUAL; }
"\""   { return QUOTE; }
[0-9]+ { yylval.intval = atoi(yytext); return NUMBER; }
\n     { return EOL; }
[ \t]  { /* ignore whitespace */ }
\"(\\.|\"\"|[^"\n])*\" { yylval.strval = strdup(yytext); return STR; }
.      { printf("Mystery character %c\n", *yytext); }
%%

