#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "header1.h"

extern lista_sali Li_curs;


//functie de adaugare a cursului in orarul mare
void addcurs(ORAR ORAREV[], int i, int zi, int ora, nod_disciplina* pdisc, char* nume_sala,
			 int nr_grupe, int nr_optionale, int paritate, lista_grupe li_gr)
{
	ocupare *a = new ocupare;//pentru adaugare daca sunt mai multe sem si cursuri la aceeasi ora
	for(int j = 0; j < pdisc->durata[0] ;j++)
	{
		if( ORAREV[i][zi][ora + j].nume_materie == NULL)
		{
			ORAREV[i][zi][ora + j].index_titular = pdisc->index_titular;
			ORAREV[i][zi][ora + j].nume_materie = (char *)calloc( 80, sizeof( char ) );
			strcpy( ORAREV[i][zi][ora + j].nume_materie, pdisc->nume);
			ORAREV[i][zi][ora + j].paritate = paritate;
			ORAREV[i][zi][ora + j].sala = (char *)calloc( 10, sizeof( char ) );
			strcpy (ORAREV[i][zi][ora + j].sala, nume_sala);
			ORAREV[i][zi][ora + j].tip = 0;
			ORAREV[i][zi][ora + j].afisat = 0;
			ORAREV[i][zi][ora + j].pnext = NULL;
			//adaugare lista grupe, la cursuri bag toate momentan
			ORAREV[i][zi][ora + j].li_grupa = li_gr;
			//afisare liste de grupe pe cursuri
			/*
			nod_grupa *b = new nod_grupa;
			b = ORAREV[i][zi][ora + j].li_grupa.pfirst;
			while( b != NULL)
			{
				printf("%s %s\n",ORAREV[i][zi][ora + j].nume_materie, b->nume);
				b = b->pnext;
			}
			*/
		}
		else
		{
			ocupare *p = new ocupare;
			p->index_titular = pdisc->index_titular;
			p->nume_materie  = (char *)calloc( 80, sizeof( char ) );
			strcpy( p->nume_materie, pdisc->nume);
			p->paritate = paritate;
			p->sala = (char *)calloc( 10, sizeof( char ) );
			strcpy(p->sala, nume_sala);
			p->tip = 0;
			p->afisat = 0;
			p->pnext = NULL;
			p->li_grupa = li_gr;
			//afisare grupe pentru curs
			/*
			nod_grupa *b = new nod_grupa;
			b = p->li_grupa.pfirst;
			while( b != NULL)
			{
				printf("%s %s\n", p->nume_materie, b->nume);
				b = b->pnext;
			}
			*/
			if( ORAREV[i][zi][ora + j].pnext == NULL) 
				ORAREV[i][zi][ora + j].pnext = p;
			else
			{
			a = ORAREV[i][zi][ora + j].pnext;
			while( a->pnext != NULL )
				a = a->pnext;
			a->pnext = p;
			}
		}//inchidere else	
	}//inchidere for 
}

//functie de adaugare a cursului in orarul titularului
void addcurs(ORAR_PF buffer[], int indice_t, int zi, int ora, char* nume_materie, 
			 int index_sala, int durata, int paritate, char *serie)
{	
	int j;
	for(j = 0; j < durata; j++)
	{
		if(buffer[indice_t][zi][ora + j].nume_materie == NULL)
		{
			buffer[indice_t][zi][ora + j].index_sala = index_sala;
			buffer[indice_t][zi][ora + j].nume_materie = (char *)calloc( 80, sizeof( char ) );
			strcpy(buffer[indice_t][zi][ora + j].nume_materie, nume_materie);
			buffer[indice_t][zi][ora + j].serie = (char *)calloc( 15, sizeof( char ) );
			strcpy(buffer[indice_t][zi][ora + j].serie, serie);
			buffer[indice_t][zi][ora + j].paritate = paritate;
			
		}
		else//suprapunere
		{	
			//daca e aceeasi materie la mai multe serii pe aceeasi paritate
			if( ( strcmp (buffer[indice_t][zi][ora + j].nume_materie, nume_materie ) == 0 )
				&& buffer[indice_t][zi][ora + j].paritate == paritate)
			{	
				strcat(buffer[indice_t][zi][ora + j].serie, " si ");
				strcat(buffer[indice_t][zi][ora + j].serie, serie);
				continue;
			} 
			
			oc_profesor *a = new oc_profesor;
			a->index_sala = index_sala;
			a->nume_materie =  (char *)calloc( 40, sizeof( char ) );
			strcpy(a->nume_materie, nume_materie);
			a->pnext = NULL;
			a->paritate = paritate;
			
			//avem si al 2-lea curs pe alta paritate 
			if(buffer[indice_t][zi][ora + j].pnext != NULL)
			{
				strcat( buffer[indice_t][zi][ora + j].pnext->serie, " si ");
				strcat( buffer[indice_t][zi][ora + j].pnext->serie, serie);
			}
			else
			{
				//adaugare serie
				a->serie = (char *)calloc( 15, sizeof( char ) );
				strcpy(a->serie, serie);
				buffer[indice_t][zi][ora + j].pnext = a;
			}
		}

	}//inchidere for
}//inchidere fctie

