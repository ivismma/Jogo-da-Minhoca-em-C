// Fun��es que gerenciam o jogo.

#include <ncurses/ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

void desenharParedes(){
	// Paredes verticais
	for(int i = 0; i < HEIGHT+1;++i) mvprintw(i, 0, "#");
	for(int i = 1; i < HEIGHT+1;++i) mvprintw(i, WIDTH+1, "#");
	// Paredes horizontais
	for(int i = 1; i < WIDTH+2;++i) mvprintw(0, i, "#");
	for(int i = 0; i < WIDTH+2;++i) mvprintw(HEIGHT+1, i, "#");
}

bool checaColisao(Minhoca *minhoca, int x, int y){
	// Colis�o com parede vertical
	if(y == 0 || y >= HEIGHT+1) return true;
	// Colis�o com parede horizontal
	else if(x == 0 || x >= WIDTH+1) return true;

	else return false; // N�o haver� colis�o.
}

bool checaMorte(Minhoca *minhoca, int tam){
	// Pega pos. da cabe�a e verifica se atingiu o resto da minhoca.
	for(int i = 1; i < tam;++i){
		if(minhoca->x == minhoca[i].x && minhoca->y == minhoca[i].y)
			return true; // Colis�o
	}
	// A cabe�a da minhoca n�o colidiu com si mesma.
	return false;
}

Consumivel gerarConsumivel(Minhoca *minhoca, int tam){
	srand(time(NULL));
	bool disponivel; 
	// Tenta criar uma ma�� em um lugar dispon�vel (n�o ocupado pela minhoca)
	do{ 
		disponivel = true;
		pos.x = rand()%WIDTH + 1;     // Gerar coordenada dentro do mapa (+1 para evitar paredes).
		pos.y = rand()%HEIGHT + 1;
		for(int i = 0; i < tam; ++i){
			if(pos.x == minhoca[i].x && pos.y == minhoca[i].y)
				disponivel = false; // Coordenada gerada est� ocupada pela minhoca.
		}
	} while(!disponivel);
	// Retornar nova posi��o da ma�� gerada.
	return pos;
}

void desenharConsumivel(Consumivel pos){
	mvprintw(pos.y,pos.x, "-");
}

void mostrarInfo(){
	mvprintw(0,34,"Em desenvolvimento...");
	mvprintw(2,34,"Implementa��es que ainda faltam:");
	mvprintw(4,34,"Rein�cio da rodada ap�s morte, sistema de pontua��o, mensagem de fim de jogo.");
	mvprintw(5,34,"adicionar sons de efeito (SFX) e outros.");
	
	mvprintw(8,34,"Implementa��es j� criadas:");
	mvprintw(10,34,"Gera��o do mapa, Gera��o da ma��, defini��o de paredes, colis�o, ");
	mvprintw(11,34,"transferir minhoca para lado oposto da colis�o, prote��o p/ entrada inv�lida, ");
	mvprintw(12,34,"impedir minhoca de poder andar para sentido contr�rio (morte), ");
	mvprintw(13,34,"e sistema que controla a trajet�ria da minhoca (Vetor alocado dinamicamente).");
}

void mostrarInfoStats(Minhoca *minhoca, int tamanho, int cont){
	mvprintw(13,1,"Info. �teis para teste:");
	mvprintw(15,1,"Movimentos: %d", cont);
	mvprintw(16,1,"Tamanho:    %d (Cabe�a + %d segmentos)", tamanho, tamanho-1);
	mvprintw(18,1,"Rabo da minhoca: (%d,%d)", minhoca[tamanho-1].x, minhoca[tamanho-1].y);
	mvprintw(19,1,"Cabe�a: (%d,%d)", minhoca->x, minhoca->y);
	mvprintw(20,1,"Posi��o da comida: (%d,%d)", pos.x, pos.y);
	mvprintw(26,1,"Mem�ria alocada (*minhoca): %zu bytes.", (WIDTH*HEIGHT-1)*sizeof(minhoca));
}
