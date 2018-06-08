#include "hexutil.h"
#include "Teaencrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if 1
int main(int argc, char** argv){
    //HardwareUID uid;
    //uid.collect();
    //uid.print();
    int nValude = 0;
    if (argc < 2){
        printf("\n*****ERROR****expect parameters more than one*******\n");
        printf("*****ERROR****command:  security.exe [hostid]  *****\n\n");
        exit(1);
    }

    char result[1024];

    AES aes(key);

    aes.Cipher(argv[1], result);
    printf("sgcipher:%s\n",result);
    return 0;
}
#else
char intTohexChar(int x) {
    static const char HEX[16] = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    return HEX[x];
}

int main(int argc, char** argv){
    HardwareUID uid;
    uid.collect();
    uid.print();
    char result[1024] = {};

    char sour[1024] = {};
    strcpy(sour,argv[1]);

    AES aes(key);
    aes.InvCipher(sour, result);
    char str[8] = {};
    for(int i = 0; i < 8; i++){
        int b = pow(16,7-i);

       str[i] = intTohexChar((int)(uid.hostid/b%16));
    }
    printf("str:%s,result:%s\n",str,result);
    if(strcmp(str,result)==0)
        printf("%ld; %s; %s; %s\n",uid.hostid,sour,result,str);
    else
        printf("ERROR:%ld; %s; %s; %s\n",uid.hostid,sour,result,str);
}
#endif

