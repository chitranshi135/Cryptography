//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//CHITRANSHI SRIVASTAVA
//202051055
//CS304 Lab Assignment 1
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

#include<stdio.h>
#include<stdlib.h>
#define MAX 500

//######################################################################################
//this method finds the length of the string inputted
//######################################################################################
int length(char* s)
{
	int i = 0;
	while(s[i] != '\0')
		i++;
	return i;
}

//######################################################################################
//This method performs encryption using shift cipher with given key
//Here p contains the plain text which is shifted by key 
//and we take modulo with 26 to get valid alphabetic characters in cipher text c
//######################################################################################
char* encryptShift(char* p, int n, int key){
	char* c = (char*)malloc(n*1);
	for(int i = 0; i < n; i++)
	{
		if(p[i] == ' '){
			c[i] = ' ';
			continue;
		}
		int a;
		if(p[i] >= 65 && p[i] <= 90)
		{
			a = p[i]-65;
			a = (a+key) % 26 + 65;
			c[i] = (char)a;
		}
		else
		{
			a = p[i]-97;
			a = (a+key) % 26 + 97;
			c[i] = (char)a;
		}
		
	}
	return (char*) c;
}

//######################################################################################
//This method performs decryption using shift cipher with given key
//Here c contains the cipher text 
//and we take modulo with 26 to get valid alphabetic characters in cipher text c
//######################################################################################
char* decryptShift(char* c, int n, int key){
	char* p = (char*)malloc(n*1);
	for(int i = 0; i < n; i++)
	{
		if(c[i] == ' '){
			p[i] = ' ';
			continue;
		}
		int a;
		if(c[i] >= 65 && c[i] <= 90)
		{
			a = c[i]-65;
			a = (a-key+26) % 26 + 65;
			p[i] = (char)a;
		}
		else
		{
			a = c[i]-97;
			a = (a-key+26) % 26 + 97;
			p[i] = (char)a;
		}
		
	}
	return (char*) p;
}

//######################################################################################
//In this method we use the extended euclidean algorithm to find the gcd and inverse
//But in this method, the inverse returned can be negative also and it is stored in variable p
//######################################################################################

/**
 * The function implements the extended Euclidean algorithm to find the greatest common divisor and the
 * coefficients of Bezout's identity for two integers.
 * 
 * @param a The first integer input to the extended Euclidean algorithm.
 * @param b The parameter "b" in the function "extendedEuclidean" represents one of the two integers
 * for which we want to find the greatest common divisor (GCD) and the Bezout coefficients.
 * @param p The pointer to an integer variable where the function will store the value of the
 * coefficient of 'a' in the Bezout's identity for the given inputs 'a' and 'b'.
 * @param q The parameter "q" in the function "extendedEuclidean" is a pointer to an integer variable
 * that will store one of the Bezout coefficients computed by the algorithm. Specifically, it will
 * store the Bezout coefficient corresponding to the second input parameter "b".
 * 
 * @return the greatest common divisor (GCD) of the two input integers 'a' and 'b'.
 */
int extendedEuclidean(int a, int b, int* p, int* q){
    if(a == 0){
        *p = 0;
        *q = 1;
        return b;
    }

    int p1, q1;
    int gcd = extendedEuclidean(b % a, a, &p1, &q1);

    *p = q1 - (b/a) * p1;
    *q = p1;

    return gcd;
}

//######################################################################################
//This method uses the affine cipher technique to encrypt where p has the plain text,
//n is the length of p and a, b are the values used in affine cipher respectively
//######################################################################################
char* encryptAffine(char* p, int n, int a, int b){
	char* c = (char*)malloc(n*1);
	for(int i = 0; i < n; i++)
	{
		if(p[i] == ' '){
			c[i] = ' ';
			continue;
		}
		int v;
		if(p[i] >= 65 && p[i] <= 90)
		{
			v = p[i]-65;
			v = (v*a+b) % 26 + 65;
			c[i] = (char)v;
		}
		else
		{
			v = p[i]-97;
			v = (v*a+b) % 26 + 97;
			c[i] = (char)v;
		}
		
	}
	return (char*) c;
}

