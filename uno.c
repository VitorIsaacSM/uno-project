
// trabalho - lapro 1 - UNO
// Vitor Isaac Sitton Maia

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int drawTimes = 0;
int turno = 0;
int num = 2; // num � o numero de cartas que se deve comprar
int num4 = 4 ;


struct card{
	
	int val ;
	int color;
	
	
};

void print(struct card * vet); // imprime todos os elementos do baralho
void sort(struct card *n); // organiza o vetor de structs que eh o baralho, adicionando o valor das cartas em cada uma das 108 cartas
void shuffle(struct card * o, struct card * n);  // recebe 2 vetores , deixando o segundo como uma versao embaralhada do primeiro sem altera-lo
void draw(struct card * vet, struct card * d, int tam); // retorna a primeira carta do baralho e move as cartas do baralho reduzindo  1 em sua posicao
void begin(struct card * vet, struct card * h1, struct card * h2, int * nh1 , int * nh2); // distribui 7 cartas para cada jogador 
void first(struct card * vet, struct card * d,struct card * h1,int * nh1); // puxa a primeira carta da mesa
void Pturn(struct card * vet, struct card * h1, struct card * top, int * nh1, struct card * h2,int * nh2); // funcao que simula o seu turno
void Bturn(struct card * vet, struct card * h1, struct card * top, int * nh1, struct card * h2,int * nh2); // funcao que simula o turno do bot
void printC(struct card a); // recebe uma carta no formato bruto e printa ela decodificando os valores inteiros das cores para palavras
void play(struct card * h1, struct card  * top, int n,int * nh1, struct card * h2, int * nh2,struct card * vet); // joga uma carta
int decideC(struct card * h1 , int * nh1); // usada para quando o bot precisa decidir qual cor escolher ao usar um curinga
void skip();// pula um turno
int find(int val,struct card * h1,int * nh1,int * av); // acha se existe alguma carta que pode ser usada para rebater um +2 ou +4
void plus2(struct card * vet, struct card * h1,int * nh1, struct card * h2, int * nh2, struct card * top); // funcao para quando um +2 e jogado
void plus4(struct card * vet, struct card * h1,int * nh1, struct card * h2, int * nh2, struct card * top); // funcao para quando um +4 e jogado
void end(struct card * h1,int * nh1,struct card * h2, int * nh2); // funcao chamada para quando o baralho de compras acaba, decide quem venceu


int main(void){
	
	struct card n[108]; // baralho com as cartas em ordem - inalterado
	struct card b[108]; // baralho embaralhado
	struct card hand1[108]; // sua mao
	struct card hand2[108]; // mao do oponente
	struct card top[1];  // carta do topo da mesa na posi�ao 0
	char nome[20];  // nome do jogador
	
	top[0].color = 0;
	
	int nh1 = 0, nh2 = 0; // variaveis que contam quantas cartas existem na sua mao e na do bot
	
	int * ph1 = &nh1; // ponteiros para as variaveis que contam as cartas da mao
	int * ph2 = &nh2; // usado ao passar o numero de cartas dao mao para funcoes
	
	srand( (unsigned)time( NULL ) ); // reseta o seed do fator randomico
	

	printf("\ndigite o seu nome:\n");
	fgets(nome,20,stdin);
	
	sort(n);  // "cria" o baralho, com todas as cartas, em ordem
	
	shuffle(n,b); // embaralha o baralho que foi criado
	
	begin(b,hand1,hand2,ph1,ph2); // distribui 7 cartas do topo da pilha para o jogador e para o bot
	
	printf("\nComecou o jogo!  voce eh o primeiro a jogar %s \n",nome);
	
	first(b,top,hand1,ph1); // puxa a primeira carta do topo da pilha que vai para a mesa
	
	printf(" \n");
	printf(" \n");
	
	while((nh1!=0)&&(nh2!=0)){

		// enquanto nenhum jogador chegar a 0 cartas na mao este laco continua rodando
	
		num = 2; // serve para resetar o valor de cartas a se comprar ao usar um +2.
        //e necessario pois caso um +2 usado seja rebatido o valor da variavel precisa voltar ao normal no proximo turnostruct card * h2,int * nh2)
		num4 = 4; // igual a num porem para o +4
		
		
		// turno pode ser 0 ou 1, sendo 0 a vez do jogador e 1 a do bot
		// ao fim de cada jogada o valor de turno eh alterado por isso foi utilizado uma variavel global

		switch(turno){
			
			case 0:Pturn(b,hand1,top,ph1,hand2,ph2);continue;break;
			case 1:Bturn(b,hand2,top,ph2,hand1,ph1);continue;break;
			
		}
		
		// se 108 cartas forem puxadas do topo da pilha a funcao end eh chamada e o jogo acaba		
		
		if(drawTimes == 108){
			end(hand1,ph1,hand2,ph2);
			break;
		}
	
	}
	
	
	
	printf(" \n");
	
	if(nh1 == 0){
		printf("parabens voce venceu! \n");
		
	}
	
	if(nh2 == 0){
		printf("voce perdeu \n\n\n");
	}
	
	// printf(" %d cartas foram compradas \n",drawTimes);
	
	return 0;
}


