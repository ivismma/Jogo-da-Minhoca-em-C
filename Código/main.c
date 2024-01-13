#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define WIDTH 25   // Tam Horizontal
#define HEIGHT 10  // Tam Vertical

typedef struct{
	int x, y;
} Minhoca;
/* 
Struct que ser� alocada din�micamente para conter as posi��es em que se 
encontram as partes da minhoca. Ser� alocado 249 espa�os no vetor din�mico 
(WIDTH*HEIGHT-1), porque 250 � o tamanho total de espa�os dentro do jogo e 
uma delas j� � a cabe�a da minhoca.
*/ 

void desenharParedes();
void desenharMinhoca(Minhoca *minhoca, int tam);
void atualizarMinhoca(Minhoca *minhoca, int x, int y, int *tam, bool *cresceu);
bool checaColisao(int x, int y); // Colis�o com paredes.
bool checaMorte(Minhoca *minhoca, int x, int y, int tamanho); // Colis�o em si.
void mostrarInfo();

int main() {
	setlocale(LC_ALL, "pt_BR.UTF-8");
	
	// Fun��es biblioteca ncurses
    initscr(); 
    clear();
    noecho(); 
    curs_set(0); 
    keypad(stdscr, TRUE); // leitura teclas especiais
    
    // Vari�veis importantes
	int x = WIDTH / 2;
    int y = HEIGHT / 2;
	int direcao = KEY_RIGHT;
	bool cresceu = false;
	Minhoca *minhoca = (Minhoca*) calloc(WIDTH*HEIGHT-1, sizeof(Minhoca));
	minhoca[0].x = x; 
	minhoca[0].y = y-1;
	int tamanho = 1; // Tamanho da minhoca.
	int cont = 0;
	////////////////////
    
	while(1) {
		// Desenhar parede, minhoca, atualizar tela
        clear();
		desenharParedes();
		
		mvprintw(y, x, "0"); // Cabe�a
		desenharMinhoca(minhoca, tamanho); // Segmentos
		
		
		mvprintw(12,2,"Info. �teis para teste:");
		mvprintw(14,2,"Cont: %d", cont);
		mvprintw(15,2,"Tamanho: %d", tamanho);
		mvprintw(16,2,"Rabo da minhoca: (%d,%d)", minhoca[tamanho-1].x, minhoca[tamanho-1].y);
		mvprintw(17,2,"Mem�ria alocada (*minhoca): %zu bytes.", (WIDTH*HEIGHT-1)*sizeof(minhoca));
		mostrarInfo();
		
		refresh();
        
        // Capturar movimento
		int key = getch();
		usleep(10000);  // microsegundos
		
		// Atribuir nova dire��o (se n�o for atingir a parede)
		// Reiniciar� loop caso a minhoca colida com a parede.
		if(cont%6 == 0) cresceu = true;
		
		switch (key) {
            case KEY_UP:
                if(checaColisao(x, y-1)){
                	atualizarMinhoca(minhoca, x, y, &tamanho, &cresceu);
					y += HEIGHT-2;
					continue;
				}
				else{
					direcao = KEY_UP;
                	break;
				}
            case KEY_DOWN:
            	if(checaColisao(x, y+1)){
            		atualizarMinhoca(minhoca, x, y, &tamanho, &cresceu);
					y -= HEIGHT-2;
					continue;
				}
				else{
					direcao = KEY_DOWN;
                	break;
				}
            case KEY_LEFT:
            	if(checaColisao(x-1, y)){
            		atualizarMinhoca(minhoca, x, y, &tamanho, &cresceu);
					x += WIDTH-2;
					continue;
				}
				else{
					direcao = KEY_LEFT;
                	break;
				}
            case KEY_RIGHT:
            	if(checaColisao(x+1, y)){
            		atualizarMinhoca(minhoca, x, y, &tamanho, &cresceu);
					x -= WIDTH-2;
					continue;
				}
				else{
					direcao = KEY_RIGHT;
                	break;
				}
        }
		
        // Atualizar a posi��o da minhoca com base na dire��o
        switch (direcao) {
            case KEY_UP:
				atualizarMinhoca(minhoca, x, --y+1, &tamanho, &cresceu);
				if(checaMorte(minhoca, x, y, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
					getch();
					return 0;
				}
				cont++;
                break;
            case KEY_DOWN:
            	atualizarMinhoca(minhoca, x, ++y-1, &tamanho, &cresceu);
            	if(checaMorte(minhoca, x, y, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
    				getch();
					return 0;
				}
				cont++;
                break;
            case KEY_LEFT:
            	atualizarMinhoca(minhoca, --x+1, y, &tamanho, &cresceu);
            	if(checaMorte(minhoca, x, y, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
    				getch();
					return 0;
				}
				cont++;
                break;
            case KEY_RIGHT:
            	atualizarMinhoca(minhoca, ++x-1, y, &tamanho, &cresceu);
            	if(checaMorte(minhoca, x, y, tamanho)){
					free(minhoca);
    				endwin();
    				printf("Minhoca morreu :(\n");
					getch();
					return 0;
				}
				cont++;
                break;
        }
	}
    
    return 0;
}

void desenharParedes(){
	// Paredes verticais
	for(int i = 0; i < HEIGHT;++i) mvprintw(i, 0, "#");
	for(int i = 0; i < HEIGHT;++i) mvprintw(i, WIDTH, "#");
	// Paredes horizontais
	for(int i = 1; i < WIDTH;++i) mvprintw(0, i, "#");
	for(int i = 0; i < WIDTH+1;++i) mvprintw(HEIGHT, i, "#");
}

void desenharMinhoca(Minhoca *minhoca, int tam){
	for(int i = 0; i < tam; ++i){
		int x = (minhoca+i)->x;
		int y = (minhoca+i)->y;
		mvprintw(y, x, "O");
	}
}

// x, y correspondem � posi��o atual que se encontrava a cabe�a da minhoca.
void atualizarMinhoca(Minhoca *minhoca, int x, int y, int *tam, bool *cresceu) {
    for (int i = *tam - 1; i > 0; --i)
        minhoca[i] = minhoca[i-1];
	// Atualizando a cabe�a da minhoca
    minhoca[0].x = x;
    minhoca[0].y = y;
    // Verificar se a minhoca comeu a ma��, se sim, aumentar seu tamanho.
    if (*cresceu) {
        ++(*tam);
        minhoca[*tam - 1].x = minhoca[*tam - 2].x;
        minhoca[*tam - 1].y = minhoca[*tam - 2].y;
        *cresceu = false;
    }
}

bool checaMorte(Minhoca *minhoca, int x, int y, int tam){
	for(int i = 0; i < tam;++i){
		if(x == minhoca[i].x && y == minhoca[i].y)
			return true; // Colis�o
	}
	// A cabe�a da minhoca n�o colidiu com si mesma.
	return false;
}

// x deve ser <=  WIDTH  !!
// y deve ser <=  HEIGHT !!
bool checaColisao(int x, int y){
	// Colis�o com parede vertical
	if(y == 0 || y >= HEIGHT) return true;
	// Colis�o com parede horizontal
	else if(x == 0 || x >= WIDTH) return true;

	else return false; // N�o haver� colis�o.
}


void mostrarInfo(){
	mvprintw(0,34,"Em desenvolvimento...");
	mvprintw(2,34,"Implementa��es que ainda faltam:");
	mvprintw(4,34,"Gera��o de ma�� no mapa, melhorias e modulariza��o/organiza��o do c�digo,");
	mvprintw(5,34,"impedir minhoca de se movimentar a si mesmo (morte),");
	mvprintw(6,34,"rein�cio da rodada ap�s morte, Sistema de pontua��o e outros.");
	mvprintw(7,34,"corrigir bug: controlar pelo Numpad faz com que minhoca atravesse parede.");
	
	mvprintw(9,34,"Para efeito de teste, a minhoca cresce a cada 6 movimentos.");
	mvprintw(11,34,"Implementa��es j� criadas:");
	mvprintw(13,34,"Gera��o do mapa, defini��o de paredes, colis�o, ");
	mvprintw(14,34,"transferir minhoca para lado oposto da colis�o,");
	mvprintw(15,34,"sistema que gerencia as partes da minhoca (Vetor alocado dinamicamente).");
}
