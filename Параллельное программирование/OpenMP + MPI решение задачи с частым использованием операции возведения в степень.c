//55. Найти минимальное число, большее заданного, 
//которое может быть представлено как сумма степеней 2, 3, 4 и 5 простых чисел
//(наименьшее такое число есть 60 = 22 + 23 + 24 + 25).
#include <stdio.h>
#include <malloc.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char **argv)
{
	double t1, t2;
	int n = 0, size = 0, x = 0, tmp = 0, answer = 0, numprocs = 0, myid = 0, res = 0, tmpFlag = 1;
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
			n = n * 10 + (c - '0');
		}
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int *a = (int*)malloc(n * sizeof(int) + 10);
	int *simpleBuf = (int*)malloc(n * sizeof(int) + 10);
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
	int flag = 1;
	tmp = n + 1;
	t1 = omp_get_wtime();
	while (flag)
	{
#pragma omp parallel for private(x)
		for (int two = 0; two < size; two++)
			for (int three = 0; three < size && flag != 0; three++)
				for (int four = 0; four < size; four++)
					for (int five = 0; five < size; five++)
					{
						x = tmp + myid;
						x = x - pow(simpleBuf[two], 2) - pow(simpleBuf[three], 3) - pow(simpleBuf[four], 4) - pow(simpleBuf[five], 5);
						if (x == 0)
							flag = 0;
					}
		
		MPI_Reduce(&flag, &tmpFlag, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
		MPI_Bcast(&tmpFlag, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (!tmpFlag)
		{
			if (flag == 1)
				tmp = 2000000000;
			flag = 0;
		}
		tmp += numprocs;
	}
	tmp = tmp + myid - numprocs;
	MPI_Reduce(&tmp, &res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	t2 = omp_get_wtime();
	if (myid == 0)
	{
		printf("parallel answer: %d\n",res);
		printf("parallel time: %f\n\n",(t2 - t1));
		tmp = n + 1;
		flag = 1;
		t1 = omp_get_wtime();
		while (flag)
		{
			for (int two = 0; two < size; two++)
				for (int three = 0; three < size && flag != 0; three++)
					for (int four = 0; four < size; four++)
						for (int five = 0; five < size; five++)
						{
							x = tmp  - pow(simpleBuf[two], 2) - pow(simpleBuf[three], 3) - pow(simpleBuf[four], 4) - pow(simpleBuf[five], 5);
							if (x == 0)
							{
								flag = 0;
							}
						}
			tmp++;
		}
		t2 = omp_get_wtime();
		printf("posled answer: %d\n",(tmp - 1));
		printf("posled time: %f\n\n",(t2 - t1));
	}
	MPI_Finalize();
}