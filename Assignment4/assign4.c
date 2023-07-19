/******************************************************************************
NAME : CHITRANSHI SRIVASTAVA
ROOL NO : 202051055
ASSIGNMENT NO : 4
COURSE : CS364 

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define uint16_t unsigned short int
#define uint32_t unsigned int
#define uint64_t unsigned long long int
#define uchar8_t unsigned char
#define BLOCK_SIZE 64
#define p 173
#define a 23 
#define b 11
//they are infinity but since they act as identity elements, we use them as zeros
#define thetaX 0
#define thetaY 0

//Subbyte matrix for AES
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

//Constant primitive polynomial for AES
const uint16_t P_const = 0x011b;

//mix column matrix 
const uchar8_t M[4][4] = {{0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02}};

//R constants for key scheduling of AES
const uint32_t RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};


//K constants for SHA-256
// given by the 32 first bits of the fractional parts of the cube roots of the first 64 prime numbers
const uint32_t k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t arrX[p];
uint32_t arrY[p];
//Number of points in [1, p-1] that satisfy the curve
uint32_t c = 0;


/**
 * Finds all the points on an elliptic curve defined by the equation y^2 = x^3 + ax + b (mod p).
 *
 * @param None
 *
 * @returns None
 */
void findPoints()
{
    //we check all combinations of x and y in [0, p-1] and add all valid points to the array from Zp x Zp
    for (uint32_t x = 0; x < p; x++)
    {
        for (uint32_t y = 0; y < p; y++)
        {
            uint32_t temp1 = (y * y) % p;
            uint32_t temp2 = ((x * x * x)%p + (a * x)%p + b%p) % p;
            if (temp1 == temp2)
            {
                arrX[c] = x;
                arrY[c] = y;
                c++;
                //printf("%d %d \n", x, y);
            }
        }
    }
}

/**
 * Computes the additive inverse of a number x modulo p.
 *
 * @param x The number to compute the additive inverse of.
 *
 * @returns The additive inverse of x modulo p.
 */
uint32_t additive_inv(uint32_t x)
{
    uint32_t temp = p - x;
    return temp;
}


/**
 * The function implements the extended Euclidean algorithm to find the greatest common divisor and the
 * coefficients of Bezout's identity for two integers.
 * 
 * @param r The first integer input to the extended Euclidean algorithm.
 * @param s The parameter "s" in the function "extendedEuclidean" represents one of the two integers
 * for which we want to find the greatest common divisor (GCD) and the Bezout coefficients.
 * @param t The pointer to an integer variable where the function will store the value of the
 * coefficient of 'r' in the Bezout's identity for the given inputs 'r' and 's'.
 * @param u The parameter "u" in the function "extendedEuclidean" is a pointer to an integer variable
 * that will store one of the Bezout coefficients computed by the algorithm. Specifically, it will
 * store the Bezout coefficient corresponding to the second input parameter "s".
 * 
 * @return the greatest common divisor (GCD) of the two input integers 'a' and 'b'.
 */
int extendedEuclidean(int r, int s, int* t, int* u){
    if(r == 0){
        *t = 0;
        *u = 1;
        return b;
    }

    int p1, q1;
    int gcd = extendedEuclidean(s % r, r, &p1, &q1);

    *t = q1 - (s/r) * p1;
    *u = p1;

    return gcd;
}
// multiplcative inverse using extended euclidean algorithm
/**
 * Computes the multiplicative inverse of x modulo p using the extended Euclidean algorithm.
 *
 * @param x The integer for which the multiplicative inverse is to be computed.
 *
 * @returns The multiplicative inverse of x modulo p.
 */
uint32_t multiplicative_inv(uint32_t x)
{
    int t, u;
    int gcd = extendedEuclidean(x, p, &t, &u);
    while(t < 0)
        t = t+p;
    return (uint32_t) t;
}

/**
 * Adds two points on an elliptic curve in the form of (x1, y1) and (x2, y2) and stores the result in res.
 *
 * @param x1 The x-coordinate of the first point.
 * @param y1 The y-coordinate of the first point.
 * @param x2 The x-coordinate of the second point.
 * @param y2 The y-coordinate of the second point.
 *
 * @returns None
 */
