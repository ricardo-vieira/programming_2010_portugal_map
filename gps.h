#ifndef SSIZE
	#include<stdio.h>
	#include<math.h>
	#include<allegro.h>
	#include<stdlib.h>
	#include<string.h>
	#define SSIZE 25				/* Tamanho das strings utilizadas ao longo do programa */
	#define RMT 6371				/* Raio medio da terra */
	#define NFRON 52				/* Numero de fronteiras */
	#define NDIST 18				/* Numero de distritos */
	#define D 10					/*Valor D para calculo de rotas*/
	#define FROTA "rota.rt"				/*ficheiro com a rota*/
	#define FLDF "linhas_de_fronteira.ldf"		/*ficheiro com as linha de fronteira*/
	

	/*----------------------------------------------------------------------------------------
	* COORD
	*	Estrutura ligada que guarda uma coordenada do contorno e aponta para a proxima.
	------------------------------------------------------------------------------------------*/
	typedef struct lista_coord
	{
		float latitude;
		float longitude;
		struct lista_coord *prox;
	}COORD;
	
	/*----------------------------------------------------------------------------------------
	* CTD
	*	Estrutura que guarda a lista de coordenadas do contorno e a cor do mesmo.
	------------------------------------------------------------------------------------------*/

	typedef struct contornos
	{
		COORD *coord;
		int cor;
	}CTD;
	
	/*----------------------------------------------------------------------------------------
	* LSTCTD
	*	Estrutura ligada que guarda os codigos do contorno de um distrito.
	------------------------------------------------------------------------------------------*/	
	
	typedef struct lista_contornos
	{
		int codigo;
		struct lista_contornos *prox;
	}LSTCTD;
	
	/*----------------------------------------------------------------------------------------
	* LOCAL
	*	Estrutura ligada que guarda uma localidade de um distrito.
	------------------------------------------------------------------------------------------*/

	typedef struct lista_localidades
	{
		char localidade[SSIZE];
		float latitude;
		float longitude;
		struct lista_localidades *prox;
	}LOCAL;
	
	/*----------------------------------------------------------------------------------------
	* DIST
	*	Estrutura que guarda o nome do distrito, uma lista de localidades de um distrito uma lista e uma lista de codigos de contorno.
	------------------------------------------------------------------------------------------*/
	
	typedef struct lista_distritos
	{
		char nomedist[SSIZE];
		LSTCTD *codcontorno;
		LOCAL *local;
	}DIST;
#endif

	
void CarregaDistritos(DIST *distrito);							/*carrega.c*/
void CarregaContorno(CTD *contorno);							/*carrega.c*/
LOCAL *InsereLocalidade(LOCAL *inicio,char *nomelocal,float lat,float lon);		/*carrega.c*/

void MenuGerir(DIST *distrito);								/*gestao.c*/

void MenuRotas(DIST *distrito);								/*rotas.c*/

void MenuMapas(DIST *distrito,CTD *contorno);						/*mapas.c*/
float GradToRad(float x);								/*mapas.c*/
float Pitagoras(float x,float y);							/*mapas.c*/
float transf_lat(float lat,float zoom,float reflat,int altecra);			/*mapas.c*/
float transf_lon(float lat,float lon,float zoom,float reflon,int largecra);		/*mapas.c*/
LOCAL *RetLocal(DIST *distrito, char *nomelocal);					/*mapas.c*/
