#include"gps.h"

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TCOL 0               /*Cor do texto 0 a 15*/
#define BGCOL makecol(240,240,240)		/*cor do fundo do mapa*/
#define CORDIST random(16)

void DesenhaRota(DIST *distrito, CTD *contorno);
void VerMapa(DIST *distrito, CTD *contorno,float zoom,LOCAL *localref);

/*-------------------------------------------------------
 *  MenuMapas
 *	inicia o menu de gestao de mapas
 *	
 *-------------------------------------------------------
 */

void MenuMapas(DIST *distrito, CTD *contorno)
{
	char esc, nomelocal[SSIZE];
	float zoom = 1;
	LOCAL *locref = NULL;
	while(1){
		system("clear");
		printf("0 – Regressar ao menu anterior\n");
		printf("1 – Seleccao da localidade de referência\n");
		printf("2 – Seleccao da escala\n");
		printf("3 – Visualizar as localidades em redor da referencia\n");
		printf("4 – Apresentar uma rota\n");
		printf("	Opção: ");
		esc = getchar();			/*le o primeiro caracter inserido*/
		if(getchar() != '\n'){
			while(getchar() != '\n');		/*descarta todos os outros*/
			esc = 'x';
		}
		switch(esc)
		{
			case '0':
				return;
			case '1':
				printf("Insira a localidade:");
				fgets(nomelocal,SSIZE,stdin);
				nomelocal[strlen(nomelocal)-1] = '\0';
				locref = RetLocal(distrito,nomelocal);
				break;	
			case '2':
				printf("Insira a escala (pixeis/km):");
				scanf("%f",&zoom);
				getchar();
				if(zoom < 0){
					zoom = 1;
					allegro_init();
					allegro_message("O zoom deve ser um valor superior a zero.");
					allegro_exit();
				}
				break;
			case '3':
				if(locref == NULL){		/*verifica se esta definida uma localidade de referencia*/
					allegro_init();
					allegro_message("Deve defenir primeiro uma localidade de referencia.");
					allegro_exit();
				}
				else
					VerMapa(distrito,contorno,zoom,locref);
				break;
			case '4':		
				DesenhaRota(distrito,contorno);
				break;
		}
	}
}

float transf_lat(float lat,float zoom,float reflat,int altecra)
{
	return -(zoom*RMT*tan(GradToRad(lat-reflat)))+(altecra/2);
}

float transf_lon(float lat,float lon,float zoom,float reflon,int largecra)
{
	return (zoom*RMT*cos(GradToRad(lat))*sin(GradToRad(lon-reflon)))+(largecra/2);
}

/*-------------------------------------------------------
 *  DesenhaLocal
 *	desenha o nome da localidade e um ponto no sitio da localidade
 *	
 *-------------------------------------------------------
 */

void DesenhaLocal(char *local,float lat,float lon,float latref,float lonref,float zoom)
{
	circle(screen,transf_lon(lat,lon,zoom,lonref, SCREEN_W) , transf_lat(lat,zoom,latref, SCREEN_H),1,TCOL);
	floodfill(screen,transf_lon(lat,lon,zoom,lonref, SCREEN_W) , transf_lat(lat,zoom,latref, SCREEN_H), TCOL);
	textprintf_ex(screen, font, (transf_lon(lat,lon,zoom,lonref, SCREEN_W))+3 , (transf_lat(lat,zoom,latref, SCREEN_H))-9,TCOL, -1,"%s",local);
}

/*-------------------------------------------------------
 *  DesenhaTodasLocal
 *	desenha todas as localidades no mapa
 *	
 *-------------------------------------------------------
 */

void DesenhaTodasLocal(DIST *distrito,float latref, float lonref,float zoom)
{
	LOCAL *aux;
	int dist;
	for(dist = 0;dist < NDIST;dist++)
	{
		aux = distrito[dist].local;
		while(aux != NULL)
		{
			DesenhaLocal(aux->localidade,aux->latitude,aux->longitude,latref,lonref,zoom);
			aux = aux->prox;
		}	
	}
}

