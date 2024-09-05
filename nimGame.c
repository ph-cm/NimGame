#include <stdio.h>
#include <stdlib.h>

//check if the game is over
int terminal(int *state, int size);

//iniciar o jogo e distribuir n sticks em m piles
void initGame(int n, int m, int *state);

//exibir state atual das piles
void displayState(int *state, int size);

//movimento humano
void playerMove(int *state, int size);

//IMPLEMENTAÇÃO DO ALGORITMO DE BUSCA
//XOR de todos os tamanhos dos montes
int calculateSum(int *state, int size);

//encontrar a melhor jogada
void findBestMove(int *state, int size, int *bestPile, int *bestStick);

int main()
{
    int n, m;

    printf("Enter a value of sticks between 8 and 12: \n");
    scanf("%d", &n);
    printf("Enter a value of piles between 3 and 5: \n");
    scanf("%d", &m);

    int state[5];//max tamanho

    initGame(n, m, state);

    int currentPlayer = 1;

    while(!terminal(state, m))
    {
        displayState(state, m);
        printf("Player %d's turn\n", currentPlayer);

        if(currentPlayer == 1)
        {
            //jogada humana
            playerMove(state, m);
        }
        else
        {
            //jogada do pc
            int bestPile, bestStick;
            findBestMove(state, m, &bestPile, &bestStick);
            printf("Computer removes %d sticks from pile %d\n", bestStick, bestPile);
            state[bestPile] -= bestStick;
        }
        currentPlayer = 3 - currentPlayer; //alternar entre jogadores
    }
    printf("Game over! Final state: ");
    displayState(state, m);
    printf("Player %d wins!\n", 3 - currentPlayer);//jogador anterior ganhou

    return 0;
}

int terminal(int *state, int size)
{
    for(int i = 0; i < size; i++)
    {
        //se qualquer pile ainda tiver stick, o jogo nao terminou
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
    //inicializar os piles com 1 sticks cada um
    for (int i = 0; i < m; i++)
    {
        state[i] = 1;
    }
    //distribuir os sticks restantes
    for(int i = 0; i < (n - m); i++)
    {
        state[i % m]++;//incremento de forma circular/ciclica (adiciona no primeiro pile, no segundo e no terceiro ate que os palitos acabem)
    }
}

void displayState(int *state, int size)
{
    printf("Current state: ");
    for(int i = 0; i < size; i++)
    {
        printf("[ %d ]", state[i]);//numero de sticks em cada pile
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
        pile--;//ajustar para o indice zero

        if(pile < 0 || pile >= size|| state[pile] == 0)
        {
            printf("Invalid choice. Try again.\n");
            continue;//se o movimento/escolha for vazio, pede novamente
        }

        printf("Choose the number of stick to remove (1-%d): ", state[pile]);
        scanf("%d", &stick);

        if(stick < 1 || stick > state[pile])
        {
            printf("Invalid number of stick. Try again.\n");
            continue;//se o numero de sticks for invalido, pede novamente
        }
        state[pile] -= stick;//remove os sticks da pile escolhida
        break;//sai do loop somente quando uma jogada valida eh feita
    }
}

int calculateSum(int *state, int size)
{
    int sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum ^= state[i];//calculo do XOR entre os tamanhos dos piles
    }
    return sum;
}

void findBestMove(int *state, int size, int *bestPile, int *bestStick)
{
    int sum = calculateSum(state, size);

    //se sum for 0, nao ha jogada forcada/melhor
    if(sum == 0)
    {
        for(int i = 0; i < size; i++)
        {
            if(state[i] > 0)
            {
                *bestPile = i;
                *bestStick = 1;//ao menos um stick sera removido
                return;
            }
        }
    }
    for(int i = 0; i < size; i++)
    {
        //se sum nao for 0, encontra uma jogada onde sum torna-se igual a 0
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

