//NAME : Chitranshi Srivastava
//ROLL NO. : 202051055
//CS304 ASSIGNMENT 3 : AES'

#include <stdio.h>

//defining short names for the data types unsigned
//specifying the number of bits each can hold
#define uint16_t unsigned short int
#define uint32_t unsigned long int
#define uint64_t unsigned long long int
#define uchar8_t unsigned char

//this is the standard subbyte table that is used for aes 
//it has been made constant because it value must remain same throughout the process
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

//this is the hexadecimal representation of the primitive polynomial used in aes
//the primitive polynomial is actually x^8 + x^4 + x^3 + x + 1
//the binary equivalent of this polynomial is 100011011
//then we write its hexadecimal equivalent
const uint16_t P_const = 0x011b;


//these is the matrix specified for mix column in the question
const uchar8_t M[4][4] = {
     	{0x1, 0x4, 0x4, 0x5},
     	{0x5, 0x1, 0x4, 0x4}, 
     	{0x4, 0x5, 0x1, 0x4}, 
     	{0x4, 0x4, 0x5, 0x1}
    };
//these is the matrix specified for inverse mix column in the question
const uchar8_t Minv[4][4] = {
        {0xA5, 0x07, 0x1A, 0x73},
        {0x73, 0xA5, 0x07, 0x1A},
        {0x1A, 0x73, 0xA5, 0x07},
        {0x07, 0x1A, 0x73, 0xA5}
    };

//these are the 10 Rconstants that are fixed for AES key scheduling
const uint32_t RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

//this method multiplies an 8-bit value by x under mod G(x) = x^8 + x^4 + x^3 + x + 1
uchar8_t operate(uchar8_t s)
{
	//multiply by x and store it in a separate variable
	uchar8_t t = s << 1;
	
	//overflow in this case can happen only when the original 
	//value had the MSB(8th bit) as 1 (x^7 was present in the polynomial equivalent)
	//if x^7 was present in the original, we will get x^8 on multiplying and to get answer under mod G(x)
	//we replace x^8 with x^4 + x^3 + x + 1 and xor it with the multiplied value t
	//t will not have the x^8 bit since it can only hold 8 bits
	//so we check if the original number had x^7, we xor the multiplied 
	//value t with x^4 + x^3 + x + 1 otherwise there is no need to xor
	if(s >> 7 == 1)
	{
		t = t ^ 27;
	}
	return t;
}

//this function multiplies 8-bit values with x^n
//for this is consecutively multiplies the value by x
//and then updates the current value
//it repeats this process n times by using the above "operate" function
uchar8_t operateN(uchar8_t s, int n)
{
	for(int i = 1; i <= n; i++)
	{
	    //we multiply the current value with x and update the current string with the obtained one
	    s = operate(s);
	}
	return s;
}

//This is the modified subbyte function which takes an input 'a' and finds
//subbyte for 2*a + 1
uchar8_t SubbytesModified(uchar8_t a)
{
    uint16_t t1, t2;
	
	//We use the "operate" function above to multiply by x under G(x)
	//ans then we take xor with 1
	//then we find the subbyte of this value using the table above
	//the first 4 bits give us the row number
	//the next 4 bits give us the row number
	a = operate(a)^1;
	//extracting last 4 bits
	t1 = a & 15;
	//extracting first 4 bits
	t2 = (a >> 4);
	
	//when lsb is 0, then after multiplying with x, it remains 0
	//when lsb is 1, then after multiplying with x, it becomes x, so again lsb becomes 0
	//but when x^7 was present, then we get x^8 after multiplying with x, then
	//we xor with x^4+x^3+x+1, then we get lsb 1
	//then on xor with 1, lsb = 1, when x^7 was not present
	//and lsb = 0, when x^7 was present
	
	return Sub[t2][t1];
}


//this method is used to find the inverse subbytes for the modified AES
//We find the value in the subbyte table 
//from the row number, we get first 4 bits 
//and from the column number, we get the last 4 bits
//but that value is not the original value, it is 2*value+1
//so we form cases to retrieve the original value
uchar8_t inverseSubbytesModified(uchar8_t a)
{
	int i, j;
	uchar8_t x;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			if(Sub[i][j] == a)
			{
				x = i;
				x = (x << 4) | j;
				break;
			}
		}
	}
	//as we discussed these cases in the subbytes function
	//lsb = 1, when x^7 was not present in original
	//and lsb = 0, when x^7 was present in original
	
	x = x^1;
	
	//after xoring
	//Now, lsb = 0, when x^7 was not present in original
	//and lsb = 1, when x^7 was present in original
	//if x^7 was present in original, we must have 
	//taken xor with x^4+x^3+x+1 to get the value under mod G(x)
	//In that case, to retrieve original, we must take xor with it again
	//and since on right shifting, 8th bit would become zero
	//but in the case when x^7 was present, it would have been 1
	//we make the 8th bit 1 to get the original value
	if((x & 1) == 1)
	{
	    x = x^27;
	    x = x>>1;
	    x = x^(1 << 7);
	}
	else
	{
	    x = x>>1;
	}
	return x;
}

