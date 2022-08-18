#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_CHARACTERS_CELL 32767
#define MAX_COLUMN_CSV 16384


char* remove_quotes(char* string)
{
    if (string[0] != '"')
    {
        return strdup(string);
    }
    char copy_string[strlen(string) - 2];
    int d = 0;
    for (int i = 1; i < strlen(string) - 1; i++)
    {
        
            copy_string[i - 1] = string[i];
    }
    return strdup(copy_string);
}


typedef struct node 
{
    char* value;
    struct node* next;

} node;



void print_list(node* head) 
{
    node* cursor = head;
    while (cursor)
    {
        printf("%s\n", cursor->value);
        cursor = cursor->next;
    }
}


typedef enum { EQUALS, GREATER, LESS, GREATER_EQ, LESS_EQ } relation;

typedef enum { AND, OR } operator;

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


typedef struct 
{
    char* sheetname;
    predicate* expr;
    bool select_all;
    node* columns;

} query;


typedef union 
{
    predicate* predicate;
} expr;



predicate* form_predicate(char* field, relation relation, value value, valuetype type)  
{
    predicate *p = malloc(sizeof(predicate));

    p->type = SIMPLE;

    p->simple_expr.field = strdup(field);

    p->simple_expr.operator = relation;

    switch(type)
    {
        case INT:
            p->simple_expr.value.ival = value.ival;
            p->simple_expr.type = INT;
            break;
        case STRING:
            p->simple_expr.value.sval = strdup(value.sval);
            p->simple_expr.type = STRING;
            break;
    }

    return p;   
}

predicate* form_comp_predicate(predicate* p1, predicate* p2, operator op) 
{
    predicate* p = malloc(sizeof(predicate));
    p->type = COMPOUND;
    p->compound.predicate1 = p1;
    p->compound.predicate2 = p2;
    p->compound.operator = op;
    return p;
}


query* form_query(char* sheetname, predicate* predicate, bool select_all, node* columns)
{
    query* q = malloc(sizeof(query));
    q->sheetname = strdup(sheetname);
    q->expr = predicate;
    q->select_all = select_all;
    q->columns = columns;
    return q;
}

bool compare_val(char* s1, char*s2, relation op, valuetype type)
{
    switch(type) 
    {
        case INT: 
        {
            switch(op)
            {
                case EQUALS: 
                {
                    return atoi(s1) == atoi(s2);
                }
                case GREATER:
                {
                    return atoi(s1) > atoi(s2);
                }
                case GREATER_EQ:
                {
                    return atoi(s1) >= atoi(s2);
                }
                case LESS:
                {
                    return atoi(s1) < atoi(s2);
                }
                case LESS_EQ:
                {
                    return atoi(s1) <= atoi(s2);
                }
            }
        }
        case STRING:
        {
            switch(op)
            {
                case EQUALS: 
                {
                    return strcmp(remove_quotes(s1), s2) == 0;
                }
                case GREATER:
                {
                    return strcmp(remove_quotes(s1), s2) > 0;
                }
                case GREATER_EQ:
                {
                    return strcmp(remove_quotes(s1), s2) >= 0;
                }
                case LESS:
                {
                    return strcmp(remove_quotes(s1), s2) < 0;
                }
                 case LESS_EQ:
                {
                    return strcmp(remove_quotes(s1), s2) <= 0;
                }
            }

        }
    }
}


bool satisfy_predicate(predicate* predicate, char* row[], int num_cols, char* header_row[])
{
    switch(predicate->type)
    {
        case SIMPLE :
        {
            char* fieldname = strdup(predicate->simple_expr.field);
            relation op = predicate->simple_expr.operator;
            char val[MAX_CHARACTERS_CELL];
            valuetype type = predicate->simple_expr.type;
            switch (type)
            {
                case INT:
                {
                    sprintf(val, "%d", predicate->simple_expr.value.ival);
                    break;
                }
                case STRING:
                {
                    sprintf(val, "%s", predicate->simple_expr.value.sval);
                    break;
                }
            }
            
            for (int i = 0; i < num_cols; i++) 
            {
               
                if (strcmp(fieldname, header_row[i]) == 0)
                {
                    return compare_val(row[i], val, op, type);

                }
            }
            printf("Invalid column name %s\n", fieldname);
            return false;

        }
        case COMPOUND :
        {
            switch(predicate->compound.operator)
            {
                case OR: 
                    return satisfy_predicate(predicate->compound.predicate1, row, num_cols, header_row) || satisfy_predicate(predicate->compound.predicate2, row, num_cols, header_row);
                case AND:
                    return satisfy_predicate(predicate->compound.predicate1, row, num_cols, header_row) && satisfy_predicate(predicate->compound.predicate2, row, num_cols, header_row);
            }
        }
    }

}


