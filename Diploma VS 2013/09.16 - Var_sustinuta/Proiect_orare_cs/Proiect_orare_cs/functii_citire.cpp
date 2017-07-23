#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "header1.h"

void citire_nume_orare(char* nume_orare[])
{
	FILE *fcurent;
	char line[100];
	for(int i = 0; i < NR_ORARE; i++)
		nume_orare[i] = (char *)calloc( 40, sizeof( char ) );
	fcurent = fopen("orare.in", "r");
	int i = 0;
	while(fgets(line, 100, fcurent))
	{	
		if(line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = 0;
		i++;
		if(i == NR_ORARE)
			line[strlen(line)] = 0;
		i--;
		for(int j = 0; line[j] != 0; j++)
			nume_orare[i][j] = line[j];
		i++;
		//printf("%s %d \n",line, strlen(line));
	}
	fclose(fcurent);
	//printf("randuri de orar = %d \n",i);
	//verificare citire nume_orare
	//for(int j = 0; j < NR_ORARE; j++)
	//	printf("nume_orar[%d] = %s \n", j, nume_orare[j]);
}

void citire_titulari(lista_titulari &Li_titulari)
{
	FILE *fcurent;
	Li_titulari.pfirst = NULL;
	Li_titulari.plast = NULL;
	Li_titulari.nr = 0;
	int index;
	char name[100];
	char line[100];
	fcurent = fopen("titulari.in", "r");
	while(fgets(line, 100, fcurent))
	{
		
		index = atoi(line);
		fgets(line, 100, fcurent);
		strcpy(name, line);
		if(line[strlen(name) - 1] == '\n')
				line[strlen(name) - 1] = '\0';
		//printf("adaugare lista");
		addlist(Li_titulari, index, line);
		
	}
	fclose(fcurent);
}

void citire_sali(lista_sali &Li, char* fisier)
{
	FILE *fcurent;
	Li.nr_elem = 0;
	Li.pfirst = NULL;
	Li.plast = NULL;
	fcurent = fopen(fisier, "r");
	char line[100];
	char name[LUNGIME_NUME];
	int index;
	while(fgets(line, 100, fcurent))
	{
		index = atoi(line);
		fgets(line, 100, fcurent);
		strcpy(name, line);
		//printf("adaugare lista");
		//printf("am citit %s din %s", line, fisier);
		addlist(Li, index, line);
		
	}
	fclose(fcurent);
}

//fctii citire discipline
void citire_discipline(lista_discipline L_D[], char* nume_orare[])
{	
	char line[100];
	FILE *fcurent;
	int index;
	char name[100];
	char acronim[40];
	char c;
	int tip[4];
	int durata[4];
	int paritate[4];
	int index_titular;
	nod_titulari *titular;
	char *f_disc;//fisier disc
	char *tmp;
	f_disc =  (char *)calloc( 40, sizeof( char ) );
	tmp = (char *)calloc( 20, sizeof( char ) );

	for(int i = 0; i < NR_ORARE; i++)
	{
		strcpy(f_disc, "disc/disc");
		for(int k = 4; nume_orare[i][k] != '.'; k++)
			tmp[k - 4] = nume_orare[i][k];
		strcat(f_disc, tmp);
		strcat(f_disc, ".in");
		//puts(f_disc);

		fcurent = fopen(f_disc, "r");
			
		
		//curat tmp
		tmp = (char *)calloc( 20, sizeof( char ) );
		L_D[i].nr = 0;
		L_D[i].pfirst = NULL;
		L_D[i].plast = NULL;
	
		while(fgets(line, 100, fcurent))
		{
			index = atoi(line);
			fgets(line, 100, fcurent);
			strcpy(name, line);
			fgets(line, 11, fcurent);
			//curatam acronimul de 'enter' la sfarsit
			if(line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			strcpy(acronim, line);
			for(int j = 0; j < 4; j++)
			{
				c = getc(fcurent);
				tip[j] = c - '0';
				c = getc(fcurent); //ignora caracter de separare
			}
			for(int j = 0; j < 4; j++)
			{
				c = getc(fcurent);
				durata[j] = c - '0';
				c = getc(fcurent); //ignora caracter de separare
			}
			for(int j = 0; j < 4; j++)
			{
				c = getc(fcurent);
				paritate[j] = c - '0';
				c = getc(fcurent); //ignora caracter de separare
			}
			index_titular = atoi( fgets(line, 10, fcurent) );
			//c = getc(fcurent);
			//index_titular = c - '0';
			//c = getc(fcurent);//scos enter
			addlist(L_D[i], index, name, acronim, tip, durata, paritate, index_titular);
			//fctie de adaugare an
		}
		//printf("a inchis fisiereul %s \n", f_disc);
		fclose(fcurent);
	}
}

//fctie de citire nr_grupe pe orar
void citire_nr_grupe(numar_grupe v_grupe[])
{	
	FILE *fcurent;
	char line[100];
	fcurent = fopen("grupe.in", "r");
	int index_gr, nr_gr, nr_o;
	while (fscanf( fcurent, "%s", line ) > 0)
	{
		index_gr = atoi(line) - 1;
		fscanf( fcurent, "%s", line );
		nr_gr = atoi(line);
		v_grupe[index_gr].nr_grupe = nr_gr;
		fscanf( fcurent, "%s", line );
		nr_o = atoi(line);
		v_grupe[index_gr].nr_optionale = nr_o;
		
	}
	fclose(fcurent);
}

//fctie de initializare pentru orare
void initializare_orar(ORAR ORAREV[])
{
	//pentru fiecare orar
	for(int i = 0; i < NR_ORARE; i++)
	{
		//pentru fiecare casuta de ocupare
		for(int linie = 0; linie < 5; linie++)
			for(int col = 0; col < 22; col++)
			{
				ORAREV[i][linie][col].index_titular = 0;
				ORAREV[i][linie][col].li_grupa.nr_elem = 0;
				ORAREV[i][linie][col].li_grupa.pfirst = NULL;
				ORAREV[i][linie][col].li_grupa.plast = NULL;
				ORAREV[i][linie][col].nume_materie = NULL;
				ORAREV[i][linie][col].paritate = 0;
				ORAREV[i][linie][col].pnext = NULL;
				ORAREV[i][linie][col].sala = 0;
				ORAREV[i][linie][col].semigrupa = 0;
			}
	}
}