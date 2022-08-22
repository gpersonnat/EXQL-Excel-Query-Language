#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char* create_row(char* pairs[], int num_columns)
{
    char row_string[] = "";
    for (int i = 0; i < num_columns; i++)
    {       
            strcat(row_string, pairs[i]);
            break;
   

    }
    return strdup(row_string);
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





typedef struct node 
{
    char* value;
    struct node* next;

} node;



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

char* read_fname(char* filename)
{
    char name[strlen(filename - 5)];
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

int main()
{

    char destination[1000] = "Hello ";
    char source[1000] = "World!";
    printf("Concatenated String: %s\n",  strcat(destination,source));
    return 0;

    // char* pairs[] = {"test1", "test2"};

    // char* row_string = strcat("", "test");
    // printf("%s\n", row_string);
    // char* name = read_fname("products.xlsx");

    // printf("%s\n", name);
    
    // char* excel_file = "products.xlsx";

    // char name[30];


    // sscanf(excel_file, "%s.xlsx", name);

    // printf("%s\n", name);
    // node* n = malloc(sizeof(node));

    // n->value = "test1";

    // insert_node(&n, "test2");

    // insert_node(&n, "test4");

    // insert_node(&n, "test5");

    // reverse_node(&n);


    // node* cursor = n;

    // while (cursor)
    // {
    //     printf("%s\n", cursor->value);
    //     cursor = cursor->next;
    // }



    // free(n);
}