//functie de adaugare curs in orarul salii
void addcurs(ORAR_CURS buffer[], int indice_sala, int zi, int ora, 
			 nod_disciplina *pdisc, int paritate, char* serie)
{
	int j;
	for(j = 0; j < pdisc->durata[0]; j++)
	{	
		if(buffer[indice_sala][zi][ora + j].nume_materie == NULL)
		{
			buffer[indice_sala][zi][ora + j].nume_materie = (char *)calloc( 80, sizeof( char ) );
			strcpy(buffer[indice_sala][zi][ora + j].nume_materie, pdisc->nume);
			buffer[indice_sala][zi][ora + j].acronim = (char *)calloc( 10, sizeof( char ) );
			strcpy(buffer[indice_sala][zi][ora + j].acronim, pdisc->acronim);
			//printf("adaugat %s (%s) ziua %din sala %d\n",
			//pdisc->nume, pdisc->acronim, zi, indice_sala);
			buffer[indice_sala][zi][ora + j].paritate = paritate;
			buffer[indice_sala][zi][ora + j].serie = (char *)calloc( 15, sizeof( char ) );
			strcpy(buffer[indice_sala][zi][ora + j].serie, serie);
		}
		else //suprapunere
		{
			if( ( strcmp (buffer[indice_sala][zi][ora + j].nume_materie, pdisc->nume) == 0 )
				&& buffer[indice_sala][zi][ora + j].paritate == paritate)
				//suprapunere, trebuie sa fie mai multe serii deodata
			{	
				strcat(buffer[indice_sala][zi][ora + j].serie, " #si# ");
				strcat(buffer[indice_sala][zi][ora + j].serie, serie);
				continue;
			}
			//printf("suprapunere %d %d %s cu %s [%d/%d]in sala =%d=\n", zi, ora + j, 
			//	buffer[indice_sala][zi][ora + j].nume_materie, pdisc->nume, 
			//	buffer[indice_sala][zi][ora + j].paritate, paritate, indice_sala);
			oc_curs *a = new oc_curs;
			a->acronim = (char *)calloc( 10, sizeof( char ) );
			strcpy(a->acronim, pdisc->acronim);
			a->nume_materie = (char *)calloc( 40, sizeof( char ) );
			strcpy(a->nume_materie, pdisc->nume);
			a->paritate = paritate;
			a->pnext = NULL;
			
			//avem si al 2-lea curs pe alta paritate 
			if(buffer[indice_sala][zi][ora + j].pnext != NULL)
			{
				strcat( buffer[indice_sala][zi][ora + j].pnext->serie, " si ");
				strcat( buffer[indice_sala][zi][ora + j].pnext->serie, serie);
			}
			else
			{
				//adaugat serie
				a->serie = (char *)calloc( 15, sizeof( char ) );
				strcpy(a->serie, serie);
				buffer[indice_sala][zi][ora + j].pnext = a;
			}
		}
	
	}//inchidere for
}

//adaugare in ORARUL mare a lab/seminarii/proiecte
void addsem(ORAR ORAREV[], int i ,int zi, int ora, int tip, 
			char* acronim, int durata, int paritate, char *nume_sala, nod_grupa* g)
{
	ocupare *a = new ocupare;
	for(int j = 0; j < durata; j++)
	{
		if( ORAREV[i][zi][ora + j].nume_materie == NULL) //nu e nimic acolo
		{
			ORAREV[i][zi][ora + j].nume_materie = (char *)calloc( 12, sizeof( char ) );
			strcpy( ORAREV[i][zi][ora + j].nume_materie, acronim);
			ORAREV[i][zi][ora + j].pnext = NULL;
			ORAREV[i][zi][ora + j].paritate = paritate;
			ORAREV[i][zi][ora + j].sala = (char *)calloc( 10, sizeof( char ) );
			strcpy( ORAREV[i][zi][ora + j].sala, nume_sala);
			ORAREV[i][zi][ora + j].tip = tip;
			addlist(ORAREV[i][zi][ora + j].li_grupa, g->nume);
			//strcmp
			//adaugare grupe
		}
		else //se suprapune cu ceva
		{
			ocupare *p = new ocupare;
			p->nume_materie = (char *)calloc( 12, sizeof( char ) );
			strcpy( p->nume_materie, acronim );
			p->sala = (char *)calloc( 10, sizeof( char ) );
			strcpy( p->sala, nume_sala );
			p->paritate = paritate;
			p->tip = tip;
			p->pnext = NULL;
			init_lista_grupe(p->li_grupa);
			addlist( p->li_grupa, g->nume);
			if( ORAREV[i][zi][ora + j].pnext == NULL )
				ORAREV[i][zi][ora + j].pnext = p;
			else
			{
				a = ORAREV[i][zi][ora + j].pnext;
				while( a->pnext != NULL )
					a = a->pnext;
				a->pnext = p;
			}
		}//inchidere else
	}//inchidere for
}

