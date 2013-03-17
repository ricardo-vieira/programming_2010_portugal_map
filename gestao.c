#include"gps.h"

void MostraLocals(DIST *distrito);
void ProcuraLocal(DIST *distrito);
void CriaLocalidade(DIST *distrito);
void ApagaLocalidade(DIST *distrito);

/*-------------------------------------------------------
 *  MenuGerir
 *	inicia o menu de gestao de localidades
 *	
 *-------------------------------------------------------
 */

void MenuGerir(DIST *distrito)
{
	char esc;
	while(1)
	{
		system("clear");
		printf("0 – Regressar ao menu anterior\n");
		printf("1 – Mostrar as localidades de um distrito\n");
		printf("2 – Procurar uma localidade\n");
		printf("3 – Inserir uma localidade\n");
		printf("4 – Apagar uma localidade\n");
		printf("	Opção: ");
		esc = getchar();
		if(getchar() != '\n'){
			while(getchar() != '\n');		/*descarta todos os outros*/
			esc = 'x';
			}
		switch(esc)
		{
			case '0':
				return;
			case '1':
				MostraLocals(distrito);
				break;	
			case '2':
				ProcuraLocal(distrito);
				break;
			case '3':
				CriaLocalidade(distrito);
				break;
			case '4':		
				ApagaLocalidade(distrito);
				break;
		}
	}
}

/*-------------------------------------------------------
 *  GradToRad
 *	recebe um valor em graus e passa esse valor para radianos
 *	
 *-------------------------------------------------------
 */

float GradToRad(float x)
{
	return (3.14159 * x)/180;
}

float Calculay(float lat)
{
	return RMT*tan(GradToRad(lat-38.706));
}

float Calculax(float lat,float lon)
{
	return RMT*cos(GradToRad(lat))*sin(GradToRad(lon+9.135));
}

float Pitagoras(float x,float y)
{
	return sqrt((x*x)+(y*y));
}

/*-------------------------------------------------------
 *  MostraInfo
 *	mostra no terminal a informação relativa a uma localidade
 *	
 *-------------------------------------------------------
 */

void MostraInfo(char *nome,float latitude,float longitude)
{
	printf("\nLocalidade: %s\n",nome);
	printf("Latitude: %2.3f Longitude: %2.3f\n",latitude,longitude);
	printf("Coordenadas (%.0f,%.0f)\n",Calculax(latitude,longitude),Calculay(latitude));
	printf("Distancia a Lisboa: %.2fkm\n",Pitagoras(Calculax(latitude,longitude),Calculay(latitude)));
}

/*-------------------------------------------------------
 *  MostraLocals
 *	pergunta ao utilizador o nome do distrito que pertende consultar e mostra a informação de todas as localidades
 *	
 *-------------------------------------------------------
 */

void MostraLocals(DIST *distrito)
{
	LOCAL *aux;
	int dist;
	char nomedist[SSIZE];
	printf("Insira o distrito:");
	fgets(nomedist,SSIZE,stdin);
	nomedist[strlen(nomedist)-1] = '\0';
	for(dist = 0;dist < NDIST;dist++)			/*Corre os distritos*/
		if(strcmp(distrito[dist].nomedist,nomedist) == 0)	/*Pára no distrito com o nome igual ao inserido.("dist" guarda o numero do distrito)*/
			break;
	if(strcmp(distrito[dist].nomedist,nomedist) != 0)       /*se sair do ciclo anterior sem encontrar o distrito*/
	{
		allegro_init();
		allegro_message("Não foram encontradas distrito com o nome \"%s\".",nomedist);
		allegro_exit();
		return;
	}
	if(distrito[dist].local == NULL)
	{
		allegro_init();
		allegro_message("Distrito vazio.");
		allegro_exit();
		return;
	}
	aux = distrito[dist].local;
	while(aux != NULL)			/*corre todas as localidades do distrito*/
	{
		MostraInfo(aux->localidade,aux->latitude,aux->longitude);
		aux = aux->prox;
	}
	printf("Enter para continuar...");
	while(getchar() != '\n');
}

/*-------------------------------------------------------
 *  FormatStr
 *	retira os espaços da string recebida e poe as primeiras letras das palavras da string em maiusculas
 *	
 *-------------------------------------------------------
 */

void FormatStr(char *str)
{
	int aux;
	char auxstr[50];
	
	for(aux = 0;aux < strlen(str);aux++)
	{
		if(str[aux] != ' '){			/*Retira os espaços no inicio*/
			strcpy(str,&str[aux]);
			break;
			}
	}
	for(aux = strlen(str)-1;aux > 0;aux--)
	{
		if(str[aux] != ' '){			/*Retira os espaços no fim*/
			str[aux+1] = '\0';
			break;
			}
	}

	while(strstr(str,"  ") != NULL)				/*enquanto existirem dois espaços juntos*/
	{
		aux = strstr(str,"  ") - str +1;		/*guarda a primeira ocorencia de dois espaços*/
		strcpy(auxstr,str);
		auxstr[aux] = '\0';
		for(;aux < strlen(str);aux++)			/*procura a proxima letra*/
			if(str[aux] != ' ')
				break;
		strcat(auxstr,&str[aux]);			/*adiciona a auxstr a str a partir do sito onde encontrou a proxima letra*/
		strcpy(str,auxstr);
	}

	if(str[0] >= 'a' && str[0] <= 'z')		
		str[0] = str[0]-('a'-'A');		
	for(aux = 1;aux < strlen(str);aux++)
	{
		if(str[aux] == ' ')
		{
			if(str[aux+1] >= 'a' && str[aux+1] <= 'z')
				str[aux+1] = str[aux+1]-('a'-'A');
			aux += 2;
		}
		if(str[aux] >= 'A' && str[aux] <= 'Z')
			str[aux] = str[aux]+('a'-'A');
	}
}

