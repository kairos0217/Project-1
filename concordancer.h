#ifndef CONCORDANCER_H 
#define CONCORDANCER_H

#define MAX_WORD_LEN 128
#define INITIAL_HASH_TABLE_SIZE 1301

// Data type for nodes in linked list
typedef struct list_node {
    char word[MAX_WORD_LEN]; // Word, as a null-terminated string
    int count;               // count of this word's appearances
    struct list_node *next;  // next in chain     
} list_node_t;

// Data type for a hash table. Use chaining for collisions, so array of list_nodes
typedef struct {
    list_node_t **array; // base array for hash table
    unsigned int length; // Length of base array
} table_t;

// Data type for a concordancer
typedef struct {
    table_t *table;   // Hash table that stores words
    unsigned size;    // Number of unique words stored in the concordancer
} concordancer_t;

/*
 * Create a new hash table
 * Returns: Pointer to a table_t representing an empty hash table
 *          or NULL if an error occurs
 */
table_t *create_table();

/*
 * Create a new empty concordancer
 * Returns: Pointer to a concordancer_t representing an empty concordancer
 *          or NULL if an error occurs
 */
concordancer_t *create_concordancer();

/*
 * Return an acceptable hash code for a given string
 * word: The word to get a hash code for
 * Returns: an int that can be used as a hash code
 *  Note: There are no guarantees on the bounds of this int, it 
 *      is up to the caller to use module or other operations to
 *      convert the result into a table index
 * _good_ hash codes will try to avoid clustering
 */
int hash_code(const char* word);

/*
 * Add a new word to the concordancer, initial count set to 0 
 * cord: A pointer to a concordancer to add the word to
 * word: The new word to add, as a null-terminated string
 * Returns: 0 if the word was successfully added
 *          or -1 if the word could not be added
 */
int cord_insert(concordancer_t *cord, const char *word);

/*
 * Search for a specific word in the concordancer, if found, increment its count
 * cord: A pointer to the concordancer to search
 * query: The word to search for, as a null-terminated string
 * Returns: 0 if the score was found in the concordancer, -1 otherwise
 */
int cord_query(concordancer_t *cord, const char *query);

/*
 * Print out all words in a concordancer, in decreasing count order
 * Write a word, then a space, then its count on each line 
 * cord: A pointer to the concordancer containing all words to print
 */
void cord_print(const concordancer_t *cord);

/* 
 * Resets all word counts to 0 in the given concordancer
 * cord: A pointer to the concordancer to resett
 */
void cord_reset(concordancer_t *cord);

/*
 * Frees all memory used to store the contents of a concordancer
 * cord: A pointer to the concordancer to free
 */
void cord_free(concordancer_t *cord);

/*
 * Create a new concordancer containing all words listed in a text file
 *  when loaded this way, all words are initialized to count of 0
 * file_name: The name of the file to read words from
 * Returns: A pointer to a new concordancer containing all of the file's words
 *          or NULL if the read operation fails
 */
concordancer_t *read_cord_from_text_file(const char *file_name);

/*
 * Writes the contents of a concordancer to a text file, with words in 
 *  decreasing count order. 
 * Write a word, then a space, then its count on each line of the file
 * cord: The concordancer to write
 * file_name: The name of the text file to write to
 * Returns: 0 on success or -1 on failure
 */
int write_cord_to_text_file(const concordancer_t *cord, const char *file_name);

#endif // CONCORDANCER_H
