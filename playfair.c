#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 500

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



int main(){
	char msg[MAX];
	printf("Enter the message\n");
	gets(msg);
	int n = strlen(msg);
	char key[MAX];
	printf("Enter the Key\n");
	gets(key);
	
	char mat[5][5];
	int present[26] = {0};
	present[9] = 1;
	
	//adjusting message
	for(int i = 0; i < n; i++)
	{
		if(msg[i] == 'J')
			msg[i] = 'I';
	}
	char final_msg[MAX];
	int k = 1;
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
	if(strlen(final_msg) % 2 == 1)
		final_msg[k] = 'X';
	printf("Final message : ");
	for(int i = 0; i < strlen(final_msg); i++)
		printf("%c", final_msg[i]);
	printf("\n");
	//generating key matrix
	for(int i = 0; i < strlen(key); i++)
	{
		if(key[i] == 'J')
			key[i] = 'I';
	}
	
	int x = 0;
	int y = 0;
	for(int i = 0; i < strlen(key); i++)
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
	
	printf("Key Matrix : \n");
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			printf("%c ", mat[i][j]);
		}
		printf("\n");
	}
	char* e;
	e = encryptPlayfair(final_msg, strlen(final_msg) , mat);
	printf("Encrypted message : ");
	for(int i = 0; i < strlen(e); i++)
		printf("%c", e[i]);
	printf("\n");
	char* d;
	d = decryptPlayfair(e, strlen(e), mat);
	printf("Decrypted message : ");
	for(int i = 0; i < strlen(d); i++)
		printf("%c", d[i]);
	printf("\n");
	return 0;
}
