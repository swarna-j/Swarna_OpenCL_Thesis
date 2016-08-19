// File:       vector_add.c
// Abstract:   A simple "Vector Addition" compute example showing 
//             the sum of two vectors (Z = X + Y; X,Y,Z are vectors of fixed size)              
// Version:    <1.0>
//----------------------------------------------------------------------------//
#include <stdio.h>
//----------------------------------------------------------------------------//
#define SIZE 3

int main()
{
    int X[SIZE], Y[SIZE], Z[SIZE];
    int a,b,c;
    int count = SIZE, i;
    printf("\nEnter the values of the scalars a, b, c in the equation Z = aX + bY + c\n");
    scanf("%d %d %d", &a, &b, &c);
    printf("\nSupply inputs to the first vector of size : %d\n", count);
    for(i = 0; i < count; i++)
    {
      scanf("%d", &X[i]);
      X[i] = a*X[i] + c ; // Multiplying the first coefficient 'a' and adding 'c' to each item of vector X
    }
    printf("\nSupply inputs to the second vector of size : %d\n", count);
    for(i = 0; i < count; i++)
    {
      scanf("%d", &Y[i]);
      Y[i] = b*Y[i]; // Multiplying the second coefficient 'b' with Y
    }
    //Compute the equation Z = aX + bY + c
    printf("The resultant vector Z = \n");
    for (i = 0; i < count; i++)
    {
      Z[i] = X[i] + Y[i];
      printf("\t\t%d\n", Z[i]); 
    }        
    return 0;
}