//This function performs the shift row operation
void shiftRow(uchar8_t x[4][4])
{
    //we shift = row number
    //since for row 0, shift = 0, means we dont need to shift
    //so we start from row 1
	for(int i = 1; i < 4; i++){
        uchar8_t row[4];
        
        //we first make a copy of the row we want to shift so that we can access the actual row
        //while creating the shifted row(left shift)
        for(int j = 0; j < 4; j++) 
            row[j] = x[i][j];
        
        //we can get the correct value of each index by using (index+shift)%4
        //as every index will store the element present shift positions ahead of it circularly
        //Example : when shift = 2, value at 3rd index will come at 1st index ; 
        //value 4th index will come at 2nd index and so on
        for(int j = 0; j < 4; j++){
            x[i][j] = row[(j + i) % 4];
        }
    }
}

//this method gives us the inverse shift row
//since we shifted left during shift rows
//here we need to shift right in the same way 
//to get the original matrix
void inverseShiftRow(uchar8_t x[4][4])
{
	for(int i = 1; i < 4; i++){
        uchar8_t row[4];
        
        //we first make a copy of the row we want to shift so that we can access the actual row
        //while creating the shifted row(right shift)
        for(int j = 0; j < 4; j++)
            row[j] = x[i][j];
            
        //we can get the correct value of each index by using (index-shift+4)%4
        //as every index will store the element present shift positions before of it circularly
        //Example : when shift = 2,value at 1st index will come at 3rd index ; 
        //value at 2nd index will come at 4th index ; value at 3rd index will come at 1st index and so on
        for(int j = 0; j < 4; j++){
            x[i][j] = row[(j - i + 4) % 4];
        }
    }
}

//***********************************************************************
//***********************************************************************
/* These three methods - deg, product, longDivision were created by me to
implement the mixColumn and inverseMixColumn from the basic approach of
matrix multiplication under mod x^8+x^4+x^3+x+1
But I found more efficient ways to implement mixColumn and inverseMixColumn
using some observations that reduced the time complexity of my code
So I have not used these functions here. But if we want, we can implement all 
the operations using these also
*/


//this method finds the degree of an 16-bit polynomial
uint16_t deg(uint16_t m)
{
	uint16_t deg = 0;
	//we start from the lsb and go upto the MSB
	//whenever we find a bit as 1, we update the degree
	//and since we move towards the msb 
	//the degree finally has the highest position where the bit was 1
	for(int i = 0; i < 16; i++)
	{
		if(m & 1 == 1)
			deg = i;
		m = m >> 1;
	}
	return deg;
}

//this method finds the product of two 16-bit polynomials
uint16_t product(uint16_t a, uint16_t b)
{
	uint16_t ans = 0;
	//It multiplies b by a
	//We start from the lsb of a and go till its msb 
	//for every time a's bit is 1, we shift the value of b by the 
	//bit's position and xor it to the ans
	for(int i = 0; i < 16; i++)
	{
		if(((a >> i) & 1) == 1)
		{
			ans = ans ^ (b << i);
		}
	}
	return ans;
}

//this method finds the remainder and quotient when 
//polynomial dividend is divided by polynomial divisor
void longDivision(uint16_t divisor, uint16_t dividend, uint16_t res[])
{
	//res[1] has remainder
	//res[0] has quotient
	//d1 degree of dividend
	//d2 degree of divisor
    uint16_t d1 = deg(dividend);
	uint16_t d2 = deg(divisor);
	//division is only possible when dividend >= divisor
	//so we compare their degrees
	while(d1 >= d2)
	{
	    //the quotient for one division will be a polynomial
	    //which is formed by the difference between their MSBs, i.e, their degrees
		uint16_t temp = (1 << (d1 - d2));
		//we then take OR of all the intermediate quotients to
		//get the final quotient
		res[0] = res[0] | temp;
		
		//then we get the remaining dividend by taking XOR 
		//of the current dividend and the polynomial formed
		//by multiplying divisor by the intermediate quotient for this step
		dividend = dividend ^ (divisor << (d1-d2));
		//we also find the degree of new dividend
		d1 = deg(dividend);
		
		//if the dividend was completely divisible in this step, we stop
		if(dividend == 0)
			break; 
	}
	res[1] = dividend;
	
}