void addPQ(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t res[2])
{
    uint32_t m, x3, y3;
    //infinity point theta acts as the identity element
    if (((x2 == thetaX) && (y2 == thetaY)) || ((x1 == thetaX) && (y1 == thetaY)))
    {
        if((x2 == thetaX) && (y2 == thetaY))
        {
            x3 = x1;
            y3 = y1;
        }
        else
        {
            x3 = x2;
            y3 = y2;
        }
    }
    else if ((x1 == x2) && (y1 == additive_inv(y2)))
    {
        x3 = thetaX;
        y3 = thetaY;
    }
    else if ((x1 == x2) && (y1 == y2))
    {
        m = ((3 * x1 * x1 + a) * multiplicative_inv(2 * y1)) % p;
        x3 = ((m * m) % p + additive_inv(x1)+ additive_inv(x2)) % p;
        y3 = (y1 + (m * (x3 + additive_inv(x1)))%p) % p;
        y3 = additive_inv(y3);
    }
    
    else
    {
        m = (((y2 + additive_inv(y1))%p) * multiplicative_inv(x2 + additive_inv(x1))) % p;
        x3 = ((m * m) % p + additive_inv(x1)+ additive_inv(x2)) % p;
        y3 = (y1 + (m * (x3 + additive_inv(x1)))%p) % p;
        y3 = additive_inv(y3);
    }
    res[0] = x3;
    res[1] = y3;
}


/**
 * The function implements the Diffie-Hellman key exchange algorithm to generate a shared secret key
 * between two parties.
 * 
 * @param nA A secret integer chosen by party A
 * @param nB The private key of the second party in the Diffie-Hellman key exchange protocol.
 * @param alphaX The x-coordinate of the base point alpha in the elliptic curve used for the
 * Diffie-Hellman key exchange.
 * @param alphaY alphaY is a uint32_t variable representing the y-coordinate of the point alpha on an
 * elliptic curve used in the Diffie-Hellman key exchange algorithm.
 * @param SK An array of two uint32_t values that will store the shared secret key between two parties.
 */
void DiffieHellman(uint32_t nA, uint32_t nB, uint32_t alphaX, uint32_t alphaY, uint32_t SK[2])
{
    uint32_t t = nA*nB;
    uint32_t tempX = alphaX;
    uint32_t tempY = alphaY;
    while(t > 0)
    {
        if(t&1)
        {
            uint32_t res[2];
            addPQ(tempX, tempY, SK[0], SK[1], res);
            SK[0] = res[0];
            SK[1] = res[1];
        }
        uint32_t res[2];
        addPQ(tempX, tempY, tempX, tempY, res);
        tempX = res[0];
        tempY = res[1];
        t = t >> 1;
    }
    if(SK[0] == 0 && SK[1] == 0)
    {
        printf("Choose different secret key");
        exit(0);
    }
}


/**
 * Performs a right rotation of a 32-bit unsigned integer by n bits.
 *
 * @param x The 32-bit unsigned integer to be rotated.
 * @param n The number of bits to rotate by.
 *
 * @returns The result of the right rotation.
 */
uint32_t RotR(uint32_t x, uint32_t n){
    return (x >> n) | (x << (32 - n));
}

/**
 * Performs a right shift operation on a 32-bit unsigned integer.
 *
 * @param x The input 32-bit unsigned integer.
 * @param n The number of bits to shift the input integer to the right.
 *
 * @returns The result of the right shift operation.
 */
uint32_t ShR(uint32_t x, uint32_t n){
    return x >> n;
}

/**
 * Computes the Ch function used in SHA-256 algorithm.
 *
 * @param x The first input to the function.
 * @param y The second input to the function.
 * @param z The third input to the function.
 *
 * @returns The output of the Ch function.
 */
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ ((~x) & z);
}

/**
 * Computes the majority function of three 32-bit integers.
 *
 * @param x The first 32-bit integer.
 * @param y The second 32-bit integer.
 * @param z The third 32-bit integer.
 *
 * @returns The result of the majority function.
 */
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ (x & z) ^ (y & z);
}

/**
 * Computes the A_Sigma0 function for a given 32-bit integer.
 *
 * The A_Sigma0 function is defined as the bitwise XOR of the right rotations of the input integer by 2, 13, and 22 bits.
 *
 * @param x The input 32-bit integer.
 *
 * @returns The output of the A_Sigma0 function as a 32-bit unsigned integer.
 */
