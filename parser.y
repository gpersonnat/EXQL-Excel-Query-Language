%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "language.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int yylex();

void yyerror(char* s);


%}



%union {
 int intval;
 double floatval;
 char* strval;
 predicate* pval;
 query* qval;
 node* colval;
 update* uval;
 node_pair* pairsval;
 insert_query* insertval;
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
%token COMMA
%token SEMICOLON
%token COLUMNS
%token COLON
%token UPDATE
%token SET
%token TO
%token ADD
%token ROW
%token O_BRACE
%token C_BRACE
%token INTO
%token INSERT

%left OR_OP AND_OP


%type <qval> exp
%type <pval> predicate
%type <colval> columns
%type <uval> update
%type <pairsval> pairs pair pairs_obj
%type <insertval> insert





%%


app: /* nothing */                       
 | app exp EOL     {  evaluate($2); return 1; } 
 | app update EOL  {  update_file($2);  return 1; }
 | app insert EOL   { insert($2) ; return 1; }
 | app error EOL     { return 1; }
 | EOL               { return 1; }
 ;




predicate:
| WORD EQUAL NUMBER { value val = {.ival = $3, .type = INT}; $$ = form_predicate(strdup($1), EQUALS, val, INT); }
| WORD EQUAL STR { value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; $$ = form_predicate(strdup($1), EQUALS, val, STRING); }
| WORD GREATER_OP NUMBER { value val = {.ival = $3, .type = INT}; $$ = form_predicate(strdup($1), GREATER, val, INT); }
| WORD GREATER_OP STR { value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; $$ = form_predicate(strdup($1), GREATER, val, STRING); }
| WORD GREATER_EQ_OP NUMBER { value val = {.ival = $3, .type = INT}; $$ = form_predicate(strdup($1), GREATER_EQ, val, INT); }
| WORD GREATER_EQ_OP STR { value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; $$ = form_predicate(strdup($1), GREATER_EQ, val, STRING); }
| WORD LESS_OP NUMBER    { value val = {.ival = $3, .type = INT}; $$ = form_predicate(strdup($1), LESS, val, INT); }
| WORD LESS_OP STR { value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; $$ = form_predicate(strdup($1), LESS_EQ, val, STRING); }
| WORD LESS_EQ_OP NUMBER    { value val = {.ival = $3, .type = INT}; $$ = form_predicate(strdup($1), LESS_EQ, val, INT); }
| WORD LESS_EQ_OP STR { value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; $$ = form_predicate(strdup($1), LESS_EQ, val, STRING); }
| predicate OR_OP predicate {$$ = form_comp_predicate($1, $3, OR); }
| predicate AND_OP predicate {$$ = form_comp_predicate($1, $3, AND); }
| OPEN predicate CLOSE       {$$ = $2; }
;

columns:
| WORD                                    { node* n = malloc(sizeof(node)); n->value = strdup($1); $$ = n;  }
| WORD COMMA columns                       {  insert_node(&($3), strdup($1)); $$ = $3;  }
;


exp:     
  | CHOOSE SHEET WORD SELECT WHERE predicate {$$ = form_query($3, $6, false, NULL); }
  | CHOOSE SHEET WORD SELECT ALL             {$$ = form_query($3, NULL, true,  NULL); }
  | CHOOSE SHEET WORD SELECT WHERE predicate SEMICOLON COLUMNS COLON columns { reverse_node(&($10));$$ = form_query($3, $6, false, $10); }
  | CHOOSE SHEET WORD SELECT ALL SEMICOLON COLUMNS COLON columns { reverse_node(&($9)); $$ = form_query($3, NULL, true, $9); }
 ;

 update:
  | UPDATE SHEET WORD WHERE predicate SET columns TO NUMBER { value val = {.ival = $9, .type = INT}; query* q = form_query($3, $5, false, $7); $$ = form_update(q, val); }
  | UPDATE SHEET WORD WHERE predicate SET columns TO STR    { value val = {.sval = remove_quotes(strdup($9)), .type = STRING}; query* q = form_query($3, $5, false, $7); $$ = form_update(q, val); }
  | UPDATE SHEET WORD ALL SET columns TO NUMBER             { value val = {.ival = $8, .type = INT}; query* q = form_query($3, NULL, true, $6); $$ = form_update(q, val); }
  | UPDATE SHEET WORD ALL SET columns TO STR                { value val = {.sval = remove_quotes(strdup($8)), .type = STRING}; query* q = form_query($3, NULL, true, $6); $$ = form_update(q, val); }
  ;

 pair:
  | WORD COLON NUMBER    {value val = {.ival = $3, .type = INT}; pair p = {.column = strdup($1), .value = val}; node_pair* n = malloc(sizeof(node_pair)); n->value = p; $$ = n; }
  | WORD COLON STR    {value val = {.sval = remove_quotes(strdup($3)), .type = STRING}; pair p = {.column = strdup($1), .value = val}; node_pair* n = malloc(sizeof(node_pair)); n->value = p; $$ = n; }
  ;
 pairs:
 | pair               { $$ = $1; }
 | pair COMMA pairs   { insert_node_pair(&($3), $1->value); $$ = $3}
 ;

 pairs_obj:
 | O_BRACE pairs C_BRACE { $$ = $2; }

insert:
  | INSERT pairs_obj INTO SHEET WORD     {$$ = form_insert($5, $2); }




;
%%




char* read_fname(char* filename)
{
    char name[strlen(filename) - 5];
    for (int i = 0; i < strlen(filename); i++)
    {
        if (filename[i] == '.')
        {
            return strdup(name);
        }
        name[i] = filename[i];
    }
    return strdup(name);
}




int main(int argc, char **argv)
{

  

  char* excel_file = argv[1];


  char* name = read_fname(excel_file);

  char* dirname = name;

  mkdir(dirname,0777);


  set_workbook(name);

  char command[100];

  sprintf(command, "python3 convert.py %s %s", excel_file, name);

  system(command);

  while(true) 
  {
    printf("%s> ", name);
    yyparse();
  }

}



void yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}