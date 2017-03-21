#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int comparator(const void *a, const void *b)
{
    return ( *(char *)a - *(char *)b );
}


int findNextBiggerCharacter(char *str, char first, int startIndex, int endIndex)
{
    int ceilIndex = startIndex;

    int i;
    for (i = startIndex+1; i <= endIndex; i++)
        if (str[i] > first && str[i] < str[ceilIndex])
            ceilIndex = i;

    return ceilIndex;
}

void writeToFile(FILE* targetFile, char* str) {
    fputs(str, targetFile);
    fprintf(targetFile, "\n");
}


void swap(char* a, char* b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

void generatePermutations(char str[], FILE* targetFile)
{
    int size = strlen(str);
    qsort(str, size, sizeof(str[0]), comparator);

    int isDone = 0;
    while (!isDone)
    {
        writeToFile(targetFile, str);
        int i;
        for (i = size - 2; i >= 0; --i)
            if (str[i] < str[i+1])
                break;
        if (i == -1)
            isDone = 1;
        else
        {
            int indexOfNextBiggerCharacter = findNextBiggerCharacter(str, str[i], i + 1, size - 1);
            swap(&str[i], &str[indexOfNextBiggerCharacter]);
            qsort(str + i + 1, size - i - 1, sizeof(str[0]), comparator);
        }
    }
}

double totalNoOfCombinations = 0;
double noOfCombinationsPermutatted = 0;

struct CharactersOnKey {
    char *character;
};

FILE* createOutputFile(char* filename) {
    FILE *df = fopen(filename, "w");
    return df;
}

double getTotalCombinations(int noOfKeysPressed, struct CharactersOnKey charactersOnKey[]) {
    double totalCombinations = 1;
    int i;
    for (i = 0; i < noOfKeysPressed; ++i) {
        struct CharactersOnKey key = charactersOnKey[i];
        int j = 0;
        while (key.character[j] != '\0') {
            j++;
        }

        totalCombinations *= j;
    }

    return totalCombinations;
}

void generateCombinations(FILE *outputFile, int noOfKeysPressed, struct CharactersOnKey charactersOnKey[], char *selectedCharacters, int currentIndex) {

    if (currentIndex == noOfKeysPressed) {
        selectedCharacters[currentIndex] = '\0';
        char clone[noOfKeysPressed+1];
        strcpy(clone, selectedCharacters);
        generatePermutations(clone, outputFile);
        noOfCombinationsPermutatted++;
        printf("%.2f%% Completed\r", (noOfCombinationsPermutatted/totalNoOfCombinations) * 100);
        fflush(stdout);
        return;
    }

    struct CharactersOnKey currentKey = charactersOnKey[currentIndex];
    int charactersOnCurrentKey = 3;

    int i;
    for (i = 0; i < charactersOnCurrentKey; ++i) {
        if (currentKey.character[i] == '\0')
            break;
        selectedCharacters[currentIndex] = currentKey.character[i];
        generateCombinations(outputFile, noOfKeysPressed, charactersOnKey, selectedCharacters, currentIndex + 1);
    }
}

void generatePasswordFile(char* passwordOutputFilePath) {
    FILE* fout = createOutputFile(passwordOutputFilePath);
    if (fout == NULL) {
        printf("Cannot read/create the passwords file %s\n", passwordOutputFilePath);
        exit(0);
    }

    int noOfKeysPressed = 8;
    struct CharactersOnKey charactersOnKey[noOfKeysPressed];

    char characters0[] = {'q', 'Q', '@', '\0'};
    charactersOnKey[0].character = characters0;

    char characters1[] = {'w', 'W', '\0'};
    charactersOnKey[1].character = characters1;

    char characters2[] = {'5', '%', '\0'};
    charactersOnKey[2].character = characters2;

    char characters3[] = {'(', '8', '[', '\0'};
    charactersOnKey[3].character = characters3;

    char characters4[] = {'=', '0', '}', '\0'};
    charactersOnKey[4].character = characters4;

    char characters5[] = {'i', 'I', '\0'};
    charactersOnKey[5].character = characters5;

    char characters6[] = {'*', '+', '~', '\0'};
    charactersOnKey[6].character = characters6;

    char characters7[] = {'n', 'N', '\0'};
    charactersOnKey[7].character = characters7;

    char selectedCharacters[noOfKeysPressed+1];

    totalNoOfCombinations = getTotalCombinations(noOfKeysPressed, charactersOnKey);
    printf("Generating the possible password combinations:\n");
    generateCombinations(fout, noOfKeysPressed, charactersOnKey, selectedCharacters, 0);
    fclose(fout);
    printf("Saved the password dictionary file at %s\n", passwordOutputFilePath);
}