//adaugare in orarul de sali de laborator/seminar
void addsem(ORAR_LAB buffer[], int indice_sala, int zi, int ora,
			nod_disciplina * pdisc, int tip, char* serie, nod_grupa* g)
{
	int j;
	for(int j = 0; j < pdisc->durata[tip]; j++)
	{
		if(buffer[indice_sala][zi][ora + j].nume == NULL) 
		{
			buffer[indice_sala][zi][ora + j].nume = (char *)calloc( 21, sizeof( char ) );
			strcpy(buffer[indice_sala][zi][ora + j].nume, pdisc->acronim );
			//strcat(buffer[indice_sala][zi][ora + j].nume, " ");
			//buffer[indice_sala][zi][ora + j].serie = (char *)calloc( 10, sizeof(char));
			strcpy(buffer[indice_sala][zi][ora + j].serie, serie);
			buffer[indice_sala][zi][ora + j].paritate = pdisc->paritate[tip];
			buffer[indice_sala][zi][ora + j].pnext = NULL;
			addlist(buffer[indice_sala][zi][ora + j].li_grupa, g->nume);
			//printf("1#sala %d zi%d ora%d %s \n", indice_sala, zi, ora + j, buffer[indice_sala][zi][ora + j].nume);
			//adaugare grupe
			
		}
		else //suprapunere
		{
			oc_lab *p = new oc_lab;
			p->nume = (char *)calloc( 21, sizeof( char ) );
			strcpy(p->nume, pdisc->acronim);
			//strcat(p->nume, " ");
			//p->nume = (char *)calloc( 10, sizeof( char ) );
			strcpy(p->serie, serie);
			p->paritate = pdisc->paritate[tip];
			p->pnext = NULL;
			init_lista_grupe(p->li_grupa);
			addlist( p->li_grupa, g->nume);
			if (buffer[indice_sala][zi][ora + j].pnext == NULL)
			{
				buffer[indice_sala][zi][ora + j].pnext = p;
				//printf("2#sala %d zi%d ora%d %s \n", indice_sala, zi, ora + j, 
				//	buffer[indice_sala][zi][ora + j].pnext->nume);
			}
			else
			{
				oc_lab *a = new oc_lab;
				a = buffer[indice_sala][zi][ora + j].pnext;
				while( a->pnext != NULL )
					a = a->pnext;
				a->pnext = p;
				//printf("3#sala %d zi%d ora%d %s \n", indice_sala, zi, ora + j, 
				//	a->pnext->nume);
			}
			//adaugare grupe 
			//printf("suprapunere sala%d zi%d ora%d la %s cu %s\n", 
			//	indice_sala, zi, ora, buffer[indice_sala][zi][ora + j].nume, pdisc->acronim);
			
			
			//verificare erori dupa ce adaug grupe

		}//inchidere else suprapunere
	}//inchidere for
}

//afisare orar mare

