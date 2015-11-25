#ifndef TOKENIZER_H
#define TOKENIZER_H
/*
 * tokenizer.c
 */

// Enum type represents the token type
typedef enum TokenT_{CTR, OPE, MAL, OCTAL, HEX, DEC, WORD, FLOAT } TokenT;

typedef struct TokenizerT_ TokenizerT;

TokenT TKGetType(TokenizerT* tk);
/*
 *  TKCreate initializes a TokenizerT object
 */
TokenizerT *TKCreate( char *);

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 */
void TKDestroy( TokenizerT *);
/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 */
char *TKGetNextToken( TokenizerT *);
#endif