/*-------------------------------------------------------
 *  VerMapa
 *	inicia o modo grafico com a ajuda da biblioteca allegro desenhando o mapa com a localidade de referencia e o zoom inserido
 *	
 *-------------------------------------------------------
 */

void VerMapa(DIST *distrito, CTD *contorno,float zoom,LOCAL *localref)
{
	int ret,cod;
	COORD *aux, *coordenadas;
	double oldlatitude, oldlongitude;
	double latitude, longitude;
	allegro_init();
	ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGURA_JANELA, ALTURA_JANELA, 0, 0);
	if (ret != 0)
	{
		allegro_message("Graphic error- Exiting");
		return;
	}
	floodfill(screen,1,1,BGCOL);							/*Pinta a tela (cor do fundo) com a cor defenida em BGCOL*/
	textprintf_ex(screen, font,3,3,TCOL, -1,"Centrado em: \"%s\" Zoom = %.2f px/km",localref->localidade,zoom);
	for(cod = 0 ; cod < NFRON; cod++)		/*Corre todas as listas de contornos*/
	{	
		coordenadas = contorno[cod].coord;
		aux = coordenadas;
		while(aux->prox != NULL)		/*Le a lista do contorno ate ao fim*/
		{
			oldlatitude = transf_lat(aux->latitude,zoom,localref->latitude,SCREEN_H);
			oldlongitude = transf_lon(aux->latitude,aux->longitude,zoom,localref->longitude,SCREEN_W);
			aux = aux->prox;
			latitude = transf_lat(aux->latitude,zoom,localref->latitude,SCREEN_H);
			longitude = transf_lon(aux->latitude,aux->longitude,zoom,localref->longitude, SCREEN_W);
			line(screen,oldlongitude,oldlatitude,longitude,latitude,contorno[cod].cor);
		}
	}
	DesenhaTodasLocal(distrito,localref->latitude,localref->longitude,zoom);
	install_keyboard();	
	readkey();              /*Espera que uma tecla seja clicada*/
	allegro_exit();
}

/*-------------------------------------------------------
 *  RetLocal
 *	retorna o endereço de UMA localide encontrada em todos os distritos com o nome contido na string nomelocal
 *	
 *-------------------------------------------------------
 */

LOCAL *RetLocal(DIST *distrito, char *nomelocal)
{
	LOCAL *aux = NULL;
	int dist;
	for(dist = 0;dist < NDIST;dist++)
	{
		aux = distrito[dist].local;
		while(aux != NULL)
		{
			if(strcmp(aux->localidade,nomelocal) == 0)
			{
				return aux;
			}
			aux = aux->prox;
		}
	}
	allegro_init();
	allegro_message("Não foram encontradas localidades com o nome \"%s\".",nomelocal);
	allegro_exit();
	return NULL;
}

/*-------------------------------------------------------
 *  CalcZoom
 *	calcula o zoom do mapa quando é mostrada uma rota tedo em conota a distancia entre a localidade de partida e chegada
 *	
 *-------------------------------------------------------
 */

float CalcZoom(LOCAL *rota)
{
	LOCAL *aux;
	float dist, startlat, startlon, endlat, endlon;
	aux = rota;
	startlat = rota->latitude;
	startlon = rota->longitude;
	while(aux->prox != NULL)
		aux = aux->prox;
	endlat = aux->latitude;
	endlon = aux->longitude;
	dist = Pitagoras(transf_lon(startlat,startlon,1,endlon,0),transf_lat(startlat,1,endlat,0));     /*Distancia entre a localidade de partida e a de chegada*/
	dist += 0.1*dist;							/*Adiciona uma margem*/
	return ALTURA_JANELA/dist;					/*Para esta distacia caber no ecra no maximo na vertical*/
}

