#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define uint16_t unsigned short int
#define uint32_t unsigned int
#define uint64_t unsigned long long int
#define uchar8_t unsigned char
#define BLOCK_SIZE 64

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

uint32_t RotR(uint32_t x, uint32_t n){
    return (x >> n) | (x << (32 - n));
}

uint32_t ShR(uint32_t x, uint32_t n){
    return x >> n;
}

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ ((~x) & z);
}

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t A_Sigma0(uint32_t x){
    return RotR(x, 2) ^ RotR(x, 13) ^ RotR(x, 22);
}

uint32_t A_Sigma1(uint32_t x){
    return RotR(x, 6) ^ RotR(x, 11) ^ RotR(x, 25);
}

uint32_t B_Sigma0(uint32_t x){
    return RotR(x, 7) ^ RotR(x, 18) ^ ShR(x, 3);
}

uint32_t B_Sigma1(uint32_t x){
    return RotR(x, 17) ^ RotR(x, 19) ^ ShR(x, 10);
}


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

	//convert bit length 
    for(size_t i = new_len - 8; i < new_len; i++){
        padded[i] = (m_len >> (56 - 8*(i - (new_len - 8)))) & 0xFF;
    }

    //Update the length of padded message
    *len = new_len;

    return padded;

}

//generate 64 32-bit words
void SHA_generate_words(uchar8_t *m, uint32_t *w){
    for (size_t i = 0; i < 16; i++)
    {
        w[i] = (m[i*4] << 24) | (m[i*4 + 1] << 16) | (m[i*4 + 2] << 8) | (m[i*4 + 3]);
    }
    for (size_t i = 16; i < 64; i++)
    {
        w[i] = B_Sigma1(w[i-2]) + w[i-7] + B_Sigma0(w[i-15]) + w[i-16];
    }
}

//hash-computation from message m
void hash(uchar8_t* m, uint32_t* h)
{
    size_t len = 1;
    m = SHA_padding(m, &len);
    //print len
    printf("%zu\n", len);
    
    printf("%s\n", "Padded message:");
    for(size_t i = 0; i < len; i++)
    {
        printf("%02x ", m[i]);
    }
    
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

int main(){
    
    uchar8_t message[1] = {0x61};
    //hash-computation from message m
    uint32_t h[8];
    hash(message, h);

    printf("Hash of message = ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("%x ", h[i]);
    }

    return 0;
}
