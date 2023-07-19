
#include <stdio.h>

#define p 29
#define a 13
#define b 23

int additive_inv(int x)
{
    int temp = p - x;
    return temp;
}

// multiplcative inverse using extended euclidean algorithm
int multiplicative_inv(int x)
{
    int r1 = p;
    int r2 = x;
    int t1 = 0;
    int t2 = 1;
    int q = 0;
    int r = 0;
    int t = 0;

    while (r2 > 0)
    {
        q = r1 / r2;
        r = r1 - q * r2;
        r1 = r2;
        r2 = r;

        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }

    if (t1 < 0)
    {
        t1 = t1 + p;
    }

    return t1;
}

void addPQ(int x1, int y1, int x2, int y2)
{
    int m, x3, y3;
    if ((x1 != x2) && (y1 != y2))
    {
        m = (((y2 + additive_inv(y1))%p) * multiplicative_inv(x2 + additive_inv(x1))) % p;
        x3 = ((m * m) % p + additive_inv(x1)+ additive_inv(x2)) % p;
        y3 = (y1 + (m * (x3 + additive_inv(x1)))%p) % p;
    }
    if ((x1 == x2) && (y1 == y2))
    {
        m = ((3 * x1 * x1 + a) * multiplicative_inv(2 * y1)) % p;
    }
    else if ((x1 == x2) && (y1 == additive_inv(y2)))
    {
        printf("The point is at infinity");
    }
    printf("%d %d %d", m, x3, additive_inv(y3));
}

int main()
{
    int arr1[p];
    int arr2[p];
    int c = 0;

    for (int x = 0; x < p; x++)
    {
        for (int y = 0; y < p; y++)
        {
            int temp1 = (y * y) % p;
            int temp2 = ((x * x * x) + (a * x) + b) % p;
            if (temp1 == temp2)
            {
                arr1[c] = x;
                arr2[c] = y;
                c++;
                // printf("%d %d \n", x, y);
            }
        }
    }
    addPQ(16,21,9,12);

}