//***********************************************************************
//***********************************************************************


//This method performs the mix column operation 
void mixColumn(uchar8_t x[4][4])
{
    
    //We use the values 1 4 4 5 circularly for each element
    //means the first entry has to be multiplied by 1 
    //the second entry has to be multiplied by x^2
    //the third entry has to be multiplied by x^2
    //the fourth entry has to be multiplied by (x^2 + 1)
    // for (x^2 + 1), we multiply the entry by x^2 and then xor it with the actual entry
    //to get the combined product for (x^2 + 1)
	for(int c = 0; c < 4; c++)
	{
	    //For each column, we create a temporary column to store the final values for each entry
        //of the column so that the original column does not change mid operation otherwise the result 
        //will be wrong because for each calculation, we need the original column values
		uchar8_t temp[4];
		for(int r = 0; r < 4; r++)
		{
			temp[r] = x[r][c] ^ operateN(x[(r+1)%4][c], 2) ^  operateN(x[(r+2)%4][c],2) ^  operateN(x[(r+3)%4][c], 2) ^ x[(r+3)%4][c];
			
		}
		
		//after we have calculated the mixColumn for column c, we store it in the original matrix
		//because each column is independ of each other
		for(int r = 0; r < 4; r++)
		{
			x[r][c] = temp[r];
		}
	}
}

//this method performs the inverse mix column operation
void inverseMixColumn(uchar8_t x[4][4])
{
    //We use the values 165 7 26 115 circularly for each element
    //means the first entry has to be multiplied by (x^7 + x^5 + x^2 + 1)
    //the second entry has to be multiplied by (x^2 + x + 1)
    //the third entry has to be multiplied by (x^4 + x^3 + x)
    //the fourth entry has to be multiplied by (x^6 + x^5 + x^4 + x + 1)
    //for each multiplication with polynomial, we multiply the entry with each power of x 
    //and then take the xor of all those to get the complete product
	for(int c = 0; c < 4; c++)
	{
	    //For each column, we create a temporary column to store the final values for each entry
        //of the column so that the original column does not change mid operation otherwise the result 
        //will be wrong because for each calculation, we need the original column values
		uchar8_t temp[4];
		for(int r = 0; r < 4; r++)
		{
		    
		    uchar8_t p1 = operateN(x[(r)%4][c], 7) ^ operateN(x[(r)%4][c], 5) ^ operateN(x[(r)%4][c], 2) ^ x[(r)%4][c];
		    uchar8_t p2 = operateN(x[(r+1)%4][c], 2) ^ operateN(x[(r+1)%4][c], 1) ^ x[(r+1)%4][c]; 
		    uchar8_t p3 = operateN(x[(r+2)%4][c], 4) ^ operateN(x[(r+2)%4][c], 3) ^ operateN(x[(r+2)%4][c], 1);
		    uchar8_t p4 = operateN(x[(r+3)%4][c], 6) ^ operateN(x[(r+3)%4][c], 5) ^ operateN(x[(r+3)%4][c], 4) ^ operateN(x[(r+3)%4][c], 1) ^ x[(r+3)%4][c];
			temp[r] = p1 ^ p2 ^ p3 ^ p4;
			
		}
		
		//after we have calculated the mixColumn for column c, we store it in the original matrix
		//because each column is independ of each other
		for(int r = 0; r < 4; r++)
		{
			x[r][c] = temp[r];
		}
	}
}

//this method left rotates the 32 bit word by 8 bits
uint32_t rotword(uint32_t B)
{
    //we left shift by 8 to get the last 24 bits and then
    //OR it with the first 8 bits that we get by right shifting by 24
    //we OR in a way such that the last 24 bits come first in the rotated word
	uint32_t res = (B << 8) | ((B >> 24) & 0xFF);
	return res;
}

//This method finds the subbyte for the given input "a"
uchar8_t SubbytesOriginal(uchar8_t a)
{
	uint16_t t1, t2;
	//then we find the subbyte of this value using the table above
	//the first 4 bits give us the row number
	//the next 4 bits give us the row number
	t1 = (a & 15);
	t2 = (a >> 4);
	return Sub[t2][t1];
}