uint32_t A_Sigma0(uint32_t x){
    return RotR(x, 2) ^ RotR(x, 13) ^ RotR(x, 22);
}

/**
 * Applies the Sigma1 function to a 32-bit unsigned integer.
 *
 * The Sigma1 function is a component of the SHA-256 hash function.
 * It applies three right rotations to the input, and then performs an XOR operation.
 *
 * @param x The input 32-bit unsigned integer.
 *
 * @returns The result of applying the Sigma1 function to the input.
 */
uint32_t A_Sigma1(uint32_t x){
    return RotR(x, 6) ^ RotR(x, 11) ^ RotR(x, 25);
}

/**
 * Computes the Sigma0 function used in SHA-256 algorithm.
 *
 * @param x The input value to the function.
 *
 * @returns The output of the Sigma0 function.
 */
uint32_t B_Sigma0(uint32_t x){
    return RotR(x, 7) ^ RotR(x, 18) ^ ShR(x, 3);
}

/**
 * Applies the Sigma1 function of the SHA-256 algorithm to a 32-bit unsigned integer.
 *
 * @param x The input 32-bit unsigned integer.
 *
 * @returns The output of the Sigma1 function applied to x.
 */
uint32_t B_Sigma1(uint32_t x){
    return RotR(x, 17) ^ RotR(x, 19) ^ ShR(x, 10);
}


/**
 * Pads the input message to a multiple of 512 bits as per SHA-1 padding rules.
 *
 * @param m The input message to be padded.
 * @param len The length of the input message in bytes.
 *
 * @returns A pointer to the padded message.
 */
uchar8_t* SHA_padding(uchar8_t *m, size_t *len){
    //count bits in message m
	uint64_t m_len = (*len) * 8;
	uint64_t zero = 0;

	while((m_len + 1 + zero + 64) % 512 != 0){
		zero++;
	}

	//create a new padded message
	size_t new_len = (m_len + zero + 1 + 64)/8;
    if(new_len % 64 != 0){
        printf("Error in padding");
        exit(0);
    }

	//copy the message
	uchar8_t *padded = (uchar8_t *)malloc(new_len*sizeof(uchar8_t));

	for (size_t i = 0; i < *len; i++)
	{
		padded[i] = m[i];
	}

	//add  1 bit
	padded[*len] = 0x80;

	//add 0 bits
	for (size_t i = *len + 1; i < new_len - 8; i++)
	{
		padded[i] = 0x00;
	}

	//add bit length to the final message 
    for(size_t i = new_len - 8; i < new_len; i++){
        padded[i] = (m_len >> (56 - 8*(i - (new_len - 8)))) & 0xFF;
    }

    //Update the length of padded message
    *len = new_len;


    // printf("Padded message\n");
    // for(size_t i = 0; i < (*len); i++)
    //     printf("%x ", padded[i]);
    // printf("\n");
    return padded;

}

/**
 * Generates 64 32-bit words for use in the SHA algorithm.
 *
 * @param m A pointer to the input message.
 * @param w A pointer to the output array of 32-bit words.
 *
 * @returns None
 */
//generate 64 32-bit words
void SHA_generate_words(uchar8_t *m, uint32_t *w){
    //generate the words for further processing
    for (size_t i = 0; i < 16; i++)
    {
        w[i] = (m[i*4] << 24) | (m[i*4 + 1] << 16) | (m[i*4 + 2] << 8) | (m[i*4 + 3]);
    }
    for (size_t i = 16; i < 64; i++)
    {
        w[i] = B_Sigma1(w[i-2]) + w[i-7] + B_Sigma0(w[i-15]) + w[i-16];
    }
}

/**
 * Computes the SHA-256 hash of a message.
 *
 * @param m A pointer to the message to be hashed.
 * @param h A pointer to an array of 8 32-bit integers representing the initial hash values.
 * @param len The length of the message in bytes.
 *
 * @returns None
 */