void afisare_orar(ORAR VO[], char* nume_orare[], numar_grupe v_grupe[], lista_titulari Li_titulari)
{
 //generare nume fisiere
	FILE *fcurent;
	char *f_disc;
	char *serie;
	lista_grupe li_afisare;
	char* saptamana[5];
	int nr_grupe_ramase;
	saptamana[0] = "LUNI";
	saptamana[1] = "MARTI";
	saptamana[2] = "MIERCURI";
	saptamana[3] = "JOI";
	saptamana[4] = "VINERI";
	int zi, ora;
	int suprapunere = 0, afisat_zi;
	int gasire;
	int afisat_curs = 0;
	int fara_v = 0;
	int afis_semi, afis_gr;
	for(int i = 0; i < NR_ORARE; i++)
	{
		f_disc =  (char *)calloc( 40, sizeof( char ) );
		serie = (char*)calloc(15, sizeof( char ));
		strcpy(f_disc, "CSV_out/");
		strcat(f_disc, nume_orare[i]);
		//deschid fisierul CSV de orar
		fcurent = fopen(f_disc, "w");
		zi = 0;
		ora = 0;
		printf("deschid %s \n", f_disc);
		//numele Orar_Serie
		int c_serie = 0;
		int c_ss = 0;
		while( !isdigit(nume_orare[i][c_ss]) )
			c_ss++;
		while (nume_orare[i][c_ss] != '.')
			serie[c_serie++] = nume_orare[i][c_ss++];
		fprintf(fcurent, "\t ###ORAR %s ####\n", serie);
		fprintf(fcurent, "\t Semestrul I    Anul universitar 2014 – 2015\n");
		fprintf(fcurent, "Facultatea: AUTOMATICA SI CALCULATOARE\n");
		fprintf(fcurent, "Specializarea: CALCULATOARE\n");
		fprintf(fcurent, "ZIUA,ORA");
		//randul cap cu grupele
		li_afisare = generare_nume_grupe(i);
		nod_grupa *ngr = li_afisare.pfirst;
		while( ngr != NULL)
		{
			if (i < 14)
				fprintf(fcurent, ",%s,", ngr->nume);
			else
				fprintf(fcurent, ",%s", ngr->nume);
			ngr = ngr->pnext;
		}
		fprintf(fcurent, "\n");
		//randul cap cu disciplina si sala
		fprintf(fcurent, ",");
		for(int j = 0; j < v_grupe[i].nr_grupe; j++)
		{
			if( i < 14)
				fprintf(fcurent, ",Disciplina si sala,");
			else
				fprintf(fcurent, ",Disciplina si sala");
		}
		fprintf(fcurent, "\n");
		//incep randurile normale
		ocupare *poc = new ocupare;
		ocupare *aux = new ocupare;
//#######// studii licenta, merg pe semigrupe
		if (i < 14)
		{
			afisat_zi = 0;
			for(int z = 0; z < 5; z++)
			{
				fprintf(fcurent, "%s,", saptamana[z]);
				afisat_zi = 1;
				for(int o = 8; o < 22; o++)
				{
					//ora
					if (afisat_zi == 0)
						fprintf(fcurent, ",");//virgula dinainte de ore
					if (o < 10)
						fprintf(fcurent, "0%d", o);
					else
						fprintf(fcurent, "%d", o);
					fprintf(fcurent, "-");
					if ( o + 1 < 10)
						fprintf(fcurent, "0%d", o + 1);
					else
						fprintf(fcurent, "%d", o + 1);
					fprintf(fcurent, ",");
					//incep materiile
					if(VO[i][z][o].nume_materie != NULL)
					{
						
						//*poc = VO[i][z][o]; 
						poc = &VO[i][z][o];
						
print_curs:				if (poc->tip == 0) //este curs
						{
							if (suprapunere == 1)
								fprintf(fcurent, "#");
							if (suprapunere == 1)
								fara_v = 1;
							nod_titulari *pnt = getat(Li_titulari, poc->index_titular);
							fprintf(fcurent, "\"%s (c), %s (%s)",
								poc->nume_materie, pnt->nume, poc->sala );
							if(poc->paritate != 0)
								fprintf(fcurent, "[%d]\"", poc->paritate);
							else 
								fprintf(fcurent, "\"");
							suprapunere = 0;
							
							if( poc->pnext != NULL)
							{
								if(poc->pnext->tip == 0 )
								{
									poc = poc->pnext;//vad suprapunere cu alt curs, pe alta paritate
									suprapunere = 1;
									goto print_curs;
								}
								//vad daca mai sunt lab la grupe
								else
								{
									poc = poc->pnext;
									goto print_restul;
								}
							}
						
						}
						 
							//caut prima semigrupa, apoi restul
print_restul:			if(poc->tip != 0)
						{
							
							for(int j = 0; j < 2 * v_grupe[i].nr_grupe; j++)
							{
								gasire = 0;
								aux = poc;
								suprapunere = 0;
								//printf("orar %d grupa %d\n", i ,j);
								while( aux != NULL)
								{
									//am un curs bagat untre grupe
									if (aux->tip == 0)
									{	
										if(afisat_curs == 1)
										{
											aux = aux->pnext;
											continue;
										}
										afisat_curs = 1;
										fara_v = 1;
										fprintf(fcurent, ",");
										nod_titulari *pnt = getat(Li_titulari, aux->index_titular);
										fprintf(fcurent, "\"%s (c), %s (%s)",
											aux->nume_materie, pnt->nume, aux->sala );
										if(poc->paritate != 0)
											fprintf(fcurent, "[%d]\"", aux->paritate);
										else 
											fprintf(fcurent, "\"");
										
									}
									else
										if( (afis_semi = exista_grupa(*aux, j, i) )!= 0)
										{
											if (suprapunere == 1)
												fprintf(fcurent, "#");
											suprapunere = 1;//am deja o grupa acolo
											fprintf(fcurent,"%s(%d) - %s",
												aux->nume_materie, aux->tip, aux->sala);
											//printf("%s(%d) - %s",
											//	aux->nume_materie, aux->tip, aux->sala);
											if(aux->paritate !=0)
											{
												fprintf(fcurent, "[");
												fprintf(fcurent, "%d", aux->paritate);
												fprintf(fcurent, "]");
											}
											if (afis_semi != 3)
											{
												fprintf(fcurent, "{");
												fprintf(fcurent, "%d", afis_semi);
												fprintf(fcurent, "}");
											}
											gasire = 1;
											//fprintf(fcurent, ",");
										}//if exista_grupa
									aux = aux->pnext;
								}//while
								if(fara_v != 1)
									fprintf(fcurent, ",");
								fara_v = 0;
								suprapunere = 0;
							}//for semigrupe
						}//inchidere if seminar
						
					}//if inchidere casuta ocupare
					fprintf(fcurent, "\n");
					fara_v = 0;
					afisat_curs = 0;
					afisat_zi = 0;
					//suprapunere = 0;
				}//inchidere for ore
			}//inchidere for zile
		fprintf(fcurent, "E"); //printam caraterul de iesire pentru a putea fi reutilizate fisierele
		}//incheiere un an de licenta
///###studii ani de master, merg pe grupe		
		if( (i >= 14 ) && ( i < NR_ORARE) )
		{
			afisat_zi = 0;
			for(int z = 0; z < 5; z++)
			{
				fprintf(fcurent, "%s,", saptamana[z]);
				afisat_zi = 1;
				for(int o = 8; o < 22; o++)
				{
					//ora
					if (afisat_zi == 0)
						fprintf(fcurent, ",");//virgula dinainte de ore
					if (o < 10)
						fprintf(fcurent, "0%d", o);
					else
						fprintf(fcurent, "%d", o);
					fprintf(fcurent, "-");
					if ( o + 1 < 10)
						fprintf(fcurent, "0%d", o + 1);
					else
						fprintf(fcurent, "%d", o + 1);
					fprintf(fcurent, ",");
					//incep materiile
					if(VO[i][z][o].nume_materie != NULL)
					{
						poc = &VO[i][z][o];
print_curs_m:			if (poc->tip == 0) //este curs
						{
						//daca apare suprapunere
							if (suprapunere == 1)
								fprintf(fcurent, "#");
							if (suprapunere == 1)
								fara_v = 1;
							nod_titulari *pnt = getat(Li_titulari, poc->index_titular);
							fprintf(fcurent, "\"%s (c), %s (%s)",
								poc->nume_materie, pnt->nume, poc->sala );
							if(poc->paritate != 0)
								fprintf(fcurent, "[%d]\"", poc->paritate);
							else 
								fprintf(fcurent, "\"");
							suprapunere = 0;

							if( poc->pnext != NULL)
							{
								if(poc->pnext->tip == 0 )
								{
									poc = poc->pnext;//vad suprapunere cu alt curs, pe alta paritate
									suprapunere = 1;
									goto print_curs_m;
								}
								//vad daca mai sunt lab la grupe
								else
								{
									poc = poc->pnext;
									goto print_restul_m;
								}
							}


						}//if curs
print_restul_m:			if(poc->tip != 0)
						{
							
							for(int j = 0; j < v_grupe[i].nr_grupe; j++)
							{
								gasire = 0;
								aux = poc;
								suprapunere = 0;
								//printf("orar %d grupa %d\n", i ,j);
								while( aux != NULL)
								{
									//am un curs bagat untre grupe
									if (aux->tip == 0)
									{	
										if(afisat_curs == 1)
										{
											aux = aux->pnext;
											continue;
										}
										afisat_curs = 1;
										fara_v = 1;
										fprintf(fcurent, ",");
										nod_titulari *pnt = getat(Li_titulari, aux->index_titular);
										fprintf(fcurent, "\"%s (c), %s (%s)",
											aux->nume_materie, pnt->nume, aux->sala );
										if(poc->paritate != 0)
											fprintf(fcurent, "[%d]\"", aux->paritate);
										else 
											fprintf(fcurent, "\"");
										
									}
									else
										if( (afis_gr = exista_grupa(*aux, j , i) )!= 0)
										{
											if (suprapunere == 1)
												fprintf(fcurent, "#");
											suprapunere = 1;//am deja o grupa acolo
											fprintf(fcurent,"%s(%d) - %s",
												aux->nume_materie, aux->tip, aux->sala);
											//printf("%s(%d) - %s",
											//	aux->nume_materie, aux->tip, aux->sala);
											if(aux->paritate !=0)
											{
												fprintf(fcurent, "[");
												fprintf(fcurent, "%d", aux->paritate);
												fprintf(fcurent, "]");
											}
											if (afis_gr != 3)
											{
												fprintf(fcurent, "{");
												fprintf(fcurent, "%d", afis_semi);
												fprintf(fcurent, "}");
											}
											gasire = 1;
											//fprintf(fcurent, ",");
										}//if exista_grupa
									aux = aux->pnext;
								}//while
								if(fara_v != 1)
									fprintf(fcurent, ",");
								fara_v = 0;
								suprapunere = 0;
							}//for grupe
						}//inchidere if seminar

					}//if casuta ocupata
					fprintf(fcurent, "\n");
					fara_v = 0;
					afisat_curs = 0;
					afisat_zi = 0;
				}//for ore

			}//for zile
		fprintf(fcurent, "E"); //printam caraterul de iesire pentru a putea fi reutilizate fisierele
		}//inchidere an master
		free(f_disc);
		//free(serie);
	}//inchidere for ORARE 1 -> NR_ORARE
}

