// Copyright Alexandru-Andrei Ionita 312CAa 2022-2023
#include <stdio.h>
#include <stdlib.h>

#define MOD 10007  // For sum and multiplication of matrix, to not overflow

// function to read the matrix
void read(int **matrix, int rows, int colls)
{
	for (int i = 0 ; i < rows; i++) {
		for (int j = 0; j < colls; j++)
			scanf("%d", &matrix[i][j]);
	}
}

// function to print the matrix
void print(int **matrix, int rows, int colls)
{
	for (int i = 0 ; i < rows; i++) {
		for (int j = 0; j < colls; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

// this is just a prototype of a function that I'll explain later
void Q(int ***v, int **nm, int size);

// function add a new matrix into the rezizable array of matrices
void new_elem(int ****v, int ***nm, int  *size)
{
	(*size)++;
	/* we realloc both the array of matrices and the matrix of rows and columns
	in a temporary (it's okay even for the first element because I declared
	the pointers to NULL) */
	int ***tmp_v = (int ***)realloc(*v, *size * sizeof(int **));
	int **tmp_nm = (int **)realloc(*nm, *size * sizeof(int *));
	/*if alloc didn't work, we free everything and exit the program (this will
	be done for all the allocs) */
	if (!tmp_v || !tmp_nm) {
		free(tmp_v);
		free(tmp_nm);
		Q(*v, *nm, *size);
		exit(-1);
	}
	// we link the temporary to the original pointers
	*v = tmp_v;
	*nm = tmp_nm;
	/* for the matrix nm I will calloc only 2 columns per line, the first one
	is for the lines, and the other, the columns, and the index of the first
	parameter will be the indexes of the certain matrix */
	tmp_nm[*size - 1] = (int *)calloc(2, sizeof(int));
	if (!tmp_nm[*size - 1]) {
		Q(*v, *nm, *size);
		exit(-1);
	}
	for (int i = 0; i < 2; i++)
		scanf("%d", &tmp_nm[*size - 1][i]);
	// firstly, we alloc the lines of the new matrix
	tmp_v[*size - 1] = (int **)malloc(tmp_nm[*size - 1][0] * sizeof(int *));
	if (!tmp_v) {
		free(tmp_v[*size - 1]);
		Q(*v, *nm, *size);
		exit(-1);
	}
	/* here we alloc all*/
	for (int i = 0; i < tmp_nm[*size - 1][0]; i++) {
		tmp_v[*size - 1][i] = (int *)malloc(tmp_nm[*size - 1][1] * sizeof(int));
		if (!tmp_v[*size - 1][i]) {
			for (; i >= 0; i--)
				free(tmp_v[*size - 1][i]);
			Q(*v, *nm, *size);
			exit(-1);
		}
	}
}

/* The main difference between this function and new_elem is that here we
already have the matrix*/
void new_elem1(int ****v, int ***nm, int  *size, int nm1[2], int **aux)
{
	(*size)++;
	int ***tmp_v = (int ***)realloc(*v, *size * sizeof(int **));
	int **tmp_nm = (int **)realloc(*nm, *size * sizeof(int *));
	if (!tmp_v || !tmp_nm) {
		free(tmp_v);
		free(tmp_nm);
		Q(*v, *nm, *size);
		exit(-1);
	}
	*v = tmp_v;
	*nm = tmp_nm;
	tmp_nm[*size - 1] = nm1;
	tmp_v[*size - 1] = aux;
}

/* this function creates 2 arrays for cropping the given matrix, inplace*/
void C(int ***matrix, int nr, int ***nm, int arraysize)
{
	int l, c, i, j;
	scanf("%d", &l);
	int *nl = (int *)malloc(l * sizeof(int));
	if (!nl) {
		free(nl);
		Q(matrix, *nm, arraysize);
		exit(-1);
	}
	for (i = 0; i < l ; i++)
		scanf("%d", &nl[i]);
	scanf("%d", &c);
	int *nc = (int *)malloc(c * sizeof(int));
	if (!nc) {
		free(nc);
		Q(matrix, *nm, arraysize);
		exit(-1);
	}
	for (i = 0; i < c ; i++)
		scanf("%d", &nc[i]);
	if (nr < arraysize && nr >= 0) {
		int **aux = (int **)malloc(l * sizeof(int *));
		if (!aux) {
			free(aux);
			Q(matrix, *nm, arraysize);
			exit(-1);
		}
		for (i = 0; i < l; i++)
			aux[i] = (int *)malloc(c * sizeof(int));
		for (i = 0; i < l; i++) {
			for (j = 0; j < c; j++)
				aux[i][j] = matrix[nr][nl[i]][nc[j]];
		}
		for (i = 0; i < (*nm)[nr][0]; i++)
			free(matrix[nr][i]);
		free(matrix[nr]);
		matrix[nr] = aux;
		(*nm)[nr][0] = l;
		(*nm)[nr][1] = c;
	} else {
		printf("No matrix with the given index\n");
	}
	free(nl);
	free(nc);
}

// we multiply the matrices after given indexes in the matrix array
int **M(int ***matrix, int **nm, int m1, int m2)
{
	int i, j, k;
	int **aux = (int **)malloc(nm[m1][0] * sizeof(int *));
	for (i = 0; i < nm[m1][0]; i++)
		aux[i] = (int *)malloc(nm[m2][1] * sizeof(int));
	for (i = 0; i < nm[m1][0]; i++) {
		for (j = 0; j < nm[m2][1]; j++) {
			aux[i][j] = 0;
			for (k = 0; k < nm[m1][1]; k++)
				aux[i][j] += ((matrix[m1][i][k] * matrix[m2][k][j]) % MOD);
			aux[i][j] %= MOD;
			if (aux[i][j] < 0)
				aux[i][j] += MOD;
		}
	}
	return aux;
}

// in this function we free everything we succesfully alocated
void Q(int ***v, int **nm, int size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < nm[i][0]; j++)
			free(v[i][j]);
		free(v[i]);
	}
	for (int i = 0; i < size; i++)
		free(nm[i]);
	free(nm);
	free(v);
}

/* here we sum up the elements of all the matrices and after
that we sort them after their sum */
void O(int ***v, int **nm, int *size)
{
	int *sum = (int *)calloc(*size, sizeof(int));
	int i, j, k;
	for (i = 0; i < *size; i++) {
		for (j = 0; j < nm[i][0]; j++) {
			for (k = 0; k < nm[i][1]; k++) {
				sum[i] = (sum[i] + v[i][j][k]) % MOD;
				if (sum[i] < 0)
					sum[i] += MOD;
			}
		}
	}
	int aux1, *aux2, **aux3;
	for (i = 0; i < *size - 1; i++) {
		for (j = 0; j < *size - i - 1; j++) {
			if (sum[j] > sum[j + 1]) {
				aux1 = sum[j];
				sum[j] = sum[j + 1];
				sum[j + 1] = aux1;

				aux2 = nm[j];
				nm[j] = nm[j + 1];
				nm[j + 1] = aux2;

				aux3 = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux3;
				}
			}
	}
	free(sum);
}

// here we make the transpose of the matrix
void T(int ***v, int **nm, int n)
{
	int i, j, aux1;
	int **aux = (int **)malloc(nm[n][1] * sizeof(int *));
	for (i = 0; i < nm[n][1]; i++)
		aux[i] = (int *)malloc(nm[n][0] * sizeof(int));
	for (i = 0; i < nm[n][1]; i++) {
		for (j = 0; j < nm[n][0]; j++)
			aux[i][j] = v[n][j][i];
	}
	for (i = 0; i < nm[n][0]; i++)
		free(v[n][i]);
	free(v[n]);
	v[n] = aux;
	aux1 = nm[n][0];
	nm[n][0] = nm[n][1];
	nm[n][1] = aux1;
}

/* here we copy in matrix a from matrix b with the condition that both matrices
have the same size */
void cpy(int **a, int **b, int size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++)
			a[i][j] = b[i][j];
	}
}

