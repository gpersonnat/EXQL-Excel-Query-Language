%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "language.h"
#include <string.h>

int yylex();

void yyerror(char* s);

%}



%union {
 int intval;
 double floatval;
 char* strval;
 expr* eval;
 predicate* pval;
 query* qval;
}

/* declare tokens */
%token <intval> NUMBER
%token <strval> WORD
%token <strval> STR
%token EQUAL
%token EOL
%token CHOOSE
%token SHEET
%token SELECT 
%token ALL
%token WHERE
%token QUOTE


%type <qval> exp

%type <pval> predicate




%%


app: /* nothing */                       
 | app exp EOL     {  evaluate($2); return 1; } 
 | app error EOL     { return 1; }
 | EOL               { return 1; }
 ;




predicate:
| WORD EQUAL NUMBER { value val = {.ival = $3}; $$ = form_predicate(strdup($1), EQUALS, val, INT); }
| WORD EQUAL STR { value val = {.sval = remove_quotes(strdup($3))}; $$ = form_predicate(strdup($1), EQUALS, val, STRING); }

;

exp:     
//  | CHOOSE SHEET WORD SELECT ALL  { $$ = strdup($3); }
  | CHOOSE SHEET WORD SELECT WHERE predicate { $$ = form_query($3, $6); }

 ;



;
%%


int main(int argc, char **argv)
{
  

  char* excel_file = argv[1];

  char command[100];

  sprintf(command, "python3 convert.py %s", excel_file);

  system(command);

  while(true) 
  {
    printf("db> ");
    yyparse();
  }

}

void yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}