//afisare orar titulari
void afisare_orar(ORAR_PF buffer_titulari[], lista_titulari Li_titulari)
{
	//generare nume fisiere
	FILE *fcurent;
	char *f_disc;
	char* saptamana[5];
	saptamana[0] = "LUNI";
	saptamana[1] = "MARTI";
	saptamana[2] = "MIERCURI";
	saptamana[3] = "JOI";
	saptamana[4] = "VINERI";
	int suprapunere = 0;
	oc_profesor *poc_pf = new oc_profesor;
	nod_titulari *pnt= new nod_titulari; 
	//for mare, pentru fiecare titular
	for(int i = 1; i <= Li_titulari.nr; i++)
	{
		f_disc =  (char *)calloc( 40, sizeof( char ) );
		strcpy(f_disc, "T_PF_out/");
		pnt = getat(Li_titulari, i);
		strcat(f_disc, pnt->nume);
		strcat(f_disc, ".csv");
		//deschid fisierul orar
		fcurent = fopen(f_disc, "w");
		fprintf(fcurent, "\t ###ORAR %s ####\n", pnt->nume);
		fprintf(fcurent, "\t Semestrul I    Anul universitar 2014 – 2015\n");
		fprintf(fcurent, "ZIUA,ORA, Materia si sala\n");
		//incep parcurgerea orarului
		int afisat_zi = 0;
		for(int z = 0; z < 5; z++)
		{
			fprintf(fcurent, "%s,", saptamana[z]);
			afisat_zi = 1;
			for(int o = 8; o < 22; o++)
			{
				//ora
				if (afisat_zi == 0)
					fprintf(fcurent, ",");//virgula dinainte de ore
				if (o < 10)
					fprintf(fcurent, "0%d", o);
				else
					fprintf(fcurent, "%d", o);
				fprintf(fcurent, "-");
				if ( o + 1 < 10)
					fprintf(fcurent, "0%d", o + 1);
				else
					fprintf(fcurent, "%d", o + 1);
				fprintf(fcurent, ",");

				//incep materiile
				if(buffer_titulari[i - 1][z][o].nume_materie != NULL)
				{
					poc_pf = &buffer_titulari[i - 1][z][o];
					//if (suprapunere == 1)
					//S	fprintf(fcurent, "#");
afisare:			fprintf(fcurent, "\"%s  %s ",
						poc_pf->nume_materie, poc_pf->serie);
					nod_sali *p;
					p = Li_curs.pfirst;
					int nr = poc_pf->index_sala - 1;
					while( nr > 0)
					{
						p = p->pnext;
						nr--;
					}
					fprintf(fcurent, "%s ", p->nume);
					if( poc_pf->paritate != 0 )
						fprintf(fcurent, "[%d]", poc_pf->paritate );
					fprintf(fcurent, " \"");
					//poate mai e vreun curs pe alta paritate
					fprintf(fcurent, ",");
					while(poc_pf->pnext != NULL)
					{
						poc_pf = poc_pf->pnext;
						goto afisare;
					}

				}
				fprintf(fcurent, "\n");
				afisat_zi = 0;
			}//for ora
		}//for zile
		fprintf(fcurent, "E");
	}//for mare
	free(f_disc);


}

