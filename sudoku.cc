
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

/*Defini磯 da estrutura de n󊪅ssa estrutura representa uma c鬵la da tabela do jogo de sudoku
*n - valor da c鬵la: 0 para c鬵la vazia ou de 1-9 para celula preenchida
*isFix - flag que indica se essa c鬵la 頦ixa, ou seja, se veio preenchida no estado inicial
*possible - vetor de numeros possiveis para o preenchimento da celula, valores de 1-9
*pos - indicador de que posi磯 do vetor possible esta sendo explorada
*checked - flag que indica se a posi磯 foi preenchida ou n㯊*i - indicador da linha da celula
*j - indicador da coluna da celula
*/

typedef struct node
{
	int n;
	int isFix;
	vector<int> possible;
	int pos;
	int checked;
	int i, j;
} node;

//Vetor que vai conter todos os n󳬠usado no MVR para localizar o n󠱵e possui menos possibilidades
vector<node *> possibleSizes;
//contador de itera絥s
int contagem = 0;

#define FAIL 0
#define SUCCESS 1

//Tipos de busca
enum types
{
	SIMPLES,
	ADIANTE,
	MVR
};

//flag que ira indicar o tipo de busca a ser realizada
int flag;
/*
* isPossible - fun磯 que checa se determinado valor pode ser inserido numa determinada c鬵la
* i,j - coordenadas da posi磯 que vai ser testada
* val - o valor que serᠴestado
* sudoku[9][9] - o estado atual da tabela
*/
int isPossible(int i, int j, int val, node sudoku[9][9])
{
	int k, l, m, n;

	//verifica se o valor pode ser inserido na linha
	for (k = 0; k < 9; k++)
	{
		if (k != i && val == sudoku[k][j].n)
			return FAIL;
	}

	//verifica se o valor pode ser inserido na coluna
	for (k = 0; k < 9; k++)
	{
		if (k != j && val == sudoku[i][k].n)
			return FAIL;
	}

	//verifica se o valor pode ser inserido no quadrante
	n = j / 3;
	m = i / 3;
	for (k = m * 3; k < (m + 1) * 3; k++)
	{
		for (l = n * 3; l < (n + 1) * 3; l++)
			if (k != i && l != j && val == sudoku[k][l].n)
				return FAIL;
	}

	return SUCCESS;
}
/*
*makePossible - fun磯 feita para gerar o vetor de valores possiveis em uma determinada c鬵la
*i,j - coordenadas da celula
*sudoku[9][9] - estado atual da tabela
*/
void makePossible(int i, int j, node sudoku[9][9])
{
	sudoku[i][j].possible.clear();

	int val;

	for (val = 1; val < 10; val++)
	{
		if (isPossible(i, j, val, sudoku))
			sudoku[i][j].possible.push_back(val);
	}
}

