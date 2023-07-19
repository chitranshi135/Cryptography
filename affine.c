#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100

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

char* decryptAffine(char* c, int n, int a, int b){
	
    char* p = (char*)malloc(n*1);
    int x, y;
    int gcd = extendedEuclidean(a, 26, &x, &y);
    int mi = x;
    
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

int main(){
	char msg[MAX];
	printf("Enter a message \n");
	gets(msg);
	char* e;
	printf("Enter the key \n");
	int a, b;
	printf("Enter a (0 to 25): ");
    scanf("%d", &a);
    printf("Enter b (0 to 25): ");
    scanf("%d", &b);
    int v1, v2;
    if(extendedEuclidean(a, 26, &v1, &v2); == 1)
    {
		e = encryptAffine(msg, strlen(msg), a, b);
		printf("Encrypted message : ");
		for(int i = 0; i < strlen(msg); i++)
			printf("%c", e[i]);
		printf("\n");
		char* d;
		d = decryptAffine(e, strlen(e), a, b);
		printf("Decrypted message : ");
		for(int i = 0; i < strlen(msg); i++)
			printf("%c", d[i]);
		printf("\n");
	}
	else
	{
		printf("multiplicative inverse of a is not possible, do decryption will not be possible\n");
	}
	return 0;
}
