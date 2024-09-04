#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//check if the game is over
int terminal(int *state, int size);

//sreach behoud the sucessors which one is the best
void getSucessor(int *state, int size, int **sucessor, int *numSucessor);

int utility(int *state, int size, int maximizingPlayer);

int minimax(int *state, int size, int depth, int a, int b, int maximizingPlayer);
//calc the best move per round
int* bestMove(int *state, int size, int maximizingPlayer);
//initialize the game
void initGame(int n, int m, int *state);

//IMPLEMENTAÇÃO DO ALGORITMO DE BUSCA
int calculateSum(int *state, int size);

void findBestMove(int *state, int size, int *bestPile, int *bestStick);

int main()
{
    int n, m;

    printf("Enter a value of sticks between 8 and 12: \n");
    scanf("%d", &n);
    printf("Enter a value of piles between 3 and 5: \n");
    scanf("%d", &m);

    int state[5];

    initGame(n, m, state);

    int size = m;
    int currentPlayer = 1;

    while(!terminal(state, size))
    {
        printf("Current state: ");
        for(int i = 0; i < size; i++)
        {
            printf("[ %d ]", state[i]);
        }
        printf("\n");
        int *nextState = bestMove(state, size, currentPlayer);
        for(int i = 0; i < size; i++)
        {
            state[i] = nextState[i];
        }
        currentPlayer = !currentPlayer;
    }
    printf("Game Over!\n");
    printf("Final state: ");
    for(int i = 0; i < size; i++)
    {
        printf("[ %d ]", state[i]);
    }
    printf("\n");

    return 0;
}

int terminal(int *state, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(state[i] != 0) return 0;
    }
        return 1;
    }

void getSucessor(int *state, int size, int **sucessor, int *numSucessor)
{
    *numSucessor = 0;
    for(int i = 0; i < size; i++)
    {
        for(int j = 1; j <= state[i]; j++)
        {
            int *newState = (int *)malloc(size * sizeof(int));
            for (int k = 0; k < size; k++)
            {
                newState[k] = state[k];
            }
            newState[i] -= j;
            sucessor[(*numSucessor)++] = newState;
        }
    }
}

int utility(int *state, int size, int maximizingPlayer)
{
    return maximizingPlayer ? -1 : 1;
}

int minimax(int *state, int size, int depth, int a, int b, int maximizingPlayer)
{
    if (depth == 0 || terminal(state, size))
    {
        return utility(state, size, maximizingPlayer);
    }

    if(maximizingPlayer)
    {
        int maxEval = INT_MIN;
        int *sucessor[100];
        int numSucessor;
        getSucessor(state, size, sucessor, &numSucessor);
        for(int i = 0; i < numSucessor; i++)
        {
            int eval = minimax(sucessor[i], size, depth - 1, a, b, 0);
            maxEval = (eval > maxEval) ? eval : maxEval;
            a = (a > eval) ? a : eval;
            if(b <= a)
            {
                break;
            }
        }
        return maxEval;
    }
    else
    {
        int minEval = INT_MAX;
        int *sucessor[100];
        int numSucessor;
        getSucessor(state, size, sucessor, &numSucessor);
        for(int i = 0; i < numSucessor; i++)
        {
            int eval = minimax(sucessor[i], size, depth - 1, a, b, 1);
            minEval = (eval < minEval) ? eval : minEval;
            b = (b < eval) ? b : eval;
            if (b <= a)
            {
                break;
            }
        }
        return minEval;
    }
}

int* bestMove(int *state, int size, int maximizingPlayer)
{
    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;
    int *bestMove = NULL;
    int *sucessor[100];
    int numSucessor;
    getSucessor(state, size, sucessor, &numSucessor);
    for(int i = 0; i < numSucessor; i++)
    {
        int value = minimax(sucessor[i], size, size, INT_MIN, INT_MAX, !maximizingPlayer);
        if((maximizingPlayer && value > bestValue) || (!maximizingPlayer && value < bestValue))
        {
           bestValue = value;
           bestMove = sucessor[i];
        }
    }
    return bestMove;
}

void initGame(int n, int m, int *state)
{
    if(n < 8 || n > 12)
    {
        printf("The number of sticks has to be between 8 and 12\n");
        exit(1);
    }
    if(m < 3 || m > 5)
    {
        printf("The number of piles has to be between 3 and 5\n");
        exit(1);
    }
    for (int i = 0; i < m; i++)
    {
        state[i] = 1;
    }
    for(int i = 0; i < n - m; i++)
    {
        state[i % m]++;
    }
}

int calculateSum(int *state, int size)
{
    int sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum ^= state[i];
    }
    return sum;
}

void findBestMove(int *state, int size, int *bestPile, int *bestStick)
{
    int sum = calculateSum(state, size);

    if(sum == 0)
    {
        for(int i = 0; i < size; i++)
        {
            if(state[i] > 0)
            {
                *bestPile = i;
                *bestStick = 1;
                return;
            }
        }
    }
    for(int i = 0; i < size; i++)
    {
        if(state[i] > 0)
        {
            int target = state[i] ^sum;
            if(target < state[i])
            {
                *bestPile = i;
                *bestStick = state[i] = target;
                return;
            }
        }
    }

}

