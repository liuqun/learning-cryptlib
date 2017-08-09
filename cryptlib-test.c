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

#define MAXLINE 200
#define cryptUser CRYPT_UNUSED

int  status;


int print_hash(unsigned char buf[], int length){

    unsigned char * pin = buf;
    char str[2*length + 1];
    const char * hex = "0123456789abcdef";
    char * pout = str;
    for(;  pin < buf+length; pout+=2, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
    }
    pout[0] = 0;
    printf("Hex      : %s \n",str);
    if (strcmp(str, "6dabb4f4a33f91cd45eb9c4c6f6bca08967615c7") != 0) {
        return 1;
    }
    return 0;
}

/***************************************************************************************/
int main (int argc, char *args[]) {

    char buffer[MAXLINE] = "cryptlib";
    char hashName[MAXLINE];
    char hashValue[MAXLINE];
    int length = strlen(buffer);

    status = cryptInit();
    puts("Cryptlib : performing basic hash test in C.\n");
    if( status != CRYPT_OK ){
        puts("CryptLib initialisation failed.");
        exit(2);
    }

    CRYPT_CONTEXT hashContext;
    unsigned char hash[ CRYPT_MAX_HASHSIZE ];
    int hashLength;
    int numBytes;

    /* CRYPT_CTXINFO_NAME_ALGO hashName; */

    status = cryptCreateContext( &hashContext, cryptUser, CRYPT_ALGO_SHA1 );
    cryptAddRandom( NULL, CRYPT_RANDOM_SLOWPOLL );
    cryptEncrypt( hashContext, buffer, length );
    cryptEncrypt( hashContext, buffer, 0 );
    status = cryptGetAttributeString( hashContext, CRYPT_CTXINFO_HASHVALUE, hash, &hashLength );
    status = cryptGetAttributeString( hashContext, CRYPT_CTXINFO_NAME_ALGO, hashName, &numBytes );
    printf("Hash algo: %s \n",hashName);
    printf("Data     : %s \n",buffer);
    printf("Hash     : %s \n",hash);
    status = print_hash(hash, 20);
    if (status)
        puts("ERROR: sha1 hash failed.");
    else
        puts("SUCCESS  sha1 hash succeeded.");
    cryptDestroyContext( hashContext );
    status = cryptEnd();
}
