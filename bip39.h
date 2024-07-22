/**
*
* | ============================================================ |
* |  Coded By : Dr.Linux                                         |
* |  Date : 2024/07/21 - 17:49:37                                |
* |  BIP0039 Header File "bip39.h" !                             |
* |  Defines The API For The BIP0039 Implementation .            |
* | ============================================================ |
*
**/



#ifndef BIP39_H
#define BIP39_H


/* ==================== Include Headers ==================== */
#include <stdbool.h>


/* ==================== Define Header Macros ==================== */
#define BIP39_WORDS_SIZE 2048
#define WORDS_LENGTH 12



/* ==================== Functions Declarations ==================== */


void FreeBIP39Array(char **bip39_array);
void GetBIP39Words(char *(*bip39_array)[BIP39_WORDS_SIZE]);
int GetBIP39WordIndex(char **bip39_array, const char word[]);
bool CheckBIP39Words(char **bip39_array, char **words);


#endif