void skip(){
	
	if(turno == 0){
		turno--;
		printf("\nseu oponente perdeu o turno \n");
	}
	
	if(turno == 1){
		turno++;
		printf("\nvoce perdeu o turno \n");
		getchar();
	}
	
}

int find(int val,struct card * h1,int * nh1,int * av){
	
	int i,j = 0;
	av[0] = -1; // serve para checar se av foi alterado
	
	for(i=0; i<*nh1;i++){
		
		if( h1[i].val == val ){
			
			av[j] = i;
			j++;
			
		}
	}
	
	if(av[0] == -1){
		
		return 0;
		
	}
	
	return j;
}

void plus4(struct card * vet, struct card * h1,int * nh1, struct card * h2, int * nh2, struct card * top){
	int i;
	 // vetor que armazena as posi�oes da sua mao ondem existem +2's disponiveis para serem jogados ao rebater
	
	 // numero de cartas +2 disponiveis para se rebater
	int k; // opcao escolhida para se rebater
	
	if(turno == 0){
		
		int	av[10];
		
		int j = find(21,h2,nh2,av);
	
		
		if(j==0){
				
			for(i = 0; i<num4; i++){
					
				draw(vet,h2,*nh2);
				*nh2 += 1;
			}
				
			printf("\nseu oponente comprou %d cartas e perdeu a vez \n",num4);
			
			if(num4 == 4){
				turno--;
			}
					
		}
			
			
		else{
			
			num4 = num4 + 4;
			turno++;
			printf("\nseu oponente rebateu seu +4 \n");
			play(h2,top,av[0],nh2,h1,nh1,vet);
			turno--;	
		}
			
	}
	
	if(turno == 1){
		
		int	av[10];
		
		int j = find(21,h1,nh1,av);
		
		if(j==0){
				
			for(i = 0; i<num4; i++){
					
				draw(vet,h1,*nh1);
				*nh1 += 1;
			}
				
			printf("\nvoce comprou %d cartas e perdeu a vez \n",num4);
			
			if(num4 == 4){
				turno++;
			}	
		}
		
		else{
			
			printf("\nvoce pode rebater o +4 que foi jogado: \n");
			
			for(i = 0; i<j; i++){
				
				printf("- %d. jogar: ",i);
				printC(h1[av[i]]);
				
			}
			printf("-%d. nao rebater \n",i);
			
			scanf("%d",&k);
			getchar();
			
			if(k <= j){
				
				k = av[k];
				num4 = num4 + 4;
				printf("\nvoce rebateu o +4 do oponente\n");
				turno--;
				play(h1,top,k,nh1,h2,nh2,vet);
				turno++;
				
			}
			if(k > j){
				
				for(i = 0; i<num4; i++){
					
					draw(vet,h1,*nh1);
					*nh1 += 1;
					
				}
				
				printf("\nvoce comprou %d cartas e perdeu a vez \n",num4);
				
				if(num4 == 4){
					turno++;
				}
			
			}
			
		}
		
		
	}	
			
}

