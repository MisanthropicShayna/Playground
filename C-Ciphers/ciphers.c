#include "ciphers.h"

char* vigenere(const char* plain_string, const char* key) {
    // If the cipher key isn't all alphabetical, then return NULL.
    for(int i=0; i<strlen(key); ++i) {
        if(!isalpha(key[i])) {
            return NULL;
        }
    }

    // Create a copy of the plain input text, which will be ciphered before being returned.
    char* ciphered_string = malloc(strlen(plain_string)+1); 
    strcpy_s(ciphered_string, strlen(plain_string)+1, plain_string);
    
    // Allocates the vigenere substitution grid.    
    char** vigenere_grid = malloc(sizeof(char*) * 26);
    for(int i=0; i<26; vigenere_grid[i++] = malloc(26));
    
    // Creates the vigenere substitution grid.
    for(int x=0; x<26; ++x) {
        for(int y=0; y<26; ++y) {
            int shifted_index = x + y;
            while(shifted_index > 25) shifted_index -= 26;
            vigenere_grid[x][y] = ('A' + shifted_index);
        }
    }
    
    // Iterates over ciphered_string, and applies vigenere substitution to applicable characters.
    for(int string_index=0, key_index=0; string_index < strlen(ciphered_string); ++string_index) {
        // Reset the key index to 0 if it goes out of bounds, making it loop from the start.
        if(key_index >= strlen(key)) key_index = 0;
        
        // The current plain character being processed.
        const char string_char = ciphered_string[string_index];
        
        // The current key character being processed.
        const char key_char = key[key_index];        
        
        // Don't bother substituting the current plain character if it's not alphabetical.
        if(isalpha(string_char)) {
            // The alphabetical index of the current plain character.
            int string_char_index = (string_char - (islower(string_char) ? 'a' : 'A'));
            
            // The alphabetical index of the current key character.
            int key_char_index = (key_char - (islower(key_char) ? 'a' : 'A'));
            
            // The substituted plain character.
            char sub_char = vigenere_grid[string_char_index][key_char_index];
            
            // Downcases the substituted character if the original plain character was also lowercase.
            if(islower(string_char)) sub_char += 32;
            
            // Overwrites the plain character, with the substituted cipher character.
            ciphered_string[string_index] = sub_char;
            
            // Increments the key index.
            ++key_index;
        }
    }
    
    // Returns the ciphered copy of the plain string.
    return ciphered_string;
}

// Better way of vigenere 
char* vigenere_prototype(const char* plain_string, const char* key) {

}

char* rot13(const char* plain_string) {
    // Create a copy of the plain plain text, which will then be ciphered before being returned.
    char* ciphered_string = malloc(strlen(plain_string)+1);
    strcpy_s(ciphered_string, strlen(plain_string)+1, plain_string);

    // Iterates over ciphered_string, and applies Rot13 substitution to the applicable characters.
    for(int i=0; i<strlen(ciphered_string); ++i) {
        // The current character being processed.
        const char character = ciphered_string[i];

        // Only substitute if the character is alphabetical.
        if(isalpha(character)) {
            const char marker = isupper(character) ? 'M' : 'm';
            if(character > marker) ciphered_string[i] -= 13;
            if(character < marker) ciphered_string[i] += 13;
        }
    }

    // Returns the ciphered copy of the plain string.
    return ciphered_string;
}