int refreshPossibles(int i, int j, node sudoku[9][9])
{
	int k, l, n, m;
	int result = SUCCESS;

	for (k = 0; k < 9 && result; k++)
	{
		makePossible(k, j, sudoku);
		if (sudoku[k][j].possible.size() == 0)
			result = FAIL;
	}

	for (k = 0; k < 9 && result; k++)
	{
		makePossible(i, k, sudoku);
		if (sudoku[i][k].possible.size() == 0)
			result = FAIL;
	}

	m = i / 3;
	n = j / 3;
	for (k = m * 3; k < (m + 1) * 3 && result; k++)
	{
		for (l = n * 3; l < (n + 1) * 3 && result; l++)
		{
			makePossible(k, l, sudoku);
			if (sudoku[k][l].possible.size() == 0)
				result = FAIL;
		}
	}

	return result;
}
/*
*diff - fun磯 que analisa se a celula esta vazia ou se existe algum erro de linha coluna ou quadrante
*/
int diff(int i, int j, node sudoku[9][9])
{
	int k, l, m, n;

	if (sudoku[i][j].n == 0)
		return FAIL;

	//linha diferente
	for (k = 0; k < 9; k++)
		if (k != i && sudoku[i][j].n == sudoku[k][j].n)
			return FAIL;

	//coluna diferente
	for (k = 0; k < 9; k++)
		if (k != j && sudoku[i][j].n == sudoku[i][k].n)
			return FAIL;

	//quadrante diferente
	n = j / 3;
	m = i / 3;
	for (k = m * 3; k < (m + 1) * 3; k++)
		for (l = n * 3; l < (n + 1) * 3; l++)
			if (k != i && l != j && sudoku[i][j].n == sudoku[k][l].n)
				return FAIL;

	return SUCCESS;
}
/*
*makePointers - fun磯 que adiciona todos os n󳠤a tabela no vetor possibleSizes para uso da busca MVR
*sudoku[9][9] - a configura磯 atual da tabela
*/
void makePointers(node sudoku[9][9])
{
	int i, j;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			possibleSizes.push_back(&sudoku[i][j]);

}
/*
*readSudoku - fun磯 para ler a configura磯 inicial da tabela
*/
void readSudoku(node sudoku[9][9])
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			scanf("%d", &sudoku[i][j].n);
			if (sudoku[i][j].n == 0)
			{
				sudoku[i][j].isFix = 0;
				sudoku[i][j].checked = 0;
				sudoku[i][j].i = i;
				sudoku[i][j].j = j;
				if (flag != SIMPLES)
					sudoku[i][j].pos = 0;
			}
			else
			{
				sudoku[i][j].isFix = 1;
				sudoku[i][j].checked = 1;
			}
		}
	}

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			makePossible(i, j, sudoku);

	if(flag == MVR)
		makePointers(sudoku);
}
/*
*printfSudoku - fun磯 para imprimir a configura磯 de um Sudoku
*/
void printfSudoku(node sudoku[9][9])
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			printf("%d ", sudoku[i][j].n);
		}
		printf("\n");
	}
	printf("\n");
}

/*
*isResult - fun磯 que analisa se o sudoku esta resolvido
*/
int isResult(node sudoku[9][9])
{
	int i, j, result = SUCCESS;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9 && result; j++)
			if (diff(i, j, sudoku) == 0)
				result = FAIL;

	return result;
}

//metodo de compara磯 para ordenar o vetor possibleSizes, ordenando pela celula que contem a menor quantidade de valores possiveis para ser inseridos nela
bool comp(node *x, node *y) { return ( x->possible.size() < y->possible.size() ); }