//afisare orar sali_curs/sali_lab/sali_sem
void afisare_orar(lista_sali Li_sali, ORAR VO[], char *tip)
{
	FILE *fcurent;
	char* saptamana[5];
	saptamana[0] = "LUNI";
	saptamana[1] = "MARTI";
	saptamana[2] = "MIERCURI";
	saptamana[3] = "JOI";
	saptamana[4] = "VINERI";
	int suprapunere = 0;
	nod_sali* pns = new nod_sali;
	ocupare *aux = new ocupare; 
	char *f_disc = (char *)calloc( 70, sizeof( char ) );
	strcpy(f_disc, "Program de folosire a salilor de " );
	strcat(f_disc, tip);
	strcat(f_disc, " IS_CTI_2014_2015_sem1.csv");
	//printf("%s\n", f_disc);
	fcurent = fopen( f_disc, "w"); 
	 

	char *titlu = (char *)calloc( 60, sizeof( char ) );
	strcpy(titlu, "\t ###ORAR de folosire a salilor de ");
	strcat(titlu, tip);
	strcat(titlu, " ####");
	fprintf(fcurent, "%s\n", titlu);
	fprintf(fcurent, "\t Semestrul I    Anul universitar 2014 – 2015\n");
	//scriu antetul cu zilele saptamanii
	fprintf(fcurent, ","); //libera pentu sala
	for(int i = 0; i < 5; i++)
		fprintf(fcurent, "%s,,,,,,,,,,,,,,", saptamana[i]);
	fprintf(fcurent, "\n");
	fprintf(fcurent, "SALA,");
	for(int j = 1; j <= 5; j++)
		for(int i = 1; i <= 14; i++)
		{
			int o1, o2;
			o1 = i + 7;
			o2 = o1 + 1;
			if (o1 < 10)
				fprintf(fcurent, "0%d-", o1);
			else
				fprintf(fcurent, "%d-", o1);
			if (o2 < 10)
				fprintf(fcurent, "0%d,", o2);
			else
				fprintf(fcurent, "%d,", o2);
		}
	fprintf(fcurent, "\n");
	//bucla mare, pentru fiecare sala
	pns = Li_sali.pfirst;
	while (pns != NULL)
	{
		fprintf(fcurent, "%s,", pns->nume);
		//for pentru orare
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				for(int i = 0; i < NR_ORARE; i++)
				{
					if(VO[i][z][o].nume_materie != NULL)
					{
						aux = &VO[i][z][o];
						while(aux != NULL)
						{
							if(strcmp(aux->sala, pns->nume) == 0)
							{
								fprintf(fcurent, "\" %s Seria%d", aux->nume_materie, i );
								if(aux->tip != 0)
								{
									if(aux->tip == 1)
										fprintf(fcurent, "(lab)");
									if(aux->tip == 2)
										fprintf(fcurent, "(sem)");
									if(aux->tip == 3)
										fprintf(fcurent, "(p)");

								}
								if (aux->paritate != 0)
									fprintf(fcurent, "[%d]\"", aux->paritate);
								else
									fprintf(fcurent, "\"");
							}
							aux = aux->pnext;//daca avem 1 seminar si apoi curs, a in cazul Introducere in Informatica
						}
					}//for casuta plina
				}//for orare
				fprintf(fcurent, ",");
			}//for ore
		fprintf(fcurent, "\n");
		pns = pns->pnext;
	}//inchidere while
	//free(titlu);
	//free(f_disc);
}