//hash-computation from message m
void hash(uchar8_t* m, uint32_t* h, size_t len)
{
    // printf("message\n");
    // for(size_t i = 0; i < len; i++)
    //     printf("%02x ", m[i]);
    // printf("\n");
    
    //pad the input message
    m = SHA_padding(m, &len);
    
    //Initialize hash values
    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
    
    //operating on each 512-bit block of message
    for (size_t i = 0; i < len; i += BLOCK_SIZE)
    {
        uchar8_t m_block[BLOCK_SIZE];
        for (size_t j = 0; j < BLOCK_SIZE; j++)
        {
            m_block[j] = m[i + j];
        }
        //generate 64 32-bit words
        uint32_t w[64];
        SHA_generate_words(m_block, w);
        //Initialize hash values
        uint32_t A = h[0];
        uint32_t B = h[1];
        uint32_t C = h[2];
        uint32_t D = h[3];
        uint32_t E = h[4];
        uint32_t F = h[5];
        uint32_t G = h[6];
        uint32_t H = h[7];
        uint32_t T1, T2;

        for(int t = 0; t < 64; t++)
        {
            T1 = H + A_Sigma1(E) + Ch(E, F, G) + k[t] + w[t];
            T2 = A_Sigma0(A) + Maj(A, B, C);
            H = G;
            G = F;
            F = E;
            E = D + T1;
            D = C;
            C = B;
            B = A;
            A = T1 + T2;
        }

        h[0] += A;
        h[1] += B;
        h[2] += C;
        h[3] += D;
        h[4] += E;
        h[5] += F;
        h[6] += G;
        h[7] += H;
    }
}

/**
 * Performs the Galois Field multiplication operation on a given 8-bit unsigned char.
 *
 * @param s The input 8-bit unsigned char.
 *
 * @returns The result of the Galois Field multiplication operation.
 */
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


/**
 * Applies the SubBytes operation to a single byte.
 *
 * @param s The input byte to be substituted.
 *
 * @returns The output byte after substitution.
 */
uchar8_t Subbytes(uchar8_t s)
{
	uint16_t t1, t2;
	t1 = (s & 15);
	t2 = (s >> 4);
	return Sub[t2][t1];
}


/**
 * Computes the inverse of a given byte using the Substitution Box (S-Box) of the Advanced Encryption Standard (AES).
 *
 * @param s The input byte to be inverted.
 *
 * @returns The inverted byte.
 */
uchar8_t inverseSubbytes(uchar8_t s)
{
	int i, j;
	uchar8_t x;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			if(Sub[i][j] == s)
			{
				x = (i << 4) | j;
				break;
			}
		}
	}
	return x;
}

/**
 * Shifts each row of a 4x4 matrix to the left by its row index.
 *
 * @param x A 4x4 matrix of 8-bit unsigned characters.
 *
 * @returns None
 */
void shiftRow(uchar8_t x[4][4])
{
	for(int i = 1; i < 4; i++){
        uchar8_t row[4];
        for(int j = 0; j < 4; j++) 
            row[j] = x[i][j];
            
        for(int j = 0; j < 4; j++){
            x[i][j] = row[(j + 
            
            
            
            
            i) % 4];
        }
    }
}

/**
 * Performs the inverse shift row operation on a 4x4 matrix of 8-bit unsigned characters.
 *
 * @param x A 4x4 matrix of 8-bit unsigned characters.
 *
 * @returns None
 */
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

/**
 * Mixes the columns of a 4x4 matrix using the Rijndael MixColumns operation.
 *
 * @param x The 4x4 matrix to be mixed.
 *
 * @returns None
 */
void mixColumn(uchar8_t x[4][4])
{
	for(int c = 0; c < 4; c++)
	{
		uchar8_t temp[4];
		for(int r = 0; r < 4; r++)
		{
			temp[r] = operate(x[r][c]) ^ operate(x[(r+1)%4][c]) ^ x[(r+1)%4][c] ^ x[(r+2)%4][c] ^ x[(r+3)%4][c];
			
		}
		
		for(int r = 0; r < 4; r++)
		{
			x[r][c] = temp[r];
			//printf("%d ", x[r][c]);
		}
		//printf("\n");
	}
}

/**
 * Applies the inverse mix column operation on a 4x4 matrix of bytes.
 *
 * @param x A 4x4 matrix of bytes.
 *
 * @returns None
 */