// here we multiply 2 given matrices that have size in an auxiliar matrix
void multiply(int **matrix1, int **matrix2, int size, int ***aux)
{
	int i, j, k;
	*aux = (int **)malloc(size * sizeof(int *));
	if (!(*aux)) {
		free(aux);
		return;
	}
	for (i = 0; i < size; i++)
		(*aux)[i] = (int *)malloc(size * sizeof(int));
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			(*aux)[i][j] = 0;
			for (k = 0; k < size; k++)
				(*aux)[i][j] += matrix1[i][k] * matrix2[k][j];
			(*aux)[i][j] %= MOD;
			if ((*aux)[i][j] < 0)
				(*aux)[i][j] += MOD;
		}
	}
}

/* here we have the recursive for of exponantiation by squaring*/
int **expo(int ***v, int **nm, int a, int b)
{
	if (b == 1)
		return v[a];
	if (b % 2 == 1) {
		int **tmp_exp = expo(v, nm, a, b - 1);
		int **aux;
		multiply(tmp_exp, v[a], nm[a][0], &aux);
		for (int i = 0; i < nm[a][0]; i++)
			free(tmp_exp[i]);
		free(tmp_exp);
		return aux;
	}
	int **tmp_exp = expo(v, nm, a, b / 2);
	int **aux;
	multiply(tmp_exp, tmp_exp, nm[a][0], &aux);
	if (b > 2) { //in case b = 2 we just return, because the last call we
				 //multiply v[a] with itself
		for (int i = 0; i < nm[a][0]; i++)
			free(tmp_exp[i]);
		free(tmp_exp);
	}
	return aux;
}