//This method finds the inverse subbyte for the given input "a"
uchar8_t inverseSubbytesOriginal(uchar8_t a)
{
	int i, j;
	uchar8_t x;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
		    //We find the value in the subbyte table 
            //from the row number, we get first 4 bits 
            //and from the column number, we get the last 4 bits
			if(Sub[i][j] == a)
			{
				x = (i << 4) | j;
				break;
			}
		}
	}
	return x;
}

//this method finds the subbyte of the 32-bit word
uint32_t subword(uint32_t B)
{
	//subbyte of last 8 bits
	uchar8_t b4 = SubbytesOriginal(B & 0xFF);
	//subbyte of second-last 8 bits
	uchar8_t b3 = SubbytesOriginal((B >> 8) & 0xFF);
	//subbyte of second set of 8 bits
	uchar8_t b2 = SubbytesOriginal((B >> 16) & 0xFF);
	//subbyte of first 8 bits
	uchar8_t b1 = SubbytesOriginal((B >> 24) & 0xFF);
	
	//we simply combine all these bits in position
	B = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
	return B;
}

//this method takes the secret key and generates the 11 round keys
void generateKeys(uchar8_t K[16], uchar8_t roundKey[11][4][4])
{
	uint32_t word[44];
	//we create the first 4 words by combining 4 bytes of secret key in sequence
	for(int i = 0; i <= 3; i++)
	{
		word[i]= (K[4*i]<<24) | (K[4*i+1]<<16) | (K[4*i+2]<<8) | K[4*i+3];
	}
	
	//we then create the remaining words following the algorithm 
	for(int i = 4; i < 44; i++)
	{
		uint32_t temp = word[i-1];
		if(i % 4 == 0)
		{
		    //here we use RCon[i/4-1] because the Rcon array is zero-indexed
		    //so if we need RCon[4], you need to get the RCon[3] from the array
			temp = subword(rotword(temp)) ^ RCon[i/4-1];
		}
		word[i] = word[i-4] ^ temp;
	}
	
	
	//then we combine 4 words in sequence to generate each round key 
	//and we store the round keys in a matrix, where each round key is stored column wise
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

//this method performs the round function for AES 
void roundFunction(int r, uchar8_t c[4][4])
{
    //We first need to find the subbytes for each byte in the matrix
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			c[i][j] = SubbytesModified(c[i][j]);
		}
	}
	//then we perform the shift rows
	shiftRow(c);
	//for the first 9 rounds, we have the mixColumns also
	//but for the 10th round, we only have subbytes and shift row
	if(r < 10)
		mixColumn(c);
}

//this method performs the round function for AES  decryption
void inverseRoundFunction(int r, uchar8_t p[4][4])
{
    //for the first 9 rounds, we have the mixColumns also
	//but for the 10th round, we only have subbytes and shift row
	//so we perform inverse mix columns for all rounds < 10
	if(r < 10)
		inverseMixColumn(p);
		
	//then the inverse shift row
	inverseShiftRow(p);
	
	//and then finally inverse subbytes
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			p[i][j] = inverseSubbytesModified(p[i][j]);
		}
	}
}

//this method takes the plain text and the secret key
//and gives us the cipher text after performing all rounds of AES
void encrypt(uchar8_t p[4][4], uchar8_t k[16], uchar8_t c[4][4])
{
    //we first generate all the round keys using the generateKeys method
	uchar8_t roundKey[11][4][4];
	generateKeys(k, roundKey);
	
	for(int r = 0; r < 11; r++)
	{
		for(int i = 0; i < 4; i++)
		{
		    //for each round we take the xor of the message with the round key progressively
			for(int j = 0; j < 4; j++)
			{
				p[i][j] = p[i][j]^roundKey[r][i][j];
			}
		}
		//for all ten rounds, we call the round function
		//but in the last step we only take xor with 11th round key
		if(r < 10)
		    roundFunction(r+1, p);
	}
	//we store the final cipher text 
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			c[i][j] = p[i][j];
		}
	}
}

//this method takes the cipher text and the secret key
//and gives us the plain text after reversing all rounds of AES
void decrypt(uchar8_t c[4][4], uchar8_t k[16], uchar8_t p[4][4])
{
    //we first generate all the round keys using the generateKeys method
	uchar8_t roundKey[11][4][4];
	generateKeys(k, roundKey);
	
	for(int r = 10; r >= 0; r--)
	{
		for(int i = 0; i < 4; i++)
		{
		    //for each round we take the xor of the message with the round key progressively
			for(int j = 0; j < 4; j++)
			{
				c[j][i] = c[j][i]^roundKey[r][j][i];
			}
		}
		//for all ten rounds in reverse order, we call the inverseRoundFunction
		//and in the last step we just take xor
		if(r > 0)
		    inverseRoundFunction(r, c);
	}
	
	//we store the final plain text 
	for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            p[i][j] = c[i][j];
        }
    }
	
}

