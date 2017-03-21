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
    char fdline[20], dline[20];

    //These variables are used to traverse the SHA1 hashed password file
    int sha1outputlength = SHA_DIGEST_LENGTH * 2 + 1;
    char fhline[sha1outputlength], hline[sha1outputlength];
    printf("Checking collisions:\n");

    while (fgets(fdline, 20, df) != NULL) {
        //printf("%s",fdline);

        //Set dline to a empty string
        memset(dline, 0, 20);

        //Trim the \n charater of the fdline
        if (fdline[strlen(fdline) - 1] == '\n') {
            strncpy(dline, fdline, strlen(fdline) - 1);
        } else {
            strncpy(dline, fdline, strlen(fdline));
        }

        unsigned char digest[SHA_DIGEST_LENGTH];
        //char string[] = "Password1 ";
        //SHA1((unsigned char*)&string, strlen(string), (unsigned char*)&digest);
        SHA1((unsigned char *) &dline, strlen(dline), (unsigned char *) &digest);

        //printf("Digest: %s\n", digest);

        char mdString[SHA_DIGEST_LENGTH * 2 + 1];
        int i;
        for (i = 0; i < SHA_DIGEST_LENGTH; i++)
            sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);

//        printf("SHA1 of \"%s\" is: %s\n", dline, mdString);

        //Done with hashing password from dictionary, now compare the hashed value with the values in hased password file
        rewind(hf); //Rewind the hashed file when checking the next password in the dictionary
        while (fgets(fhline, sha1outputlength, hf) != NULL) {
            //Set hline to a empty string
            memset(hline, 0, sha1outputlength);
            //Trim the \n charater of the fdline
            if (fhline[strlen(fhline) - 1] == '\n') {
                strncpy(hline, fhline, strlen(fhline) - 1);
            } else {
                strncpy(hline, fhline, strlen(fhline));
            }

            if (compareStrings(mdString, hline) == 1) {
                printf("Password found: %s\n", dline);
                fclose(df);
                fclose(hf);
                return 1;
            }
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