void inverseMixColumn(uchar8_t x[4][4])
{
	mixColumn(x);
	mixColumn(x);
	mixColumn(x);
}

/**
 * Rotates a 32-bit word left by 1 byte.
 *
 * @param B The 32-bit word to be rotated.
 *
 * @returns The rotated 32-bit word.
 */
uint32_t rotword(uint32_t B)
{
	uint32_t res = (B << 8) | ((B >> 24) & 0xFF);
	return res;
}

/**
 * Applies the Subbytes operation to each byte of a 32-bit input and returns the result.
 *
 * @param B The 32-bit input to be processed.
 *
 * @returns The result of applying the Subbytes operation to each byte of the input.
 */
uint32_t subword(uint32_t B)
{
	
	uchar8_t b4 = Subbytes(B & 0xFF);
	uchar8_t b3 = Subbytes((B >> 8) & 0xFF);
	uchar8_t b2 = Subbytes((B >> 16) & 0xFF);
	uchar8_t b1 = Subbytes((B >> 24) & 0xFF);
	
	B = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
	return B;
}

/**
 * Generates round keys for the AES encryption algorithm.
 *
 * @param K The 32-byte key used for encryption.
 * @param roundKey The output array of round keys.
 *
 * @returns None
 */
void generateKeys(uchar8_t K[32], uchar8_t roundKey[15][4][4])
{
	uint32_t word[60];
    //modified key generation for AES-256
    //here we need 60 words as there are 14 rounds
	for(int i = 0; i < 8; i++)
	{
		word[i]= (K[4*i]<<24) | (K[4*i+1]<<16) | (K[4*i+2]<<8) | K[4*i+3];
	}
	
	for(int i = 8; i < 60; i++)
	{
		uint32_t temp = word[i-1];
		if(i % 8 == 0)
		{
			temp = subword(rotword(temp)) ^ RCon[i/8-1];
		}
        else if(i % 8 == 4)
        {
            temp = subword(temp);
        }
		word[i] = word[i-8] ^ temp;
	}
	
	for(int i = 0; i < 15; i++)
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

/**
 * Performs one round of the AES encryption algorithm.
 *
 * @param r The current round number.
 * @param c The 4x4 matrix of bytes to be processed.
 *
 * @returns None
 */
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
	if(r < 14)
		mixColumn(c);
}

/**
 * Performs the inverse round function of the AES encryption algorithm.
 *
 * @param r The round number.
 * @param P The 4x4 matrix of bytes representing the state of the AES algorithm.
 *
 * @returns None
 */
void inverseRoundFunction(int r, uchar8_t P[4][4])
{
	if(r < 14)
		inverseMixColumn(P);
		
	inverseShiftRow(P);
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			P[j][i] = inverseSubbytes(P[j][i]);
		}
	}
}

/**
 * The function encrypts a 128-bit plaintext using a 256-bit key and an initialization vector (IV) by
 * applying the AES-256 algorithm with 15 rounds of encryption.
 * 
 * @param P a 4x4 matrix representing the plaintext to be encrypted
 * @param roundKey a 3-dimensional array of size 15x4x4 containing the round keys used in the
 * encryption process.
 * @param c The output ciphertext matrix of size 4x4.
 * @param iv The initialization vector (IV) is a fixed-size input to the encryption algorithm that is
 * typically used to ensure that the same plaintext does not encrypt to the same ciphertext when
 * encrypted multiple times. It is a 4x4 matrix of bytes (uchar8_t).
 */
void encrypt(uchar8_t P[4][4], uchar8_t roundKey[15][4][4], uchar8_t c[4][4], uchar8_t iv[4][4])
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            c[j][i] = P[j][i];
        }
    }
    for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			c[j][i] = c[j][i]^iv[j][i];
		}
	}
	for(int r = 0; r < 15; r++)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				c[j][i] = c[j][i]^roundKey[r][j][i];
			}
		}
		if(r < 14)
		    roundFunction(r+1, c);
	}
}

