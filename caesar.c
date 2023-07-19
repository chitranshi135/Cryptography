#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100

char* encrypt(char* p, int n){
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
			a = (a+3) % 26 + 65;
			c[i] = (char)a;
		}
		else
		{
			a = p[i]-97;
			a = (a+3) % 26 + 97;
			c[i] = (char)a;
		}
		
	}
	return (char*) c;
}

char* decrypt(char* c, int n){
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
			a = (a-3+26) % 26 + 65;
			p[i] = (char)a;
		}
		else
		{
			a = c[i]-97;
			a = (a-3+26) % 26 + 97;
			p[i] = (char)a;
		}
		
	}
	return (char*) p;
}

int main() {
	char msg[MAX];
	printf("Enter a message \n");
	gets(msg);
	char* e;
	e = encrypt(msg, strlen(msg));
	printf("Encrypted message : ");
	for(int i = 0; i < strlen(msg); i++)
		printf("%c", e[i]);
	printf("\n");
	char* d;
	d = decrypt(e, strlen(e));
	printf("Decrypted message : ");
	for(int i = 0; i < strlen(msg); i++)
		printf("%c", d[i]);
	printf("\n");
	return 0;
}
