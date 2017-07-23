#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "header1.h"
//extern ORAR ORAREV[NR_ORARE]; 
//extern ORAR ORARE_L[NR_ORARE];

void setare_semestru(lista_discipline L_D[], int sem)
{
	nod_disciplina *p;
	for(int i = 0; i < NR_ORARE; i++)
	{
		p = L_D[i].pfirst;
		while(p != NULL)
		{
			p->sem = sem;
			p = p->pnext;
		}
	}
}

//fctie de setarea materiilor care pot avea lab pe semigrupe, pune un 2 unde e cazul
void setare_grupelab(lista_discipline L_D[])
{
	FILE *fcurent;
	fcurent = fopen("lab_semigrupe1.in", "r");
	char line[100];
	nod_disciplina *p;
	//citire an 1
	while(fgets(line, 100, fcurent))
	{
		for(int i = 0; i < 3; i++)
		{
			p = L_D[i].pfirst;
			while( p != NULL)
			{
				if( strcmp(line, p->acronim) == 0 )
					p->nr_lab = 2;
				p = p->pnext;
			}
		}
	}
	fclose(fcurent);
	//citire an 2
	fcurent = fopen("lab_semigrupe2.in", "r");
	while(fgets(line, 100, fcurent))
	{
		for(int i = 3; i < 6; i++)
		{
			p = L_D[i].pfirst;
			while( p != NULL)
			{
				if( strcmp(line, p->acronim) == 0 )
					p->nr_lab = 2;
				p = p->pnext;
			}
		}
	}
	fclose(fcurent);
	//citire an 3
	fcurent = fopen("lab_semigrupe3.in", "r");
	while(fgets(line, 100, fcurent))
	{
		for(int i = 6; i < 9; i++)
		{
			p = L_D[i].pfirst;
			while( p != NULL)
			{
				if( strcmp(line, p->acronim) == 0 )
					p->nr_lab = 2;
				p = p->pnext;
			}
		}
	}
	fclose(fcurent);
	//citire an 4
	fcurent = fopen("lab_semigrupe4.in", "r");
	while(fgets(line, 100, fcurent))
	{
		for(int i = 9; i < 14; i++)
		{
			p = L_D[i].pfirst;
			while( p != NULL)
			{
				if( strcmp(line, p->acronim) == 0 )
					p->nr_lab = 2;
				p = p->pnext;
			}
		}
	}
	fclose(fcurent);

}

void init_lista_grupe(lista_grupe &li)
{
	li.nr_elem = 0;
	li.pfirst = li.pfirst = NULL;
}

void init_ocupare(ocupare *oc)
{
	oc->nume_materie = NULL;
	init_lista_grupe(oc->li_grupa);
	oc->pnext = NULL;
}

//atentie la indici i - numarul orarului din ORAREV[] si j e un nr de grupa
//returneaza 1 pentru prima semigrupa, 2 pentru aIIa si 3 pentru grupa mare
int exista_grupa(ocupare poc, int j, int i)
{
	int nr_semi; //nr semigrupei
	int nr_grupa;//nr grupei mari
	nod_grupa *g;
	nod_grupa *g_mare;
	nod_grupa *p;
	
	if (j % 2 == 0)
		nr_semi = 1;//suntem la prima
	if (j % 2 == 1)
		nr_semi = 2;//suntem la a 2a
	nr_grupa = j / 2 + 1;
	if (i >= 14)
		nr_grupa = j;
	if (i < 14)
		g_mare = generare_nume_grupa(i, nr_grupa, 0);
	if( i >= 14)
		g_mare = generare_nume_grupa(i, nr_grupa + 1, 0);
	g = generare_nume_grupa(i, nr_grupa, nr_semi);
	//vedem daca semigrupa se afla in lista de grupe pentru un laborator

	//pentru prima semigrupa afisez si daca se afla grupa in lista si daca se afla grupa mare
	if(poc.tip != 0)
	{
		p = poc.li_grupa.pfirst;
		//la ani licenta
		if (i < 14)
			while(p != NULL)
			{
				if( nr_semi == 1)
				{
					if ( strcmp(p->nume, g->nume) == 0)
						return 1;
					if ( strcmp(p->nume, g_mare->nume) == 0) 
						return 3;
				}
				if( nr_semi == 2)
				{
					if ( strcmp(p->nume, g->nume) == 0)
						return 2;
				}
				p = p->pnext;
			}
		//la master
		if (i >= 14)
			while(p != NULL)
			{
				if ( strcmp(p->nume, g_mare->nume) == 0) 
						return 3;
				p = p->pnext;
			}
	}
	
	
	return 0;
}