/**
 * The function decrypts a 128-bit ciphertext using the AES algorithm with a given round key and
 * initialization vector.
 * 
 * @param c a 4x4 matrix representing the ciphertext to be decrypted
 * @param roundKey a 3D array of size 15x4x4 containing the round keys used in the decryption process.
 * @param P a 4x4 matrix representing the plaintext message
 * @param iv The initialization vector (IV) is a fixed-size input to the encryption algorithm that is
 * typically used to ensure that the same plaintext message does not encrypt to the same ciphertext
 * message when encrypted multiple times with the same key. In this function, the IV is used in the
 * final step of the decryption process to
 */
void decrypt(uchar8_t c[4][4], uchar8_t roundKey[15][4][4], uchar8_t P[4][4], uchar8_t iv[4][4])
{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            P[j][i] = c[j][i];
        }
    }

	for(int r = 14; r >= 0; r--)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				P[j][i] = P[j][i]^roundKey[r][j][i];
			}
		}
		if(r > 0)
		    inverseRoundFunction(r, P);
	}
	
	for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            P[j][i] = iv[j][i]^P[j][i];
        }
    }
	
}

//this method takes a matrix of bytes as input
//and prints each byte in column-wise order with spaces
/**
 * The function prints a 4x4 matrix of hexadecimal values.
 * 
 * @param x The parameter `x` is a 2-dimensional array of type `uchar8_t` with dimensions 4x4. It is
 * used to store a block of data that represents text. The function `printText` takes this block of
 * data as input and prints it to the console in a specific
 */
void printText(uchar8_t x[4][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%02hhx ", x[j][i]);
        }
    }
}

//this method prints the secret key
//with each byte separated by space
/**
 * The function "printKey" prints out a given array of 32 unsigned char values in hexadecimal format.
 * 
 * @param K K is an array of 32 unsigned 8-bit characters (uchar8_t). It represents a cryptographic key
 * that needs to be printed in hexadecimal format. The function printKey takes this array as input and
 * prints each element of the array in hexadecimal format, separated by a space. The output is printed
 */
void printKey(uchar8_t K[32])
{
    for(int i = 0; i < 32; i++)
    {
        printf("%02hhx ", K[i]);
    }
    printf("\n");
}


/**
 * The function performs CBC encryption on a given number of blocks using a key, initialization vector,
 * and substitution matrix.
 * 
 * @param n The number of blocks to be encrypted in the CBC mode.
 * @param S A 3-dimensional array of size n x 4 x 4, representing the plaintext blocks to be encrypted.
 * @param K The encryption key used for generating round keys.
 * @param iv Initialization vector, a 4x4 matrix of bytes used as the initial input to the encryption
 * algorithm. It is used to add randomness and prevent patterns in the encrypted output.
 * @param c The parameter "c" is a 3-dimensional array of size (n+1) x 4 x 4, where n is the number of
 * blocks to be encrypted. It stores the ciphertext generated by the CBC encryption algorithm. The
 * first index of the array represents the block number, and the
 */
void CBC_encryption(int n, uchar8_t S[n][4][4], uchar8_t K[32], uchar8_t iv[4][4], uchar8_t c[n+1][4][4])
{
    uchar8_t roundKey[15][4][4];
    generateKeys(K, roundKey);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            c[0][j][i] = iv[j][i];
        }
    }
    for(int i = 0; i < n; i++)
    {
        encrypt(S[i], roundKey, c[i+1], c[i]);
    }
}

/**
 * The function performs CBC decryption on a given number of blocks using a key and initialization
 * vector to generate round keys and output the decrypted blocks.
 * 
 * @param n The number of blocks to be decrypted.
 * @param c A 3-dimensional array of size (n+1) x 4 x 4, representing the ciphertext blocks to be
 * decrypted.
 * @param K The encryption key used to generate the round keys for the decryption process.
 * @param iv The initialization vector (IV) is a 4x4 matrix of 8-bit unsigned characters used in CBC
 * (Cipher Block Chaining) mode of operation for encryption and decryption. It is used to initialize
 * the first block of plaintext before encryption and to XOR with the previous block of ciphertext
 * during decryption.
 * @param P A 3-dimensional array of size n x 4 x 4, representing the plaintext blocks to be decrypted.
 */
void CBC_decryption(int n, uchar8_t c[n+1][4][4], uchar8_t K[32], uchar8_t iv[4][4], uchar8_t P[n][4][4])
{
    uchar8_t roundKey[15][4][4];
    generateKeys(K, roundKey);
    for(int i = 0; i < n; i++)
    {
        decrypt(c[i+1], roundKey, P[i], c[i]);
    }
}



