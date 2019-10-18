#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

int min(int x, int y)
{
	if (x <= y) return x;
	else return y;
}

void push(int *buf, long pos, long size) {
	int tmp;
	int child;
	tmp = buf[pos];
	while (pos <= size / 2)
	{
		child = pos * 2;
		if (child < size && buf[child] < buf[child + 1])
			child++;
		if (tmp >= buf[child])
			break;
		buf[pos] = buf[child];
		pos = child;
	}
	buf[pos] = tmp;
}

void sort(int *a, int size)
{
	int tmp;
	for (int i = size / 2; i >= 0; i--) 
		push(a, i, size - 1);
	for (int i = size - 1; i > 0; i--)
	{
		tmp = a[i];
		a[i] = a[0];
		a[0] = tmp;
		push(a, 0, i - 1);
	}
}

void concat(int *sortedBuf, int *newBuf, int firstOfFirst, int lastOfFirst, int lastOfLast)
{
	int fof = firstOfFirst;
	int lof = lastOfFirst + 1;
	int nbIter = firstOfFirst;
	while ((fof <= lastOfFirst) && (lof <= lastOfLast))
	{
		if (sortedBuf[fof]<sortedBuf[lof])
		{
			newBuf[nbIter] = sortedBuf[fof];
			fof++;
		}
		else 
		{
			newBuf[nbIter] = sortedBuf[lof];
			lof++;
		}
		nbIter++;
	}
	for (; fof <= lastOfFirst; nbIter++, fof++)
		newBuf[nbIter] = sortedBuf[fof];
	for (; lof <= lastOfLast; nbIter++, lof++)
		newBuf[nbIter] = sortedBuf[lof];
}

void parallelSort(int* sortedBuf, int* mergeBuf, int bufSize, int threadsNum)
{
	int *sortedTmpBuf = sortedBuf;
	int *mergeTmpBuf = mergeBuf;
	int *tmp;
	int i, eoSecPart, eoFirstPart, k, count = 0;
	int partSize = (int)ceil((float)bufSize / (float)threadsNum);
#pragma omp parallel for private(i) default(none) shared(threadsNum, sortedTmpBuf, partSize, bufSize)
	for (i = 0; i < threadsNum; i++)
		sort(sortedTmpBuf + i*partSize, min(partSize, bufSize - i*partSize));
	while (partSize < (2 * bufSize))
	{
#pragma omp parallel for private(i, eoSecPart, eoFirstPart) default(none) shared(threadsNum, sortedTmpBuf, mergeTmpBuf, partSize, bufSize)
		for (i = 0; i < bufSize; i += partSize)
		{
			eoSecPart = min(i + partSize - 1, bufSize - 1);
			eoFirstPart = ((i + i + partSize - 1) / 2);
			concat(sortedTmpBuf, mergeTmpBuf, i, eoFirstPart, eoSecPart);
		}

#pragma omp critical
		tmp = sortedTmpBuf;
		sortedTmpBuf = mergeTmpBuf;
		mergeTmpBuf = tmp;
		partSize *= 2;
	}
#pragma omp parallel for private(k) default(none) shared(sortedBuf, mergeBuf, bufSize)
		for (k = 0; k < bufSize; k++)
			sortedBuf[k] = mergeBuf[k];
}

int main()
{
	printf("size: ");
	int size = 0;
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
		size = size * 10 + (c - '0');
	}
	int *bufForSort = (int*)malloc(size * sizeof(int)+10);
	int *bufForParallelSort = (int*)malloc(size * sizeof(int)+10);
	int *helpBuf = (int*)malloc(size * sizeof(int)+10);

	for (int i = 0; i < size; i++)
	{
		int r = rand() % 10000;
		bufForSort[i] = r;
		bufForParallelSort[i] = r;
	}

	double start_time = omp_get_wtime();
	sort(bufForSort, size);
	printf("posled time: %f \n", omp_get_wtime() - start_time);

	start_time = omp_get_wtime();
	parallelSort(bufForParallelSort, helpBuf, size, 16);

	printf("parallel time: %f \n", omp_get_wtime() - start_time);

	short flag = 1;
	for (int i = 0; i < size; i++)
	if (bufForParallelSort[i] != bufForSort[i])
		flag = 0;
	if (flag)
		printf("true\n");
	else
		printf("false\n");

	free(bufForParallelSort);
	free(bufForSort);
	free(helpBuf);

	return 0;
}