/*
nextVariable - fun磯 que determina qual a proxima celula a ser modificada
*/
void nextVariable(int *i, int *j, int *iQuad, int *jQuad, node sudoku[9][9])
{
	if (flag != MVR) //para backtracking simples (sem heur�ica) 
	{

		if (*i == 3 + *iQuad && *j < 2 + *jQuad)
		{
			*i = 0;
			++(*j);
		}

		//pula os fixos
		while (sudoku[*i][*j].isFix && *i < 2 + *jQuad)
			++(*i);
		//caso a linha ja esteja feita
		if (sudoku[*i][*j].isFix && *i == 2 + *iQuad && *j < 2 + *jQuad)
		{
			*i = 0;
			++(*j);
			// (*jQuad) += 3;

		}else if (sudoku[*i][*j].isFix && *i == 2 + *iQuad && *j == 2 + *jQuad)
		{
			*i = 0;
			*j = 0;

			if (*i == 8 && *j == 2 + *jQuad){				
				(*jQuad) += 3;
			}
			
			(*iQuad) = 0;
		}

		// if (*j == 2 + *jQuad){
		// 	(*iQuad) += 3;
		// 	*j = 0;
		// }

		// if (*i == 3){
		// 	*i = 0;
		// 	(*jQuad) += 3;	
		// }
		

		//se ja acabou a linha
		// if (*i == 9 && *j < 8)
		// {
		// 	*i = 0;
		// 	++(*j);
		// }

		// //pula os fixos
		// while (sudoku[*i][*j].isFix && *i < 8)
		// 	++(*i);
		// //caso a linha ja esteja feita
		// if (sudoku[*i][*j].isFix && *i == 8 && *j < 8)
		// {
		// 	*i = 0;
		// 	++(*j);
		// }
	}
	else //para backtracking com heur�ica MVR
	{
		//faz o MVR
		int k = 0 , found = 0;
		//ordena o vetor de possibleSizes
		sort(possibleSizes.begin(), possibleSizes.end(), comp);

		//com a ordena磯 das celulas indicando qual tem a menor possibilidade de numeros, pega a primeira que ainda nao tenha sido preenchida
		while( k < possibleSizes.size() && found == 0)
		{
			if( possibleSizes[k]->checked == 0 )
				found = 1;
			++k;
		}

		if(found)
		{
			*i = possibleSizes[k-1]->i;
			*j = possibleSizes[k-1]->j;
		}
	}
}
/*
*nextValue - fun磯 que determina qual vai ser o valor colocado na celula que vai ser trabalhada
*/
int nextValue(int i, int j, node sudoku[9][9])
{
	int result = SUCCESS;

	if (flag == SIMPLES) //para backtracking sem heur�ica
	{
		sudoku[i][j].n++;

		while (diff(i, j, sudoku) == FAIL && sudoku[i][j].n < 10)
			sudoku[i][j].n++;

		if (sudoku[i][j].n == 10)
		{
			sudoku[i][j].n = 0;
			result = FAIL;
		}else
			contagem++;
	}
	else //para backtracking com heur�ica de verifi磯 adiante e MVR
	{
		if (sudoku[i][j].pos < sudoku[i][j].possible.size())
		{
			sudoku[i][j].n = sudoku[i][j].possible[sudoku[i][j].pos++];

			while ((result = refreshPossibles(i, j, sudoku)) == FAIL
			        && sudoku[i][j].pos < sudoku[i][j].possible.size())
				sudoku[i][j].n = sudoku[i][j].possible[sudoku[i][j].pos++];
		}
		else result = FAIL;

		if (result == FAIL)
		{
			sudoku[i][j].n = 0;
			sudoku[i][j].pos = 0;
			sudoku[i][j].checked = 0;
			refreshPossibles(i, j, sudoku);
		}
		else
		{
			sudoku[i][j].checked = 1;
			contagem++;
		}
	}
	return result;
}
/*
*backtrack - Ffun磯 que realiza o backtracking
*/
int backtrack(int i, int j, int iQuad, int jQuad, node sudoku[9][9])
{
	int result = FAIL;

	if (isResult(sudoku))
		result = SUCCESS;
	else
	{
		nextVariable(&i, &j, &iQuad, &jQuad, sudoku);

		while (!result && nextValue(i + iQuad, j + jQuad, sudoku))
			result = backtrack(i + 1, j, iQuad, jQuad, sudoku);
	}

	return result;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR = usage: %s param\n", argv[0]);
		return 1;
	}
	else
	{
		string value(argv[1]);

		if (value == "simples")
			flag = SIMPLES;
		else if (value == "adiante")
			flag = ADIANTE;
		else if (value == "mvr")
			flag = MVR;
		else
		{
			printf("ERROR = possible params: simples, adiante, mvr.\n");
			return 1;
		}
	}

	int nTests;
	node sudoku[9][9];
	// int *iQuad = 0

	scanf("%d\n", &nTests);
	while (nTests > 0)
	{
		readSudoku(sudoku);
		backtrack(0, 0, 0, 0, sudoku);
		printfSudoku(sudoku);
		printf("%d %d\n", contagem, nTests);
		contagem=0;

		nTests--;
	}

	return 0;
}
