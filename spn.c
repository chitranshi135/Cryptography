//CHITRANSHI SRIVASTAVA
//202051055
//CRYPTOGRAPHY ASSIGNMENT 2

#include <stdio.h>
#include <stdlib.h>
//defining data types used for ease of usage
#define  ul unsigned long
#define   ui unsigned int

//hard-coding the substitution box given
ui S[16] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
//hard-coding the permutation box given
int P[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
//here the inverse permutation is same as initial permutation, so we can reuse P for decryption

//hard-coding the inverse permutation box
ui S_inv[16] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};  


//this method takes the round number and the secret key as input
//and generates the round key for that round using binary operations 
ui roundKey(int r, ul key)
{
    //left-shifting to put the k(4r-3)th bit at LSB
	ui r_key = key << (4*r-3);
    //using bit masking to get only the first consecutive bits
	r_key = r_key & 0xFFFF;
	return r_key;
}

//this function uses bitmasking to perform substitution using substitution box
ui substitution(ui p)
{
    //masking to get first 4 bits to get first hex bit and then shifting it to move it to LSB
    //similarly we do it to extract other hex bits
	ui p1 = S[(p & 0xF000) >> 12];
	ui p2 = S[(p & 0x0F00) >> 8];
	ui p3 = S[(p & 0x00F0) >> 4];
	ui p4 = S[(p & 0x000F)];
    //assembling all subparts to generate final message
	p = (p1 << 12) | (p2 << 8) | (p3 << 4) | p4;
	return p;
}

//this function uses permutation box to generated permuted message
ui permutation(ui p)
{
	ui c = 0;
	for(int i = 0; i < 16; i++)
	{
        //we find the nth bit and shift it to LSB and then we take & with 1 to extract LSB
		ui curr = (p >> (16-P[i])) & 1;
        //we then move this bit to its correct position i
		curr = curr << (15-i);
        //we then accumulate all bits in c
		c = c | curr;
	}
	return c;
}

//this method takes the input message and secret key
//and performs the steps of encryption
//and returns the cipher text
ui encryption(ui p, ul key)
{
    //perform both substitution and permutation in first three rounds
	for(int i = 1; i <= 3; i++)
	{
		ui k = roundKey(i, key);
		ui u = p^k;
		ui v = substitution(u);
		ui w = permutation(v);
		p = w;
	}
    //perform substitution in round 4 with xor
	ui k_4 = roundKey(4, key);
	ui c = p^k_4;
	c = substitution(c);
    //perform only xor in round 5
	ui k_5 = roundKey(5, key);
	c = c^k_5;
	return c;
}

//this method uses inverse substitution box
//for the decryption process 
ui inverseSubstitution(ui c)
{
	ui p1 = S_inv[(c & 0xF000) >> 12];
	ui p2 = S_inv[(c & 0x0F00) >> 8];
	ui p3 = S_inv[(c & 0x00F0) >> 4];
	ui p4 = S_inv[(c & 0x000F)];
	c = (p1 << 12) | (p2 << 8) | (p3 << 4) | p4;
	return c;
}

//this method follows steps of encryption in reverse order
//to generate plain text from cipher text
ui decryption(ui c, ul key)
{
    ui k_5 = roundKey(5, key);
    c = c^k_5;
    c = inverseSubstitution(c);
    for(int i = 4; i >= 2; i--)
    {
        ui k = roundKey(i, key);
        ui w = c^k;
        ui v = permutation(w);
        ui u = inverseSubstitution(v);
        c = u;
    }
    ui k_1 = roundKey(1, key);
    ui p = c^k_1;
    return p;
}


int main(){
	ui msg;
    //input plain text and secret key
	printf("Enter the plain text\n");
	scanf("%x", &msg);
	printf("Enter secret key\n");
	ul key;
	scanf("%x", &key);
	//encrypt(msg, key);
	printf("PLAIN TEXT : %x\n", msg);
	printf("SECRET KEY : %x\n", key);
    //calling method for encryption
	ui c = encryption(msg, key);
	printf("ENCRYPTED MESSAGE : %x\n", c);
    //calling method for decryption
    ui p = decryption(c, key);
    printf("DECRYPTED MESSAGE : %x", p);
	return 0;
}