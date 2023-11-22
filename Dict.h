#ifndef DICT_H
#define DICT_H

typedef struct Dict_t Dict;

/**
 * @brief Create an empty dictionary
 *
 * @param m the size of the hash table
 * @return Dict*
 */
Dict* dictCreate(int m);

/**
 * @brief Free the dictionary. The keys are freed but not the values.
 *
 * @param d the dictionary to be freed.
 */
void dictFree(Dict* d);

/**
 * @brief Return the value associated with the key in the dictionary. NULL if
 *        the key does not appear in the dictionary.
 *
 * @param d the dictionary
 * @param key the key
 * @return a void pointer (NULL if the key does not appear in the dictionary)
 */
void* dictSearch(Dict* d, char* key);

/**
 * @brief Return 1 if the key belongs to the dictionary, 0 otherwise.
 *
 * @param d the dictionary
 * @param key the key
 * @return 1 if the key is in the dictionary, 0 otherwise
 */
int dictContains(Dict* d, char* key);

/**
 * @brief Insert a new (key, value) pair in the dictionary. If the
 *        key already belongs to the dictionary, its value is overwritten
 *        with the new value.
 *
 * @param d the dictionary
 * @param key the key to be inserted
 * @param value the value to be inserted
 */
void dictInsert(Dict* d, char* key, void* value);

#endif
