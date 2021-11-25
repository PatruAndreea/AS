#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#define COMMAND_SIZE 20

using namespace std;
int size = 0;
int counter = 0;

bool Prime(int n)
{
	for (int i = 2; i <= n / 2; i ++ )
	{
		if (n % i == 0)
			return false;
	}
	return true;
}

void childProc(int readPipe, int writePipe)
{
	int a;
	int b;
	int number;
	int primeNumber[200];
	int position = 0;
	int dimension = 0;

	read(readPipe, &a, sizeof(a));
	read(readPipe, &b, sizeof(b));
	for (int i = a; i < b: i++)
	{
		number = Prime(i);
		if (number == true)
		{
			primeNumber[position] = i;
			position = position + 1;
			dimension = dimension + 1;
		}
	}
	size = dimension;
	write(writePipe, &dimension, sizeof(dimension));
	write(writePipe, primeNumber, sizeof(primeNumber));
	exit(1);
}

void parentProc(int readPipe, int writePipe)
{
	int a;
	int b;
	int primeNumber[200];
	a = (counter - 1) * 1000;
	b = counter * 1000;
	write(writePipe, &a, sizeof(int));
	write(writePipe, &b, sizeof(int));
	read(readPipe, &size, sizeof(size));
	read(readPipe, &primeNumber, sizeof(primeNumber));
	for (int i = 0; i < size; i++)
	{
		printf("%d ", primeNumber[i]);
	}
	    printf("\n\n");
}
int main()
{
   int firstPipe[2];
   int secondPipe[2];
   int pid = 1;
   if (pipe(firstPipe))
   {
	cout<< "The pipe couldn't be created!" << endl;
       exit(-1);
   }
   if (pipe(secondPipe))
   {
       printf("The pipe couldn't be created!");
       exit(-1);
   }
   for(int i = 0; i < 10; i++)
   {
       pid = fork();
       counter = counter + 1;
       if (pid)
       {
           if (pid == -1)
           {
               printf("The process child couldn't be created!");
               exit(-1);
           }
           parentProc(secondPipe[0], firstPipe[1]);
       }
     else
       {
           childProc(firstPipe[0], secondPipe[1]);
       }
   }
}

}