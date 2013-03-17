#include"gps.h"

/*-------------------------------------------------------
 *  GuardaDados
 *	Função que guarda todos os dados que foram lidos no inicio do programa e as suas alterações
 *	
 *-------------------------------------------------------
 */

void GuardaDados(DIST *distrito)
{
	LOCAL *auxlocal;
	LSTCTD *auxctd;
	FILE *fdist;
	char esc;
	int dist;
	char auxdist[SSIZE];
	printf("Deseja guardar as alterações?(s/n):");   /*Pergunta ao utilizador se quer guardar as alterações, recebe "s" ou "n"*/
	esc = getchar();
	if(getchar() != '\n'){
		while(getchar() != '\n');
		esc = 'x';
	}  
	if(esc == 'n')		/*se a resporta for "n" termina o programa*/
		exit(0);
	if(esc == 's')		/*se a resposta for "s" guarda os dados*/
	{
		for(dist = 0; dist < NDIST;dist++)		/*função que corre todos os distritos*/
		{
			strcpy(auxdist , distrito[dist].nomedist); /*auxdist ira guardar o nome do ficheiro*/
			strcat(auxdist,".scd");
			fdist = fopen(auxdist,"w");		/*abre o ficheiro*/
			if(fdist == NULL)
			{
				printf("ERRO a guardar codigos de distritos)\n");
				return;
			}
			auxctd = distrito[dist].codcontorno;		/*auxctd guarda o endereço do primeiro contorno do distrito*/
			while(auxctd != NULL)				/*le todos os contornos do distrito*/
			{
				fprintf(fdist,"%d\n",auxctd->codigo);	/*escreve no ficheiro os codigos dos contornos*/
				auxctd = auxctd->prox;
			}
			fprintf(fdist,"/* LOCALIDADES */\n");
			auxlocal = distrito[dist].local;		/*auxlocal guarda o endereços do primeira localidade*/
			while(auxlocal != NULL)			/*corre todas as localidades do distrito*/
			{
				fprintf(fdist,"%s|%2.3f %2.3f\n",auxlocal->localidade, auxlocal->latitude, auxlocal->longitude);/*escreve no ficheiro a informação das localidades*/
				auxlocal = auxlocal->prox;
			}
			fclose(fdist);
		}
		exit(0);	/*termina o programa*/
	}
	GuardaDados(distrito); /*se a resposta não for "s" nem "n" volta a executar a função*/
}

/*-------------------------------------------------------
 *  IniciaMenu
 *	Função que inicia o primeiro menu do programa
 *	retorna 1 caso a opçãoi escolhida nao seja contemplada e 0 caso a opção com o numrero 0 seja a esclhida
 *-------------------------------------------------------
 */

int IniciaMenu(DIST *distrito,CTD *contorno)
{
	char esc;
	system("clear");			/*Limpa a consola*/
	printf("0 – Fim do programa\n");
	printf("1 – Gestão de Localidades\n");
	printf("2 – Rotas\n");
	printf("3 – Mapas\n");
	printf("	Opção: ");
	esc = getchar();			/*le o primeiro caracter inserido*/
	if(getchar() != '\n'){
			while(getchar() != '\n');		/*descarta todos os outros*/
			esc = 'x';
	}
	switch(esc)
	{
		case '0':
			return 0;	
		case '1':
			MenuGerir(distrito);
			break;
		case '2':
			MenuRotas(distrito);
			break;
		case '3':
			MenuMapas(distrito, contorno);
			break;
	}
	return 1;
}

int main(void)
{
	DIST distrito[NDIST];				/*inicializa a variavel que ira guardar a informação dos distritos e das localidades*/
	CTD contorno[NFRON];				/*inicializa a variavel que ira guardar os contornos dos distritos*/
	CarregaContorno(contorno);
	CarregaDistritos(distrito);
	while(IniciaMenu(distrito, contorno));		/*quado a função "IniciaMenu" retorna 1 corre essa função outra vez se retornar 0 passa á proxima linha do programa*/
	GuardaDados(distrito);
	return 0;	
}
END_OF_MAIN()
