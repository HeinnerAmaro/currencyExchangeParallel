#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mpi.h"

void initializeArr(float* currency);
void normal(float* currency);
void mpi();
void convert();
void wait();

const int N = 87000; //Number of transactions
int cpu, numcpus;

int main(int argc, char *argv[]) {

  double elapsed_time, elapsed_time2;

  float currency[N];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &cpu);
  MPI_Comm_size(MPI_COMM_WORLD, &numcpus);
  initializeArr(currency);
  elapsed_time -= MPI_Wtime();
  normal(currency);
  elapsed_time += MPI_Wtime();

  elapsed_time2 -= MPI_Wtime();
  mpi(currency);
  elapsed_time2 += MPI_Wtime();
  printf("Normal time:\n");
  printf("Time taken: %.15f seconds.\n", elapsed_time);
  printf("MPI time:\n");
  printf("Time taken: %.15f seconds.\n", elapsed_time2);
  MPI_Finalize();
  return 0;
}

void initializeArr(float* currency) {
	int i;
	for (i = 0; i < N; i++) {
		currency[i] = (i+1)*5;
	}
}

/*********************************************************************
*
*		NORMAL CODE FOR AUTOMATED CURRENCY EXCHANGE MACHINE
*
*********************************************************************/
void normal(float* currency) {
	int i;
	for (i = 0; i < N; i++) {
		convert(currency[i], numcpus);
	}
}

/*********************************************************************
*
*		MPI CODE FOR AUTOMATED CURRENCY EXCHANGE MACHINE
*
*********************************************************************/
void mpi(float* currency) {
	int i, othercpus;
	MPI_Status status;
	int partitions = N / numcpus;

	/*
	 * For the master cpu
	*/
	if (cpu == 0) {
		for (othercpus = 1; othercpus < numcpus; othercpus++) {
			MPI_Send(&currency[partitions*othercpus], partitions, MPI_FLOAT, othercpus, 1, MPI_COMM_WORLD);
		}

		for (i = 0; i < partitions; i++) {
			convert(currency[i], numcpus);
		}

		for (othercpus = 1; othercpus < numcpus; othercpus++) {
			MPI_Recv(&currency[partitions*othercpus], partitions, MPI_FLOAT, othercpus, 2, MPI_COMM_WORLD, &status);
		}
	}



	/*
	 * For the other cpus, number specified by user
	*/
	else {
		float data[partitions];

		MPI_Recv(&data[0], partitions, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);

		for (i = 0; i < partitions; i++) {
			convert(data[i], numcpus);
		}

		MPI_Send(&data[0], partitions, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
	}
}

/*********************************************************************
*
*		CONVERT FUNCTION FOR OUR CURRENCY (CPU chooses percentage)
*
*********************************************************************/
void convert(double dollar, int cpucount) {
	/**
	* GBP - UK, yen - Japan, rmb - China, cad - Canada, lira - Turkey, thb - Thailand, peso - Mexican
	**/
	double peso, euro, yen, cad, lira, gbp;
	printf("Dollar amount: $%.2f\n", dollar);
	dollar = dollar * .20;
	//dollar to peso
	//if (cpucount >= 1) {
		peso = dollar * 19.495;
		printf("$%.2f = %.2f mex peso\n", dollar, peso);
	//}
	//dollar to euro
	//if (cpucount >= 2) {
		euro = dollar * 0.9028;
		printf("$%.2f = %.2f euro\n", dollar, euro);
	//}
	//dollar to gbp
	//if (cpucount >= 3) {
		gbp = dollar * 0.7736;
		printf("$%.2f = %.2f gbp\n", dollar, gbp);
	//}
	//dollar to yen
	//if (cpucount >= 4) {
		yen = dollar * 108.3845;
		printf("$%.2f = %.2f yen\n", dollar, yen);
	//}
	//dollar to rmb
	//rmb = dollar * 7.036;
	//printf("$%.2f = %.2f rmb\n", dollar, rmb);
	//dollar to cad
	//if (cpucount >= 5) {
		cad = dollar * 1.33191;
		printf("$%.2f = %.2f cad\n", dollar, cad);
	//}
	//dollar to lira
	//if (cpucount >= 6) {
	//	lira = dollar * 5.7019;
	//	printf("$%.2f = %.2f lira\n", dollar, lira);
	//}
	//dollar to thb
	//thb = dollar * 30.184120;
	//printf("$%.2f = %.2f thb\n", dollar, thb);
}
