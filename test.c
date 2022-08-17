#include <stdio.h>
#include <string.h>
#include <stdbool.h>


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





int main()
{
    char* row[5];

    int cols = split_line("7,Mobile Finance Manager,6,5828,\"Mobile Finance Manager (MFM) integrates three key mobile banking technologies — app, WAP, and SMS — with mobile hardware innovations to grant dynamic anytime/anywhere access to feature-rich mobile banking. MFM applications provide a rich mobile banking experience on all major devices including iOS, Android, BlackBerry, Kindle Fire, Windows, Palm, and Java. App functionality includes bill pay, mobile deposit capture, secure messages, and multiple transfer flavors. WAP banking provides important banking features through any browser-enabled smartphone. The WAP is smart enough to identify the browser and respond accordingly with options appropriate to the mobile browser. SMS text message-based banking is available to all MFM users, but is especially convenient for users with older mobile devices that might not support apps or WAP. In addition to standard banking functions, SMS includes banking alerts that can notify users of issues without needing to log in to an app or online banking.\"\n", row);

    for (int i = 0; i < 5; i++) 
    {
        printf("%s\n", row[i]);
    }
}