// here is the main function exponantiation that verifies every case
void R(int ***v, int **nm, int *size)
{
	int m, p;
	scanf("%d%d", &m, &p);
	if (m < *size && m >= 0) {
		if (p > 0) {
			if (nm[m][0] == nm[m][1]) {
				int **aux = expo(v, nm, m, p);
				for (int i = 0; i < nm[m][0]; i++)
					free(v[m][i]);
				free(v[m]);
				v[m] = aux;
			} else {
				printf("Cannot perform matrix multiplication\n");
			}
		} else if (p == 0) {
			for (int i = 0; i < nm[m][0]; i++) {
				for (int j = 0; j < nm[m][0]; j++) {
					if (i == j)
						v[m][i][j] = 1;
					else
						v[m][i][j] = 0;
				}
			}
		} else {
			printf("Power should be positive\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

//this function frees an given matrix by index
void F(int ****v, int ***nm, int *size, int n)
{
	for (int i = 0; i < (*nm)[n][0]; i++)
		free((*v)[n][i]);
	free((*v)[n]);
	free((*nm)[n]);

	for (int i = n; i < *size - 1; i++) {
		(*v)[i] = (*v)[i + 1];
		(*nm)[i] = (*nm)[i + 1];
	}
	(*size)--;
	int ***tmp_v = (int ***)realloc(*v, (*size) * sizeof(int **));
	int **tmp_nm = (int **)realloc(*nm, (*size) * sizeof(int *));
	*v = tmp_v;
	*nm = tmp_nm;
}

int main(void)
{
	int n, size = 0, ***v = NULL, **nm = NULL, m;
	char x;
	do {
		scanf(" %c", &x);
		switch (x) {
		case 'L': {
			new_elem(&v, &nm, &size);
			read(v[size - 1], nm[size - 1][0], nm[size - 1][1]);
		} break;
		case 'D':{
			scanf(" %d", &n);
			if (n < size && n >= 0)
				printf("%d %d\n", nm[n][0], nm[n][1]);
			else
				printf("No matrix with the given index\n");
		} break;
		case 'P':{
			scanf(" %d", &n);
			if (n < size && n >= 0)
				print(v[n], nm[n][0], nm[n][1]);
			else
				printf("No matrix with the given index\n");
		} break;
		case 'C':{
			scanf(" %d", &n);
			C(v, n, &nm, size);
		} break;
		case 'M':{
			scanf(" %d%d", &n, &m);
			if (n < size && m < size && n >= 0 && m >= 0) {
				if (nm[n][1] == nm[m][0]) {
					int *nm1 = (int *)calloc(2, sizeof(int));
					nm1[0] = nm[n][0];
					nm1[1] = nm[m][1];
					new_elem1(&v, &nm, &size, nm1, M(v, nm, n, m));
				} else {
					printf("Cannot perform matrix multiplication\n");
				}
			} else {
				printf("No matrix with the given index\n");
			}
		} break;
		case 'O':{
			O(v, nm, &size);
		} break;
		case 'T':{
			scanf(" %d", &n);
			if (n < size && n >= 0)
				T(v, nm, n);
			else
				printf("No matrix with the given index\n");
		} break;
		case 'R':{
			R(v, nm, &size);
		} break;
		case 'F':{
			scanf(" %d", &n);
			if (n < size && n >= 0)
				F(&v, &nm, &size, n);
			else
				printf("No matrix with the given index\n");
		} break;
		case 'Q':{
			Q(v, nm, size);
		} break;
		default:{
			printf("Unrecognized command\n");
		} break;
		}
	} while (x != 'Q');
	return 0;
}
