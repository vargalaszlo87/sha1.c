/*	
 *	sha1.c	
 *
 *		desc.:	This is a simple SHA1 hash generator in pure C
 *			(without external library). Workd with 7bit ASCII.
 *			Made for WebSocket handshake response, but you can
 *			use it in any project.
 *
 *		usage:	char *msg = "hittheroadjack";
 *			char *hash = sha1ASCII7bit(msg);
 *
 *		author:		2020 vargalaszlo.com
 *		license:	GPLv3
 *		version:	1.0
 *			
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 512

unsigned int stack[MAX_INPUT_LENGTH] = {-1};
int stackPointer = -1;

void push(int item) {
	if (stackPointer <= MAX_INPUT_LENGTH/2) {
		stackPointer++;
		*(stack + stackPointer) = item;
	}		
}

int pop() {
	int out = *(stack + stackPointer);
	*(stack + stackPointer) = -1;
	stackPointer--;
	return out;
}
    
int rotate_left(a, b) {
	return (a << b) | ((unsigned)a >> (32 - b));
}

const char * toHexString(int num) {
	char str[16] = {0};
    int i = 0; 
    if (!num)
    	return "0\0";
    while(num!=0) 
    {    
	    int temp  = num % 16;
        *(str + i) = (temp < 10) ? temp + 48 : temp + 55;
        num = num/16; 
        i++;
    }
    *(str + (i+1)) = '\0';
    return strrev(str);
}

const char * toHex(int num) {
	char str[16] = {0};
	int i;
        for (i = 7; i >= 0; i--)
        	strcat(str, toHexString(((unsigned)num >> (i * 4)) & 0x0f));		
	char *result = (char *)malloc(strlen(str)+1);
	strcpy(result,str);
	return result;
}

char * sha1ASCII7bit(char * msg) {
	if (strlen(msg)>MAX_INPUT_LENGTH)
		return 0;
	unsigned int i, blockstart, temp;
	unsigned int H[] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	unsigned int X[5] = {0};
    unsigned int W[80] = {0};
    char out[512] = {0};
    /*dev-msg-utf8*/
    int msg_len = strlen(msg);
     for (i = 0; i < msg_len - 3; i += 4) {
        push((int)*(msg + i) << 24 | (int)*(msg + (i+1)) << 16 | (int)*(msg + (i+2)) << 8 | (int)*(msg + (i+3)));    	
	 }
    switch (msg_len % 4) {
        case 0:
        	push(0x080000000);
            break;
        case 1:
        	push((int)*(msg + (msg_len - 1)) << 24 | 0x0800000);
            break;
        case 2:
        	push((int)*(msg + (msg_len - 2)) << 24 | (int)*(msg + (msg_len - 1)) << 16 | 0x08000);
            break;
        case 3:
        	push((int)*(msg + (msg_len - 3)) << 24 | (int)*(msg + (msg_len - 2)) << 16 | (int)*(msg + (msg_len - 1)) << 8 | 0x80);
            break;
    }	 
    while (((stackPointer+1) % 16) != 14) 
		push(0);
    push((unsigned)msg_len >> 29);
    push((msg_len << 3) & 0x0ffffffff);	    
    for (blockstart = 0 ; blockstart < stackPointer+1 ; blockstart += 16) {
        for (i = 0; i < 16; i++) 
			W[i] = stack[blockstart + i];
        for (i = 16; i <= 79; i++) 
			W[i] = rotate_left(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
		for (i = 0 ; i < 5 ; i++)
			X[i] = H[i];	
        for (i = 0; i <= 19; i++) {
            temp = (rotate_left(X[0], 5) + ((X[1] & X[2]) | (~X[1] & X[3])) + X[4] + W[i] + 0x5A827999) & 0x0ffffffff;
            X[4] = X[3];
            X[3] = X[2];
            X[2] = rotate_left(X[1], 30);
            X[1] = X[0];
            X[0] = temp;
        }
        for (i = 20; i <= 39; i++) {
            temp = (rotate_left(X[0], 5) + (X[1] ^ X[2] ^ X[3]) + X[4] + W[i] + 0x6ED9EBA1) & 0x0ffffffff;
            X[4] = X[3];
            X[3] = X[2];
            X[2] = rotate_left(X[1], 30);
            X[1] = X[0];
            X[0] = temp;
        }
        for (i = 40; i <= 59; i++) {
            temp = (rotate_left(X[0], 5) + ((X[1] & X[2]) | (X[1] & X[3]) | (X[2] & X[3])) + X[4] + W[i] + 0x8F1BBCDC) & 0x0ffffffff;
            X[4] = X[3];
            X[3] = X[2];
            X[2] = rotate_left(X[1], 30);
            X[1] = X[0];
            X[0] = temp;
        }
        for (i = 60; i <= 79; i++) {
            temp = (rotate_left(X[0], 5) + (X[1] ^ X[2] ^ X[3]) + X[4] + W[i] + 0xCA62C1D6) & 0x0ffffffff;
            X[4] = X[3];
            X[3] = X[2];
            X[2] = rotate_left(X[1], 30);
            X[1] = X[0];
            X[0] = temp;
        }
        for (i = 0 ; i < 5 ; i++)
        	H[i] = (H[i] + X[i]) & 0x0ffffffff;	
    } 
	i = 0;
	while (H[i]) {
		strcat(out, toHex(H[i]));
		i++;
	}
	i = 0;
	while(out[i]) {
      out[i] = tolower(out[i]);
      i++;
	}	
	char *result = (char *)malloc(strlen(out)+1);
	strcpy(result,out);
	return result;
}

int main(int argc, char *argv[]) {
	char *msg = "hittheroadjack";
	char *hash = sha1ASCII7bit(msg);
	printf ("SHA1-Convert (%s): %s",msg,hash);	
	return 0;
}
				       
