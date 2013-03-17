#include"gps.h"

/*-------------------------------------------------------
 *  ncorask
 *	retorna um valor inteiro de acordo com a string recebida sendo esta uma cor ex: "amarelo" ou "azul"
 *	
 *-------------------------------------------------------
 */

int ncorask(char *cor)
{
	int aux;
	char *cores[16];
	cores[0] ="preto";
	cores[1] ="azul escuro";
	cores[2] ="verde escuro";
	cores[3] ="ciano escuro";
	cores[4] ="vermelho escuro";
	cores[5] ="mangenta escuro";
	cores[6] ="laranja";
	cores[7] ="cinzento";
	cores[8] ="cinzento escuro";
	cores[9] ="azul";
	cores[10] ="verde";
	cores[11] ="ciano";
	cores[12] ="vermelho";
	cores[13] ="mangenta";
	cores[14] ="amarelo";
	cores[15] ="branco";
	
	for(aux = 0;aux < 16;aux++)		/*corre todas as cores*/
		if(strncmp(cores[aux],cor,strlen(cores[aux])) == 0)	/*compara as string da cor e a string recebida e retorna 1 quando encontrada*/
			break;
	return aux;	/*retorna o valor da cor*/
}

/*-------------------------------------------------------
 *  CarregaCoord
 *	insere todas as coosdenadas de um contorno
 *	retonrna o endereço para a primeira coordenada
 *-------------------------------------------------------
 */

COORD *CarregaCoord(FILE *fcont,char *linha)
{
	COORD *auxcoord = NULL, *inicio = NULL, *ante = NULL;
	while(fgets(linha,2*SSIZE,fcont) != NULL )	/*pára quando encontrar o fim do ficheiro*/
	{
		if(linha[0] == 'N')			/*ou quando encontrar outro contorno*/
			return inicio;
		inicio = (COORD *)malloc(sizeof(COORD));
		inicio->prox = ante;
		auxcoord = inicio;
		sscanf(linha,"%f %f\n",&(auxcoord->latitude),&(auxcoord->longitude));	
		if(auxcoord->latitude > 90 || auxcoord->latitude < -90 || auxcoord->longitude > 180 || auxcoord->longitude < -180)
			printf("Coordenadas invalidas!!%s\n",linha);
		ante = auxcoord;
		auxcoord = NULL;
	}
	return inicio;
}

/*-------------------------------------------------------
 *  CarregaCoord
 *	carrega todos os contornos do ficheiro "linhas_de_fronteira.ldf" para a estrutura que o endereço inicial é codcontorno
 *	
 *-------------------------------------------------------
 */

void CarregaContorno(CTD *contorno)
{
	FILE *fcont;
	char linha[2*SSIZE];
	int codigo;

	for(codigo = 0;codigo < NFRON;codigo++)			/*inicializa todas as listas de contornos a NULL e as cores a -1*/
	{
		contorno[codigo].coord = NULL;
		contorno[codigo].cor = -1;
	}

	fcont = fopen(FLDF,"r");
	if(fcont == NULL)
	{
		allegro_init();
		allegro_message("ERRO no carregamento de contornos (vereficar existencia do ficheiro \"%s\")\n\nDeve terminar o programa.",FLDF);
		allegro_exit();
		return;
	}
	fgets(linha,10,fcont);			/*le a primeira linha do ficheiro*/
	while(linha[0] == 'N')
	{
			sscanf(linha,"N%d\n",&codigo);
			fgets(linha,2*SSIZE,fcont);		/*le do ficheiro a cor do contorno*/
			contorno[codigo].cor = ncorask(linha);		/*transforma a string com o nume da cor num valor e armazena este no campo cor*/
			contorno[codigo].coord = CarregaCoord(fcont,linha); /*o endereço da primeira coordenada seraguardada no campo contorno[codigo].coord*/
	}
	fclose(fcont);
	for(codigo = 0;codigo < NFRON;codigo++)
	{
		if(contorno[codigo].coord == NULL)				/*verifica se todos os contornos foram inseridos*/
			printf("ERRO: O contorno \"%d\" não foi inserido.\n",codigo);
		if(contorno[codigo].cor == -1)					/*verifica se todas as cores foram inseridas*/
			printf("ERRO: A cor do contorno \"%d\" não foi inserido.\n",codigo);	
	}
}

