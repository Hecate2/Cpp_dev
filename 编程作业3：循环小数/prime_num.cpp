#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define PRIME_SIZE  100000
 
int main(void)
{
    int i,j=0,k;
    int a[PRIME_SIZE];
    char temp[100] = "";
    FILE*fp=fopen("prime.txt","w");
 
    a[0]=2;
    j=1;
    sprintf(temp, "%d\n", 2);
    fwrite(temp,1,strlen(temp),fp);
 
    for(i=3;i<1000000;i+=2)
    {
        for(k=0;k<j;k++)
        {
            if(i%a[k]==0)
                break;
        }
 
        if(k >= j)
        {
            //prime number
            a[j]=i;
            j++;
            sprintf(temp, "%d\n", i);
            fwrite(temp,1,strlen(temp),fp);
        }
    }
    fclose(fp);
    printf("DONE!!\n");
    return 0;
}