void copy_orar(ORAR ORARED[], ORAR ORARES[])
{
	
	ocupare *paux;
	int initial;
	for(int i = 0; i < NR_ORARE; i++)
		for(int z = 0; z < 5; z++)
			for(int ora = 8; ora < 22; ora++)
			{
				if (ORARES[i][z][ora].nume_materie != NULL)
				{
					paux = &ORARES[i][z][ora];
					initial = 0;
				}
				else
					continue;
				while(paux != NULL)
				{
					ocupare *poc = new ocupare;
					poc->index_titular = paux->index_titular;
					poc->tip = paux->tip;
					poc->paritate = paux->paritate;
					poc->pnext = NULL;
					//copiere numele de materie si de sala
					poc->nume_materie = (char*)calloc( 80, sizeof(char));
					strcpy(poc->nume_materie, paux->nume_materie);
					poc->sala = (char*)calloc( 10, sizeof(char));
					strcpy(poc->sala, paux->sala);
					poc->li_grupa.nr_elem = 0;
					poc->li_grupa.pfirst = NULL;
					poc->li_grupa.plast = NULL;
					//copiat lista grupe
					nod_grupa *ngr = paux->li_grupa.pfirst ;
					while(ngr != NULL)
					{
						addlist(poc->li_grupa, ngr->nume);
						ngr = ngr->pnext;
					}
					//copiat lista cu restul de oc
					if (initial == 0)
						ORARED[i][z][ora] = *poc;
					if(initial == 1)//trebuie ca poc sa fie in adaugat intr-o lista la ORARED[i][z][ora]
					{
						ocupare *pp;
						pp = &ORARED[i][z][ora];
						while( pp->pnext != NULL)
							pp = pp->pnext;
						pp->pnext = poc;
					}
					
					initial = 1;
					paux = paux->pnext;
					//printf("ORAR%d am copiat materie %s, zi %d ora %d\n", i, poc->nume_materie, z, ora);
						
				}//while casute 
			}
	
}

//fctie ce elibereaza memoria ocupata de un vector de orare pentru salile de lab
void eliberare_mem(ORAR_LAB *buffer, int nr_elem)
{
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				if(buffer[i][z][o].nume != NULL)
				{
					oc_lab *oc;
					oc = &buffer[i][z][o];
					free(oc->nume);
					while(oc->pnext != NULL)
					{
						oc = oc->pnext;
						free(oc->nume);
					}
				}
			}
	free(buffer);
}

//fctie ce elibereaza memoria ocupata de un vector de orare pentru salile de curs
void eliberare_mem(ORAR_CURS *buffer, int nr_elem)
{
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				if(buffer[i][z][o].nume_materie != NULL)
				{
					oc_curs *oc;
					oc = &buffer[i][z][o];
					free(oc->nume_materie);
					free(oc->acronim);
					free(oc->serie);
					while(oc->pnext != NULL)
					{
						oc = oc->pnext;
						free(oc->nume_materie);
						free(oc->acronim);
						free(oc->serie);
					}
				}
			}
	free(buffer);
}

void eliberare_mem(ORAR ORARED[])
{
	for(int i = 0; i < NR_ORARE; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				if(ORARED[i][z][o].nume_materie != NULL)
				{
					ocupare *oc;
					oc = &ORARED[i][z][o];
					free(oc->nume_materie);
					free(oc->sala);
					while(oc->pnext != NULL)
					{
						oc = oc->pnext;
						free(oc->nume_materie);
						free(oc->sala);
					}
				}
			}
}