/*-------------------------------------------------------
 *  InsereCodFim
 *	carrega um CODIGO do contorno de um distrito no fim da lista
 *	retorna o endereço para a primeira posição
 *-------------------------------------------------------
 */

LSTCTD *InsereCodFim(LSTCTD *inicio,int cod)
{
	LSTCTD *aux;
	if(inicio == NULL)	 	/*se a lista estiver vazia*/
	{
		inicio = (LSTCTD *)malloc(sizeof(LSTCTD));
		if (inicio == (LSTCTD *) NULL) 
		{
        		printf("Falta de memoria durante a  insercao\n");
			return NULL;
		}
		inicio->codigo = cod;		/*guarda o valor recebido*/
		inicio->prox = NULL;
	}
	else
	{
		aux = inicio;
		while(aux->prox != NULL)	/*vai para o final da lista*/
		{
			aux = aux->prox;
		}
		aux->prox = (LSTCTD *)malloc(sizeof(LSTCTD));
		aux = aux->prox;
		if (aux == (LSTCTD *) NULL) 
		{
        		printf("Falta de memoria durante a  insercao\n");
			return NULL;
		}
		aux->codigo = cod;		/*e insere o valor recebido*/
		aux->prox = NULL;
	}
	return inicio;
}

/*-------------------------------------------------------
 *  InsereLocalidade
 *	insere as informações de uma localidade no fim da lista com o tipo LOCAL
 *	retorna um endereço para o inicio da lista de localidades ou NULL caso ocorra um erro 
 *-------------------------------------------------------
 */

LOCAL *InsereLocalidade(LOCAL *inicio,char *nomelocal,float lat,float lon)
{
	LOCAL *aux;
	if(inicio == NULL)                               /*cria a lista se esta estiver vazia*/
	{
		inicio = (LOCAL *)malloc(sizeof(LOCAL));
		if (inicio == (LOCAL *) NULL) 
		{
        		printf("Falta de memoria durante a  insercao\n");
			return NULL;
		}
		strcpy(inicio->localidade,nomelocal);
		inicio->latitude =lat;
		inicio->longitude =lon;
		inicio->prox = NULL;
	}
	else
	{
		aux = inicio;
		while(aux->prox != NULL)		/*vai para o final da lista*/
		{
			aux = aux->prox;
		}
		aux->prox = (LOCAL *)malloc(sizeof(LOCAL));
		aux = aux->prox;
		if (aux == (LOCAL *) NULL) 
		{
        		printf("Falta de memoria durante a  insercao\n");
			return NULL;
		}
		strcpy(aux->localidade,nomelocal);
		aux->latitude =lat;
		aux->longitude =lon;
		aux->prox = NULL;
	}
	return inicio;
}

/*-------------------------------------------------------
 *  CarregaLocalidades
 *	insere todas as localidades de todos os distritos
 *	
 *-------------------------------------------------------
 */

