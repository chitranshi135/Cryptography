//NAME : Chitranshi Srivastava
//ROLL NO. : 202051055

#include <stdio.h>

#define uint16_t unsigned short int
#define uint32_t unsigned long int
#define uint64_t unsigned long long int
#define uchar8_t unsigned char


const uchar8_t Sub[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

const uint16_t P_const = 0x011b;

const uchar8_t M[4][4] = {{0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02}};

const uint32_t RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};


uchar8_t operate(uchar8_t s)
{
	//multiply by x
	uchar8_t t = s << 1;
	
	if(s >> 7 == 1)
	{
		t = t ^ 27;
	}
	return t;
}


uchar8_t Subbytes(uchar8_t a)
{
	uint16_t t1, t2;
	t1 = (a & 15);
	t2 = (a >> 4);
	return Sub[t2][t1];
}

uchar8_t inverseSubbytes(uchar8_t a)
{
	int i, j;
	uchar8_t x;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			if(Sub[i][j] == a)
			{
				x = (i << 4) | j;
				break;
			}
		}
	}
	return x;
}

void shiftRow(uchar8_t x[4][4])
{
	for(int i = 1; i < 4; i++){
        uchar8_t row[4];
        for(int j = 0; j < 4; j++) 
            row[j] = x[i][j];
            
        for(int j = 0; j < 4; j++){
            x[i][j] = row[(j + i) % 4];
        }
    }
}

void inverseShiftRow(uchar8_t x[4][4])
{
	for(int i = 1; i < 4; i++){
        uchar8_t row[4];
        for(int j = 0; j < 4; j++)
            row[j] = x[i][j];
        for(int j = 0; j < 4; j++){
            x[i][j] = row[(j - i + 4) % 4];
        }
    }
}

uint16_t deg(uint16_t m)
{
	uint16_t deg = 0;
	for(int i = 0; i < 16; i++)
	{
		if(m & 1 == 1)
			deg = i;
		m = m >> 1;
	}
	return deg;
}

uint16_t product(uint16_t a, uint16_t b)
{
	uint16_t ans = 0;
	for(int i = 0; i < 16; i++)
	{
		if(((a >> i) & 1) == 1)
		{
			ans = ans ^ (b << i);
		}
	}
	return ans;
}

void longDivision(uint16_t a, uint16_t b, uint16_t res[])
{
	//a = divisor
	//b = dividend
	//res[1] has remainder
	//res[0] has quotient
	//d1 degree of dividend
	//d2 degree of divisor
    uint16_t d1 = deg(b);
	uint16_t d2 = deg(a);
	while(d1 >= d2)
	{
		uint16_t temp = (1 << (d1 - d2));
		res[0] = res[0] | temp;
		b = b ^ (a << (d1-d2));
		d1 = deg(b);
		
		if(b == 0)
			break; 
	}
	res[1] = b;
	
}


void mixColumn(uchar8_t x[4][4])
{
	uchar8_t u[4];
	for(int c = 0; c < 4; c++)
	{
		for(int j = 0; j < 4; j++)
		{
			uint16_t val = 0;
			for(int i = 0; i < 4; i++)
			{
				val = val ^ product(M[j][i], x[i][c]);
			}
			uint16_t res[2];
			longDivision(P_const, val, res);
			u[j] = res[1] & 0xFF;
		}
		for(int j = 0; j < 4; j++)
		{
			x[j][c] = u[j];
		}
	}
}

void inverseMixColumn(uchar8_t x[4][4])
{
	mixColumn(x);
	mixColumn(x);
	mixColumn(x);
}

uint32_t rotword(uint32_t B)
{
	uint32_t res = (B << 8) | ((B >> 24) & 0xFF);
	return res;
}

uint32_t subword(uint32_t B)
{
	
	uchar8_t b4 = Subbytes(B & 0xFF);
	uchar8_t b3 = Subbytes((B >> 8) & 0xFF);
	uchar8_t b2 = Subbytes((B >> 16) & 0xFF);
	uchar8_t b1 = Subbytes((B >> 24) & 0xFF);
	
	B = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
	return B;
}

void generateKeys(uchar8_t K[16], uchar8_t roundKey[11][4][4])
{
	uint32_t word[44];
	for(int i = 0; i <= 3; i++)
	{
		word[i]= (K[4*i]<<24) | (K[4*i+1]<<16) | (K[4*i+2]<<8) | K[4*i+3];
	}
	
	for(int i = 4; i < 44; i++)
	{
		uint32_t temp = word[i-1];
		if(i % 4 == 0)
		{
			temp = subword(rotword(temp)) ^ RCon[i/4-1];
		}
		word[i] = word[i-4] ^ temp;
	}
	
	for(int i = 0; i < 11; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    int shift = 24;
		    for(int k = 0; k < 4; k++)
		    {
			    roundKey[i][k][j] = (word[4*i+j] >> shift) & 0xFF; 
			    shift = shift-8;
		    }
		}
	}
}

void roundFunction(int r, uchar8_t c[4][4])
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			c[j][i] = Subbytes(c[j][i]);
		}
	}
	shiftRow(c);
	if(r < 10)
		mixColumn(c);
}

void inverseRoundFunction(int r, uchar8_t p[4][4])
{
	if(r < 10)
		inverseMixColumn(p);
		
	inverseShiftRow(p);
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			p[j][i] = inverseSubbytes(p[j][i]);
		}
	}
}

void encrypt(uchar8_t p[4][4], uchar8_t k[16], uchar8_t c[4][4])
{
	uchar8_t roundKey[11][4][4];
	generateKeys(k, roundKey);
	
	for(int r = 0; r < 11; r++)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				p[j][i] = p[j][i]^roundKey[r][j][i];
			}
		}
		if(r < 10)
		    roundFunction(r+1, p);
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			c[j][i] = p[j][i];
		}
	}
}

void decrypt(uchar8_t c[4][4], uchar8_t k[16], uchar8_t p[4][4])
{
	uchar8_t roundKey[11][4][4];
	generateKeys(k, roundKey);
	
	for(int r = 10; r >= 0; r--)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				c[j][i] = c[j][i]^roundKey[r][j][i];
			}
		}
		if(r > 0)
		    inverseRoundFunction(r, c);
	}
	
	for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            p[j][i] = c[j][i];
        }
    }
	
}

void print(uchar8_t x[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%02x ", x[j][i]);
        }
    }
}

int main()
{
// 	printf("Enter plain text\n");
 	uchar8_t S[4][4] =  {
        {0x01, 0x89, 0xfe, 0x76},
        {0x23, 0xab, 0xdc, 0x54},
        {0x45, 0xcd, 0xba, 0x32},
        {0x67, 0xef, 0x98, 0x10}
 	    
 	};

	
    // printf("Enter secret key\n");
    uchar8_t K[16] =  {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59,
                      0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
    
    
    
    
    uchar8_t c[4][4];
    encrypt(S, K, c);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%02x ", c[j][i]);
        }
    }
    
    printf("\n");
    uchar8_t p[4][4];
    decrypt(c, K, p);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%02x ", p[j][i]);
        }
    }
    
    
	return 0;
}


