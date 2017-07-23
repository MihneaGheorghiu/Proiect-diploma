#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "header1.h"
#include <time.h>

extern int apeluri_optimizare;
extern int apeluri_optimizare_grupe_l;
extern int apeluri_optimizare_2;
extern int tip_apel;
extern int tip_apel_gr;
int call_stack;
//ceas pentru a masura cat dureaza rularea programului
clock_t start, finish;


//variabile globale
numar_grupe v_grupe[NR_ORARE];
char* nume_orare[NR_ORARE];//lista cu numele orarelor
//vector de orare pentru a retine orarele vechi, cate 1 
ORAR ORAREV[NR_ORARE]; 
//vector de liste discipline (ptr fiecare orar cate o lista)
lista_discipline L_D[NR_ORARE];
//vector de orare pentru salile de laborator
ORAR_LAB *buffer_lab;
//vector orare sali de seminar
ORAR_LAB *buffer_seminar;
//vector de orare pentru salilele de laborator si de seminar in lucru
ORAR_LAB *buffer_lab_l;
ORAR_LAB *buffer_seminar_l;
//vector de orare pentru salilele de laborator si de seminar optim
ORAR_LAB *buffer_lab_f;
ORAR_LAB *buffer_seminar_f;
ORAR_LAB *buffer_p;
ORAR_LAB *buffer_p_f;
ORAR_LAB *buffer_p_l;
//vector de orare pentru salile de curs
ORAR_CURS *buffer_curs_l;
ORAR_CURS *buffer_curs_f;
//liste de sali laborator
lista_sali Li_lab;
lista_sali Li_sem;
lista_sali Li_curs;
lista_sali Li_p;

//costul pentru fiecare semigrupa in orarul vechi (licenta)
COST *cost_init_1;
//costul pentru fiecare grupa din orarul vechi (master)
COST *cost_init_2;
//costul pentru fiecare grupa din orarul vechi (licenta)
COST *cost_init_gr;
//ora de pornire si ora de terminare a orelor pentru fiecare semigrupa pentru fiecare zi
val_ore *ora_start[5];//vector cu 5 pointeri la structuri de cate 2 int
val_ore *ora_finis[5];
val_ore *ora_start_gr[5];//vector cu 5 pointeri la structuri de cate 2 int
val_ore *ora_finis_gr[5];
//costuri finale optime, dupa procesare
COST *cost_f_1, *cost_f_2, *cost_f_gr;
val_ore *ora_f_start[5], *ora_f_finis[5], *ora_f_start_gr[5], *ora_f_finis_gr[5];
//costuri de lucru
COST *cost_l_1, *cost_l_2, *cost_l_gr;
//intervale de ore pe timpul cautarii optimului
val_ore *ora_l_start[5], *ora_l_finis[5];
val_ore *ora_l_start_gr[5], *ora_l_finis_gr[5];
//vector de orare finale
ORAR ORARE_F[NR_ORARE];
//vector de orare de lucru
ORAR ORARE_L[NR_ORARE];
int suma_nr_grupe_l; //nr semigrupe licenta
int suma_nr_grupe_m;//nr grupe master 
int nr_grupe_l;//nr grupe la licenta
//costul total initial
int cost_t_i_1; //licenta
int cost_t_i_2; //master
int cost_t_o_1;
int cost_t_o_2;
int cost_t_i_gr;
int cost_t_o_gr;
//tabel cu titulari la sfarsit
ORAR_PF *buffer_titulari_f;