void CarregaLocalidades(DIST *distrito)
{
	FILE *fdist;
	int dist;
	float latitude,longitude;
	char auxdist[SSIZE], linha[2*SSIZE];
	for(dist = 0; dist < NDIST;dist++)		/*corre todos os distritos*/
	{
		distrito[dist].local = NULL;
		strcpy(auxdist , distrito[dist].nomedist);	/*auxdist ira guardar o nome do ficheiro a abrir*/
		strcat(auxdist,".scd");
		fdist = fopen(auxdist,"r");
		if(fdist == NULL)
		{
			allegro_init();
			allegro_message("ERRO no carregamento de localidades (vereficar existencia do ficheiro \"%s\")\n\nDeve terminar o programa.",auxdist);
			allegro_exit();
			return;
		}
		fgets(linha,2*SSIZE,fdist);
		while(strncmp(linha,"/* LOCALIDADES */",strlen("/* LOCALIDADES */")) != 0)	/*vai até á linha com a string  LOCALIDADES */
		{
			fgets(linha,2*SSIZE,fdist);
		}
		while(fgets(linha,2*SSIZE,fdist) != NULL)		/*le todas as linhas até que chege ao fim do ficheiro*/
		{
			sscanf(strchr(linha,'|'),"|%f %f\n",&latitude,&longitude);	/*le as coordenadas a partir da pos em que se encontra o caracter '|'*/
			linha[strchr(linha,'|')-linha] = '\0';					/*linha guarda agora o nome da localidade*/
			distrito[dist].local = InsereLocalidade(distrito[dist].local,linha,latitude,longitude);
		}
		fclose(fdist);
	}
}

/*-------------------------------------------------------
 *  CarregaCodigos
 *	insere todos os codigos de todos os distritos
 *	
 *-------------------------------------------------------
 */

void CarregaCodigos(DIST *distrito)
{
	FILE *fdist;
	int dist,codigo;
	char auxdist[SSIZE];
	char linha[SSIZE];
	for(dist = 0; dist < NDIST;dist++)		/*corre todos os distritos*/
	{
		distrito[dist].codcontorno = NULL;
		strcpy(auxdist , distrito[dist].nomedist);	/*auxdist ira guardar o nome do ficheiro a abrir*/
		strcat(auxdist,".scd");
		fdist = fopen(auxdist,"r");
		if(fdist == NULL)
		{
			allegro_init();
			allegro_message("ERRO no carregamento de codigos de distritos (vereficar existencia do ficheiro \"%s\"\n\nDeve terminar o programa.)\n",auxdist);
			allegro_exit();
			return;
		}
		fgets(linha,20,fdist);
		while(strncmp(linha,"/* LOCALIDADES */",strlen("/* LOCALIDADES */")) != 0)	/*para quando encontrar a string LOCALIDADES no ficheiro*/
		{
			sscanf(linha,"%d\n",&codigo);
			distrito[dist].codcontorno = InsereCodFim(distrito[dist].codcontorno,codigo);	/*distrito[dist].codcontorno guarda o endereço do primeiro codigo do contorno*/
			fgets(linha,20,fdist);
		}
		fclose(fdist);
	}
		
}

/*-------------------------------------------------------
 *  CarregaDistritos
 *	guarda todas as informações contidas nos ficheiros de todos os distritos
 *	
 *-------------------------------------------------------
 */

void CarregaDistritos(DIST *distrito)
{
	/*guarda o nome dos distritos*/
	strcpy(distrito[0].nomedist,"Aveiro");	
	strcpy(distrito[1].nomedist,"Beja");
	strcpy(distrito[2].nomedist,"Braga");
	strcpy(distrito[3].nomedist,"Braganca");
	strcpy(distrito[4].nomedist,"Castelo Branco");
	strcpy(distrito[5].nomedist,"Coimbra");
	strcpy(distrito[6].nomedist,"Evora");
	strcpy(distrito[7].nomedist,"Faro");
	strcpy(distrito[8].nomedist,"Guarda");
	strcpy(distrito[9].nomedist,"Leiria");
	strcpy(distrito[10].nomedist,"Lisboa");
	strcpy(distrito[11].nomedist,"Portalegre");
	strcpy(distrito[12].nomedist,"Porto");
	strcpy(distrito[13].nomedist,"Santarem");
	strcpy(distrito[14].nomedist,"Setubal");
	strcpy(distrito[15].nomedist,"Viana Do Castelo");
	strcpy(distrito[16].nomedist,"Vila Real");
	strcpy(distrito[17].nomedist,"Viseu");

	CarregaCodigos(distrito);
	CarregaLocalidades(distrito);
}
