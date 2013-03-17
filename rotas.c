#include"gps.h"

void CalcRota(DIST *distrito);

void MenuRotas(DIST *distrito)
{
	char esc;
	while(1){
		do
		{
			printf("0 – Regressar ao menu anterior\n");
			printf("1 – Calculo de uma Rota\n");
			printf("	Opção: ");
			esc = getchar();
			if(getchar() != '\n'){
				while(getchar() != '\n');
				esc = 'x';
			}
		}
		while(esc != '0' && esc != '1' && esc != '2' && esc != '3'  && esc != '4');
		switch(esc)
		{
			case '0':
				return;
			case '1':
				CalcRota(distrito);
				break;
		}
	}
}

LOCAL *ProxLoc(DIST *distrito,LOCAL *part,LOCAL *chegada,int Dist)
{
	LOCAL *auxloc = NULL, *locmaispross;
	int dist;
	float distancia = 1000;
	for(dist = 0;dist < NDIST;dist++)
	{
		auxloc = distrito[dist].local;
		while(auxloc != NULL)
		{
					/*distancia entre a partida e a proxima localidade < Raio de procura*/
			if(Pitagoras(transf_lon(part->latitude,part->longitude,1,auxloc->longitude,0),transf_lat(part->latitude,1,auxloc->latitude,0)) < Dist 
					/*verifica se a poxima é igual á partida*/
			&& strcmp(part->localidade,auxloc->localidade) != 0
					/*distancia entre a proxima localidade e a chegada é menor que a anterior distancia*/
			&& Pitagoras(transf_lon(auxloc->latitude,auxloc->longitude,1,chegada->longitude,0),transf_lat(auxloc->latitude,1,chegada->latitude,0)) <= distancia)
			{
				distancia = Pitagoras(transf_lon(auxloc->latitude,auxloc->longitude,1,chegada->longitude,0),transf_lat(auxloc->latitude,1,chegada->latitude,0));
				locmaispross = auxloc;
			}
			auxloc = auxloc->prox;
		}
	}
			/*nao aceita a proxima localidade se esta se encontrar a uma distancia+margem da chegada, maior do que a partida á chegada*/
	if(Pitagoras(transf_lon(part->latitude,part->longitude,1,chegada->longitude,0),transf_lat(part->latitude,1,chegada->latitude,0)) < distancia+(D-2))
		return NULL;
	return locmaispross;
}

LOCAL *Rota(DIST *distrito, LOCAL *part,LOCAL *chegada)
{
	LOCAL *auxloc = NULL, *proxloc = NULL;
	int aux = 0;
	auxloc = InsereLocalidade(auxloc,part->localidade,part->latitude,part->longitude);	/*insere a localidade de partida*/
	if(strcmp(part->localidade,chegada->localidade) != 0)					/*se a partida e a chegada forem diferentes*/
	{
		do{
		proxloc = ProxLoc(distrito,part,chegada,D+aux);					/*proxloc sera a proxima localidade na rota */
		aux++;										/*incrementa o raio de procura*/
		}
		while(proxloc == NULL);
		auxloc->prox = Rota(distrito,proxloc,chegada);					/*calcula a rota entre a proxima localidade e a chegada*/
	}
	return auxloc;
}

void CalcRota(DIST *distrito)
{ 
	char nomelocal[SSIZE];
	FILE *frota;
	LOCAL *rota = NULL, *apaga = NULL;
	LOCAL *aux, *partida, *chegada;

	printf("\nInsira a localidade de partida:");
	fgets(nomelocal,SSIZE,stdin);
	nomelocal[strlen(nomelocal)-1] = '\0';
	partida = RetLocal(distrito,nomelocal);

	printf("\nInsira a localidade de chegada:");
	fgets(nomelocal,SSIZE,stdin);
	nomelocal[strlen(nomelocal)-1] = '\0';
	chegada = RetLocal(distrito,nomelocal);

	if(partida == NULL || chegada == NULL)
		return;

	rota = Rota(distrito,partida,chegada);
	frota = fopen(FROTA,"w");
	aux = rota;
	while(aux != NULL)
	{
		fprintf(frota,"%s\n",aux->localidade);			/*guarda a rota no ficheiro*/
		apaga = aux;
		aux = aux->prox;
		free(apaga);
	}
	fclose(frota);
}
