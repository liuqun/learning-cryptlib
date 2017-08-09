/*
*
* This file is part of the cryptlib test package
* File: cryptlib-test.c
* Version  : 1.0
* License  : BSD
*
*
* Copyright (c) 2016
*	Ralf Senderek, Ireland.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	   This product includes software developed by Ralf Senderek.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cryptlib/cryptlib.h>

void hexdump(FILE* f, const void *data_, int length){

    const char *hexmap = "0123456789abcdef";
    const char *data = (const char *)data_;
    char ch[2];
    int i;
    for (i=0; i<length; i++){
        ch[0] = hexmap[0x0F & (data[i]>>4)];
        ch[1] = hexmap[0x0F & data[i]];
        fprintf(f, "%c%c", ch[0], ch[1]);
    }
}

/***************************************************************************************/
int main (int argc, char *args[]) {

    int  status;

    char message[1024] = "cryptlib";
    int length = strlen(message);

    status = cryptInit();
    printf("Cryptlib: performing basic hash test in C.\n");
    if( status != CRYPT_OK ){
        printf("CryptLib initialisation failed!\n");
        exit(2);
    }

    CRYPT_CONTEXT hashContext;
    const CRYPT_USER cryptUser = CRYPT_UNUSED;
    const CRYPT_ALGO_TYPE hashAlgo = CRYPT_ALGO_SHA1;
    unsigned char digest[ CRYPT_MAX_HASHSIZE ];
    int digestLength;

    status = cryptCreateContext( &hashContext, cryptUser, hashAlgo );
    cryptAddRandom( NULL, CRYPT_RANDOM_SLOWPOLL );
    cryptEncrypt( hashContext, message, length );
    cryptEncrypt( hashContext, message, 0 );
    status = cryptGetAttributeString( hashContext, CRYPT_CTXINFO_HASHVALUE, digest, &digestLength );
    printf("Message: %s\n", message);
    printf("Message length: %d bytes\n", length);
    printf("SHA1 digest: ");
    hexdump(stdout, digest, 20);
    printf("\n");
    printf("Expected value: 6dabb4f4a33f91cd45eb9c4c6f6bca08967615c7\n");
    cryptDestroyContext( hashContext );
    status = cryptEnd();
}
