#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <omp.h>
#include <time.h>
#include <cmath>

int main()
{
	clock_t t1, t2;
	int n = 0, size = 0, x = 0, tmp = 0;
	printf("n = ");
	while (1)
	{
		char c = getchar();
		if (((c < '0') || (c > '9')) && (c != '\n'))
		{
			printf("Input error\n");
			return 0;
		}
		if (c == '\n')
			break;
		n = n * 10 + (c - '0');
	}
	int *a = (int*)malloc(n * sizeof(int)+10);
	int *simpleBuf = (int*)malloc(n * sizeof(int)+10);

	for (int i = 0; i < n + 1; i++)
		a[i] = i;
	for (int p = 2; p < n + 1; p++)
	{
		if (a[p] != 0)
		{
			for (int j = p * 2; j < n + 1; j += p)
				a[j] = 0;
		}
	}
	for (int i = 2; i < n && a[i] < sqrt(n); i++)
	{
		if (a[i] != 0)
		{
			simpleBuf[size] = a[i];
			size++;
		}
	}
	short flag = 1;
	tmp = n + 1;
	t1 = clock();
	while (flag)
	{
#pragma omp parallel for
		for (int two = 0; two < size; two++)
		for (int three = 0; three < size && flag != 0; three++)
		for (int four = 0; four < size; four++)
		for (int five = 0; five < size; five++)
		{
			x = tmp - pow(simpleBuf[two], 2) - pow(simpleBuf[three], 3) - pow(simpleBuf[four], 4) - pow(simpleBuf[five], 5);
			if (x == 0)
			{
				flag = 0;
				printf("\nanswer: %d (%d ^2 + %d ^3 + %d ^4 + %d ^5)\n",tmp, simpleBuf[two], simpleBuf[three], simpleBuf[four], simpleBuf[five]);
				}

		}
		tmp++;
	}
	t2 = clock();

	printf("parallel time %f \n\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
	tmp = n + 1;
	flag = 1;
	t1 = clock();
	while (flag)
	{
		for (int two = 0; two < size; two++)
		for (int three = 0; three < size && flag != 0; three++)
		for (int four = 0; four < size; four++)
		for (int five = 0; five < size; five++)
		{
			x = tmp - pow(simpleBuf[two], 2) - pow(simpleBuf[three], 3) - pow(simpleBuf[four], 4) - pow(simpleBuf[five], 5);
			if (x == 0)
			{
				printf("answer: %d (%d ^2 + %d ^3 + %d ^4 + %d ^5)\n", tmp, simpleBuf[two], simpleBuf[three], simpleBuf[four], simpleBuf[five]);
				flag = 0;
			}
		}
		tmp++;
	}
	t2 = clock();
	printf("posled time %f \n", (double)(t2 - t1) / CLOCKS_PER_SEC);
	return 0;
}