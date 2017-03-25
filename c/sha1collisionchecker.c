#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

int compareStrings(char str1[], char str2[]);

double getNoOfPasswords(FILE* passwordsFile) {
    char ch;
    double lines = 0;
    while(!feof(passwordsFile))
    {
        ch = fgetc(passwordsFile);
        if(ch == '\n')
        {
            lines++;
        }
    }

    rewind(passwordsFile);
    return lines;
}

int checkCollisions(char* passwordDictionaryFilePath, char* hashedPasswordsFilePath) {
//    printf("Received dictionaryfile=%s hashedpasswordfile=%s\n", passwordDictionaryFilePath, hashedPasswordsFilePath);

    //Load dictionary password file
    FILE *df = fopen(passwordDictionaryFilePath, "r");
    if (df == NULL) {
        printf("Cannot open password dictionary file %s\n", passwordDictionaryFilePath);
        exit(0);
    }

    //Load hashed password file
    FILE *hf = fopen(hashedPasswordsFilePath, "r");
    if (hf == NULL) {
        printf("Cannot open hashed passwords file %s\n", hashedPasswordsFilePath);
        exit(0);
    }

    printf("Counting number of passwords present in password file: ");
    fflush(stdout);
    double totalPasswords = getNoOfPasswords(df);
    printf("%g\n", totalPasswords);
    fflush(stdout);
    double completedPasswords = 0;

    //These variables are used to traverse the dictionary file
    char password[20], trimmedPassword[20];

    //These variables are used to traverse the SHA1 hashed password file
    int sha1outputlength = SHA_DIGEST_LENGTH * 2 + 1, noOfHashes;

    char originalHash[sha1outputlength], trimmedOriginalHash[sha1outputlength];

    if (fgets(originalHash, sha1outputlength, hf) != NULL) {
        //Set trimmedOriginalHash to a empty string
        memset(trimmedOriginalHash, 0, sha1outputlength);
        //Trim the \n charater of the password
        if (originalHash[strlen(originalHash) - 1] == '\n') {
            strncpy(trimmedOriginalHash, originalHash, strlen(originalHash) - 1);
        } else {
            strncpy(trimmedOriginalHash, originalHash, strlen(originalHash));
        }
    } else {
        printf("The Hashed password file does not contain any hashes");
        exit(1);
    }
    fclose(hf);

    printf("Checking collisions:\n");

    while (fgets(password, 20, df) != NULL) {
        //printf("%s",password);

        //Set trimmedPassword to a empty string
        memset(trimmedPassword, 0, 20);

        //Trim the \n charater of the password
        if (password[strlen(password) - 1] == '\n') {
            strncpy(trimmedPassword, password, strlen(password) - 1);
        } else {
            strncpy(trimmedPassword, password, strlen(password));
        }

        unsigned char digest[SHA_DIGEST_LENGTH];
        //char string[] = "Password1 ";
        //SHA1((unsigned char*)&string, strlen(string), (unsigned char*)&digest);
        SHA1((unsigned char *) &trimmedPassword, strlen(trimmedPassword), (unsigned char *) &digest);

        //printf("Digest: %s\n", digest);

        char mdString[SHA_DIGEST_LENGTH * 2 + 1];
        int i;
        for (i = 0; i < SHA_DIGEST_LENGTH; i++)
            sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);

//        printf("SHA1 of \"%s\" is: %s\n", trimmedPassword, mdString);

        //Done with hashing password from dictionary, now compare the hashed value with the values in hased password file
        if (compareStrings(mdString, trimmedOriginalHash) == 1) {
            printf("Password found: %s\n", trimmedPassword);
            fclose(df);
            return 1;
        }

        completedPasswords++;
        printf("%.2f%% Completed\r", (completedPasswords/totalPasswords) * 100);
    }
    printf("There is no password in the dictionary that matches with the hashed values");

    fclose(df);
    fclose(hf);
    return 0;
}

int compareStrings(char str1[], char str2[]) {
    if (strlen(str1) != strlen(str2)) {
        return 0;
    }
    int i;
    for (i = 0; str1[i] != '\0' && str2[i] != '\0'; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}
