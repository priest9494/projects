//36. Число называется укорачиваемым простым слева, если последовательное удаление его десятичных цифр, 
//    начиная со старшей, дает простые числа (пример: 3797, 797, 97, 7). Это же число является 
//    укорачиваемым простым справа (3797, 379, 37, 3). Найти максимальные укорачиваемые простые 
//    слева, справа, с обеих сторон числа, меньшие заданного N
#include <stdio.h>
#include <mpi.h>
#include <malloc.h>
int isSimple(int *simpleBuf, int pos, int value)
{
	for (int i = pos; i > 0; i--)
		if (simpleBuf[i] == value)
			return 1;
	return 0;
}

int reverse(int value)
{
	int newValue = 0;
	while (value > 0)
	{
		newValue = newValue * 10 + (value % 10);
		value /= 10;
	}
	return newValue;
}

void check(int i, int *answerBuf, int *simpleBuf, int size)
{
	if(i < 0)
		return;
	int copyForRightCheck = simpleBuf[i];
	int copyForLeftCheck = reverse(simpleBuf[i]);
	while (copyForRightCheck > 0)
	{
		copyForRightCheck /= 10;
		if (!isSimple(simpleBuf, i, copyForRightCheck))
			break;
	}
	while (copyForLeftCheck > 0)
	{
		copyForLeftCheck /= 10;
		if (!isSimple(simpleBuf, i, copyForLeftCheck))
			break;
	}
	if (copyForLeftCheck == 0 && answerBuf[0] == 0)
		answerBuf[0] = simpleBuf[i];
	if (copyForRightCheck == 0 && answerBuf[1] == 0)
		answerBuf[1] = simpleBuf[i];
	if (copyForRightCheck == 0 && copyForLeftCheck == 0 && answerBuf[2] == 0)
		answerBuf[2] = simpleBuf[i];
}

int main(int argc, char **argv)
{
	int N = 0, size = 0, copyForRightCheck = 0, copyForLeftCheck = 0, myid = 0, numprocs = 0, rMax = 0, lMax = 0, lrMax = 0;
	double startwtime = 0.0;
	int answerBuf[4] = { 0, 0, 0 }; // 0 - слева, 1 - справа, 2 - с обеих
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	if (myid == 0)
	{
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
			N = N * 10 + (c - '0');
		}
	}
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int *simpleBuf = (int*)malloc(N * sizeof(int) + 10);
	int *a = (int*)malloc(N * sizeof(int) + 10);
	for (int i = 0; i < N + 1; i++)
		a[i] = i;
	for (int p = 2; p < N + 1; p++)
	{
		if (a[p] != 0)
		{
			for (int j = p * 2; j < N + 1; j += p)
				a[j] = 0;
		}
	}
	for (int i = 2; i < N && a[i] < N; i++)
	{
		if (a[i] != 0)
		{
			simpleBuf[size] = a[i];
			size++;
		}
	}
	free(a);
	startwtime = MPI_Wtime();
	if(myid == 0)
	{
		for (int i = size - 1; i > 0; i--)
		{
			check(i, answerBuf, simpleBuf, size);
		}
		printf("chislo sleva = %d \n", answerBuf[0]);
		printf("chislo sprava = %d \n", answerBuf[1]);
		printf("chislo s obeix storon = %d \n", answerBuf[2]);
		printf("posled time = %f\n\n\n", MPI_Wtime() - startwtime);
	}

	//MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//printf("size = %d myid = %d \n",size,myid);
	//MPI_Bcast(&simpleBuf, size, MPI_INT, 0, MPI_COMM_WORLD);
	startwtime = MPI_Wtime();
	for (int i = size; i > 0; i -= numprocs)
		check(i + myid, answerBuf, simpleBuf, size);
	MPI_Reduce(&answerBuf[0], &lMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&answerBuf[1], &rMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&answerBuf[2], &lrMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myid == 0)
	{
		if (lMax > answerBuf[0])
			answerBuf[0] = lMax;
		if (rMax > answerBuf[1])
			answerBuf[1] = rMax;
		if (lrMax > answerBuf[2])
			answerBuf[2] = lrMax;
		printf("chislo sleva = %d \n", answerBuf[0]);
		printf("chislo sprava = %d \n", answerBuf[1]);
		printf("chislo s obeix storon = %d \n", answerBuf[2]);
		printf("parallel time = %f\n\n\n", MPI_Wtime() - startwtime); 
	}
	free(simpleBuf);
	
	MPI_Finalize();
}