///////////////////////////////////////////////////////////////////////////////
// Words.c    rr566
// Reads in a dictionary and strings from standard input. If the strings
// can be broken up into real words, displays words using two-word and
// dynamic methods.
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "/c/cs223/hw7/hash.h"

#define MAX_LINE_LENGTH 1024


// Function Prototypes
Hash LoadDictionary(char * file_name);
void TwoWords (char * str, Hash h);
void Dynamic (char *str, Hash h);
void RecursivePrint (int n, int matrix[][n], char str[], int i, int j);

bool debug_flag = false;

///////////////////////////////////////////////////////////////////////////////
// main
// Reads in a string and tries to separate it into words
///////////////////////////////////////////////////////////////////////////////
int main (int argc, char * argv[])
{
  char input[MAX_LINE_LENGTH]; // Standard Input string
  char dict_name[MAX_LINE_LENGTH] = "words"; // Name of dictionary

  // Command Line Arguments
  for (int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-dict") && i < argc-1)
      strcpy(dict_name, argv[++i]);
    else if (!strcmp(argv[i], "-debug"))
      debug_flag = true;
    else
      fprintf(stderr, "usage: Words [-dict filename] [-debug]\n");
  }
  Hash dict = LoadDictionary(dict_name);

  while (fgets(input, MAX_LINE_LENGTH, stdin))
  {
      if (input[strlen(input)-1] == '\n')
        input[strlen(input)-1] = '\0';
      if (debug_flag)
        printf("Input: '%s'\n", input);
      bool non_alpha = false;
      for (int i = 0; i < strlen(input); i++)
      {
        if (!isalpha(input[i]))
        {
          fprintf(stderr, "Non-alpha character: [%c] in %s\n", input[i], input);
          non_alpha = true;
        }
        else
          input[i] = tolower(input[i]);
      }
      if (!non_alpha)
      {
        TwoWords(input, dict);
        Dynamic(input, dict);
      }
      else
      {
        fprintf(stderr, "Error: not processing %s\n", input);
      }
      printf("---\n");
  }
  HashDestroy(dict);
}


///////////////////////////////////////////////////////////////////////////////
// LoadDictionary
// Returns a hashtable with all words
///////////////////////////////////////////////////////////////////////////////
Hash LoadDictionary(char * file_name)
{
  int word_count = 0;
  if (debug_flag)
    printf("Loading dictionary: %s\n", file_name);
  Hash h = HashCreate(); // Dictionary
  FILE *input_file; // Pointer to input file
  char c[MAX_LINE_LENGTH];
  input_file = fopen(file_name, "r");
  if (input_file == NULL)
  {
    fprintf(stderr, "Fatal error: dictionary not found.\n");
    HashDestroy(h);
    exit(1);
  }
  while (fscanf(input_file, "%s", c) != EOF)
  {
    int ignore_word = false;
    for(int i = 0; c[i]; i++)
    {
      if (isalpha(c[i]))
        c[i] = tolower(c[i]);
      else
        ignore_word = true;
    }
    if (!ignore_word)
    {
      HashInsert(h, c);
      word_count++;
    }
  }
  if (debug_flag)
    printf("Word Count: %d\n", word_count);
  return h;
}

///////////////////////////////////////////////////////////////////////////////
// TwoWords
// Tries to separate a string into two words
///////////////////////////////////////////////////////////////////////////////
void TwoWords (char * str, Hash h)
{
  printf("Two words: ");
  bool success = false;
  for (int i = 1; i < strlen(str); i++)
  {
    char first[strlen(str)];
    strncpy(first, str, i);
    first[i] = '\0';
    char * last = &str[i];
    if (HashSearch(h, first) && HashSearch(h, last))
    {
      if (!success)
      {
        printf("SUCCESS: ");
        success = true;
      }
      printf("%s %s.  ", first, last);
    }
  }
  if (!success)
    printf("FAILURE");
  printf("\n");
}

///////////////////////////////////////////////////////////////////////////////
// Dynamic
// Looks for a combination of words that fit a string
///////////////////////////////////////////////////////////////////////////////
void Dynamic (char *str, Hash h)
{
  char substr[strlen(str)];
  int matrix[strlen(str)][strlen(str)];
  memset(matrix, -1, sizeof(matrix[0][0]) * strlen(str) * strlen(str));

  for (int l = 1; l <= strlen(str); l++)
  {
    for (int i = 0; i+l <= strlen(str); i++)
    {
      strncpy(substr, str+i, l);
      substr[l] = '\0';
      if (HashSearch(h, substr))
      {
        matrix[i][i+l-1] = i;
      }
      else
      {
        for (int split = i; split < i+l-1; split++)
        {
          if ((matrix[i][split] != -1) && (matrix[split+1][i+l-1] != -1))
          {
            matrix[i][i+l-1] = split;
          }
        }
      }
    }
  }
  printf("DP: ");
  if (strlen(str) > 0 && matrix[0][strlen(str)-1] != -1)
  {
    printf("SUCCESS: ");
    RecursivePrint (strlen(str), matrix, str, 0, strlen(str)-1);
    printf("\n");
    if (debug_flag) // Print Matrix
    {
      for (int i = 0; i < strlen(str); i++)
      {
        for (int j = 0; j < strlen(str); j++)
          printf(" %d ", matrix[i][j]);
        printf("\n");
      }
    }
  }
  else
    printf("FAILURE\n");
}

///////////////////////////////////////////////////////////////////////////////
// RecursivePrint
// Auxillary function for Dynamic--prints out the words that match a given
// matrix
///////////////////////////////////////////////////////////////////////////////
void RecursivePrint (int n, int matrix[][n], char str[], int i, int j)
{
  int split = matrix[i][j];
  char substr[strlen(str)];
  if (matrix[i][j] == -1)
    return;
  if (split+1 < strlen(str) && matrix[i][split] != -1 && matrix[split+1][j] != -1)
  {
    RecursivePrint (n, matrix, str, i, split);
    RecursivePrint (n, matrix, str, split+1, j);
  }
  else
  {
    strncpy(substr, str+i, j-i+1);
    substr[j-i+1] = '\0';
    if (i == 0)
      printf("%s", substr);
    else
      printf(" %s", substr);
  }
}
