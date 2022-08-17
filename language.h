#include <stdbool.h>


char* remove_quotes(char* string);

typedef enum { AND, OR, EQUALS } relation;

typedef union 
{
    int ival;
    char* sval;
} value;

typedef enum { INT, STRING } valuetype;

typedef struct 
{
    char* field;
    relation operator;
    value value;
}
simple;

typedef enum { SIMPLE, COMPOUND } predicate_type;

typedef struct predicate
{

    simple simple_expr;

    struct {
    struct predicate* predicate1;
    relation operator;
    struct predicate* predicate2;
    } compound ;

    predicate_type type;

    
} predicate;


typedef struct 
{
    char* sheetname;
    predicate* expr;

} query;


typedef union 
{
    predicate* predicate;
} expr;

predicate* form_predicate(char* field, relation relation, value value, valuetype type);

predicate* form_comp_predicate(predicate* p1, predicate* p2, relation op);

query* form_query(char* sheetname, predicate* predicate, bool select_all);


bool satisfy_predicate(predicate* predicate, char* row[], int num_cols, char* header_row[]);

void evaluate(query* q);