//######################################################################################
//This method performs decryption of affine cipher. Here we use the extended euclidean 
//algorithm to find the multiplicative inverse of a under modulo m(here m = 26).
//The inverse can be negative, so we add m(here m = 26) to make it positive.
//######################################################################################
char* decryptAffine(char* c, int n, int a, int b){
	
    char* p = (char*)malloc(n*1);
    int x, y;
    int gcd = extendedEuclidean(a, 26, &x, &y);
    if(gcd != 1)
    	printf("Not a valid set of keys\n");
    int inv = x;
    if(inv < 0)
    	inv += 26;
    
	for(int i = 0; i < n; i++)
	{
		if(c[i] == ' '){
			p[i] = ' ';
			continue;
		}
		int v;
		if(c[i] >= 65 && c[i] <= 90)
		{
			v = c[i]-65;
			v = ((v-b+26)*inv)%26 + 65;
			p[i] = (char)v;
		}
		else
		{
			v = c[i]-97;
			v = ((v-b+26)*inv)%26 + 97;
			p[i] = (char)v;
		}
		
	}
	return (char*) p;
}

//######################################################################################
//This method is used in Playfair Cipher to find the row and column position of a 
//character in the key matrix. It returns the values in the form of an array
//where 0th index gives row number and 1st index gives column number
//######################################################################################
int* getPos(char mat[5][5], char ch)
{
	int* pos = (int*)malloc(2*4);
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(mat[i][j] == ch)
			{
				pos[0] = i;
				pos[1] = j;
			}
		}
	}
	return pos;
}

//######################################################################################
//This method takes the plain text and key matrix to generate the cipher text using 
//playfair technique. First the plain text is divided in pairs, if both are present in 
//same column, then we take elements just below them; if in same row, then elements on 
//their right; if none of the above, then we make the rectange and take horizontally
//opposite characters of matrix
//######################################################################################
char* encryptPlayfair(char* p, int n, char mat[5][5]){
	char* c = (char*)malloc(n*1);
	for(int i = 0; i < n; i += 2)
	{
		char ch1 = p[i];
		char ch2 = p[i+1];
		int* pos1 = getPos(mat, ch1);
		int* pos2 = getPos(mat, ch2);
		
		if(pos1[1] == pos2[1])
		{
			c[i] = mat[(pos1[0]+1)%5][pos1[1]];
			c[i+1] = mat[(pos2[0]+1)%5][pos1[1]];
		}
		else if(pos1[0] == pos2[0])
		{	
			c[i] = mat[pos1[0]][(pos1[1]+1)%5];
			c[i+1] = mat[pos1[0]][(pos2[1]+1)%5];
		}
		else
		{
			c[i] = mat[pos1[0]][pos2[1]];
			c[i+1] = mat[pos2[0]][pos1[1]];
		}
	}
	return c;
}

//######################################################################################
//This method performs the decryption using playfair technique.First the plain text is 
//divided in pairs, if both are present in same column, 
//then we take elements just above them; if in same row, then elements on 
//their left; if none of the above, then we make the rectange and take horizontally
//opposite characters of matrix
//######################################################################################
char* decryptPlayfair(char* c, int n, char mat[5][5]){
	char* p = (char*)malloc(n*1);
	for(int i = 0; i < n; i += 2)
	{
		char ch1 = c[i];
		char ch2 = c[i+1];
		int* pos1 = getPos(mat, ch1);
		int* pos2 = getPos(mat, ch2);
		
		if(pos1[1] == pos2[1])
		{
			p[i] = mat[(pos1[0]-1+5)%5][pos1[1]];
			p[i+1] = mat[(pos2[0]-1+5)%5][pos1[1]];
		}
		else if(pos1[0] == pos2[0])
		{	
			p[i] = mat[pos1[0]][(pos1[1]-1+5)%5];
			p[i+1] = mat[pos1[0]][(pos2[1]-1+5)%5];
		}
		else
		{
			p[i] = mat[pos1[0]][pos2[1]];
			p[i+1] = mat[pos2[0]][pos1[1]];
		}
	}
	return p;
}


