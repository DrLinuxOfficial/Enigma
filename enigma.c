/**
*
* | ============================================================ |
* |  Coded By : Dr.Linux                                         |
* |  Date : 2024/07/21 - 17:49:37                                |
* |  Enigma Mnemonic Phrase (BIP0039) Calculator Project :)      |
* |  Written In C Programming Language :)                        |
* | ============================================================ |
*
**/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bip39.h"



#if defined(__linux__)
   #define PLATFORM "Linux"
#elif defined(_WIN32)
   #define PLATFORM "Windows"
#else
   #define PLATFORM NULL
#endif



char **split_words(const char *string_buffer, int *words_count);
void clear_terminal();
void words_array_swap(char **buffer1, char **buffer2);
void permute_words(char **words_array, int start, int end, int find_count);



char *bip39_words[BIP39_WORDS_SIZE];
int words_iter = 0;
int correct_iter = 0;



int main()
{
	GetBIP39Words(&bip39_words);
	// Simple BIP0039 : "excite clerk prevent future limit rebel extend december peace iron jungle awake"
	char string_buffer[108];
	printf("Enter The Words : ");
	fgets(string_buffer, sizeof(string_buffer), stdin);
	string_buffer[strcspn(string_buffer, "\n")] = '\0';
	int word_count;
	char **words = split_words(string_buffer, &word_count);
	if (word_count == WORDS_LENGTH)
	{
		for (int i = 0; i < word_count; i++)
		{
			if (GetBIP39WordIndex(bip39_words, words[i]) == -1)
			{
				printf("Error: \"%s\" Word Not Found In BIP0039 English !\n", words[i]);
				return EXIT_FAILURE;
			}
		}
		printf("\n\n1. Find By Count\n2. Find All\n");
		int find_option;
		printf("\n\tPlease Choose an Option : ");
		scanf("%d", &find_option);
		if (find_option == 1)
		{
			int find_count;
			printf("\tPlease Enter The Count Of Phrases : ");
			scanf("%d", &find_count);
			clear_terminal();
			permute_words(words, 0, WORDS_LENGTH - 1, find_count);
		}
		else if (find_option == 2)
		{
			clear_terminal();
			permute_words(words, 0, WORDS_LENGTH - 1, 0);
		}
	}
	else
	{
		printf("Error: The Number Of Entered Words Is Not Equal To %d !\n", WORDS_LENGTH);
		return EXIT_FAILURE;
	}
}


char **split_words(const char *string_buffer, int *words_count)
{
	char **words = (char **)malloc(0);
	char *word;
	const char *temp = string_buffer;
	int counter = 0;
	int word_length = 0;
	while (*temp)
	{
		while (*temp == ' ')
		{
			temp++;
		}
		word_length = 0;
		while (temp[word_length] && temp[word_length] != ' ')
		{
			word_length++;
		}
		if (word_length > 0)
		{
			words = (char **)realloc(words, sizeof(char *) * (counter + 1));
			word = (char *)malloc(word_length + 1);
			strncpy(word, temp, word_length);
			word[word_length] = '\0';
			words[counter] = word;
			counter++;
			temp += word_length;
		}
	}
	*words_count = counter;
	return words;
}


void clear_terminal()
{
	if (!strcmp(PLATFORM, "Linux"))
	{
		system("clear");
	}
	else if (!strcmp(PLATFORM, "Windows"))
	{
		system("cls");
	}
}


void words_array_swap(char **buffer1, char **buffer2)
{
	char *temp = *buffer1;
	*buffer1 = *buffer2;
	*buffer2 = temp;
}


void permute_words(char **words_array, int start, int end, int find_count)
{
	if (start == end)
	{
		words_iter++;
		char **phrases = (char **)malloc(WORDS_LENGTH * sizeof(char *));
		for (int i = 0; i <= end; i++)
		{
			phrases[i] = strdup(words_array[i]);
		}
		if (CheckBIP39Words(bip39_words, phrases))
		{
			correct_iter++;
			printf("Phrases(%d): ", correct_iter);
			for (int i = 0; i <= end; i++)
			{
				printf("%s", phrases[i]);
				if (i < end)
				{
					putchar(' ');
				}
			}
			putchar('\n');
		}
		printf("Search(s) : %d\r", words_iter);
		fflush(stdout);
		if (find_count != 0 && correct_iter >= find_count)
		{
			printf("\n\nEnd !\n");
			FreeBIP39Array(bip39_words);
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		for (int i = start; i <= end; i++)
		{
			words_array_swap(&words_array[start], &words_array[i]);
			permute_words(words_array, start + 1, end, find_count);
			words_array_swap(&words_array[start], &words_array[i]);
		}
	}
}

