alpha [a-zA-Z]
digit [0-9]


%{


  #include "language.h"
  
  #include "parser.tab.h"

  #include <stdbool.h>


%}


%%
"CHOOSE" { return CHOOSE; }
"SHEET" { return SHEET; }
"SELECT" { return SELECT; }
"ALL"   { return ALL; }
"WHERE" { return WHERE; }
{alpha}({alpha}|{digit})* { yylval.strval = strdup(yytext); return WORD; }
"="    { return EQUAL; }
"\""   { return QUOTE; }
[0-9]+ { yylval.intval = atoi(yytext); return NUMBER; }
\n     { return EOL; }
[ \t]  { /* ignore whitespace */ }
\"(\\.|\"\"|[^"\n])*\" { yylval.strval = strdup(yytext); return STR; }
.      { printf("Mystery character %c\n", *yytext); }
%%

