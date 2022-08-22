#include <stdbool.h>



char* remove_quotes(char* string);

typedef enum { EQUALS, GREATER, LESS, GREATER_EQ, LESS_EQ } relation;

typedef enum { AND, OR } operator;

typedef enum { INT, STRING } valuetype;


typedef struct 
{
    int ival;
    char* sval;
    valuetype type;
} value;


typedef struct 
{
    char* field;
    relation operator;
    value value;
    valuetype type;
}
simple;

typedef enum { SIMPLE, COMPOUND } predicate_type;

typedef struct predicate
{

    simple simple_expr;

    struct {
    struct predicate* predicate1;
    operator operator;
    struct predicate* predicate2;
    } compound ;

    predicate_type type;

    
} predicate;


typedef struct node 
{
    char* value;
    struct node* next;

} node;




typedef struct 
{
    char* sheetname;
    predicate* expr;
    bool select_all;
    node* columns;

} query;

typedef struct 
{
    query* query;
    value value;

} update;

typedef struct {
    char* column;
    value value;
} pair;


typedef struct node_pair
{
    pair value;
    struct node_pair* next;
} node_pair;

typedef struct 
{
    node_pair* pairs;
    char* sheetname;

} insert_query;




void insert_node(node** head, char* string);


predicate* form_predicate(char* field, relation relation, value value, valuetype type);

predicate* form_comp_predicate(predicate* p1, predicate* p2, operator op);

query* form_query(char* sheetname, predicate* predicate, bool select_all, node* columns);

insert_query* form_insert(char* sheetname, node_pair* pairs);


bool satisfy_predicate(predicate* predicate, char* row[], int num_cols, char* header_row[]);

void evaluate(query* q);


void print_list(node* head);


void reverse_node(node** head);

update* form_update(query* query, value value);

void update_file(update* update);

void insert_node_pair(node_pair** head, pair pair);

void print_pair_list(node_pair* head);


void insert(insert_query* query);

void set_workbook(char* name);