//######################################################################################
//The main method takes the input message and performs the operations mentioned step by step
//######################################################################################
int main(){
	char msg[MAX];
	//taking input message from user
	printf("Enter the message\n");
	gets(msg);
	int n = length(msg);
	char key[MAX];
	//taking the input key for playfair cipher
	printf("Enter the Key\n");
	gets(key);
	
	char mat[5][5];
	//this array keeps track of elements that have been inserted in key matrix
	int present[26] = {0};
	//We will replace all Js with Is so we mark J as present as J will have no role
	present[9] = 1;
	
	//adjusting message by replacing Js with Is
	for(int i = 0; i < n; i++)
	{
		if(msg[i] == 'J')
			msg[i] = 'I';
	}
	char final_msg[MAX];
	int k = 1;
	//now we generate the final message by handling duplicates and odd length
	final_msg[0] = msg[0];
	for(int i = 1; i < n; i++)
	{
		if(msg[i] == final_msg[k-1])
		{
			final_msg[k] = 'X';
			k++;
		}
		final_msg[k++] = msg[i];
	}
	final_msg[k] = '\0';
	int len = length(final_msg);
	if(len % 2 == 1)
	{
		final_msg[k] = 'X';
		len += 1;
	}
	final_msg[k+1] = '\0';
	printf("Length of %s is %d\n", final_msg, len);
	printf("######################################################################################\n");
	printf("(1) Final input message : ");
	for(int i = 0; i < len; i++)
		printf("%c", final_msg[i]);
	printf("\n");
	printf("######################################################################################\n");
	//generating key matrix
	int lenKey = length(key);
	for(int i = 0; i < lenKey; i++)
	{
		if(key[i] == 'J')
			key[i] = 'I';
	}
	
	int x = 0;
	int y = 0;
	for(int i = 0; i < lenKey; i++)
	{
		char ch = key[i];
		if(present[ch-'A'] == 0)
		{
			mat[x][y++] = ch;
			present[ch-'A'] = 1;
			if(y == 5)
			{
				y = 0;
				x++;
			}
		}
	}
	
	for(int i = 0; i < 26; i++)
	{
		if(present[i] == 0)
		{
			mat[x][y++] = (char)(i+65);
			present[i] = 1;
			if(y == 5)
			{
				y = 0;
				x++;
			}
		}
	}
	printf("######################################################################################\n");
	printf("(2) Key Matrix : \n");
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			printf("%c ", mat[i][j]);
		}
		printf("\n");
	}
	printf("######################################################################################\n");
	char* c1;
	c1 = encryptPlayfair(final_msg, len, mat);
	printf("######################################################################################\n");
	printf("(3) Cipher Text C1 : ");
	for(int i = 0; i < len; i++)
		printf("%c", c1[i]);
	printf("\n");
	printf("######################################################################################\n");
	char* c2;
	c2 = encryptAffine(c1, len, 11, 15);
	printf("######################################################################################\n");
	printf("(4) Cipher Text C2 : ");
	for(int i = 0; i < len; i++)
		printf("%c", c2[i]);
	printf("\n");
	printf("######################################################################################\n");
	
	printf("Enter Key for Shift Cipher\n");
	int keyShift;
	scanf("%d", &keyShift);
	keyShift = keyShift % 26;
	printf("Key = %d\n", keyShift);
	
	char* c3;
	c3 =  encryptShift(c2, len, keyShift);
	printf("######################################################################################\n");
	printf("(5) Cipher Text C3 : ");
	for(int i = 0; i < len; i++)
		printf("%c", c3[i]);
	printf("\n");
	printf("######################################################################################\n");
	
	printf("#########################################################################################\n");
	printf("                               Decryption Process\n");
	char* d3;
	d3 = decryptShift(c3, len, keyShift);
	printf("(6) Decryption Layer 3(Shift Cipher) : ");
	for(int i = 0; i < length(d3); i++)
		printf("%c", d3[i]);
	printf("\n");
	
	char* d2;
	d2 = decryptAffine(d3, len, 11, 15);
	printf("(7) Decryption Layer 2(Affine Cipher) : ");
	for(int i = 0; i < len; i++)
		printf("%c", d2[i]);
	printf("\n");
	
	char* d1;
	d1 = decryptPlayfair(d2, len, mat);
	printf("(8) Decryption Layer 3(Playfair Cipher) : ");
	for(int i = 0; i < len; i++)
		printf("%c", d1[i]);
	printf("\n");
	printf("#########################################################################################\n");
	return 0;
}
