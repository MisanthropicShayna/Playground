#ifndef CIPHERS_H
#define CIPHERS_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

extern char* vigenere(const char*, const char*);
extern char* rot13(const char*);

extern char* vigenere_prototype(const char*, const char*);

#endif