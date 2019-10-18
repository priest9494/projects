// 44.Найти максимальное число, меньшее заданного, которое равно сумме его десятичных цифр, возведенных в степень, большую 1

#include <stdio.h> 
#include <time.h> 
#include <mpi.h>

int isSum(int num)
{
	if(num <= 0)
		return 0;
	int size = 0, tmpNum = num, deg = 1, digits[100], maxDegree[100], degree[100];
	while (tmpNum > 0)
	{
		digits[size++] = tmpNum % 10;
		tmpNum /= 10;
	}
	for (int i = 0; i < size; ++i) 
	{
		degree[i] = 2;
		maxDegree[i] = 2;
	}
	if (size == 1)
		return 0;
	for (int i = 0; i < size; i++)
	{
		deg = 1;
		int res = digits[i];
		if (res == 0 || res == 1)
		{
			deg = 2;
			break;
		}
		while (res < num)
		{
			res *= digits[i];
			deg += 1;
		}
		maxDegree[i] = deg;
	}
	while (1)
	{
		degree[0] += 1;
		for (int i = 0; i < size - 1; i++)
		{
			if (degree[i] > maxDegree[i])
			{
				degree[i] = 2;
				degree[i + 1]++;
			}
		}
		if (degree[size - 1] > maxDegree[size - 1])
			return 0;
		long sum = 0;
		for (int i = 0; i < size; i++)
		{
			sum += pow(digits[i], degree[i]);
		}
		if (sum == num)
		{
			//for(int i = 0; i < size; i++)
			//	printf("%d ",degree[i]);
			//printf('\n');
			return 1;
		}
			
	}
	return 0;
}

int main(int argc, char **argv)
{
	int N = 0, max = 0, myid = 0, numprocs = 0, result = 0;
	double startwtime = 0.0;
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
		startwtime = MPI_Wtime();
		for (int i = N; i > 0; i--)
		if (isSum(i)) 
		{
			max = i;
			break;
		}
		printf("posled answer = %d\n", max);
		printf("posled time = %f\n\n\n", (MPI_Wtime() - startwtime));
		max = 0;
	}
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	startwtime = MPI_Wtime();
	int step = 100;
	max = 0;
	for (int i = N; i > 0; i -= numprocs)
	{
		if (isSum(i + myid)) 
		{
			max = i + myid;
			break;
		}
		if(i%step == 0)
		{
			MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
			if (result != 0)
			{
				if (myid == 0)
				{
					printf("parallel answerrrrr = %d\n", result);
					printf("parallel time = %f\n\n\n", (MPI_Wtime() - startwtime));
				}
				return;
			}
		}
		
	}
	MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myid == 0)
	{
		printf("parallel answer = %d\n", result);
		printf("parallel time = %f\n\n\n", (MPI_Wtime() - startwtime));
	}
		MPI_Finalize();

}