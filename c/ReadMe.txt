1. Compile the code: gcc *.c -lcrypto
    -lcrypto is the option to compile with openssl
    openssl must be installed first
2. Execute with command: ./a.out dictionary.txt hashedpassword.txt
    + ./bfsha1match: the execute file compiled from 1
    + dictionary.txt: password dictionary file (generate from 16 to 19 character as described in the analyze report)
    + hashedpassword.txt: the administrator's hashed password that we want to compare. I use a file to store this in case we want to
    extend the program to find multiple passwords.