//adaugare grupe suplimentare pentru laburi
void adaugare_grupe_lab_sup()
{
	FILE *fcurent;
	fcurent = fopen("supliment_lab.in","r");
	char line[100];
	int nr_orar, zi, ora, tip, nr_gr, paritate;
	char *acronim, *sala, *numar_curent;
	int cursor, c_p;
	
	

	while(fgets(line, 100, fcurent))
	{
		acronim = (char*)calloc( 15, sizeof(char));
		sala = (char*)calloc( 10, sizeof(char));
		numar_curent = (char*)calloc( 4, sizeof(char));
		cursor = 0;
		c_p = 0;
		paritate = 0;//daca nu am nevoie va ramane 0
		if(line[0] == '#')
			continue;
		//nr_orar
		while( !isspace(line[cursor]) )
			numar_curent[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		nr_orar = atoi(numar_curent);
		//zi
		numar_curent = (char*)calloc( 4, sizeof(char));
		c_p = 0;
		while( !isspace(line[cursor]) )
			numar_curent[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		zi = atoi(numar_curent);
		//ora
		numar_curent = (char*)calloc( 4, sizeof(char));
		c_p = 0;
		while( !isspace(line[cursor]) )
			numar_curent[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		ora = atoi(numar_curent);
		//acronim
		c_p = 0;
		while( !isspace(line[cursor]) )
			acronim[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		//tip
		numar_curent = (char*)calloc( 4, sizeof(char));
		c_p = 0;
		while( !isspace(line[cursor]) )
			numar_curent[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		tip = atoi(numar_curent);
		//sala
		c_p = 0;
		while( !isspace(line[cursor]) )
			sala[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;
		//nr_grupa
		numar_curent = (char*)calloc( 4, sizeof(char));
		c_p = 0;
		while( !isspace(line[cursor]) )
			numar_curent[c_p++] = line[cursor++];
		while(isspace(line[cursor]))
			cursor++;//merge cat sunt spatii, deci si pana la [
		nr_gr = atoi(numar_curent);
		//paritate
		if (line[cursor] == '[')
		{
			cursor++;//trece de [
			numar_curent = (char*)calloc( 4, sizeof(char));
			c_p = 0;
			while( isdigit(line[cursor])  )
				numar_curent[c_p++] = line[cursor++];
			paritate = atoi(numar_curent);
		}

		nod_disciplina *pdisc = new nod_disciplina;
		pdisc = getat(acronim, L_D[nr_orar], tip);
		nod_grupa *g = new nod_grupa;
		g = generare_nume_grupa(nr_orar, nr_gr, 0);
		//adaug grupele suplimentare in orarul mare
		if ( ( strcmp( ORAREV[nr_orar][zi][ora].nume_materie, acronim) == 0)
			&& (ORAREV[nr_orar][zi][ora].paritate == paritate) )
			for(int j = 0; j < pdisc->durata[tip]; j++)
				addlist(ORAREV[nr_orar][zi][ora + j].li_grupa, g->nume);
		else //atunci am suprapunere
		{
			ocupare *po;
			for(int j = 0; j < pdisc->durata[tip]; j++)
			{
				po = ORAREV[nr_orar][zi][ora + j].pnext;
				while( po != NULL)
				{
					if( ( strcmp(po->nume_materie, acronim) == 0 )  && (po->paritate == paritate) )
					{
						addlist(po->li_grupa, g->nume);
						break;
					}
					else
						po = po->pnext;
				}
			}
		}
		//adaugare lab/semiar in orarul salii
		if(tip == 1)
		{
			int indice_sala = getat(Li_lab, sala);
			//fac numele pentru comparare
			//char nume_m[10];
			//int c_n_m = 0;
			//while( !isspace( buffer_lab[indice_sala - 1][zi][ora].nume[c_n_m]) )
			//	nume_m[c_n_m++] = buffer_lab[indice_sala - 1][zi][ora].nume[c_n_m++];
			if( (strcmp(buffer_lab[indice_sala - 1][zi][ora].nume, acronim) == 0) 
				&& (buffer_lab[indice_sala - 1][zi][ora].paritate == paritate) )
			{
				for(int j = 0; j < pdisc->durata[tip]; j++)
					addlist(buffer_lab[indice_sala - 1][zi][ora + j].li_grupa, g->nume);
				goto cont;
			}
			///in caz de suprapunere
			if(buffer_lab[indice_sala - 1][zi][ora].pnext != NULL)
			{
				oc_lab *p;
				for(int j = 0; j < pdisc->durata[tip]; j++)
				{
					p = buffer_lab[indice_sala - 1][zi][ora + j].pnext;
					while( p != NULL)
					{
						if( (strcmp(p->nume, acronim) == 0) && (p->paritate == paritate) )
							addlist(p->li_grupa, g->nume);
						p = p->pnext;
					}
				}//inchidere for durata
			}
				
			
		}
		if(tip == 2)
		{
			int indice_sala = getat(Li_sem, sala);
			if( (strcmp(buffer_seminar[indice_sala - 1][zi][ora].nume, acronim) == 0) 
				&& (buffer_seminar[indice_sala - 1][zi][ora].paritate == paritate))
			{
				for(int j = 0; j < pdisc->durata[tip]; j++)
					addlist(buffer_seminar[indice_sala - 1][zi][ora + j].li_grupa, g->nume);
				goto cont;
			}
			///in caz de suprapunere
			if(buffer_seminar[indice_sala - 1][zi][ora].pnext != NULL)
			{
				oc_lab *p;
				for(int j = 0; j < pdisc->durata[tip]; j++)
				{
					p = buffer_seminar[indice_sala - 1][zi][ora + j].pnext;
					while( p != NULL)
					{
						if( (strcmp(p->nume, acronim) == 0) && (p->paritate == paritate) )
							addlist(p->li_grupa, g->nume);
						p = p->pnext;
					}
				}//inchidere for durata
			}	
		}
cont:	continue; //ptr rand nou
	free(acronim);
	free(sala);
	free(numar_curent);
	}//inchidere while citire pe randuri

}

//generare nume grupa pentru o singura grupa pentru seminarii/lab
nod_grupa* generare_nume_grupa(int i, int nr, int semigrupa)
{
	nod_grupa* a = new nod_grupa;
	a->pnext = NULL;
	a->nume = (char*)calloc( 15, sizeof(char) );
	int cursor = 0;
	int cs, s;
	int an;
	char* nr_an, *nr_gr, *s_gr;
	char* serie;
	nr_an = (char*)calloc( 3, sizeof(char) );
	nr_gr = (char*)calloc( 3, sizeof(char) );
	s_gr = (char*)calloc( 3, sizeof(char) );
	//generare numele seriei
	serie = (char *)calloc( 15, sizeof( char ) );
	if( serie != NULL )
      ;
   else
      printf( "Eroare la alocare serie\n" );

	cs = 0;
	while( nume_orare[i][cs] != 'C' )
		cs++;
	//incepe numele seriei
	s = 0;
	while( nume_orare[i][cs] != '.')
		serie[s++] = nume_orare[i][cs++];
	if( nr > v_grupe[i].nr_grupe)
		printf("###eroare grupe orar %d grupa %d", i, nr);
	if(i < 9) //in primii 3 ani
	{
		strcpy(a->nume, "3");
		an = i/3 + 1;
		_itoa(an ,nr_an, 10);
		strcat(a->nume, nr_an);
		_itoa(nr, nr_gr, 10);
		strcat(a->nume, nr_gr);
		strcat(a->nume, serie);
		free(serie);
		goto inchidere;
	}
	//anul 4
	if( (i >= 9) && (i < 14) )
	{
		//vad daca e grupa normala
		if( nr <= v_grupe[i].nr_grupe - v_grupe[i].nr_optionale)
		{
			strcpy(a->nume, "3");
			_itoa(4, nr_an, 10);
			strcat(a->nume, nr_an);
			_itoa(nr, nr_gr, 10);
			strcat(a->nume, nr_gr);
			strcat(a->nume, serie);
			//free(serie);

		}
		//daca e optionala
		else
		{
			if( i <= 12)
				if(nr == v_grupe[i].nr_grupe - v_grupe[i].nr_optionale + 1)
					strcpy(a->nume, "Optional 1 si 2");
				else
					printf("###eroare grupe orar %d grupa %d", i, nr);
			if( i == 13)
			{
				strcpy(a->nume, "Optional ");
				if(nr == 3)
					strcat(a->nume, "1 si 2");
				if(nr == 4)
					strcat(a->nume, "3 si 4");
			}
		}//inchid else optionala
		//free(serie);
		goto inchidere;
	}//inchid anul 4
	//anul 5
	if(i < 25)
	{
		if( i == 17)
		{
			strcpy(a->nume, "Optional ");
			_itoa(nr, nr_gr, 10);
			strcat(a->nume, nr_gr);
		}
		else
			if (nr <= v_grupe[i].nr_grupe - v_grupe[i].nr_optionale)
			{
				strcpy(a->nume, "Subgrupa ");
				_itoa(nr, nr_gr, 10);
				strcat(a->nume, nr_gr);
			}
			else
				strcpy(a->nume, "Optionali");
		free(serie);
		goto inchidere;	
	}//inchid anul 5

	//anul 6
	if(nr <= v_grupe[i].nr_grupe - v_grupe[i].nr_optionale )
	{
		strcpy(a->nume, "Subgrupa ");
		_itoa(nr, nr_gr, 10);
		strcat(a->nume, nr_gr);
	}
	else
		strcpy(a->nume, "Optionali");
	

inchidere:	if(semigrupa != 0)
	{
		strcat(a->nume, "{");
		_itoa(semigrupa, s_gr, 10);
		strcat(a->nume, s_gr);
		strcat(a->nume, "}");
	}
	free(nr_an);
	free(nr_gr);
	free(s_gr);
	
	return a;
}

//functie de generare nume_grupe
lista_grupe generare_nume_grupe(int i)
{
	
	lista_grupe li_gr;
	li_gr.nr_elem = 0;
	li_gr.pfirst = li_gr.plast = NULL;
	char *grupa = (char *)calloc( 15, sizeof( char ) );
	char *serie = (char *)calloc( 15, sizeof( char ) );
	char* an = NULL;
	char *nr, *nr2;
	nr = (char *)calloc( 2, sizeof( char ) );
	int cs, s;
	int sp;//la anul 4 pentru specializari
	if (i < 3) // anul 1
		an = "1";
	else 
		if (i < 6) //anul 2
			an = "2";
		else if (i < 9) // anul 3
				an = "3";
	
	if( an != NULL)
	{
		
		for(int j = 0; j < v_grupe[i].nr_grupe - v_grupe[i].nr_optionale; j++)
		{
			strcpy(grupa, "3");
			//grupa[100]= 'a';
			strcat(grupa, an);
			_itoa( j + 1, nr, 10);
			strcat(grupa, nr);
			
			//generare numele seriei
			
			serie = (char *)calloc( 15, sizeof( char ) );
			cs = 0;
			while( nume_orare[i][cs] != 'C' )
				cs++;
			//incepe numele seriei
			s = 0;
			while( nume_orare[i][cs] != '.')
				serie[s++] = nume_orare[i][cs++];
			strcat(grupa, serie);
			addlist(li_gr, grupa);
		}
		
		return li_gr;
	}
//////anul 4
	if (i < 14) 
	{
		//grupe normale
		for(int j = 0; j < v_grupe[i].nr_grupe - v_grupe[i].nr_optionale; j++)
		{
			an = "4";
			
			strcpy(grupa, "3");
			strcat(grupa, an);
			_itoa( j + 1, nr, 10);
			strcat(grupa, nr);
			strcat(grupa, "C");
			sp = i % 8;
			_itoa( sp, nr, 10);
			strcat(grupa, nr);
			addlist(li_gr, grupa);
		}
		//optionale
		if (i < 12)
		{
			strcpy(grupa, "Optional 1 si 2");
			addlist(li_gr, grupa);
			
		}
		if (i == 12)
			for(int j = 0; j < v_grupe[i].nr_optionale; j++)
			{
				strcpy(grupa, "Optional 1 si 2");
				//_itoa( j + 1, nr, 10);
				//strcat(grupa, nr);
				addlist(li_gr, grupa);
			}
		if ( i == 13)
			for(int j = 0; j < v_grupe[i].nr_optionale; j++)
			{
				strcpy(grupa, "Optional ");
				if (j == 0)
				{
					nr = "1";
					nr2 = "2";
				}
				else
				{
					nr = "3";
					nr2 = "4";
				}
				strcat(grupa, nr);
				strcat(grupa, " si ");
				strcat(grupa, nr2);
				addlist(li_gr, grupa);
			}
		return li_gr;
	}
///////inchidere an 4
	
/////anul 5
	if (i < 25) 
	{
		if( i == 17)
		{	
			for(int j = 0; j < v_grupe[i].nr_optionale; j++)
			{
				strcpy(grupa, "Optional ");
				_itoa( j + 1, nr, 10);
				strcat(grupa, nr);
				addlist(li_gr, grupa);
			}
			free(nr);
			return li_gr;
		}
		for(int j = 0; j < v_grupe[i].nr_grupe - v_grupe[i].nr_optionale; j++)
		{	
			strcpy(grupa, "Subgrupa");
			_itoa( j + 1, nr, 10);
			strcat(grupa, nr);
			addlist(li_gr, grupa);
		}
		strcpy(grupa, "Optionali");
		addlist(li_gr, grupa);
		free(nr);
		return li_gr;
	}
//////anul 6
	for(int j = 0; j < v_grupe[i].nr_grupe - v_grupe[i].nr_optionale; j++)
	{
		strcpy(grupa, "Subgrupa ");
		_itoa( j + 1, nr, 10);
		strcat(grupa, nr);
		addlist(li_gr, grupa);
	}
	for(int j = 0; j < v_grupe[i].nr_optionale; j++)
	{
		strcpy(grupa, "Optionali");
		addlist(li_gr, grupa);
	}
	free(nr);
	return li_gr;
}

void  main()
{	
	//pornire ceas pentru executie
	start = clock();
/*variabile*/
	//nume tabele finale
	char *lab = "orar_sali_laboratoare";
	char *tabel_curs = "program_folosire_sali_curs";
	char *tabel_seminarii = "program_folosire_sali_seminar";

	FILE *fcurent; //pointer pentru parcurgeri
	char word[100];
	char line[200];
	char name[100];


//****incep citirea orarului vechi
//****incep citirea fisierelor.in
	
	//citeste vector de nume orare
	citire_nume_orare(nume_orare);

	//creare lista profesori din titulari.in
	lista_titulari Li_titulari;
	citire_titulari(Li_titulari);
	
	//citeste lista de discipline
	citire_discipline(L_D, nume_orare);

	//setarea semstrului (lucrez pe 1)
	setare_semestru(L_D, 1);
	
	//citire nr de grupe pe serie (orar)
	citire_nr_grupe(v_grupe);
	
	//citire sali de curs
	citire_sali(Li_curs,"sali_curs.in");
	
	//citire sali de laborator
	citire_sali(Li_lab,"sali_lab.in");
	
	//citire sali de seminar
	citire_sali(Li_sem,"sali_seminar.in");
	
	//citire sali de proiect
	citire_sali(Li_p, "sali_p.in");
	
	//setare nr de semigrupe pentru laborator
	//pentru anul I
	setare_grupelab(L_D);

	//le leg intre ele folosind fisierele din orare.in
		
	//ptr fiecare titular cate un orar specific
	ORAR_PF *buffer_titulari;
	buffer_titulari = (ORAR_PF *)calloc( Li_titulari.nr, sizeof( ORAR_PF ) );
	buffer_titulari_f = (ORAR_PF *)calloc( Li_titulari.nr, sizeof( ORAR_PF ) );
	
	
	//orar sali de lab - initializarea
	buffer_lab = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
	buffer_lab_l = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
	buffer_lab_f = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
	//orar sali de curs
	ORAR_CURS *buffer_curs;
	buffer_curs = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
	buffer_curs_l = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
	buffer_curs_f = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
	//orar sali de seminar - initializare
	buffer_seminar = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
	buffer_seminar_l = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
	buffer_seminar_f = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
	//orar sali de proiect
	buffer_p = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
	buffer_p_l = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
	buffer_p_f = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
	//initializare vector de orare vechi & cele cu care voi lucra
	initializare_orar(ORAREV);
	initializare_orar(ORARE_F);
	initializare_orar(ORARE_L);


//**incep citirea orarelor.csv
	int zi;
	int rand_titlu; //rand cu antet 
	int ora;
	int inceput;
	int cursor;
	int k; //parcurgere acronim
	int s; //parcurgere pentru numele seriei
	int cs; //cursor pentru mai sus
	char *cuv;
	char *acronim;
	char *f_disc;//fisier disc
	int paritate;
	int semigrupa;
	int nr_grupe;
	int nr_optionale;
	int nr_gr; //nr grupei pentru adaugare in orare
	int gr_c = 0;
	lista_grupe li_gr;
	
	for(int i = 0; i < NR_ORARE; i++) //i < NR_ORARE
	{
		f_disc =  (char *)calloc( 40, sizeof( char ) );
		strcpy(f_disc, "CSV/");
		strcat(f_disc, nume_orare[i]);
		//deschid fisierul CSV de orar
		fcurent = fopen(f_disc, "r");
		printf("deschid %s \n", f_disc);
		
		inceput = 0;//inca nu a inceput orarul efectiv
		zi = 0; //
		char *c_ora;
		c_ora = (char *)calloc( 2, sizeof( char ) );
		int t; //ptr c_ora
		ora = 0;
		char *nume_sala;
		char *nume_titular;
		char *serie;
		int trecut_ora;
		int tip; // 1 ptr lab, 2 seminar 3 proiect
		char ctip; //caracterul ptr tip
		rand_titlu = 0;
		while(fgets(line, 200, fcurent))
		{
			cursor = 0;
			trecut_ora = 0;
			//parcurg fisierul pana dau de Luni
			//daca am terminat
			if(line[0] == 'E')
				break;
			//daca dau de luni
			if (line[0] == 'L')
				inceput = 1;
		
			if( !inceput )
				continue;
			else
			{
				if( isalpha(line[0]) )
				{
					zi++;//o noua zi a sapt
					while(line[cursor] != ',') //termin cuv ptr ziua sapt
					{	//printf("%c", line[cursor]);
						cursor++;
					}
					//printf(" ");
				}
				//acum sunt la ,
				while(line[cursor] != '\n')
				{	
					// avanseaza pana gaseste ora/ cursuri
					while(line[cursor] == ',')
					{
						if( trecut_ora == 1)
							gr_c++;
						cursor++;
					}
				
					//iau ora pentru acest rand
					if (isdigit(line[cursor]) )
					{
						t = 0;
						while(isdigit(line[cursor]) )
							c_ora[t++] = line[cursor++];
						ora = atoi(c_ora);
						trecut_ora = 1; //a fost ora pe randul respectiv
						gr_c = 0;
						//printf("%d ", ora);
						//restul de ora
						while(line[cursor] !=  ',')
							cursor++;
					}
					//incepe un seminar/lab/proiect
sem:				if (isalpha(line[cursor]) )
					{	
						paritate = 0; //daca nu va fi necesar
						semigrupa = 0;//daca nu va fi necesar
						k = 0;
						acronim = (char *)calloc( 33, sizeof( char ) );
						while(line[cursor] != '(')
							acronim[k++] = line[cursor++];
						//printf("%s", acronim);
						//printf("in");
						cursor++; //trec de (
						ctip = line[cursor];
						cursor++; //sunt la )
						while( !isalpha(line[cursor]) )
							cursor++;
						nume_sala = (char *)calloc( 12, sizeof( char ) );
						t = 0;
						while( isalpha(line[cursor]) || isdigit(line[cursor]) )
							nume_sala[t++] = line[cursor++];
				
						//printf("%s   ", nume_sala);
						//sunt la [ sau { sau ,
						if (line[cursor] == '[') //daca am paritate
						{
							cursor++; //trec de [
							paritate = line[cursor] - '0';
							cursor++; //trec la]
							cursor++;//trec de ]
						}
						//sunt la { sau ,
						if (line[cursor] == '{') //daca am semigrupe
						{
							cursor++; //trec de {
							semigrupa = line[cursor] - '0';
							cursor += 2; //trec de cifra si de }
						}
						//caut acronim in lista de discipline
						if( ctip == 'l')
							tip = 1;
						if( ctip =='s' )
							tip = 2;
						if( ctip =='p' )
							tip = 3;
						//aflu disciplina pentru a afla durata
						nod_disciplina *pdisc = new nod_disciplina;
						pdisc = getat(acronim, L_D[i], tip);
						//adaugare lab/seminar/proiect in orar mare
						//adaugare grupe 
						nod_grupa *g = new nod_grupa;
						if (i < 14)
						{
							nr_gr = gr_c / 2;
							if( (gr_c % 2) == 1)
								nr_gr++;
						}
						else
							nr_gr = gr_c;
						g = generare_nume_grupa(i, nr_gr, semigrupa);
						addsem(ORAREV, i , zi - 1, ora, tip,
							acronim, pdisc->durata[tip], paritate, nume_sala, g);
						//generare numele seriei
						serie = (char *)calloc( 15, sizeof( char ) );
						cs = 0;
						while( !isdigit( nume_orare[i][cs]) )
							cs++;
						//incepe numele seriei
						s = 0;
						while( nume_orare[i][cs] != '.')
							serie[s++] = nume_orare[i][cs++];
						//adaugare in tabel sali lab/seminar
						if( tip == 1)
						{
							int indice_sala = getat(Li_lab, nume_sala);
						    addsem(buffer_lab, indice_sala - 1, zi - 1, ora, pdisc, tip, serie, g);
						}
						//adaugare in tabel sali seminar
						if( tip == 2) 
						{
							int indice_sala = getat(Li_sem, nume_sala);
							addsem(buffer_seminar, indice_sala - 1, zi - 1, ora, pdisc, tip, serie, g);
						}
						if ( tip == 3)
						{
							int indice_sala = getat(Li_p, nume_sala);
							addsem(buffer_p, indice_sala - 1, zi - 1, ora, pdisc, tip, serie, g);
						}

						if (line[cursor] == '#') //avem suprapunere
						{
							cursor++;
							if (isalpha(line[cursor]) ) //incepe un seminar
								goto sem;
							if (line[cursor] == '"') //incepe un curs
								goto curs;
						}
						
					}

					//incepe un curs
curs:				if (line[cursor] =='"')
					{
						paritate = 0; //daca vom avea nevoie de paritate
						t = 0;
						cuv = (char *)calloc( 80, sizeof( char ) );
						cursor++; //scap de " si intru in numele cursului
						while (line[cursor] != '(') 
							cuv[t++] = line[cursor++];
						//printf("%s cu ", cuv);
						//acum sunt la (
						while(line[cursor] != ',')
							cursor++;
						//acum sunt la , dinainte de titular
						while(!isalpha(line[cursor]) )
							cursor++;
						//iau numele titularului
						t = 0;
						nume_titular = (char *)calloc( 40, sizeof( char ) );
						while(line[cursor] != '(')
							nume_titular[t++] = line[cursor++];
						//printf("%s in", nume_titular);
						//iau numele salii
						cursor++; //scap de ( si trec pe numele salii
						t = 0;
						nume_sala = (char *)calloc( 10, sizeof( char ) );
						while(line[cursor] != ')')
						{
							if ( isspace(line[cursor]) )
								cursor++;
							else
							 nume_sala[t++] = line[cursor++];
						}
						//printf("%s ", nume_sala);
						//sunt la')'
						cursor++; //trec de )
						//sar peste vreun spatiu pus accidental
						while(isspace(line[cursor]))
							cursor++;
						if(line[cursor] == '[') //avem paritate la curs
						{
							cursor++; //trec de [
							paritate = line[cursor] - '0';
							cursor += 2; //trec de paritate si de ]
						}
						cursor++; //trec de " de inchidere
						//adaugare
						nod_disciplina *pdisc = new nod_disciplina;
						pdisc = getat(cuv, L_D[i], 0);
						//creat lista de grupe
						nr_grupe = v_grupe[i].nr_grupe;
						nr_optionale = v_grupe[i].nr_optionale;
						lista_grupe li_gr = generare_nume_grupe(i);
						//adaugare in orar mare
						addcurs(ORAREV, i , zi - 1, ora, pdisc,
							nume_sala, nr_grupe, nr_optionale, paritate, li_gr);
						//adaugare in orarul profesorului
						int indice_titular = getat(Li_titulari, nume_titular);
						//printf("%d  ", indice_titular);
						int indice_sala = getat(Li_curs, nume_sala);
						//printf("sala = %d\n", indice_sala);
						//generare numele seriei
						
						serie = (char *)calloc( 15, sizeof( char ) );
						cs = 0;
						while( !isdigit( nume_orare[i][cs]) )
							cs++;
						//incepe numele seriei
						s = 0;
						while( nume_orare[i][cs] != '.')
							serie[s++] = nume_orare[i][cs++];
						addcurs(buffer_titulari, indice_titular - 1, zi - 1, ora, cuv, 
							indice_sala, pdisc->durata[0], paritate, serie);
						
						addcurs(buffer_curs, indice_sala - 1, zi - 1, ora, pdisc, paritate, serie);
						
						if(line[cursor] == '#') //daca avem suprapunere
						{
							cursor++; //trec de #
							if (isalpha(line[cursor]) ) //incepe un seminar
								goto sem;
							if (line[cursor] == '"') //incepe un curs
								goto curs;
						}
					}
					
				
				}//inchid while de rand
				//printf("\n");
			
				
			}//inchidere else
		
		}//inchidere while pe randuri fisier

	fclose(fcurent);
	} //inchidere for de citit orare vechi
	
//###//incarcare lab/sem/p care sunt la mai multe grupe
	adaugare_grupe_lab_sup();
	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "timp de lucru citire date intrare %f secunde\n\n", duration );	

//###################ALGORITM
	
	//	test_extern();
//calculez costul pentru fiecare semigrupa din orarul vechi
	printf("PRIMA ETAPA DE OPTIMIZARE #1 _semigrupe\n");
	suma_nr_grupe_l = 0; //numarul de semigrupe totale ptr licenta
	for(int i = 0; i < ANI_LIC; i++)
		suma_nr_grupe_l += 2 * v_grupe[i].nr_grupe;
	
		cost_init_1 = (COST*)calloc( suma_nr_grupe_l, sizeof(COST));

	suma_nr_grupe_m = 0; //numarul de grupe totale ptr master
	for(int i = 0; i < ANI_MASTER; i++)
		suma_nr_grupe_m += v_grupe[ANI_LIC + i].nr_grupe;
	cost_init_2 = (COST*)calloc( suma_nr_grupe_m, sizeof(COST));

//****alocare a memoriei pentru vectorii care tin intervalele depornire/incepere a orelor
	for(int z = 0; z < 5; z++)
	{	
		ora_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_l_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_l_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_f_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_f_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
	}

	int contor;
	tip_apel= 0;
	contor = 0; //tine nr grupei
	//calcul cost initial pentru anii de licenta
	for(int i = 0; i < ANI_LIC; i++)
		for(int j = 0; j < 2 * v_grupe[i].nr_grupe; j++)
		{
			cost_init_1[contor].cost_impar = calc_cost_1(ORAREV[i], contor, ora_start, ora_finis, 1);
			cost_init_1[contor].cost_par = calc_cost_1(ORAREV[i], contor, ora_start, ora_finis, 2);
			//printf("orar %d grupa %d costul pe impara %d \n", i + 1, j + 1, cost_init_1[contor].cost_impar);
			//printf("orar %d grupa %d costul pe para %d \n", i + 1, j + 1, cost_init_1[contor].cost_par);
			contor++;	
		}
	
	//printf("suma_nr_grupe_l = %d\n", suma_nr_grupe_l);
	//printf("contor = %d\n", contor);
	
	//calcul cost initial pentru anii de master
	for(int i = 0; i < ANI_MASTER; i++)
		for(int j = 0; j < v_grupe[ANI_LIC + i].nr_grupe; j++)
		{
			cost_init_2[contor - suma_nr_grupe_l].cost_impar = 
				calc_cost_2(ORAREV[ANI_LIC + i], contor, ora_start, ora_finis, 1);
			cost_init_2[contor - suma_nr_grupe_l].cost_par = 
				calc_cost_2(ORAREV[ANI_LIC + i], contor, ora_start, ora_finis, 2);
			//printf("orar %d grupa %d costul pe impara %d \n",
			//	ANI_LIC + i, j + 1, cost_init_2[contor - suma_nr_grupe_l].cost_impar);
			//printf("orar %d grupa %d costul pe para %d \n",
			//	ANI_LIC + i, j + 1, cost_init_2[contor - suma_nr_grupe_l].cost_par);
			contor++;	
		}

	//costurile initiale pentru toate grupele/semigrupele care vor da eficienta orarului
	cost_t_i_1 = 0;
	cost_t_i_2 = 0;
	for(int i = 0; i < suma_nr_grupe_l; i++)
	{
		cost_t_i_1 += cost_init_1[i].cost_impar;
		cost_t_i_1 += cost_init_1[i].cost_par;
	}
	cost_t_o_1 = cost_t_i_1;
	for(int i = 0; i < suma_nr_grupe_m; i++)
	{
		cost_t_i_2 += cost_init_2[i].cost_impar;
		cost_t_i_2 += cost_init_2[i].cost_par;
	}
	cost_t_o_2 = cost_t_i_2;

	//de copiat valorile pentru orele de lucru ale grupelor
	for(int i = 0; i < suma_nr_grupe_l + suma_nr_grupe_m; i++)
		for(int z = 0; z < 5; z++)
		{
			ora_l_start[z][i].val_impar = ora_start[z][i].val_impar;
			ora_l_start[z][i].val_par = ora_start[z][i].val_par;
			ora_l_finis[z][i].val_impar = ora_finis[z][i].val_impar;
			ora_l_finis[z][i].val_par = ora_finis[z][i].val_par;
		}
	
	//printf("suma_nr_grupe_m = %d\n", suma_nr_grupe_m);
	//printf("copiez ORAREV in ORARE_Lucru\n");
	copy_orar(ORARE_L, ORAREV);
	//printf("copiez ORAREV in ORARE_Finale\n");
	copy_orar(ORARE_F, ORAREV);
	//printf("copiez vectori de laboratoare\n");
	copy_orar_lab(buffer_lab_l, buffer_lab, Li_lab.nr_elem);
	
	copy_orar_lab(buffer_lab_f, buffer_lab, Li_lab.nr_elem);
	//printf("copiez vectori de seminarii\n");
	
	copy_orar_lab(buffer_seminar_l, buffer_seminar, Li_sem.nr_elem);
	copy_orar_lab(buffer_seminar_f, buffer_seminar, Li_sem.nr_elem);
	//printf("copiez vectori de proiecte\n");
	copy_orar_lab(buffer_p_l, buffer_p, Li_p.nr_elem);
	copy_orar_lab(buffer_p_f, buffer_p, Li_p.nr_elem);
	//printf("copiez vectori de sali curs\n");
	copy_orar_curs(buffer_curs_l, buffer_curs, Li_curs.nr_elem);
	copy_orar_curs(buffer_curs_f, buffer_curs, Li_curs.nr_elem);
	//printf("gata cu orarele de curs\n");
	//copie pentru costuri de lucru
	cost_l_1 = (COST*)calloc( suma_nr_grupe_l, sizeof(COST));
	/*if(cost_l_1 != NULL)
		 printf( "Allocated %d COST\n", suma_nr_grupe_l );
	else
      printf( "Can't allocate memory\n" );*/


	cost_l_2 = (COST*)calloc( suma_nr_grupe_m, sizeof(COST));
	
	for(int i = 0; i < suma_nr_grupe_l; i++)
	{
		cost_l_1[i] = cost_init_1[i];
		//printf("cost_l_1[%d]=%d \n" , i, cost_l_1[i]);
	}
	for(int i = 0; i < suma_nr_grupe_m; i++)
		cost_l_2[i] = cost_init_2[i];
	
	verifica_copiere_orar(buffer_lab_l, buffer_lab, Li_lab.nr_elem);
	verifica_copiere_orar(buffer_seminar_l, buffer_seminar, Li_sem.nr_elem);
	verifica_copiere_orar(buffer_p_f, buffer_p, Li_p.nr_elem);
	verifica_copiere_orar(buffer_curs_f, buffer_curs, Li_curs.nr_elem);
	

//#####rulam prima optimizare - semigrupe licenta
	apeluri_optimizare = 0;
	call_stack = 1;
	
	
	//for(int i = 0; i < suma_nr_grupe_l; i++)
		optimizare_1(ORARE_L, 0, 0);

	printf("costul total initial(semigrupe) este %d\n",cost_t_i_1);
	printf("costul total optim(semigrupe) este %d\n", cost_t_o_1);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "timp de lucru cu etapa 1 %f secunde\n\n", duration );

//#####rulam a 2a optimizare grupe licenta
//calculez costul pentru fiecare grupa din orarul vechi
	nr_grupe_l = 0; //numarul de grupe totale ptr licenta
	printf("ETAPA DE OPTIMIZARE #2 _grupe licenta \n");
	for(int i = 0; i < ANI_LIC; i++)
		nr_grupe_l += v_grupe[i].nr_grupe;
	
	cost_init_gr = (COST*)calloc( nr_grupe_l, sizeof(COST));
//****alocare a memoriei pentru vectorii care tine intervalele depornire/incepere a orelor
	
	for(int z = 0; z < 5; z++)
	{	
		ora_start_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
		ora_finis_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
		ora_l_start_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
		ora_l_finis_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
		ora_f_start_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
		ora_f_finis_gr[z] = (val_ore*)calloc( nr_grupe_l, sizeof(val_ore));
	}
	
	tip_apel_gr = 0;
	contor = 0;//scos int dupa test
	//calcul cost initial pentru anii de licenta
	for(int i = 0; i < ANI_LIC; i++)
		for(int j = 0; j < v_grupe[i].nr_grupe; j++)
		{
			cost_init_gr[contor].cost_impar = calc_cost_gr(ORARE_F[i], contor, ora_start_gr, ora_finis_gr, 1);
			cost_init_gr[contor].cost_par = calc_cost_gr(ORARE_F[i], contor, ora_start_gr, ora_finis_gr, 2);
			//printf("orar %d grupa %d costul pe impara %d \n", i + 1, j + 1, cost_init_gr[contor].cost_impar);
			//printf("orar %d grupa %d costul pe para %d \n", i + 1, j + 1, cost_init_gr[contor].cost_par);
			contor++;	
		}
	
	//costurile initiale pentru toate grupele care va da eficienta orarului
	cost_t_i_gr = 0;
	for(int i = 0; i < nr_grupe_l; i++)
	{
		cost_t_i_gr += cost_init_gr[i].cost_impar;
		cost_t_i_gr += cost_init_gr[i].cost_par;
	}
	cost_t_o_gr = cost_t_i_gr;

///copiat intervalele pentru orele de lucru ale grupelor
	for(int i = 0; i < nr_grupe_l; i++)
		for(int z = 0; z < 5; z++)
		{
			ora_l_start_gr[z][i].val_impar = ora_start_gr[z][i].val_impar;
			ora_l_start_gr[z][i].val_par = ora_start_gr[z][i].val_par;
			ora_l_finis_gr[z][i].val_impar = ora_finis_gr[z][i].val_impar;
			ora_l_finis_gr[z][i].val_par = ora_finis_gr[z][i].val_par;
		}
	//printf("costul total initial(grupe) este %d\n",cost_t_i_gr);


	//pregatirea orarelor de lucru pentru etapa a 2-a, finalele de la prima optimizare devin de lucru
	//eliberare_mem(ORARE_L);
	
	copy_orar(ORARE_L, ORARE_F);
	//copiez orarul salilor de laborator
	
	eliberare_mem(buffer_lab_l, Li_lab.nr_elem);
	buffer_lab_l = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_lab_l, buffer_lab_f, Li_lab.nr_elem);
	verifica_copiere_orar(buffer_lab_l, buffer_lab_f, Li_lab.nr_elem);

	//copiez orarul salilor de seminar
	
	eliberare_mem(buffer_seminar_l, Li_sem.nr_elem);
	buffer_seminar_l = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_seminar_l, buffer_seminar_f, Li_sem.nr_elem);
	verifica_copiere_orar(buffer_seminar_l, buffer_seminar_f, Li_sem.nr_elem);
	
	//copiez orarul salilor de proiect
	
	eliberare_mem(buffer_p_l, Li_p.nr_elem);
	buffer_p_l = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_p_l, buffer_p_f, Li_p.nr_elem);
	verifica_copiere_orar(buffer_p_l, buffer_p_f, Li_p.nr_elem);
	
	//copiez orarul salilor de curs
	
	eliberare_mem(buffer_curs_l, Li_curs.nr_elem);
	buffer_curs_l = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
	copy_orar_curs(buffer_curs_l, buffer_curs_f, Li_curs.nr_elem);
	verifica_copiere_orar(buffer_curs_l, buffer_curs_f, Li_curs.nr_elem);

	//copie pentru costuri de lucru
	cost_l_gr = (COST*)calloc( nr_grupe_l, sizeof(COST));
	for(int i = 0; i < nr_grupe_l; i++)
	{
		cost_l_gr[i] = cost_init_gr[i];
	}

	apeluri_optimizare_grupe_l = 0;
	for(int i = 0; i < 10; i++)
		optimizare_grupe_licenta(ORARE_L, i, 0);

	printf("costul total initial(grupe) este %d\n",cost_t_i_gr);
	printf("costul total optim(grupe) este %d\n", cost_t_o_gr);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "timp de lucru cu a 2a etapa %f secunde\n\n", duration );	


//#####rulam a 3-a optimizare grupe masterat

	printf("ETAPA DE OPTIMIZARE #3 _grupe masterat \n");
//intervalele de timp trebuie refacute
for(int z = 0; z < 5; z++)
	{	
		ora_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_l_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_l_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_f_start[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
		ora_f_finis[z] = (val_ore*)calloc( suma_nr_grupe_l + suma_nr_grupe_m, sizeof(val_ore));
	}

//costul initial pentru a 3-a optimizare, refacerea intervalurilor si costurilor
	contor = suma_nr_grupe_l;
	for(int i = 0; i < ANI_MASTER; i++)
		for(int j = 0; j < v_grupe[ANI_LIC + i].nr_grupe; j++)
		{
			cost_init_2[contor - suma_nr_grupe_l].cost_impar = 
				calc_cost_2(ORARE_F[ANI_LIC + i], contor, ora_start, ora_finis, 1);
			cost_init_2[contor - suma_nr_grupe_l].cost_par = 
				calc_cost_2(ORARE_F[ANI_LIC + i], contor, ora_start, ora_finis, 2);
			//printf("grupa %d ora start %d ora finis %d pe impara\n", 
			//	contor, ora_start[0][contor].val_impar, ora_finis[0][contor].val_impar);
			//printf("grupa %d ora start %d ora finis %d pe para\n", 
			//	contor, ora_start[0][contor].val_par, ora_finis[0][contor].val_par);
			//printf("orar %d grupa %d costul pe impara %d \n",
			//	ANI_LIC + i, j + 1, cost_init_2[contor - suma_nr_grupe_l].cost_impar);
			//printf("orar %d grupa %d costul pe para %d \n",
			//	ANI_LIC + i, j + 1, cost_init_2[contor - suma_nr_grupe_l].cost_par);
			contor++;	
		}

//copiat intervale cost de lucru
	for(int i = 0; i < suma_nr_grupe_m; i++)
	{
		cost_l_2[i].cost_impar = cost_init_2[i].cost_impar;
		cost_l_2[i].cost_par = cost_init_2[i].cost_par;
		//printf("cost_l_1[%d]=%d \n" , i, cost_l_1[i]);
	}
	//printf("cost %d\n", cost_l_2[123].cost_par);
	//exit(0);

//copiez orele de inceput si de final - cele de lucru
	for(int z = 0; z < 5; z++)
		for(int i = suma_nr_grupe_l; i < suma_nr_grupe_l + suma_nr_grupe_m; i++)
		{
			ora_l_start[z][i] = ora_start[z][i];
			ora_l_finis[z][i] = ora_finis[z][i];
		}


	//pregatirea orarelor de lucru pentru etapa a 3-a, finalele de la a2-a optimizare devin de lucru
	initializare_orar(ORARE_L);
	copy_orar(ORARE_L, ORARE_F);
	//copiez orarul salilor de laborator
	
	eliberare_mem(buffer_lab_l, Li_lab.nr_elem);
	buffer_lab_l = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_lab_l, buffer_lab_f, Li_lab.nr_elem);
	verifica_copiere_orar(buffer_lab_l, buffer_lab_f, Li_lab.nr_elem);

	//copiez orarul salilor de seminar
	
	eliberare_mem(buffer_seminar_l, Li_sem.nr_elem);
	buffer_seminar_l = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_seminar_l, buffer_seminar_f, Li_sem.nr_elem);
	verifica_copiere_orar(buffer_seminar_l, buffer_seminar_f, Li_sem.nr_elem);
	
	//copiez orarul salilor de proiect
	
	eliberare_mem(buffer_p_l, Li_p.nr_elem);
	buffer_p_l = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
	copy_orar_lab(buffer_p_l, buffer_p_f, Li_p.nr_elem);
	verifica_copiere_orar(buffer_p_l, buffer_p_f, Li_p.nr_elem);
	
	//copiez orarul salilor de curs
	
	eliberare_mem(buffer_curs_l, Li_curs.nr_elem);
	buffer_curs_l = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
	copy_orar_curs(buffer_curs_l, buffer_curs_f, Li_curs.nr_elem);
	verifica_copiere_orar(buffer_curs_l, buffer_curs_f, Li_curs.nr_elem);
	
	

	apeluri_optimizare_2 = 0;
	for(int i = 0; i < suma_nr_grupe_m ;i++)
		optimizare2(ORARE_L, suma_nr_grupe_l + i, 0);


	//afisare grupe master
	/*printf("%d\n", suma_nr_grupe_m);
	nod_grupa *ngr;
	contor = 0;
	for(int j = 14; j <=34; j++)
		for(int i = 0; i < v_grupe[j].nr_grupe; i++ )
		{
			ngr = generare_nume_grupa(j, i + 1, 0);
			printf("grupa %d orar %d %s\n", suma_nr_grupe_l + contor, j, ngr->nume);
			contor++;
		}
		*/
	printf("costul total initial(grupe master) este %d\n",cost_t_i_2);
	printf("costul total optim(grupe master) este %d\n", cost_t_o_2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "timp de lucru cu a 3-a etapa %f secunde\n\n", duration );	

