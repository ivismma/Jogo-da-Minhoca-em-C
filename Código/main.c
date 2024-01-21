#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>

#include "header.h"

Consumivel pos; // Vari�vel global - Posi��o atual da ma��

int main(){
	setlocale(LC_ALL, "pt_BR.UTF-8");
	// Fun��es biblioteca ncurses
    initscr();
    clear();
    noecho();
    curs_set(0); 
    keypad(stdscr, TRUE); // leitura teclas especiais (Numpad)
    
	int direcao = KEY_DOWN;
	int anterior = KEY_DOWN;
	int tamanho = 3; // Tamanho da minhoca, inicia como 3.
	int tamanhoM = 3; // Tamanho m�ximo atingido.
	int cont = 0; // Movimentos
	bool jogar = true;
	// Aloca��o de mem�ria da minhoca.
	Minhoca *minhoca = (Minhoca*) calloc(WIDTH*HEIGHT, sizeof(Minhoca));
	if(minhoca == NULL){
		endwin();
		fprintf(stderr, "Falha na aloca��o de mem�ria.\n");
		return -1;
	}
	
	inicializarMinhoca(minhoca);
	Consumivel pos = gerarConsumivel(minhoca, tamanho);
    
    // Loop do jogo.
	while(jogar){
		clear();
		desenharParedes();
		desenharConsumivel(pos);
		desenharMinhoca(minhoca, tamanho); // Segmentos
		
		mostrarInfo();
		mostrarInfoStats(minhoca, tamanho, tamanhoM, cont);
		
		// Captar movimento:
		int tecla;
		do tecla = getch(); // Enquanto movimento for inv�lido.
		while(!checaMovimento(anterior, tecla)); 
		
		anterior = tecla;
		++cont; // Movs.
		usleep(10000);  // microsegundos
		
		// Posi��o do rabo (tail) da minhoca:
		int tailX = minhoca[tamanho-1].x;
		int tailY = minhoca[tamanho-1].y;
		
		// Computar tecla e verificar se pr�ximo movimento ter� colis�o com parede.
		bool colidiu = false;
		colidiu = preComputarMovimento(tecla, minhoca, tamanho, &direcao);
		if(!colidiu) movimentoNormal(direcao, minhoca, tamanho);
		
		// Checar se a minhoca morreu ap�s o movimento efetuado.
	    if(checaMorte(minhoca, tamanho)){
			jogar = selecionar();
			if(jogar) reiniciaJogo(minhoca, &tamanho, &cont, &anterior);
			continue;
		}
		// Checar se a minhoca comeu a ma�� com o movimento efetuado.
    	if(checaSeComeu(minhoca, pos)){
        	crescerMinhoca(minhoca, &tamanho, tailX, tailY);
        	pos = gerarConsumivel(minhoca, tamanho);
        	if(tamanho >= tamanhoM) tamanhoM = tamanho;
        }
        
		refresh(); // Atualizar tela
	}
    
    free(minhoca);
    endwin();
	printf("Fim do jogo.\n");
    getch();
    return 0;
}
