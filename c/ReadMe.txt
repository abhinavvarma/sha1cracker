1. Compile the code: gcc *.c -lcrypto
    -lcrypto is the option to compile with openssl
    openssl must be installed first
2. Execute with command: ./a.out dictionary.txt hashedpassword.txt
    + ./a.out: the object file compiled from Step 1
    + dictionary.txt: Name of the Password dictionary file (this will be created by the program)
    + hashedpassword.txt: Path to the file consisting of hashed passwords, one password in each line.