void plus2(struct card * vet, struct card * h1,int * nh1, struct card * h2, int * nh2, struct card * top){
	
	
	int i;
	 // vetor que armazena as posi�oes da sua mao ondem existem +2's disponiveis para serem jogados ao rebater
	
	 // numero de cartas +2 disponiveis para se rebater
	int k; // opcao escolhida para se rebater
	
	if(turno == 0){
		
		int	av[10];
		
		int j = find(12,h2,nh2,av);
	
		if(j==0){
				
			for(i = 0; i<num; i++){
					
				draw(vet,h2,*nh2);
				*nh2 += 1;
			}
				
			printf("\nseu oponente comprou %d cartas e perdeu a vez \n",num);
			if(num == 2){
				turno--;
			}
		}
			
			
		else{
	
			num = num + 2;
			turno++;
			printf("\nseu oponente rebateu seu +2 \n");
			play(h2,top,av[0],nh2,h1,nh1,vet);
			turno--;
				
		}
			
	}
	
	if(turno == 1){
		
		int	av[10];
		
		int j = find(12,h1,nh1,av);
		
		if(j==0){
					
			for(i = 0; i<num; i++){
					
				draw(vet,h1,*nh1);
				*nh1 += 1;
			}
				
			printf("\nvoce comprou %d cartas e perdeu a vez \n",num);
			
			if(num == 2){
				turno++;
			}
		}
		
		else{
			
			printf("\nvoce pode rebater o +2 que foi jogado: \n");
			
			for(i = 0; i<j; i++){
				
				printf("- %d. jogar: ",i);
				printC(h1[av[i]]);
				
			}
			printf("-%d. nao rebater \n",i);
			
			scanf("%d",&k);
			getchar();
			
			if(k <= j){
				
				k = av[k];
				num = num + 2;
				printf("\nvoce rebateu o +2 do oponente\n");
				turno--;
				play(h1,top,k,nh1,h2,nh2,vet);
				turno++;
		
			}
			
			if(k > j){
				
				for(i = 0; i<num; i++){
					
					draw(vet,h1,*nh1);
					*nh1 += 1;
					
				}
				
				printf("\nvoce comprou %d cartas e perdeu a vez \n",num);
				
				if(num == 2){
					turno++;
				}
			
			}
			
		}
		
		
	}	
	
}


void play(struct card * h1, struct card  * top, int n,int * nh1, struct card * h2, int * nh2,struct card * vet){
	
	int cor = 0;
 	int av[10]; // vai ser utilizado para caso um +2 ou +4 seja jogado e voce possa rebater ele com um da sua mao
	
	if(turno == 0){
	
		if(h1[n].color >= 5){
			
			printf("qual cor voce deseja? \n");
			printf("digite 1 para azul.\ndigite 2 para amarelo.\ndigite 3 para verde.\ndigite 4 para vermelho.\n");
			while( (cor< 1) || (cor>4)){
				scanf("%d",&cor);
				getchar();
				
				if((cor< 1) || (cor>4)){
					
					printf("entrada invalida digite outra vez. \n");
					
				}	
			}
			
			h1[n].color = cor;
			
		}
		
		top[0] = h1[n];
		int i;
		
		for(i=n; i<(*nh1)-1 ; i++){
			
			h1[i] = h1[i+1];
			
		}
		
		*nh1 -= 1;
		
		printf("voce jogou um ");
		printC(top[0]);
		
		if(top[0].val == 12){
			
			plus2(vet,h1,nh1,h2,nh2,top);
			
		}
		
		if(top[0].val == 21){
			
			plus4(vet,h1,nh1,h2,nh2,top);
			
		}
	}
	
	else{
		
		
		if(h1[n].color >= 5){
		
			cor = decideC(h1,nh1);
			
			h1[n].color = cor;
		
		}
		
		top[0] = h1[n];
		int i;
		
		for(i=n; i<(*nh1)-1 ; i++){
			
			h1[i] = h1[i+1];
			
		}
		
		*nh1-=1;
		
		printf("o oponente jogou um ");
		printC(top[0]);
		printf("\n\n");
		
		if(top[0].val == 12){
			
			plus2(vet,h2,nh2,h1,nh1,top);
			
		}
		
		if(top[0].val == 21){
			
			plus4(vet,h2,nh2,h1,nh1,top);
			
		}
			
	}
	
	if((top[0].val == 10) || (top[0].val == 11)){
			skip();
	}
	
}

int decideC(struct card * h1 , int * nh1){
	
	int b,y,g,r,maior = 0,i;
	
	for(i = 0; i<*nh1; i++){
		
		switch(h1[i].color){
			
			case 1: b++;break;
			case 2: y++;break;
			case 3: g++;break;
			case 4: r++;break;
			
			
		}
	}
	
	if(b>maior)
		maior = b;
	
	if(y>maior)
		maior = y;
		
	if(g>maior)
		maior = g;
			
	if(r>maior)
		maior = r;
	
	if(maior == b)
		return 1;

	else
		if(maior == y)
			return 2;
			
		else
			if(maior == g)	
				return 3;
				
			else
				return 4;	
	
}