//this method takes a matrix of bytes as input
//and prints each byte in column-wise order with spaces
void printText(uchar8_t x[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%02hhx ", x[j][i]);
        }
    }
    printf("\n");
}

//this method prints the secret key
//with each byte separated by space
void printKey(uchar8_t K[16])
{
    for(int i = 0; i < 16; i++)
    {
        printf("%02hhx ", K[i]);
    }
    printf("\n");
}

//this method calls the encryption and decryption methods and prints the results
void execute(uchar8_t S[4][4], uchar8_t K[16])
{
    printf("Original message  : ");
    printText(S);
    
    printf("Secret Key        : ");
    printKey(K);
    
    uchar8_t c[4][4];
    encrypt(S, K, c);
    
    printf("Encrypted message : ");
    printText(c);
    
    
    uchar8_t p[4][4];
    decrypt(c, K, p);
    printf("Decrypted message : ");
    printText(p);
}


//this is the main method which takes the input of the message and secret key
int main()
{
    //option list
    printf("*******************This a modified version of AES********************\n");
    printf("Enter 1 for sample test case 1\n");
    printf("Enter 2 for sample test case 2\n");
    printf("Enter 3 for sample test case 3\n");
    printf("Enter 4 for sample test case 4\n");
    printf("Enter 5 for custom test case 4\n");
    int choice;
    scanf("%d", &choice);
    
    
    if(choice == 1)
    {
        uchar8_t S[4][4] = {
                                {0x01, 0x89, 0xfe, 0x76},
                                {0x23, 0xab, 0xdc, 0x54},
                                {0x45, 0xcd, 0xba, 0x32},
                                {0x67, 0xef, 0x98, 0x10}
                     	    };
 	    uchar8_t K[16] =  {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
        
        execute(S, K);
 	     
    }
    
    else if(choice == 2)
    {
        uchar8_t S[4][4] = {
                                {0x0f, 0x47, 0x8e, 0xc5},
                                {0x2b, 0x63, 0xaa, 0xe1},
                                {0x47, 0x7f, 0xb6, 0xed},
                                {0x63, 0x9b, 0xd2, 0x19}
                            };
        
        uchar8_t K[16] = {0x62, 0x8c, 0xfd, 0xb2, 0x8e, 0x6e, 0x31, 0x8d, 0x35, 0x83, 0xd6, 0x25, 0x3f, 0xc4, 0xd4, 0x4a};
        
        execute(S, K);
    }
    
    else if(choice == 3)
    {
        uchar8_t S[4][4] = {
                                {0x15, 0x41, 0x5b, 0x5d},
                                {0xea, 0x32, 0x47, 0x15},
                                {0x1e, 0x24, 0x75, 0x84},
                                {0x2d, 0xad, 0xb7, 0x5f}
                            };
        
        uchar8_t K[16] =  {0x62, 0x8c, 0xfd, 0xb2, 0x8e, 0x6e, 0x31, 0x8d, 0x35, 0x83, 0xd6, 0x25, 0x3f, 0xc4, 0xd4, 0x4a};
        
        execute(S, K);
    }
    
    else if(choice == 4)
    {
        uchar8_t S[4][4] = {
                                {0x00, 0x11, 0x22, 0x33},
                                {0x44, 0x55, 0x66, 0x77},
                                {0x88, 0x99, 0xaa, 0xbb},
                                {0xcc, 0xdd, 0xee, 0xff}
                            };
        
        uchar8_t K[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
        
        execute(S, K);
    }
    else if(choice == 5)
    {
        uchar8_t S[4][4];
        uchar8_t K[16];
        //taking user input
        printf("Please enter the inputs in pairs of hexadecimal digits separated by single space like a1 12 ...ca 45 ec");
        //storing plain text input column-wise
        printf("Enter plain text\n");
        
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                scanf("%02hhx", &S[j][i]);
            }
        }
        //storing secret key in 1-D array
        printf("Enter secret key\n");
        
        for(int i = 0; i < 16; i++)
        {
            scanf("%02hhx", &K[i]);
        }
        
        execute(S, K);
    }
    
	return 0;
}
