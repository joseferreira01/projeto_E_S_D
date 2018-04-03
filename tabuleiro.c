#include "funcoes.h"

// mostra o tabuleiro e realiza as jogadas
void tabuleiro()
{
    ALLEGRO_BITMAP *baralho=NULL; // vari�vel representando aa imagem
    ALLEGRO_DISPLAY *janela=ini_allegro(); // vari�vel representando a janela principal
    pilha piaux=embaralha(), // pilha auxiliar que receber� o baralho devidamente embaralhado
          pi_inf[NUM_PILHAS], // vetor de  oito pilhas referente as pilhas inferiores do jogo
          pi_sup[NUM_PILHAS]; // vetor de oito pilhas referente as pilhas base e as c�lulas de reten��o
    int i,win,tamanho=NUM_PILHAS-1,  // tamanho das pilhas inferiores de 1 a 4
        origem,destino,maior; // vari�vel para armazenar a jogada e  vari�vel para armazenar o tamanho m�ximo da coluna

    // loop para cria��o e inicializa��o das pilhas
    for(i=0;i<NUM_PILHAS;i++)
    {
        pi_inf[i]=cria_pilha();
        pi_sup[i]=cria_pilha();

        // tamanho das pilhas inferiores de 5 a 8
        if(i>=NUM_PILHAS/2)
            tamanho=NUM_PILHAS-2;

        // o baralho � dividido em 4 pilhas
        divide_baralho(piaux,pi_inf[i],tamanho);
    }

    // loop principal do jogo
    do{
        limpa_tela();

        // carrega a imagem
        baralho=al_load_bitmap("baralho.png"); win=0;

        // carrega as imagens das cartas
        for(i=0;i<NUM_PILHAS;i++)
            carrega_baralho(pi_inf[i],pi_sup[i],i+1,baralho);

        // exibe o baralho
        exibe_tabuleiro(pi_inf,pi_sup);

        printf("\n======= C�lulas de reten��o ====== Pilhas base =======\n\n");
        imprime_pilha_sup(pi_sup);
        printf("\n\n [1 ]   [2 ]   [3 ]   [4 ]   [5 ]   [6 ]   [7 ]   [8 ]\n\n\n\n");

        maior=maior_topo(pi_inf);
        for(i=0;i<=maior;i++)
            imprime_pilha_inf(pi_inf,i);

        printf("\n [9 ]   [10]   [11]   [12]   [13]   [14]   [15]   [16]\n\n");
        printf("\n======================= Colunas ======================\n\n\n");
        printf("\n===== Jogada [1-15] =============== Sair [-1] ========\n");

        // jogada
        printf("\n\t\t\tMover: ");
        scanf("%d",&origem);
        printf("\t\t\tPara: ");
        scanf("%d",&destino);

        // verificando o flag de sa�da
        if(origem!=-1 && destino!=-1)
        {
            // verificando os par�metro
            if(verifica_jogada(origem,destino))
            {
                printf("\n\tPar�metros inv�lidos\n\tForne�a um n�mero v�lido\n\n");
                prompt_universal();
            }
            else
            {
                // verificando se a jogada � v�lida
                if(!computa_jogada(pi_inf,pi_sup,origem,destino))
                {
                    printf("\n\tJogada inv�lida\n\tN�o � poss�vel mover esta carta\n\n");
                    prompt_universal();
                }
                else
                    printf("\a");
            }
        }

        // condi��es para a vit�ria
        for(i=0;i<NUM_PILHAS;i++)
            if(pilha_vazia(pi_inf[i]))
                win++;

        // quebra o loop caso o jogo tenha terminado
        if(win==NUM_PILHAS)
            break;

        // finaliza a imagem
        al_destroy_bitmap(baralho);

    }while(origem!=-1 && destino!=-1);

    // O espa�o alocado pelas pilhas � liberado
    for(i=0;i<NUM_PILHAS;i++)
    {
        termina_pilha(pi_inf[i]);
        termina_pilha(pi_sup[i]);
    }

    // verificando se o usu�rio venceu a partida
    if(win==NUM_PILHAS)
        vitoria_msg();

    fin_allegro(janela);
}

// imprime as pilhas inferiores
void imprime_pilha_inf(pilha *pi, int indice)
{
    int i;

    for(i=0;i<NUM_PILHAS;i++)
    {
        // mostra uma espa�o vazio caso n�o tenha nenhuma carta na posi��o fornecida
        if(pi[i]->topo<indice)
            printf(" |  |  ");
        else
            printf(" |%c%c|  ",converte_num_simb(pi[i]->cartas[indice].numero),pi[i]->cartas[indice].naipe);
    }
    printf("\n");
}

// imprime as pilhas superiores
void imprime_pilha_sup(pilha *pi)
{
    int i;

    for(i=0;i<NUM_PILHAS;i++)
    {
        // mostra um espa�o vazio caso a pilha esteje vazia
        if(pilha_vazia(pi[i]))
            printf(" |  |  ");
        else
            printf(" |%c%c|  ",converte_num_simb(pi[i]->cartas[pi[i]->topo].numero),pi[i]->cartas[pi[i]->topo].naipe);
    }
    printf("\n");
}

// converte o numero da carta pelo seu respectibo "s�mbolo"
char converte_num_simb(tipo_chave numero)
{
    tipo_simbolo simbolo;

    switch(numero)
    {
    case 1:simbolo='A';
        break;
    case 2:simbolo='2';
        break;
    case 3:simbolo='3';
        break;
    case 4:simbolo='4';
        break;
    case 5:simbolo='5';
        break;
    case 6:simbolo='6';
        break;
    case 7:simbolo='7';
        break;
    case 8:simbolo='8';
        break;
    case 9:simbolo='9';
        break;
    case 10:simbolo='D';
        break;
    case 11:simbolo='J';
        break;
    case 12:simbolo='Q';
        break;
    case 13:simbolo='K';
        break;
    }

    return simbolo;
}

// menssagem da vit�ria \o/
void vitoria_msg()
{
    limpa_tela();
    printf("\n\n");
    printf("\n\t\t\t     o _|_   __   __  o  __  ");
    printf("\n\t\t\t(__| |  |_, (__) |  ' | (__( ");
    printf("\n\n");
    printf("\n\t\t                ������������");
    printf("\n\t\t                ��            ��");
    printf("\n\t\t  �����        ��                ��");
    printf("\n\t\t  �     �     ��      ��    ��     ��");
    printf("\n\t\t   �     �    ��       ��    ��      ��");
    printf("\n\t\t    �    �   ��        ��    ��      ��");
    printf("\n\t\t     �   �   �                         ��");
    printf("\n\t\t   ������������                         ��");
    printf("\n\t\t  �            �    ��            ��    ��");
    printf("\n\t\t ��            �    ��            ��    ��");
    printf("\n\t\t��   �����������      ��        ��     ��");
    printf("\n\t\t�               �       �������       ��");
    printf("\n\t\t��              �                    ��");
    printf("\n\t\t �   ������������                   ��");
    printf("\n\t\t ��           �  ��                ��");
    printf("\n\t\t ������������    ��            ��");
    printf("\n\t\t                 �����������");
    printf("\n\n");
    vitoria_img();
}