//pun o casuta in locul alteia ocupate
void suprascrie(ocupare &o_dest, ocupare *oc)
{
	o_dest.nume_materie = (char*)calloc( 12, sizeof(char));
	strcpy(o_dest.nume_materie, oc->nume_materie);
	o_dest.paritate = oc->paritate;
	o_dest.tip = oc->tip;
	o_dest.sala = (char*)calloc( 10, sizeof(char));
	strcpy(o_dest.sala, oc->sala);
	o_dest.index_titular = oc->index_titular;
	//copiez lista de grupe
	init_lista_grupe(o_dest.li_grupa);
	nod_grupa *ngr = oc->li_grupa.pfirst;
	while(ngr != NULL)
	{
		addlist(o_dest.li_grupa, ngr->nume );
		ngr = ngr->pnext;
	}
	o_dest.pnext = oc->pnext;
	free(oc->nume_materie);
	free(oc->sala);

}

//adaug o ocupare la o casuta de ocupare
void copy(ocupare &o_dest, ocupare *oc)
{
	if (o_dest.nume_materie == NULL)//am o ora complet neocupata
	{
		o_dest.index_titular = oc->index_titular;
		//copiez lista de grupa
		init_lista_grupe(o_dest.li_grupa);
		nod_grupa *ngr = oc->li_grupa.pfirst;
		while(ngr != NULL)
		{
			addlist(o_dest.li_grupa, ngr->nume );
			ngr = ngr->pnext;
		}
		o_dest.nume_materie = (char*)calloc( 12, sizeof(char));
		strcpy(o_dest.nume_materie, oc->nume_materie);
		o_dest.paritate = oc->paritate;
		o_dest.tip = oc->tip;
		o_dest.sala = (char*)calloc( 10, sizeof(char));
		strcpy(o_dest.sala, oc->sala);
		o_dest.pnext = NULL;
	}
	else//am deja o materie
	{
		ocupare *aux, *aux2;
		aux = new ocupare;
		aux2 = &o_dest;
		while(aux2->pnext != NULL)
		{
			aux2 = aux2->pnext;
		}//gasesc ultimul element din lista oc
		aux->index_titular = oc->index_titular;
		//copiez lista de grupa
		init_lista_grupe(aux->li_grupa);
		nod_grupa *ngr = oc->li_grupa.pfirst;
		while(ngr != NULL)
		{
			addlist(aux->li_grupa, ngr->nume );
			ngr = ngr->pnext;
		}
		aux->nume_materie = (char*)calloc( 12, sizeof(char));
		strcpy(aux->nume_materie, oc->nume_materie);
		aux->paritate = oc->paritate;
		aux->tip = oc->tip;
		aux->sala = (char*)calloc( 10, sizeof(char));
		strcpy(aux->sala, oc->sala);
		aux2->pnext = aux;
		aux->pnext = NULL;
	}

}

void copy_orar_lab(ORAR_LAB *buffer_dest, ORAR_LAB *buffer_s, int nr_elem)
{
	//printf("intra in fctie copy_orar_lab\n");
	oc_lab *nod1, *nod2;
	oc_lab *aux2;
	oc_lab* nod_lucru;
	int contor;
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				contor = 0;
				nod1 = &buffer_s[i][z][o];
			//copiez fiecare oc_lab
				if(buffer_s[i][z][o].nume == NULL)
				{
					buffer_dest[i][z][o] = buffer_s[i][z][o];
					//printf("continue%d %d %d\n", i, z, o);
					continue;
				}
				//pentru prima oc la o pozitie
				if(buffer_dest[i][z][o].nume == NULL)
				{
					nod_grupa *aux;
					aux = nod1->li_grupa.pfirst;
					while(aux != NULL)
					{
						addlist(buffer_dest[i][z][o].li_grupa, aux->nume);
						aux = aux->pnext;
					}
					buffer_dest[i][z][o].nume = (char*)calloc( 21, sizeof(char));
					strcpy(buffer_dest[i][z][o].nume, nod1->nume);
					buffer_dest[i][z][o].paritate = nod1->paritate;
					strcpy( buffer_dest[i][z][o].serie, nod1->serie);
					buffer_dest[i][z][o].pnext = NULL;
					//printf("copiere un nod prima poz la %d %d %d\n", i, z, o);
					nod1 = nod1->pnext;//am copiat nodul unu
				}
				/*
				else
				{
					nod2 = &buffer_dest[i][z][o];
					while(nod2->pnext != NULL)
					nod2 = nod2->pnext;
				}*/
				
				while( nod1 != NULL)
				{
					//if(contor !=0)
					//	aux2->pnext = nod_lucru;
					//nod2 va arata la ultimul element din lista
					nod2 = &buffer_dest[i][z][o];
					while(nod2->pnext != NULL)
						nod2 = nod2->pnext;
					nod_lucru = new oc_lab;
					nod_grupa *aux;
					aux = nod1->li_grupa.pfirst;
					//copiez lista de grupe
					init_lista_grupe(nod_lucru->li_grupa);
					while(aux != NULL)
					{
						addlist(nod_lucru->li_grupa, aux->nume);
						aux = aux->pnext;
					}
					nod_lucru->nume = (char*)calloc( 21, sizeof(char));
					strcpy(nod_lucru->nume, nod1->nume);
					nod_lucru->paritate = nod1->paritate;
					
					strcpy( nod_lucru->serie, nod1->serie);
					nod_lucru->pnext = NULL;
					
					
					nod2->pnext = nod_lucru;//adaug nod_lucru la lista
					
					//printf("copiere un nod la %d %d %d contor %d\n", i, z, o, contor);
					nod1 = nod1->pnext;
					contor++;


				}//while
				
			}//for

}

