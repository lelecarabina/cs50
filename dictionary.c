/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 1000000
 
int word_counter = 0;

typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
} 
node;

node* hashtable[HASHTABLE_SIZE] = { NULL };
node* head = NULL;

bool hash(const char* word)
{
    int hashes = 0;
    int i;
    int n;

    //Modified Hash Function from ???
    for (i = 0; word[i] != '\0'; i++) 
    {
        if (isalpha(word[i])) 
            n = (word[i] - 'a') + 1;
            
        else 
            n = 27;
            
        hashes = ((hashes << 3) + n) % HASHTABLE_SIZE;
    }
    
    return hashes;
}
    
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    char wordkeep[LENGTH + 1]; //= malloc(sizeof(wordlen));
    int len = strlen(word);
    
    for (int i = 0; i < len; i++)
    {
        if (isalpha(word[i])) 
            wordkeep[i] = tolower(word[i]);
            
        else 
            wordkeep[i] = 27;
    }
    
    int index = hash(wordkeep);
    
    if (hashtable[index] != NULL)
    {
        head = hashtable[index];
        node* cursor = head;
        
        while (cursor != NULL)
        {
            if (strcmp(wordkeep, cursor->word) == 0)
                return true;
            
            cursor = cursor->next;
        }
    }
    
    //free(wordkeep); //Breaks Valgrind
    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary) //ORIGINAL: bool load(const char* dictionary)
{
    // TODO
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return 2;
    }
    
    //node* head = NULL; // made it global 
    
    while (!feof(file))
    {
        node* new_node = malloc(sizeof(node));
        fscanf(file, "%s\n", new_node->word);  // W/o \n Valgrind crashes
        
        int index = hash(new_node->word);
        
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }
        
        else
        {
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
        }
        
        word_counter++;
        
        free(new_node);
    }
    
    fclose(file);
    
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    if (word_counter > 0)
        return word_counter;
        
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int index = 0; index < HASHTABLE_SIZE; index++) 
    {
        while (hashtable[index] != NULL)
        {
            node* cursor = hashtable[index];
            hashtable[index] = cursor->next;
            
            //free(cursor); //breaks Valgrind
        }
    }
    
    return true;
}
