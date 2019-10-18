//55. Найти минимальное число, большее заданного, 
//которое может быть представлено как сумма степеней 2, 3, 4 и 5 простых чисел
//(наименьшее такое число есть 60 = 22 + 23 + 24 + 25).
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <cmath>

using namespace std;

__global__ void search(int* SimpleBuf, int* resultBuf, int simpleSize, int N) {
	int three = 0; int four = 0; int five = 0, x = 0, idx = threadIdx.x;
	while (idx < simpleSize)
	{
		for (three = 0; three < simpleSize; three++)
		for (four = 0; four < simpleSize; four++)
		for (five = 0; five < simpleSize; five++)
		{
			x = pow((double)SimpleBuf[idx], 2) + pow((double)SimpleBuf[three], 3) + pow((double)SimpleBuf[four], 4) + pow((double)SimpleBuf[five], 5);
			if (x > N && x < resultBuf[idx])
				resultBuf[idx] = x;
		}
		idx = idx + simpleSize;
	}
}

int main() {
	int *hostResultBuf, *deviceResultBuf, *devicehostSimpleBuf, *hostSimpleBuf;
	int n = 0;
	int x = 0;
	int answer = 2000000000;
	int min = answer;
	int size = 0;
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
	hostSimpleBuf = (int*)malloc(n * sizeof(int)+10);
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
			hostSimpleBuf[size] = a[i];
			size++;
		}
	}
	free(a);
	clock_t begin = clock();
	for (int two = 0; two < size; two++)
	for (int three = 0; three < size; three++)
	for (int four = 0; four < size; four++)
	for (int five = 0; five < size; five++)
	{
		x = pow(hostSimpleBuf[two], 2) + pow(hostSimpleBuf[three], 3) + pow(hostSimpleBuf[four], 4) + pow(hostSimpleBuf[five], 5);
		if (x > n && x < answer) 
			answer = x;
	}
	clock_t end = clock();
	printf("posled result = %d\n", answer);
	double time_spent = (double)(end - begin);
	printf("posled time = %f\n\n", time_spent);
	hostResultBuf = (int*)malloc(sizeof(int)*(size + 1));
	for (i = 0; i < size; i++)
		hostResultBuf[i] = 1000000000;

	cudaMalloc((void**)&devicehostSimpleBuf, sizeof(int)*(n + 1));
	cudaMalloc((void**)&deviceResultBuf, sizeof(int)*(size + 1));

	cudaMemcpy(devicehostSimpleBuf, hostSimpleBuf, sizeof(int)*(n + 1), cudaMemcpyHostToDevice);
	cudaMemcpy(deviceResultBuf, hostResultBuf, sizeof(int)*(size + 1), cudaMemcpyHostToDevice);

	clock_t begin1 = clock();
	search << <1, size >> > (devicehostSimpleBuf, deviceResultBuf, size, n);

	cudaMemcpy(hostResultBuf, deviceResultBuf, sizeof(int)*(size + 1), cudaMemcpyDeviceToHost);

	for (int i = 0; i < size; i++) {
		if (hostResultBuf[i] < min)
			min = hostResultBuf[i];
	}
	clock_t end1 = clock();

	printf("parallel result: %d\n", min);

	double time_spent1 = (double)(end1 - begin1);
	printf("parallel time: %f\n\n", time_spent1);

	//system("pause");
}