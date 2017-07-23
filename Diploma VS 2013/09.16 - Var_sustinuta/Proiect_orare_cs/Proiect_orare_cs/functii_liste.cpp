#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "header1.h"
#include <ctype.h>

//adaugare lista titulari
void addlist(lista_titulari &Li, int index, char name[])
{
	nod_titulari *a = new nod_titulari;
	a->index = index;
	//puts(name);
	//printf("%d", a->index);
	a->nume =  (char *)calloc( 40, sizeof( char ) );
	strcpy(a->nume, name);
	Li.nr++;
	a->pnext = NULL;
	//printf("a intrat in functie");
	if(Li.pfirst == NULL)
	{
		Li.pfirst = a;
		Li.plast = a;
	}
	else
	{
		Li.plast->pnext = a;
		Li.plast = a;
	}	
}

//adaugare lista sali 
void addlist(lista_sali &Li, int index, char name[])
{
	nod_sali *a = new nod_sali;
	a->index = index;
	
	a->nume =  (char *)calloc( LUNGIME_NUME, sizeof( char ) );
	strcpy(a->nume, name);
	Li.nr_elem++;
	a->pnext = NULL;
	//printf("a intrat in functie");
	if(Li.pfirst == NULL)
	{
		Li.pfirst = a;
		Li.plast = a;
	}
	else
	{
		Li.plast->pnext = a;
		Li.plast = a;
	}
	//printf("%d - ", a->index);
	//puts(name);
}



//adaugare lista disciplina
void addlist(lista_discipline &li, int index, char nume[], char acronim[], int tip[], 
			 int durata[], int paritate[], int index_titular)
{
	nod_disciplina *p = new nod_disciplina;
	p->index = index;
	p->nume =  (char *)calloc( LUNGIME_NUME, sizeof( char ) );
	strcpy(p->nume, nume);
	p->acronim = (char *)calloc( LUNGIME_NUME, sizeof( char ) );
	strcpy(p->acronim, acronim);
	p->sem = 1;
	p->nr_lab = 0;
	for(int i = 0; i < 4; i++)
	{
		p->tip[i] = tip[i];
		p->durata[i] = durata[i];
		p->paritate[i] = paritate[i];
	}
	p->index_titular = index_titular;
	li.nr++;
	p->pnext = NULL;
	if(li.pfirst == NULL)
	{
		li.pfirst = p;
		li.plast = p;
	}
	else
	{
		li.plast->pnext = p;
		li.plast = p;
	}
}

//adaugare in lista de grupe
void addlist(lista_grupe &Li, char *nume)
{
	nod_grupa *a = new nod_grupa;
	a->nume =  (char *)calloc( 15, sizeof( char ) );
	strcpy(a->nume, nume);
	Li.nr_elem++;
	a->pnext = NULL;
	if(Li.pfirst == NULL)
	{
		Li.pfirst = a;
		Li.plast = a;
	}
	else
	{
		Li.plast->pnext = a;
		Li.plast = a;
	}	
}

//fctie ce returneaza pointer la un titular (dupa indice)
nod_titulari* getat(lista_titulari li, int index)
{
	if(index > li.nr)
	{
		printf("pozitie inexistenta");
		return NULL;
	}
	nod_titulari *p = li.pfirst;
	for(int j = 1; j < index; j++)
		p = p->pnext;
	return p;
}

//fctie care returneaza indicele unui titular dupa nume
int getat(lista_titulari li, char *nume_cautat)
{
	nod_titulari *p;
	p = li.pfirst;
	int contor = 1;
	while( p != NULL)
	{	
		if(p->nume[strlen(p->nume) - 1] == '\n')
			p->nume[strlen(p->nume) - 1] = '\0';
		while( isspace(p->nume[strlen(p->nume) - 1]) )
			p->nume[strlen(p->nume) - 1] = '\0';
		while( isspace(nume_cautat[strlen(nume_cautat)-1]) )
			nume_cautat[strlen(nume_cautat)-1] = '\0';
		if( strcmp(p->nume, nume_cautat) ==0 )
			return p->index;
		p = p->pnext;
		contor++;
	}
	return -1;
}

//fctie care returneaza indicele unei sali de curs dupa nume
int getat(lista_sali li, char *nume_cautat)
{
 nod_sali *p;
 p = li.pfirst;
 while( p != NULL )
 {
	 if(p->nume[strlen(p->nume) - 1] == '\n')
		p->nume[strlen(p->nume) - 1] = '\0';
	 if( strcmp(p->nume, nume_cautat) == 0)
		 return p->index;
	 p = p->pnext;
 }
 return -1;
}



//returneaza pointer la disciplina pentru adaugare in orar
nod_disciplina* getat(char *cuv, lista_discipline Li, int tip)
{
	nod_disciplina *p;
	p = Li.pfirst;
	if(tip == 0) //adica este un curs
	{
		//prelucrare cuvant sa nu mai aiba spatii
		while( isspace(cuv[strlen(cuv) - 1]) )
			cuv[strlen(cuv) - 1] = '\0';
		
		while(p != NULL)
		{
			//scoate ENTER din fisierele de discipline
			if(p->nume[strlen(p->nume) - 1] == '\n')
				p->nume[strlen(p->nume) -1] = '\0';
			//sa nu fie spatii in plus in fisierele de discipline
			while (isspace(p->nume[strlen(p->nume) - 1]) )
				p->nume[strlen(p->nume) - 1] = '\0';
			if( strcmp(cuv, p->nume) == 0 )
				return p;
			p = p->pnext;
		}
	}//inchidere pentru curs
	else
	{
		while ( p != NULL)
		{
			if( strcmp( cuv, p->acronim) == 0)
				return p;
			p = p->pnext;
		}
	}
	return NULL;
}