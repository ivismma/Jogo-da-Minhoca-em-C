#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>

#include "header.h"

#define DELAY 175000 // Velocidade do jogo | Quanto maior, mais lento.

// Vari�veis globais
Consumivel pos; //Posi��o atual da ma��
int WIDTH = 14; // Comprimento
int HEIGHT = 5; // Largura

int main(){
	setlocale(LC_ALL, "pt_BR.UTF-8");
	// Acentos e cedilhas funcionar�o a depender do S.O e fatores externos envolvidos.
	
	// Fun��es biblioteca ncurses
    initscr();
    clear();
    noecho(); 
    curs_set(0);
    keypad(stdscr, TRUE); // leitura das teclas
    nodelay(stdscr, TRUE); // Esta fun��o far� com que o jogo continue (loop) independente se h� input ou n�o
    
	int direcao = KEY_DOWN;
	int anterior = KEY_DOWN;
	int tamanho = 3; // Tamanho da minhoca, inicia como 3.
	int tamanhoM = 3; // Tamanho m�ximo atingido.
	int cont = 0; // Movimentos
	bool jogar = true;
	// Aloca��o de mem�ria da minhoca.
	Minhoca *minhoca = (Minhoca*) malloc(WIDTH*HEIGHT * sizeof(Minhoca));
	if(minhoca == NULL){
		endwin();
		fprintf(stderr, "Falha na aloca��o de mem�ria.\n");
		return -1;
	}
	
    selecionarMenu();
    inicializarMinhoca(minhoca);
    pos = gerarConsumivel(minhoca, tamanho);
    
    int tecla = KEY_DOWN;
    // Loop do jogo.
	
	while(jogar){
		erase();
		desenharParedes();
		desenharConsumivel(pos);
		desenharMinhoca(minhoca, tamanho); // Segmentos
		mostrarInfo();
		mostrarInfoStats(minhoca, tamanho, tamanhoM, cont);
		refresh();
		usleep(DELAY);  // microsegundos
		
		// Captar movimento:
		tecla = getch();
		if(!checaMovimento(anterior, tecla))
			tecla = anterior;
		anterior = tecla;
		++cont; // Movs.
		
		// Limpar buffer:
		char c;
		while( (c = getch() ) != EOF);
		
		// Posi��o atual do rabo (tail) da minhoca:
		int tailX = minhoca[tamanho-1].x;
		int tailY = minhoca[tamanho-1].y;
		
		// Computar tecla e verificar se pr�ximo movimento ter� colis�o com parede.
		bool colidiu = preComputarMovimento(tecla, minhoca, tamanho, &direcao);
		if(!colidiu) movimentoNormal(direcao, minhoca, tamanho);
		
		// Checar se a minhoca morreu ap�s o movimento efetuado.
	    if(checaMorte(minhoca, tamanho)){
			jogar = selecionarReinicio(false);
			if(jogar) reiniciaJogo(minhoca, &tamanho, &cont, &anterior);
			continue;
		}
		
		// Checar se a minhoca comeu a ma�� com o movimento efetuado e 
		// checa se o vencedor comeu a �ltima ma�� do jogo (venceu).
    	if(checaSeComeu(minhoca, pos)){
        	crescerMinhoca(minhoca, &tamanho, tailX, tailY);
        	if(tamanho >= tamanhoM) tamanhoM = tamanho;
			
			// Checa se a minhoca n�o atingiu seu limite (n�o terminou o jogo).
			if(tamanho != WIDTH*HEIGHT) pos = gerarConsumivel(minhoca, tamanho);
			// Comeu a �ltima ma�� e venceu o jogo:
			else{ 
				jogar = selecionarReinicio(true); 
				if(jogar) reiniciaJogo(minhoca, &tamanho, &cont, &anterior);
			}
        }
	}
	
    free(minhoca);
    endwin();
	printf("Fim do jogo.\n");
    getch();
    
	return 0;
}