/**
 * The function MAC calculates a message authentication code using a hash function and a key.
 * 
 * @param k_byte an array of 32 unsigned 8-bit integers representing the key for the MAC algorithm
 * @param m The message to be authenticated, represented as an array of 32 unsigned 8-bit integers
 * (uchar8_t).
 * @param mac an output parameter that will store the resulting MAC (Message Authentication Code) as an
 * array of unsigned 8-bit integers (bytes)
 */
void MAC(uchar8_t k_byte[32], uchar8_t m[32], uchar8_t mac[32])
{
    uchar8_t one = 1;
    uchar8_t two = 2;
    
    
    //the first part is kA xor 1 which will give me the first 32 bytes
    uchar8_t tot[64];
    for(int i = 0; i < 31; i++)
    {
        tot[i] = k_byte[i];
    }
    tot[31] = k_byte[31]^one;
    
    
    //the second part is hash of (kA xor 2 || m)
    uchar8_t part[64];
    for(int i = 0; i < 31; i++)
    {
        part[i] = k_byte[i];
    }
    part[31] = k_byte[31]^two;
    
    for(int i = 0; i < 32; i++)
    {
        part[i+32] = m[i];
    }
    
    uint32_t temp[8];
    hash(part, temp, 64);
    

    for(int i = 0; i < 8; i++)
    {
        tot[4*i+32] = (temp[i] >> 24) & 0xFF;
        tot[4*i+33] = (temp[i] >> 16) & 0xFF;
        tot[4*i+34] = (temp[i] >> 8) & 0xFF;
        tot[4*i+35] = temp[i] & 0xFF;
    }
    
    //generating hash for tot to get ans for MAC operation
    uint32_t res[8];
    hash(tot, res, 64);

    //store res1 and res2 in unsigned char array mac using bit shifting
    for(int i = 0; i < 8; i++)
    {
        mac[4*i]   = (res[i] >> 24) & 0xFF;
        mac[4*i+1] = (res[i] >> 16) & 0xFF;
        mac[4*i+2] = (res[i] >> 8) & 0xFF;
        mac[4*i+3] = res[i] & 0xFF;
    }
}

/**
 * The function performs a Diffie-Hellman key exchange and uses the resulting shared key to encrypt and
 * decrypt messages using CBC mode and compute message authentication codes (MACs).
 * 
 * @return The main function is returning an integer value of 0.
 */
