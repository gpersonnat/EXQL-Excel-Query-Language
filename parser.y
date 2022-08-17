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
%token OR_OP AND_OP
%token EQUAL GREATER_OP LESS_OP GREATER_EQ_OP LESS_EQ_OP
%token EOL
%token CHOOSE
%token SHEET
%token SELECT 
%token ALL
%token WHERE
%token QUOTE
%token OPEN CLOSE

%left OR_OP AND_OP

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
| WORD GREATER_OP NUMBER { value val = {.ival = $3}; $$ = form_predicate(strdup($1), GREATER, val, INT); }
| WORD GREATER_OP STR { value val = {.sval = remove_quotes(strdup($3))}; $$ = form_predicate(strdup($1), GREATER, val, STRING); }
| WORD GREATER_EQ_OP NUMBER { value val = {.ival = $3}; $$ = form_predicate(strdup($1), GREATER_EQ, val, INT); }
| WORD GREATER_EQ_OP STR { value val = {.sval = remove_quotes(strdup($3))}; $$ = form_predicate(strdup($1), GREATER_EQ, val, STRING); }
| WORD LESS_OP NUMBER    { value val = {.ival = $3}; $$ = form_predicate(strdup($1), LESS, val, INT); }
| WORD LESS_OP STR { value val = {.sval = remove_quotes(strdup($3))}; $$ = form_predicate(strdup($1), LESS_EQ, val, STRING); }
| WORD LESS_EQ_OP NUMBER    { value val = {.ival = $3}; $$ = form_predicate(strdup($1), LESS_EQ, val, INT); }
| WORD LESS_EQ_OP STR { value val = {.sval = remove_quotes(strdup($3))}; $$ = form_predicate(strdup($1), LESS_EQ, val, STRING); }
| predicate OR_OP predicate {$$ = form_comp_predicate($1, $3, OR); }
| predicate AND_OP predicate {$$ = form_comp_predicate($1, $3, AND); }
| OPEN predicate CLOSE       {$$ = $2; }
;

columns:
| column 
| column 

exp:     
//  | CHOOSE SHEET WORD SELECT ALL  { $$ = strdup($3); }
  | CHOOSE SHEET WORD SELECT WHERE predicate { $$ = form_query($3, $6, false); }
  | CHOOSE SHEET WORD SELECT ALL             {$$ = form_query($3, NULL, true); }

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