/*-------------------------------------------------------
 *  ProcuraLocal
 *	pergunta ao utilizador o nome da localidade que pertende consultar e mostra a informação dessa localidade
 *	
 *-------------------------------------------------------
 */

void ProcuraLocal(DIST *distrito)
{
	LOCAL *aux;
	int dist,conta = 0;
	char nomelocal[2*SSIZE];
	printf("Insira a localidade:");
	fgets(nomelocal,2*SSIZE,stdin);
	if(nomelocal[strlen(nomelocal)-1] == '\n')			/*Retira o \n no fim da str se este existir*/
		nomelocal[strlen(nomelocal)-1] = '\0';
	else						/*se nao existir quer dizer que o fgets nao leu tudo o que foi inserido na consola*/
	{
		allegro_init();
		allegro_message("ERRO: tamanho de string excedido.");
		allegro_exit();
		return;
	}
	FormatStr(nomelocal);						/*retira os espaços desnecessarios*/
	for(dist = 0;dist < NDIST;dist++)
	{
		aux = distrito[dist].local;
		while(aux != NULL)
		{
			if(strcmp(aux->localidade,nomelocal) == 0)
			{
				printf("\nDistrito: %s\n",distrito[dist].nomedist);
				MostraInfo(aux->localidade,aux->latitude,aux->longitude);
				conta++;
			}
			aux = aux->prox;
		}
	}
	if(conta == 0)
	{
		allegro_init();
		allegro_message("Não foram encontradas localidades com o nome \"%s\".",nomelocal);
		allegro_exit();
	}
	else
	{
		printf("Enter para continuar...");
		while(getchar() != '\n');
	}
}

/*-------------------------------------------------------
 *  CriaLocalidade
 *	pergunta ao utilizador as informações que pertende inserir e cria a localidade no distrito inserido
 *	
 *-------------------------------------------------------
 */

void CriaLocalidade(DIST *distrito)
{
	char ndist[SSIZE], local[SSIZE];
	float latitude, longitude;
	int dist;
	LOCAL *aux;
	printf("Insira o distrito:");
	fgets(ndist,SSIZE,stdin);
	ndist[strlen(ndist)-1] = '\0';
	for(dist = 0;dist < NDIST;dist++)			/*procura pelo nome do distrito*/
		if(strcmp(distrito[dist].nomedist,ndist) == 0)
			break;
	if(strcmp(distrito[dist].nomedist,ndist) != 0)
	{
		allegro_init();
		allegro_message("Distrito não encontrado");
		allegro_exit();
		return;
	}
	printf("Insira o nome da localidade a criar:");
	fgets(local,SSIZE,stdin);
	local[strlen(local)-1] = '\0';
		for(dist = 0;dist < NDIST;dist++)
	{
		aux = distrito[dist].local;
		while(aux != NULL)
		{
			if(strcmp(aux->localidade,local) == 0)
			{
				allegro_init();
				allegro_message("Localidade ja existente!!");
				allegro_exit();
				return;
			}
			aux = aux->prox;
		}
	}
	printf("Latitude:");
	scanf("%f",&latitude);
	printf("Longitude:");
	scanf("%f",&longitude);
	getchar();
	if(latitude > 90 || latitude < -90 || longitude > 180 || longitude < -180)
	{
		allegro_init();
		allegro_message("Coordenadas invalidas!");
		allegro_exit();
		return;
	}
	distrito[dist].local = InsereLocalidade(distrito[dist].local,local,latitude,longitude);		/*insere no fim da lista distrito[dist].local*/
}

/*-------------------------------------------------------
 *  ApagaProx
 *	recebe o endereço da localidade anterior á que deve ser apagada e a que deve ser apagada e apaga esta
 *	
 *-------------------------------------------------------
 */

LOCAL *ApagaProx(LOCAL *inicio, LOCAL *ante, LOCAL *aux)
{
	LOCAL *apaga;
	apaga = aux;
	if(aux == ante)				/*se estivermos na primeira posição da lista*/
		inicio = aux->prox;
	else
		ante->prox = aux->prox;		
	free(apaga);
	return inicio;
}

/*-------------------------------------------------------
 *  ApagaLocalidade
 *	Apaga TODAS as localidades com o nome inserido pelo utilizador
 *	
 *-------------------------------------------------------
 */

void ApagaLocalidade(DIST *distrito)
{
	LOCAL *aux, *ante = NULL;
	int dist, conta = 0;
	char nomelocal[SSIZE];
	printf("Insira a localidade a apagar:");
	fgets(nomelocal,SSIZE,stdin);
	nomelocal[strlen(nomelocal)-1] = '\0';
	for(dist = 0;dist < NDIST;dist++)
	{
		aux = distrito[dist].local;
		ante = distrito[dist].local;
		while(aux != NULL)
		{
			if(strcmp(aux->localidade,nomelocal) == 0)
			{
				allegro_init();
				allegro_message("Localidade %s apagada do distrito %s.",aux->localidade,distrito[dist].nomedist);
				allegro_exit();
				distrito[dist].local = ApagaProx(distrito[dist].local,ante,aux);
				conta++;
			}
			ante = aux;
			aux = aux->prox;
		}
	}
	if(conta == 0)
	{
		allegro_init();
		allegro_message("Não foram encontradas localidades com o nome \"%s\".",nomelocal);
		allegro_exit();
	}
}
