#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "concordancer.h"

#define MAX_CMD_LEN 128

// A helper function to run a concordance on a specific file
// 'file_name': Name of the file to spell check
// 'cord': A concordancer containing valid words
int concordance_file(const char *file_name, concordancer_t *cord) {
    FILE *file = fopen(file_name, "r");

    if (!file) {
        printf("Concordance check failed\n");
        return -1;
    }

    int index = 0;
    char word[50];
    int ch;

    while ((ch = fgetc(file)) != EOF) {
      if (isspace(ch)) {
        if (index > 0) {  
          word[index] = '\0';
          if (cord_query(cord, word) == 0) {
              printf("%s", word);
          } else {
            printf("%s[X]", word);
          }
            index = 0; 
        }
        printf("%c",ch);
      } else {
        if (index < 49) {  
          word[index++] = ch;
        }
      }
    } 
    printf("\n");
    return 0;
}

/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
    char cmd[MAX_CMD_LEN];
    concordancer_t *cord = NULL;
    int loop = 1;
    if (argc > 1) {
        cord = read_cord_from_text_file(argv[1]);
        if (!cord) {
          printf("Failed to read word list from text file\n");
          loop = 0;
        } else {
          if (argc > 2) {
              printf("Word list successfully loaded from text file\n");
              loop = 0;
              concordance_file(argv[2], cord);
              cord_free(cord);
          }
        }
    } 
    
    if(loop) {
    
    printf("CSCI 2021 Concordancer System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to concordancer\n");
    printf("  query <word>:            queries for a word\n");
    printf("  print:                   shows all words currently in the concordancer\n");
    printf("  reset:                   resets all word counts to 0 in the concordancer\n");
    printf("  load <file_name>:        reads in word list from a file\n");
    printf("  save <file_name>:        writes concordancer to a file\n");
    printf("  concordance <file_name>: runs a concordance and spell check on the specified file\n");
    printf("  exit:                    exits the program\n");
    

    while (1) {

        printf("concordance_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }
        
        if (cord == NULL) {
          cord = create_concordancer();
        }
        
        if (strcmp("exit", cmd) == 0) {
            if(cord != NULL) {
              cord_free(cord);
            }
            return 0;
        }


        else if (strcmp("add", cmd) == 0) {
          scanf("%s",cmd);
            if (cord_insert(cord, cmd) == -1) {
                printf("Failed to add word \n");
            }
        }

        else if (strcmp("query", cmd) == 0) {
          scanf("%s", cmd);          
           if (cord_query(cord, cmd) == 0) {
               printf("'%s' present in concordancer\n", cmd);
           } else {
               printf("'%s' not found\n", cmd);
           }
        } 

        else if (strcmp("print", cmd) == 0) {
           cord_print(cord);
        }

        else if (strcmp("reset", cmd) == 0) {
           cord_reset(cord);
        } 

        else if (strcmp("load", cmd) == 0) {
           concordancer_t *copy = cord;
           scanf("%s",cmd);
           cord = read_cord_from_text_file(cmd);
           if (!cord) {
                printf("Failed to read word list from text file\n");
                cord = copy;
           } else {
                printf("Word list successfully read from text file\n");
                cord_free(copy);
           }
        } 

        else if (strcmp("save", cmd) == 0) {
            scanf("%s",cmd);

           if (write_cord_to_text_file(cord, cmd) == -1) {
                printf("Filed to write to text file\n");
           } else {
                printf("Concordance successfully written to text file\n");
           }
        } 

        else if (strcmp("concordance", cmd) == 0) {
            scanf("%s",cmd);
            concordance_file(cmd, cord);
        }
        
        else {
            printf("Unknown command %s\n", cmd);
        }
    }
    return 0;
  }
}

