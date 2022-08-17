#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_CHARACTERS_CELL 32767
#define MAX_COLUMN_CSV 16384


char* remove_quotes(char* string)
{
    char copy_string[strlen(string) - 2];
    int d = 0;
    for (int i = 1; i < strlen(string) - 1; i++)
    {
        
            copy_string[i - 1] = string[i];
    }
    return strdup(copy_string);
}


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
    valuetype type;
}
simple;

typedef enum { SIMPLE, COMPOUND } predicate_type;

typedef struct 
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

query* form_query(char* sheetname, predicate* predicate)
{
    query* q = malloc(sizeof(query));
    q->sheetname = strdup(sheetname);
    q->expr = predicate;

    return q;
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
        
            switch (predicate->simple_expr.type)
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
               
                if (strcasecmp(fieldname, header_row[i]) == 0)
                {
                    if (strcasecmp(row[i], val) == 0) 
                    {
                        return true;
                    }
                    return false;

                }
            }
            printf("Invalid column name %s\n", fieldname);
            return false;

        }
        case COMPOUND :
        {
            return satisfy_predicate(predicate->compound.predicate1, row, num_cols, header_row) || satisfy_predicate(predicate->compound.predicate2, row, num_cols, header_row);
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

int split_line(char* line, char* row[])
{

   char* new_line = remove_escape(line);

   char* token = strtok(new_line, ",");

   int column = 0;

   while (token != NULL)
   {
     
        row[column] = strdup(token);
        
        token = strtok(NULL, ",");

        column++;
   }

   return column;

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


    for (int i = 0; i < num_columns; i++) 
    {
        printf("%s", header_row[i]);

        if (i != num_columns - 1)
        {
            printf("|");
        }
    }

 


    printf("\n");



    while (fgets(buffer, MAX_CHARACTERS_CELL, sheet_data))
    {
        char* row[num_columns];

        split_line(buffer, row);


        if (!satisfy_predicate(q->expr, row, num_columns, header_row))
        {
            continue;
        }

        for (int i = 0; i < num_columns; i++) 
        {
            printf("%s", row[i]);

            if (i != num_columns - 1)
            {
            printf("|");
            }
        }
        printf("\n");

    }

    free(q->expr);
    free(q);

    fclose(sheet_data);

}


