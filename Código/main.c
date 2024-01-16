// Tamanho: Pequeno: 14 e 5, M�dio: 20 e 6, Grande: 25 e 7.
// Default: M�dio
#define WIDTH 20   // Tam Horizontal
#define HEIGHT 6   // Tam Vertical

#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>
#include "header.h"

Consumivel pos; // Posi��o da ma�� - Vari�vel global.

int main() {
	setlocale(LC_ALL, "pt_BR.UTF-8");
	// Fun��es biblioteca ncurses
    initscr(); 
    clear();
    noecho(); 
    curs_set(0); 
    keypad(stdscr, TRUE); // leitura teclas especiais
	int direcao = KEY_RIGHT;
	bool comeu = false;
	int tamanho = 2; // Tamanho da minhoca, inicia como 2.
	int cont = -1;
	Minhoca *minhoca = (Minhoca*) calloc(WIDTH*HEIGHT, sizeof(Minhoca));
	if(minhoca == NULL){
		endwin();
		fprintf(stderr, "Falha na aloca��o de mem�ria.\n");
		return -1;
	}
	inicializarMinhoca(minhoca);
	Consumivel pos = gerarConsumivel(minhoca, tamanho);
    
	while(1) {
		++cont;
		// Desenhar parede, minhoca, atualizar tela
        clear();
		desenharParedes();
		
		desenharConsumivel(pos);
		mvprintw(minhoca->y, minhoca->x, "0"); // Cabe�a
		desenharMinhoca(minhoca, tamanho); // Segmentos
		
		mvprintw(13,1,"Info. �teis para teste:");
		mvprintw(15,1,"Movimentos: %d", cont);
		mvprintw(16,1,"Tamanho:    %d (Cabe�a + %d segmentos)", tamanho, tamanho-1);
		mvprintw(18,1,"Rabo da minhoca: (%d,%d)", minhoca[tamanho-1].x, minhoca[tamanho-1].y);
		mvprintw(19,1,"Cabe�a: (%d,%d)", minhoca->x, minhoca->y);
		mvprintw(20,1,"Posi��o da comida: (%d,%d)", pos.x, pos.y);
		mvprintw(26,1,"Mem�ria alocada (*minhoca): %zu bytes.", (WIDTH*HEIGHT-1)*sizeof(minhoca));
		mostrarInfo();
        
        refresh();
        
        // Capturar movimento
		int tecla = getch();
		// Prote��o contra entrada inv�lida, fazendo com que a minhoca atravesse paredes e
		// e tenha um movimento inconsistente.
		if(tecla != KEY_UP && tecla != KEY_DOWN &&
		tecla != KEY_LEFT && tecla != KEY_RIGHT) continue;
		
		usleep(10000);  // microsegundos
		
		switch (tecla) {
            case KEY_UP:
                if(checaColisao(minhoca, minhoca->x, minhoca->y-1)){
                	atualizarMinhoca(minhoca, &tamanho);
					minhoca->y += HEIGHT-1;
					goto checarSeComeu;
					// Esse goto possui a mesma fun��o do "continue;", exceto que ele verifica se a
					// minhoca comeu antes de ir para pr�xima itera��o do jogo.
				}
				else{
					direcao = KEY_UP;
                	break;
				}
            case KEY_DOWN:
            	if(checaColisao(minhoca, minhoca->x, minhoca->y+1)){
            		atualizarMinhoca(minhoca, &tamanho);
					minhoca->y -= HEIGHT-1;
					goto checarSeComeu;
				}
				else{
					direcao = KEY_DOWN;
                	break;
				}
            case KEY_LEFT:
            	if(checaColisao(minhoca, minhoca->x-1, minhoca->y)){
            		atualizarMinhoca(minhoca, &tamanho);
					minhoca->x += WIDTH-1;
					goto checarSeComeu;
				}
				else{
					direcao = KEY_LEFT;
                	break;
				}
            case KEY_RIGHT:
            	if(checaColisao(minhoca, minhoca->x+1, minhoca->y)){
            		atualizarMinhoca(minhoca, &tamanho);
					minhoca->x -= WIDTH-1;
					goto checarSeComeu;
				}
				else{
					direcao = KEY_RIGHT;
                	break;
				}
        }
		
        // Atualizar a posi��o da minhoca com base na dire��o
        switch (direcao) {
            case KEY_UP:
				atualizarMinhoca(minhoca, &tamanho);
				--(minhoca->y);
				if(checaMorte(minhoca, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
					getch();
					return 0;
				}
                break;
            case KEY_DOWN:
            	atualizarMinhoca(minhoca, &tamanho);
            	++(minhoca->y);
            	if(checaMorte(minhoca, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
    				getch();
					return 0;
				}
                break;
            case KEY_LEFT:
            	atualizarMinhoca(minhoca, &tamanho);
            	--(minhoca->x);
            	if(checaMorte(minhoca, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
    				getch();
					return 0;
				}
                break;
            case KEY_RIGHT:
            	atualizarMinhoca(minhoca, &tamanho);
            	++(minhoca->x);
            	if(checaMorte(minhoca, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
					getch();
					return 0;
				}
                break;
        }
        // goto com a fun��o do continue, por�m, verificar antes se a minhoca comeu.
        checarSeComeu:
	    if (checaSeComeu(minhoca, pos)){
			crescerMinhoca(minhoca, &tamanho);
			pos = gerarConsumivel(minhoca, tamanho);
		}
	}
    
    return 0;
}