/*-------------------------------------------------------
 *  DesenhaRota
 *	inicia o modo grafico com a ajuda da biblioteca allegro desenhando o mapa com a rota no ficheiro "rota.rt"
 *	
 *-------------------------------------------------------
 */

void DesenhaRota(DIST *distrito, CTD *contorno)
{
	char nomelocal[SSIZE];
	LOCAL *local = NULL, *rota = NULL, *auxloc = NULL;
	FILE *frota;
	int ret,cod;
	COORD *aux, *coordenadas;
	double oldlatitude, oldlongitude;
	double latitude, longitude;
	float zoom, latref, lonref;
	/*Guarda o conteudo do ficheiro numa lista do tipo LOCAL chamada rota*/
	frota = fopen(FROTA,"r");
	if(frota == NULL){
		allegro_init();
		allegro_message("Ficheiro com a rota não encontrado.");
		allegro_exit();
		return;
	}
	fgets(nomelocal,SSIZE,frota);
	do
	{
		nomelocal[strlen(nomelocal)-1] = '\0';
		local = RetLocal(distrito,nomelocal);
		rota = InsereLocalidade(rota,local->localidade,local->latitude,local->longitude);
	}while(fgets(nomelocal,SSIZE,frota) != NULL);
	fclose(frota);
	zoom = CalcZoom(rota);		/*Define o zoom*/
	/*Define Centro do ecra */
	auxloc = rota;
	while(auxloc->prox != NULL)
		auxloc = auxloc->prox;
	latref = (rota->latitude + auxloc->latitude)/2;			/*calcula o ponto médio entre a localidade de partida e a de chegada*/
	lonref = (rota->longitude + auxloc->longitude)/2;		/*que sera o centro do ecra*/
	/*Desenha os contornos dos distritos*/
	allegro_init();
	ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGURA_JANELA, ALTURA_JANELA, 0, 0);
	if (ret != 0)
	{
		allegro_message("Graphic error- Exiting");
		return;
	}
	floodfill(screen,1,1,BGCOL);
	textprintf_ex(screen, font,3,3,TCOL, -1,"Zoom = %.2f",zoom);
	for(cod = 0 ; cod < NFRON; cod++)
	{	
		coordenadas = contorno[cod].coord;
		aux = coordenadas;
		while(aux->prox != NULL)
		{
			oldlatitude = transf_lat(aux->latitude,zoom,latref,SCREEN_H);
			oldlongitude = transf_lon(aux->latitude,aux->longitude,zoom,lonref,SCREEN_W);
			aux = aux->prox;
			latitude = transf_lat(aux->latitude,zoom,latref,SCREEN_H);
			longitude = transf_lon(aux->latitude,aux->longitude,zoom,lonref, SCREEN_W);
			line(screen,oldlongitude,oldlatitude,longitude,latitude,contorno[cod].cor);
		}
	}
	/*Desenha o nome das localidades*/
	auxloc = rota;
	while(auxloc != NULL)
		{
			DesenhaLocal(auxloc->localidade,auxloc->latitude,auxloc->longitude,latref,lonref,zoom);
			auxloc = auxloc->prox;
		}
	/*Desenha caminho*/
	auxloc = rota;
	while(auxloc->prox != NULL)
	{
		oldlatitude = transf_lat(auxloc->latitude,zoom,latref,SCREEN_H);
		oldlongitude = transf_lon(auxloc->latitude,auxloc->longitude,zoom,lonref,SCREEN_W);
		auxloc = auxloc->prox;
		latitude = transf_lat(auxloc->latitude,zoom,latref,SCREEN_H);
		longitude = transf_lon(auxloc->latitude,auxloc->longitude,zoom,lonref, SCREEN_W);
		line(screen,oldlongitude,oldlatitude,longitude,latitude,1);
	}
	install_keyboard();	
	readkey();
	allegro_exit();
}
