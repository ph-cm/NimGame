#include <stdio.h>
#include <stdlib.h>

//check if the game is over
int terminal(int *state, int size);

int utility(int *state, int size);

void initGame(int n, int m, int *state);

void displayState(int *state, int size);

void playerMove(int *state, int size);

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

    int currentPlayer = 1;

    while(!terminal(state, m))
    {
        displayState(state, m);
        printf("Player %d's turn\n", currentPlayer);

        if(currentPlayer == 1)
        {
            playerMove(state, m);
        }
        else
        {
            int bestPile, bestStick;
            findBestMove(state, m, &bestPile, &bestStick);
            printf("Computer removes %d sticks from pile %d\n", bestStick, bestPile);
            state[bestPile] -= bestStick;
        }
        currentPlayer = 3 - currentPlayer;
    }
    printf("Game over! Final state: ");
    displayState(state, m);
    printf("Player %d wins!\n", 3 - currentPlayer);

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
    for(int i = 0; i < (n - m); i++)
    {
        state[i % m]++;
    }
}

void displayState(int *state, int size)
{
    printf("Current state: ");
    for(int i = 0; i < size; i++)
    {
        printf("[ %d ]", state[i]);
    }
    printf("\n");
}

void playerMove(int *state, int size)
{
    int pile, stick;

    while(1)
    {
        printf("Choose a pile (1-%d): ", size);
        scanf("%d", &pile);
        pile--;

        if(pile < 0 || pile >= size|| state[pile] == 0)
        {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        printf("Choose the number of stick to remove (1-%d): ", state[pile]);
        scanf("%d", &stick);

        if(stick < 1 || stick > state[pile])
        {
            printf("Invalid number of stick. Try again.\n");
            continue;
        }
        state[pile] -= stick;
        break;
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