int main(){
    //Task 1
    findPoints();
    srand(time (0));
    //randomly taking a valid point on curve
    uint32_t idx = rand() % c;
    uint32_t alpha[2];
    alpha[0] = arrX[idx];
    alpha[1] = arrY[idx];
    //alpha[0] = 39;
    //alpha[1] = 14;
    printf("______________________________________________________________________________________________________________\n");
    printf("alpha = (%d, %d)\n", alpha[0], alpha[1]);
    printf("______________________________________________________________________________________________________________\n");
    uint32_t nA, nB;
    printf("Enter Alice's private key in [1, 150]  :");
    scanf("%d", &nA);
    printf("Enter Bob's private key in [1, 150] :");
    scanf("%d", &nB);
    //nA = 137;
    //nB = 99;
    uint32_t SK[2] = {0, 0};
    //creating the shared key using Diffie-Hellman
    DiffieHellman(nA, nB, alpha[0], alpha[1], SK);
    printf("______________________________________________________________________________________________________________\n");
    //Task 2
    printf("Shared key = (%d, %d)\n", SK[0], SK[1]);
    printf("______________________________________________________________________________________________________________\n");
    
    //total size of m is 64 bits
    //first 32 bits have x coordinate
    //last 32 bits have y coordinate
    uchar8_t m[8];
    size_t shift = 0;
    //store SK[0] in message by bit shifting
    //x coordinate of shared key is the first 32 bits of message
    for(size_t i = 0; i < 4; i++)
    {
        m[i] = (SK[0] >> (24 - shift)) & 0xFF;
        shift += 8;
    }
    shift = 0;
    //store SK[1] in message by bit shifting
    //y coordinate of shared key is the last 32 bits of message
    for(size_t i = 4; i < 8; i++)
    {
        m[i] = (SK[1] >> (24 - shift)) & 0xFF;
        shift += 8;
    }

    // printf("Orig message\n");
    // for(size_t i = 0; i < 8; i++)
    //     printf("%02x ", m[i]);
    // printf("\n");
    //printf("Size of orig message = %zu\n", sizeof(m));


    //hash-computation for message m, i.e., shared key
    uint32_t kA[8];
    //calling hash function and finding kA
    hash(m, kA, sizeof(m));
    printf("______________________________________________________________________________________________________________\n");
    printf("Hash for Alice - kA = ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("%02x ", kA[i]);
    }
    printf("\n");
    //hash-computation from message m, i.e, shared key
    uint32_t kB[8];
    hash(m, kB, sizeof(m));

    //if Diffie-Hellman is properly implemented, then kA and kB should be same
    printf("Hash for Bob - kB = ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("%02x ", kB[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    //input Alice's 256 bit message
    printf("Input 256-bit Alice's message :");
    uchar8_t mA[2][4][4];
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                scanf("%02hhx", &mA[i][k][j]);
                //printf("%02hhx ", mA[i][k][j]);
            }
        }
    }

    //taking iv as specified in assignment
    uchar8_t iv[4][4] = {   {0x00, 0x00, 0x00, 0x00}, 
                            {0x00, 0x00, 0x00, 0x00}, 
                            {0x00, 0x00, 0x00, 0x00}, 
                            {0x00, 0x00, 0x00, 0x00}
                            };
    uchar8_t keyA[32];
    //convery kA to an array of 32 bytes to make input format suitable for AEs
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            keyA[4*i + j] = (kA[i] >> (24 - 8*j)) & 0xFF;
        }
    }
    //print keyA
    printf("keyA = ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", keyA[i]);
    }
    printf("\n");
    //print mA
    uchar8_t cA[3][4][4];
    CBC_encryption(2, mA, keyA, iv, cA);
    printf("______________________________________________________________________________________________________________\n");
    //print cA
    printf("cA = ");
    for(int i = 1; i < 3; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                printf("%02hhx ", cA[i][k][j]);
            }
        }
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    //convert mA to an array of 32 bytes
    uchar8_t mA_byte[32];
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                mA_byte[16*i + 4*j + k] = mA[i][k][j];
            }
        }
    }
    uchar8_t macA[32];
    MAC(keyA, mA_byte, macA);
    printf("______________________________________________________________________________________________________________\n");
    printf("MAC_A = ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", macA[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    //convery kB to an array of 32 bytes
    uchar8_t keyB[32];
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            keyB[4*i + j] = (kB[i] >> (24 - 8*j)) & 0xFF;
        }
    }
    
    //Decryption for BoB
    uchar8_t mB[2][4][4];
    CBC_decryption(2, cA, keyB, iv, mB);
    printf("______________________________________________________________________________________________________________\n");
    printf("\n");
    printf("mB = ");
    for(int i = 0; i < 2; i++)
    {
        printText(mB[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    
    //convert mB to an array of 32 bytes
    uchar8_t mB_byte[32];
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                mB_byte[16*i + 4*j + k] = mB[i][k][j];
            }
        }
    }
    printf("______________________________________________________________________________________________________________\n");
    uchar8_t macB[32];
    MAC(keyB, mB_byte, macB);
    printf("MAC_B = ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", macB[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    printf("**********************************************************************************************************\n");
    printf("********************************** VERIFICATION OF ANSWER ******************************************\n");
    printf("kA = ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("%02x ", kA[i]);
    }
    printf("\n");
    printf("kB = ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("%02x ", kB[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    //print mA
    printf("mA = ");
    for(int i = 0; i < 2; i++)
    {
        printText(mA[i]);
    }
    printf("\n");
    printf("mB = ");
    for(int i = 0; i < 2; i++)
    {
        printText(mB[i]);
    }
    printf("\n");
    printf("______________________________________________________________________________________________________________\n");
    printf("MAC_A = ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", macA[i]);
    }
    printf("\n");
    printf("MAC_B = ");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ", macB[i]);
    }
    printf("\n");
    printf("**********************************************************************************************************\n");
    printf("**********************************************************************************************************\n");
    return 0;
}