void copy_orar_curs(ORAR_CURS *buffer_dest, ORAR_CURS *buffer_s, int nr_elem)
{
	oc_curs *nod1;
	oc_curs *aux2;
	int contor;
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
			//copiez fiecare oc_curs
				if(buffer_s[i][z][o].nume_materie == NULL)
				{
					buffer_dest[i][z][o] = buffer_s[i][z][o];
					//printf("copiez un NULL la %d-%d-%d\n", i, z, o);
					continue;
				}
				contor = 1;
				nod1 = &buffer_s[i][z][o];
				while( nod1 != NULL)
				{
					//adica primul oc_curs
					if(contor == 1)
					{
						buffer_dest[i][z][o].acronim = (char*)calloc( 10, sizeof(char) );
						strcpy(buffer_dest[i][z][o].acronim, nod1->acronim);
						buffer_dest[i][z][o].nume_materie = (char*)calloc( 80, sizeof(char));
						strcpy(buffer_dest[i][z][o].nume_materie, nod1->nume_materie);
						buffer_dest[i][z][o].paritate = nod1->paritate;
						buffer_dest[i][z][o].pnext = NULL;
						buffer_dest[i][z][o].serie = (char*)calloc( 41, sizeof(char));
						strcpy(buffer_dest[i][z][o].serie, nod1->serie);
						//printf("copiez un element prima poz %d-%d-%d %s\n", i, z, o, buffer_dest[i][z][o].acronim);
					}//contor = 1 
					else
					{
						oc_curs *nod_lucru = new oc_curs;
						nod_lucru->acronim = (char*)calloc( 10, sizeof(char) );
						strcpy(nod_lucru->acronim, nod1->acronim);
						nod_lucru->nume_materie = (char*)calloc( 80, sizeof(char));
						strcpy(nod_lucru->nume_materie, nod1->nume_materie);
						nod_lucru->paritate = nod1->paritate;
						nod_lucru->pnext = NULL;
						nod_lucru->serie = (char*)calloc( 41, sizeof(char));
						strcpy(nod_lucru->serie, nod1->serie);

						aux2 = &buffer_dest[i][z][o];
						while(aux2->pnext != NULL)
							aux2 = aux2->pnext;//aux2 va fi ultimul element din lista de ocupare
						aux2->pnext = nod_lucru;

						//printf("copiez un element in lista la %d-%d-%d %s\n", i, z, o, nod_lucru->acronim);
					}//contor > 1
					
					nod1 = nod1->pnext;
										
					contor++;

				}//while

			}//for
}

void elibereaza(ocupare &oc)
{
	oc.index_titular = -2;
	oc.li_grupa.nr_elem = 0;
	oc.li_grupa.plast = NULL;
	oc.li_grupa.pfirst = NULL;
	oc.nume_materie = NULL;
	oc.paritate = -2;
	oc.pnext = NULL;
	oc.tip = -2;
	oc.sala = NULL;
}

void elibereaza(ORAR_LAB* buffer, int indice_sala, int z, int o)
{
	buffer[indice_sala - 1][z][o].nume = NULL;
	buffer[indice_sala - 1][z][o].li_grupa.pfirst = NULL;
	buffer[indice_sala - 1][z][o].li_grupa.plast = NULL;
	buffer[indice_sala - 1][z][o].li_grupa.nr_elem = 0;
	buffer[indice_sala - 1][z][o].serie[0] = '\0';
	buffer[indice_sala - 1][z][o].pnext = NULL;
	buffer[indice_sala - 1][z][o].paritate = -2;
}

int egalitate(ocupare oc1, ocupare oc2)
{

	if( strcmp(oc1.sala, oc2.sala) != 0)
		return 0;
	if( strcmp(oc1.nume_materie, oc2.nume_materie) != 0)
		return 0;
	if (oc1.paritate != oc2.paritate)
		return 0;
	if (oc1.tip != oc2.tip)
		return 0;
	//verificare lista grupe
	if(oc1.li_grupa.nr_elem != oc2.li_grupa.nr_elem)
		return 0;
	nod_grupa *png1, *png2;
	png1 = oc1.li_grupa.pfirst;
	png2 = oc2.li_grupa.pfirst;
	while( png1 != NULL)
	{
		if( strcmp(png1->nume, png2->nume) != 0 )
			return 0;
		png1 = png1->pnext;
		png2 = png2->pnext;
	}
	return 1;
}