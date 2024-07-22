/**
*
* | ============================================================ |
* |  Coded By : Dr.Linux                                         |
* |  Date : 2024/07/21 - 17:49:37                                |
* |  BIP0039 Header C File "bip39.c" !                           |
* |  Implementation Of The BIP0039 Algorithm .                   |
* | ============================================================ |
*
**/



/* ==================== Include Headers ==================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"
#include "bip39.h"


/* ==================== Disable Visual Studio 4996 Warning ==================== */
#pragma warning (disable : 4996)


/* ==================== Define Macros ==================== */
#define CONCATENATION_BITS_LENGTH 11
#define CHECKSUM_RATIO 33



/* ==================== Functions Definitions ==================== */


void FreeBIP39Array(char **bip39_array)
{
	for (int i = 0; i < BIP39_WORDS_SIZE; i++)
	{
		free(bip39_array[i]);
		bip39_array[i] = NULL;
	}
}


void GetBIP39Words(char *(*bip39_array)[BIP39_WORDS_SIZE])
{
	FILE *bip39_file = fopen("BIP39.txt", "r");
	if (bip39_file == NULL)
	{
		printf("Error: Can Not Read BIP39.txt File !\n");
		exit(EXIT_FAILURE);
	}
	char line_buffer[255];
	int word_index = 0;
	while (fgets(line_buffer, sizeof(line_buffer), bip39_file))
	{
		line_buffer[strcspn(line_buffer, "\r\n")] = '\0';
		(*bip39_array)[word_index] = strdup(line_buffer);
		word_index++;
	}
	fclose(bip39_file);
}


int GetBIP39WordIndex(char **bip39_array, const char word[])
{
	for (int i = 0; i < BIP39_WORDS_SIZE; i++)
	{
		if (!strcmp(bip39_array[i], word))
		{
			return i;
		}
	}
	return -1;
}


bool CheckBIP39Words(char **bip39_array, char **words)
{
	int concatenationBitsLen = WORDS_LENGTH * CONCATENATION_BITS_LENGTH;
	int *concatenationBits = (int *)malloc(concatenationBitsLen * sizeof(int));
	for (int word_counter = 0; word_counter < WORDS_LENGTH; word_counter++)
	{
		int word_index = GetBIP39WordIndex(bip39_array, words[word_counter]);
		if (word_index == -1)
		{
			return false;
		}
		for (int i = 0; i < CONCATENATION_BITS_LENGTH; i++)
		{
			concatenationBits[(word_counter * CONCATENATION_BITS_LENGTH) + i] = (word_index & (1 << (10 - i))) != 0;
		}
	}
	int checksumBitsLen = concatenationBitsLen / CHECKSUM_RATIO;
	int entropyBitsLen = concatenationBitsLen - checksumBitsLen;
	int entropyBytesLen = entropyBitsLen / 8;
	unsigned char *words_entropy = (unsigned char *)malloc(entropyBytesLen * sizeof(unsigned char));
	for (int i = 0; i < entropyBytesLen; i++)
	{
		words_entropy[i] = 0;
		for (int j = 0; j < 8; j++)
		{
			if (concatenationBits[(i * 8) + j])
			{
				words_entropy[i] |= 1 << (7 - j);
			}
		}
	}
	unsigned char hashBytes[32];
	SHA256_CTX sha256_obj;
	sha256_init(&sha256_obj);
	sha256_update(&sha256_obj, words_entropy, entropyBytesLen);
	sha256_final(&sha256_obj, hashBytes);
	int hashBits[256];
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			hashBits[(i * 8) + j] = (hashBytes[i] & (1 << (7 - j))) != 0;
		}
	}
	for (int i = 0; i < checksumBitsLen; i++)
	{
		if (concatenationBits[entropyBitsLen + i] != hashBits[i])
		{
			return false;
		}
	}
	return true;
}