//###################AFISARI
	//afisarile de proba pe datele de intrate
	/*afisare_orar(ORAREV, nume_orare, v_grupe, Li_titulari);
	afisare_orar(buffer_titulari, Li_titulari);
	afisare_orar(Li_curs, ORAREV, "curs");
	afisare_orar(Li_lab, ORAREV, "laborator");
	afisare_orar(Li_sem, ORAREV, "seminar");*/
//###afisare orare finale
	afisare_orar(ORARE_F, nume_orare, v_grupe, Li_titulari);
	afisare_orar(buffer_titulari, Li_titulari);
	afisare_orar(Li_curs, ORARE_F, "curs");
	afisare_orar(Li_lab, ORARE_F, "laborator");
	afisare_orar(Li_sem, ORARE_F, "seminar");
//***afisari solutii
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "timp total de lucru %f secunde\n", duration );	
	
	//test_extern();//pentru a rula cu debug
	
	//*******afisari de proba*******************************
	
	//afisare de proba a orarelor mari/sali/titulari
	//acum orarele sem/lab/proiecte cu grupe complet
	/*
	ocupare *p;
	nod_grupa *pg;
	for(int i = 0; i < NR_ORARE; i++)
		for(int zi = 0; zi < 5; zi++)
			for(int ora = 8; ora < 22; ora++)
				if(ORAREV[i][zi][ora].nume_materie != NULL)
				{
					if(ORAREV[i][zi][ora].tip != 0)
					{
						printf("orar%d  zi%d ora%d %s(%d) sala %s", i + 1, zi + 1,
							ora, ORAREV[i][zi][ora].nume_materie, ORAREV[i][zi][ora].tip,
							ORAREV[i][zi][ora].sala);
						pg = ORAREV[i][zi][ora].li_grupa.pfirst;
						while( pg != NULL)
						{
							printf(" %s ", pg->nume);
							pg = pg->pnext;
						}
						printf("\n");
					}
					p = ORAREV[i][zi][ora].pnext;
					while (p != NULL)
					{
						if(p->tip != 0)
						{
							printf("orar%d  zi%d ora%d %s(%d) sala %s",i + 1, zi + 1,
								ora, p->nume_materie, p->tip, p->sala);
							pg = p->li_grupa.pfirst;
							while( pg != NULL)
							{
								printf(" %s ", pg->nume);
								pg = pg->pnext;
							}
							printf("\n");
						}
						p = p->pnext;
					}
				}
	
	*/
	

	//afisez listele de sali
	/*
	printf("lista sali curs \n");
	nod_sali * p = Li_curs.pfirst;
	for(int j = 0; j < Li_curs.nr_elem; j++)
	{
		printf(" %d %s \n", p->index, p->nume);
		p = p->pnext;
	}
	printf("lista sali lab \n");
	p = Li_lab.pfirst;
	for(int j = 0; j < Li_lab.nr_elem; j++)
	{	
		printf("%d %s\n", p->index, p->nume);
		p = p->pnext;
	}
	printf("lista sali seminar \n");
	p = Li_sem.pfirst;
	for(int j = 0; j < Li_sem.nr_elem; j++)
	{   
		printf("%d %s\n", p->index, p->nume);
		p = p->pnext;
	}
	*/

	//afisare vectori de orare sali
	/*
	for(int j = 0; j < Li_curs.nr_elem; j++)
		for(int zi = 0; zi < 5; zi++)
			for(int ora = 8; ora < 22; ora++)
				if(buffer_curs[j][zi][ora].nume_materie != NULL)
					printf("sala%d zi%d ora%d %s \n", 
					j + 1, zi, ora, buffer_curs[j][zi][ora].nume_materie);
	*/
	
	//verificare afisare grupe
	/*
	nod_grupa *a = new nod_grupa;
	lista_grupe li;
	for(int j = 0; j < NR_ORARE; j++)
	{
		li = generare_nume_grupe(j);
		a = li.pfirst;
		while( a != NULL)
		{
			printf("%d %s\n", j, a->nume);
			a = a->pnext;
		}
	}
	*/

	//afisare orare sali lab cu serii + grupe
	/*
	printf("########afisare orare sali lab#######\n");
	
	for(int j = 0; j < Li_lab.nr_elem; j++)
		for(int zi = 0; zi < 5; zi++)
			for(int ora = 8; ora < 22; ora++)
			{
				oc_lab *p;
				if(buffer_lab[j][zi][ora].nume != NULL)
				{
					printf("sala %d zi %d ora %d  %s %s ", j, zi, ora,
						buffer_lab[j][zi][ora].nume, buffer_lab[j][zi][ora].serie);
					//afisare grupe
					nod_grupa *ng = new nod_grupa;
					ng = buffer_lab[j][zi][ora].li_grupa.pfirst;
					while( ng != NULL)
					{
						printf("%s ", ng->nume);
						ng = ng->pnext;
					}
					printf("\n");
					//pentru suprapunere
					if(buffer_lab[j][zi][ora].pnext != NULL)
					{
						p = buffer_lab[j][zi][ora].pnext;
						while( p != NULL)
						{
							printf("##sala %d zi %d ora %d  %s %s ", 
								j, zi, ora, p->nume, p->serie);
							nod_grupa *ng = new nod_grupa;
							ng = p->li_grupa.pfirst;
							while( ng != NULL)
							{
								printf("%s ", ng->nume);
								ng = ng->pnext;
							}
							printf("\n");

							p = p->pnext;
						}
					}//inchidere suprapunere
				}

			}//inchidere for
	*/

