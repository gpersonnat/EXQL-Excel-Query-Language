#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "language.h"
#define MAX_CHARACTERS_CELL 32767
#define MAX_COLUMN_CSV 16384


char* workbook;



void set_workbook(char* name)
{
    workbook = strdup(name);
}


int col_number(char* string, char* array[], int num_columns)
{
    for (int i = 0; i < num_columns; i++)
    {
        if (strcmp(array[i], string) == 0)
        {
            return i;
        }
    }
    return 0;
}

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
            p->simple_expr.value.type = INT;
            p->simple_expr.type = INT;
            break;
        case STRING:
            p->simple_expr.value.sval = strdup(value.sval);
            p->simple_expr.value.type = STRING;
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

update* form_update(query* query, value value)
{
    update* u = malloc(sizeof(update));
    u->query = query;
    u->value = value;

    return u;
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
                    return 
                    satisfy_predicate(predicate->compound.predicate1, row, num_cols, header_row) 
                    || 
                    satisfy_predicate(predicate->compound.predicate2, row, num_cols, header_row);
                case AND:
                    return 
                    satisfy_predicate(predicate->compound.predicate1, row, num_cols, header_row) 
                    && 
                    satisfy_predicate(predicate->compound.predicate2, row, num_cols, header_row);
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

int node_pair_to_array(node_pair* head, pair array[]) 
{
    node_pair* cursor = head;
    int i = 0;
    while (cursor)
    {
        array[i] = cursor->value;
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

void insert_node_pair(node_pair** head, pair pair)
{
    node_pair* new_node = malloc(sizeof(node_pair));

    node_pair* cursor = *head;

    new_node->value = pair;

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


insert_query* form_insert(char* sheetname, node_pair* pairs) 
{
    insert_query* i = malloc(sizeof(insert_query));
    i->sheetname = strdup(sheetname);
    i->pairs = pairs;
    return i;

}

void print_pair_list(node_pair* head) 
{
    node_pair* cursor = head;
    while (cursor)
    {
        switch(cursor->value.value.type)
        {
            case INT:
                printf("Key: %s Value: %d \n", cursor->value.column, cursor->value.value.ival);
                break;
            case STRING:
                printf("Key: %s Value: %s \n", cursor->value.column, cursor->value.value.sval);
                break;

        }
        
        cursor = cursor->next;
    }
}

void print_pair_array(pair row[], int num_columns)
{
    for (int i = 0; i < num_columns; i++) 
    {
        switch(row[i].value.type)
        {
            case INT:
                printf("Key: %s Value: %d \n", row[i].column, row[i].value.ival);
                break;
            case STRING:
                printf("Key: %s Value: %s \n", row[i].column, row[i].value.sval);
                break;

        }
        
    }
}

bool comes_first(char* s1, char*s2, char* array[], int num_columns)
{
    for (int i = 0; i < num_columns; i++)
    {
        if (strcmp(s1, array[i]) == 0)
        {
            return true;
        }
        if (strcmp(s2, array[i]) == 0) 
        {
            return false;
        }
    }
    return false;
}

void swap(pair* x, pair* y)
{
    pair tmp = *x;
    *x = *y;
    *y = tmp;
}

// Uses bubblesort to sort key value pairs based on order of columns
void sort(pair pairs[], char* header_row[], int num_columns)
{
    for (int i = 0; i < num_columns - 1; i++)
    {
        for (int j = 0; j < num_columns - i - 1; j++)
        {
            if (!comes_first(pairs[j].column, pairs[j + 1].column, header_row, num_columns))
            {
                swap(&pairs[j], &pairs[j + 1]);
            }
        }
    }

}

void evaluate(query* q)
{
 
    char sheet[100];



    sprintf(sheet, "%s/%s.csv", workbook, q->sheetname);


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

char* create_row(pair pairs[], int num_columns)
{
    char row_string[100000] = "";
    for (int i = 0; i < num_columns; i++)
    {   
        switch(pairs[i].value.type)
        {
            case INT:
            {
                char val[MAX_CHARACTERS_CELL];
                if (i != num_columns - 1)
                {
                    sprintf(val, "%d,", pairs[i].value.ival);
                }
                else
                {
                    sprintf(val, "%d\n", pairs[i].value.ival);
                }
                strcat(row_string, val);
                break;
            }
            case STRING:
            {
                char val[MAX_CHARACTERS_CELL];
                if (i != num_columns - 1)
                {
                    sprintf(val, "%s,", pairs[i].value.sval);
                }
                else
                {
                    sprintf(val, "%s\n", pairs[i].value.sval);
                }
                strcat(row_string, val);
                break;
            }
        }

    }
    return strdup(row_string);
}




void update_file(update* update)
{

    char sheet[100];

    char* sheet_name = update->query->sheetname;

    sprintf(sheet, "%s/%s.csv", workbook,sheet_name);


    FILE* sheet_data = fopen(sheet, "r");

    char buffer[MAX_CHARACTERS_CELL];

    fgets(buffer, MAX_CHARACTERS_CELL, sheet_data);
    
    char* header_row[MAX_COLUMN_CSV];

    int num_columns = split_line(buffer, header_row);


    int row_counter = 0;

    while (fgets(buffer, MAX_CHARACTERS_CELL, sheet_data))
    {
        char* row[num_columns];

        split_line(buffer, row);

         if (!update->query->select_all)
        {
            
            if (!satisfy_predicate(update->query->expr, row, num_columns, header_row))
            {
                row_counter++;
                continue;
            }
        }
            
        char command[5000];
    
        switch(update->value.type) 
        {
            case INT:
            {
                int new_val = update->value.ival;
                char* type = "int";
                char* col_name = update->query->columns->value;
                sprintf(command, "python3 update.py %s %d %s %d %s %d %s", col_name, new_val, "int", row_counter, sheet_name, col_number(col_name, header_row, num_columns), workbook);
                system(command);
                break;
            }
            case STRING:
            {
                char* new_val = update->value.sval;
                char* col_name =  update->query->columns->value;
                sprintf(command, "python3 update.py %s %s %s %d %s %d %s", col_name, new_val, "string", row_counter, sheet_name, col_number(col_name, header_row, num_columns), workbook);
                system(command);
                break;
            }
        }
        row_counter++;
    }
    free(update);
    fclose(sheet_data);
}


void insert(insert_query* query)
{

    char sheet[100];

    char* sheet_name = query->sheetname;
 
    sprintf(sheet, "%s/%s.csv", workbook, sheet_name);


    FILE* sheet_data = fopen(sheet, "r");

    char buffer[MAX_CHARACTERS_CELL];


    fgets(buffer, MAX_CHARACTERS_CELL, sheet_data);


    char* header_row[MAX_COLUMN_CSV];
    
    int num_columns = split_line(buffer, header_row);


    pair new_row[MAX_COLUMN_CSV];

    node_pair_to_array(query->pairs, new_row);

    sort(new_row, header_row, num_columns);

    char* row_string = create_row(new_row, num_columns);

    sheet_data = fopen(sheet, "a");

    fputs(row_string, sheet_data);

    char command[50000];
    
    printf("%s\n", workbook);


    sprintf(command, "python3 insert.py %s %s %s", sheet_name, workbook, row_string);

    printf("%s\n", command);

    system(command);

    fclose(sheet_data);

    free(query);

}