void end(struct card * h1,int * nh1,struct card * h2, int * nh2){
	
	int num1=0,num2=0;
	int i;
	
	for(i=0; i<*nh1; i++){
		
		if((h1[i].val == 21) || (h1[i].val == 20))
			num1 += 50;
		
		else
			if( (h1[i].val == 10)|| (h1[i].val == 11)|| (h1[i].val == 12) )
				num1 += 20;
				
			else
				num1 = num1 + h1[i].val;
	}
	
	for(i=0; i<*nh2; i++){
		
		if((h2[i].val == 21) || (h2[i].val == 20))
			num2 += 50;
		
		else
			if( (h2[i].val == 10)|| (h2[i].val == 11)|| (h2[i].val == 12) )
				num2 += 20;
				
			else
				num2 = num2 + h1[i].val;
				
			
	}
	
	printf("a sua pontuacao eh: %d \n",num1);
	printf("a pontuacao do oponent eh: %d \n",num2);
	
	if(num1 > num2){
		
		printf("logo, voce perdeu. \n");
		return ;
		
	}
	
	if(num1 < num2){
		
		printf("logo, voce venceu. \n");
		return ;
		
	}
	
	printf("logo, eh um empate. \n");
	return ;
	
}

void Bturn(struct card * vet, struct card * h1, struct card * top, int * nh1, struct card * h2,int * nh2){
	
	int i,j = 1,k;
	int av[40];
	struct card check = top[0];
	struct card selected;
	
	printf("------------------------------------------- \n\n");
	printf("eh a vez de seu oponente \n"); 
	getchar();
	
	
	/*
	printf("a mao do seu oponente atualmente eh: \n\n");
	
	for(i=0; i<*nh1; i++){
		
		printC(h1[i]);
		
	}   */
	
	// a parte comentada serve para mostrar a mao do bot , usado para checar se o funcionamento esta ok
	
	for(i=0; i<*nh1; i++){
		
		if( (h1[i].color == top[0].color) || (h1[i].val == top[0].val) || (h1[i].color >= 5) ){
			
			av[j] = i;
			j++;
			
		}
			
	}
	
	if(j==1){
		
		draw(vet,h1,*nh1);
		*nh1 += 1;
		printf("seu oponente comprou uma carta \n\n");
		
		
	}
	
	if(j>1){
	
		k = av[1];
	
		play(h1,top,k,nh1,h2,nh2,vet);
		
	}
	
	
	
	printf("seu oponente tem %d cartas na mao \n",*nh1);
	printf("\n \n");
	turno--;
	
}

void Pturn(struct card * vet, struct card * h1, struct card * top, int * nh1, struct card * h2,int * nh2){
	
	int i,j = 1,k = 0;
	int av[40];
	struct card check = top[0];
	struct card selected;
	
	printf("------------------------------------------- \n\n");
	printf("eh sua vez de jogar \n");
	getchar();
	
	
	printf("a carta do topo da pilha eh: ");
	printC(top[0]);
	
	printf("\nvoce tem %d cartas na mao\n",*nh1);
	printf("sua mao atualmente eh: \n\n");
	
	for(i=0; i<*nh1; i++){
		
		printf(" - ");
		printC(h1[i]);
		
	}
	
	
	printf("\nescolha uma opcao para jogar: \n\n");
	
	for(i=0; i<*nh1; i++){
		
		if( (h1[i].color == top[0].color) || (h1[i].val == top[0].val) || (h1[i].color >= 5) ){
			
			
			printf(" . %d  ",j);
			printC(h1[i]);
			av[j] = i;
			j++;
			
		}
			
	}
	if(j==1){
		printf("voce nao tem nenhuma carta que possa ser jogada, deve comprar uma \n");
		getchar();
		draw(vet,h1,*nh1);
		*nh1 += 1;
		printf("voce comprou um: \n");
		printC(h1[*nh1-1]);
		getchar();
		
	}
	
	if(j>1){
	
		printf(" . %d comprar uma carta ",j);
		
		printf("\n \n");
		
		while((k<1)||(k>j)){
		
	
			scanf("%d",&k);
			getchar();
			
			if((k<1)||(k>j)){
				printf("entrada invalida digite novamente \n");
			}
		
		}
		
		if(k==j){
			
			draw(vet,h1,*nh1);
			*nh1 += 1;
			printf("voce comprou um: \n");
			printC(h1[*nh1-1]);
			getchar();
			
			
		}
		
		else{
		
			k = av[k];
	
			play(h1,top,k,nh1,h2,nh2,vet);
			
		}
		
	}

	printf("\n \n");
	turno++;
}


