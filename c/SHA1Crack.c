
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "passwordgenerator.h"
#include "bfsha1match.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s password_dictionary_file_path hashed_passwords_file_path\n", argv[0]);
        return 1;
    }

    int optionChosen = 0;

    printf("Choose the task:\n1. Generate the password dictionary file\n2. Check for collisions\n");
    scanf("%d", &optionChosen);

    char* passwordsFilePath = argv[1];
    char* hashedPasswordsFilePath = argv[2];

    if (optionChosen == 1) {
        generatePasswordFile(passwordsFilePath);
    } else if (optionChosen == 2) {
        checkCollisions(passwordsFilePath, hashedPasswordsFilePath);
    } else {
        printf("Invalid option chosen");
    }
}