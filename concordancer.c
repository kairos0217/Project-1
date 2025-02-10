#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concordancer.h"

table_t *create_table() {
    table_t *table = malloc(sizeof(table_t));
  //  printf("size of table: %lu\n", sizeof(*table));
    if(!table) {
        return NULL;
    }

    table->length = INITIAL_HASH_TABLE_SIZE;
    table->array = calloc(table->length, sizeof(list_node_t*));
  //  printf("size of: %lu\n", sizeof(table->array));


    if(!table->array) {
        free(table);
        return NULL;
    }

    return table;
}

concordancer_t *create_concordancer() {
    concordancer_t *concordancer = malloc(sizeof(concordancer_t));
  //  printf("size of cord: %lu\n", sizeof(*concordancer));

    if(!concordancer) {
        return NULL;
    }

    concordancer->table = create_table();


    if(!concordancer->table) {
        free(concordancer);
        return NULL;
    }

    concordancer->size = 0;

    return concordancer;
}


int hash_code(const char* word) {
    int hash = 31;
    int i = 0;

    int s = word[i];

    while(word[i] != '\0') {
        hash = hash + word[i] * i + s;
        i++;
    }

    return hash;
}


list_node_t *create_node(const char *word) {
    list_node_t *new = malloc(sizeof(list_node_t));

    if(!new) {
        return NULL;
    }

    new->count = 0;
    new->next = NULL;
    strcpy(new->word, word);

    return new;
}


int cord_insert(concordancer_t *cord, const char *word) {
    
    if(!cord || !word) {
        return -1;
    }
    
    
    int index = hash_code(word) % cord->table->length;

    list_node_t *dest = NULL;

    if(cord->table->array[index] != NULL) {
        list_node_t *cur = cord->table->array[index];
        
        while(cur != NULL) {
            if(strcmp(cur->word, word) == 0) {
                return -1;
            }
            dest = cur;
            cur = cur->next;
        }
    }

    list_node_t *new = create_node(word);
    if(!new) {
       return -1;
    }

    if(!dest) {
        cord->table->array[index] = new;
    } else {
        dest->next = new;
    }
    cord->size++;
    return 0;
}

int cord_query(concordancer_t *cord, const char *query) {
    int index = hash_code(query) % cord->table->length;
    list_node_t *cur = cord->table->array[index];
    while(cur != NULL) {
        if(strcmp(cur->word, query) == 0) {
            cur->count++;
            return 0;
        }
        cur = cur->next;
    }
   return -1;
}

list_node_t **sorted_nodes(const concordancer_t *cord) {

    list_node_t **sorted = malloc(cord->size * sizeof(list_node_t));

    if (!sorted) {
        return NULL;
    }

    int index = 0;
    int index_s = 0;

    while (index < cord->table->length) {
        list_node_t *cur = cord->table->array[index];
        while (cur != NULL) {
            sorted[index_s] = cur;
            cur = cur->next;
            index_s++;
        }
        index++;
    }

    list_node_t *temp;

    for (int i = 0; i < cord->size - 1; i++) {
        for (int j = 0; j < cord->size - i - 1; j++) {
            if (sorted[j]->count < sorted[j + 1]->count) {
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    return sorted;

}

void cord_print(const concordancer_t *cord) {

    if (cord->size < 1) {
      return;
    }
    
    list_node_t **sorted = sorted_nodes(cord);

    for(int i = 0; i < cord->size; i++) {
        printf("%s %d\n", sorted[i]->word, sorted[i]->count);
    }

    free(sorted);
}

void cord_reset(concordancer_t *cord) {
    int index = 0;

    while(index < cord->table->length) {
        list_node_t *cur = cord->table->array[index];
        while (cur != NULL) {
            cur->count = 0;
            cur = cur->next;
        }
        index++;
    }
}

void cord_free(concordancer_t *cord) {
    int index = 0;

    while(index < cord->table->length) {
        list_node_t *cur = cord->table->array[index];
        while (cur != NULL) {
            list_node_t *temp = cur->next;
            free(cur);
            cur = temp;
        }
        index++;
    }

    free(cord->table->array);
    free(cord->table);
    free(cord);
}

concordancer_t *read_cord_from_text_file(const char *file_name) {
    concordancer_t *cord = create_concordancer();

    FILE *file = fopen(file_name, "r");
    if (!file) {
        cord_free(cord);
        return NULL;
    }

    char line[100];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        cord_insert(cord, line);
    }

    fclose(file);

    return cord;
}

int write_cord_to_text_file(const concordancer_t *cord, const char *file_name) {
    FILE *file = fopen(file_name, "w");

    if (!file) {
        return -1;
    }

    list_node_t **sorted = sorted_nodes(cord);

    for(int i = 0; i < cord->size; i++) {
        fprintf(file,"%s %d\n", sorted[i]->word, sorted[i]->count);
    }

    free(sorted);

    return 0;
}