char* remove_escape(char* string)
{
    char new_string[strlen(string) - 1];

    for (int i = 0; i < (strlen(string) - 1); i++)
    {
        new_string[i] = string[i];
    }

    return strdup(new_string);
}

char* slice(const char *str, size_t start, size_t end)
{   
    char result[end - start];

    for (int i = start; i <= end; i++)
    {
        result[i - start] = str[i];
    }

    return strdup(result);
}

int split_line(char* line, char* row[])
{

   int column = 0;

   int start = 0;

   bool within_quote = false;

   for (int i = 0; i < strlen(line); i++)
   {
        if ((line[i] == ',' || line[i] == '\n') && !within_quote)
        {
        
           row[column] = strdup(slice(line, start, i-1));
           start = i + 1;
           column++;

        }
        if (i == 0 && line[i] == '"')
        {
            within_quote = true;
        }
        if (line[i] == '"' && i > 0 && (line[i - 1] == ',')) 
        {
            within_quote = true;
        }

        if (line[i] == '"' && (line[i + 1] == ',' || line[i + 1] == '\n'))
        {
            within_quote = false;
        }

   }
   return column;
}



void insert_node(node** head, char* string)
{
    node* new_node = malloc(sizeof(node));

    node* cursor = *head;

    new_node->value = strdup(string);

    new_node->next = NULL;


    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    cursor->next = new_node;

}

void reverse_node(node** head) 
{
    node* prev = NULL;
    node* current = *head;
    node* next = NULL;

    while (current != NULL) 
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head = prev;
}

int node_to_array(node* head, char* array[]) 
{
    node* cursor = head;
    int i = 0;
    while (cursor)
    {
        array[i] = strdup(cursor->value);
        i++;
        cursor = cursor->next;
    }
    return i;
}

bool mem(char* string, char* array[], int num_columns)
{
    for (int i = 0; i < num_columns; i++)
    {
        if (strcmp(array[i], string) == 0)
        {
            return true;
        }
    }
    return false;
}

void evaluate(query* q)
{
 
    char sheet[100];

    sprintf(sheet, "%s.csv", q->sheetname);


    FILE* sheet_data = fopen(sheet, "r");

    char buffer[MAX_CHARACTERS_CELL];

    fgets(buffer, MAX_CHARACTERS_CELL, sheet_data);

    char* header_row[MAX_COLUMN_CSV];
    
    int num_columns = split_line(buffer, header_row);

    char* selected_cols[MAX_COLUMN_CSV];

    int num_cols_selected = 0;
    

    if (q->columns) 
    {
        num_cols_selected = node_to_array(q->columns, selected_cols);
    }


    for (int i = 0; i < num_columns; i++) 
    {
        if (q->columns) 
        {
            if (mem(header_row[i], selected_cols, num_cols_selected))
            {
                printf("%s", header_row[i]);
            }
            if (i != num_cols_selected - 1 && num_cols_selected != 1)
            {
                printf("|");
            }

        }
        else 
        {
            printf("%s", header_row[i]);
            if (i != num_columns - 1)
            {
                printf("|");
            }
        }

        
    }

    printf("\n");

    printf("-----------------");
    printf("\n");


    while (fgets(buffer, MAX_CHARACTERS_CELL, sheet_data))
    {
        char* row[num_columns];

        split_line(buffer, row);

        if (!q->select_all)
        {
            
            if (!satisfy_predicate(q->expr, row, num_columns, header_row))
            {
                continue;
            }
        }
    
        for (int i = 0; i < num_columns; i++) 
        {
            if (q->columns)
            {
                if (mem(header_row[i], selected_cols, num_cols_selected))
                {
                    printf("%s", row[i]);
                }

                if (i != num_cols_selected - 1 && num_cols_selected != 1)
                {
                    printf("|");
                }

            }
            else 
            {
                printf("%s", row[i]);
                if (i != num_columns - 1)
                {
                printf("|");
             }
            }

            
        }
        printf("\n");

    }

    free(q->expr);
    free(q);

    fclose(sheet_data);
}