void begin(struct card * vet, struct card * h1, struct card * h2, int * nh1 , int * nh2){
	
	int i;
	
	for(i=0; i<7; i++){
		
		draw(vet,h1,*nh1);
		*nh1 = *nh1 + 1;
		
		draw(vet,h2,*nh2);
		*nh2 = *nh2 + 1;
	}
}

void first(struct card * vet, struct card * d,struct card * h1,int * nh1){
	
	
	while( (d[0].color == 0) || (d[0].color >= 5)  ){
		draw(vet,d,0);
	}

	if(d[0].val == 12){

	printf("a carta da mesa eh: ");
	printC(d[0]);
		
	printf("\na primeira carta foi um +2, logo o primeiro jogador a jogar compra 2 cartas e perde a vez\n");
	
		draw(vet,h1,*nh1);
		*nh1 += 1;

		draw(vet,h1,*nh1);
		*nh1 += 1;

		turno++;

		return;

	}

	if( (d[0].val == 10) || (d[0].val == 11) ){

		printf("a carta da mesa eh: ");
		printC(d[0]);
	
		printf("\na primeira carta foi um pula/inverte, logo o primeiro jogador perde a vez\n");
	
		turno++;

		return;
	
	}

	else{

		printf("a carta da mesa eh: ");
		printC(d[0]);
		return ;

	}
}


void draw(struct card * vet, struct card * d, int tam){
	
	int i;
	
	
	if(drawTimes < 108){
		
		
		d[tam] = vet[0];
	
		for(i=0; i<107-drawTimes;i++){
		
			vet[i].val = vet[i+1].val;
	
			vet[i].color = vet[i+1].color;
	
		}
	
		drawTimes++;
	}
}

void printC(struct card a){
	
	char cor[12];
	
	switch(a.color){
		
		case 1 : strcpy(cor,"azul"); break;
		case 2 : strcpy(cor,"amarelo"); break;
		case 3 : strcpy(cor,"verde"); break;
		case 4 : strcpy(cor,"vermelho"); break;
		default : strcpy(cor,"preto");

	}
	
		
	if(a.val == 20){	
		printf("coringa  %s  \n",cor);	
	}
		
	if(a.val == 21){	
		printf("+4  %s  \n",cor);
	}
	
	if(a.color < 5){
		
		if(a.val == 10){
			
			printf("pula  %s  \n",cor);
			
		}
		
		else{
			
			if(a.val == 11){
				
				printf("inverte  %s  \n",cor);
				
			}
			
			else{
				
					if(a.val == 12){
					
						printf("+2  %s  \n",cor);
					
					}
				
					else{
					
						if((a.val != 20) && (a.val != 21))
							printf("%d  %s  \n",a.val,cor);
					
					}
							
			}
		
		}
	}
	
}


void shuffle(struct card * o, struct card * n){
	
	int i;
	int randVal;
	int aux;
	
	// copia o vetor que contem todas as cartas para o vetor que ira conter todas as cartas por�m embaralhadas
	
	for(i=0; i<108; i++){
		
		n[i].val = o[i].val;
		n[i].color = o[i].color;
		
	}
	
	for(i=0; i<108; i++){
		
		randVal = rand() % 108;
		
		aux = n[i].val;
		n[i].val = n[randVal].val;
		n[randVal].val = aux;
		
		aux = n[i].color;
		n[i].color = n[randVal].color;
		n[randVal].color = aux;
		
	}
	
}

void print(struct card * vet){
	int i;
	
	for(i = 0; i<108; i++){
		printC(vet[i]);
	}
}

void sort(struct card *n){
	
	int i,j = 0,k = 1,l = 0; // variaveis para usar de contador
	
	for(i=0; i<108; i++){
		
		if(j==0 && (k<5)){
			l = 1;
		}
		
		if(j==13){
			
			j = 0;
			l++;
			k++;
			
		}
		
		if((k==5) && (j==0)) {
			l = 0;
			j=20;
		
			
			
		}
	
		n[i].val = j;
		n[i].color = k;
		
		if((k==5)&&(l==3)){
			k++;
			j = 21;
		}
			
		l++;
		
		if(l==2 && (k<5)){
			l = 0;
			j++;
			
		}
	}
	
}