//test afisare numele fiecarei grupe cu generare_nume_grupa
	/*
	nod_grupa* a = new nod_grupa;
	for(int i = 0; i < NR_ORARE; i++)
		for(int j = 1; j <= v_grupe[i].nr_grupe; j++)
		{
			a = generare_nume_grupa(i, j, 0);
			puts(a->nume);
		}
	a = generare_nume_grupa(34, 1, 0);
	puts(a->nume);
	*/

	//for(int i = 0; i < NR_ORARE; i++)
	//	printf("%d %s\n", i, nume_orare[i]);

//#########################################################
//####afisare lista titulari cu indice + verifica repetari#
//#########################################################	
	/*
	nod_titulari *pnod_t;
	pnod_t = Li_titulari.pfirst;
	for(int i = 0; i < Li_titulari.nr; i++)
	{
		printf("%d == %d == %s\n", i + 1, pnod_t->index, pnod_t->nume);
		pnod_t = pnod_t->pnext;
	}
	if(pnod_t != NULL)
	{
		printf("##### a ramas %d = %s\n", pnod_t->index, pnod_t->nume);
		pnod_t = pnod_t->pnext;
	}
	//verific repetari
	nod_titulari *pnod_t1;
	nod_titulari *pnod_t2;
	pnod_t1 = Li_titulari.pfirst;
	while(pnod_t1 != NULL)
	{
		pnod_t2 = pnod_t1->pnext;
		while(pnod_t2 != NULL)
		{
			if( strcmp(pnod_t1->nume, pnod_t2->nume) == 0 )
				printf("suprapunere %d cu %d %s\n", pnod_t1->index, pnod_t2->index, pnod_t2->nume);
			pnod_t2 = pnod_t2->pnext;
		}
		pnod_t1 = pnod_t1->pnext;
	}		
	*/

}//inchidere main