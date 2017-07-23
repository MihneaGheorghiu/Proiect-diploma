#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "header1.h"
#include <time.h>

int apeluri_optimizare;
int apeluri_optimizare_grupe_l;
int apeluri_optimizare_2;
int tip_apel;
int tip_apel_gr;
extern int call_stack;
//var globale externe
extern clock_t start, finish;
extern numar_grupe v_grupe[NR_ORARE];
extern int suma_nr_grupe_l;
extern int nr_grupe_l;
extern int suma_nr_grupe_m;
extern COST *cost_l_1;
extern COST *cost_l_gr;
extern COST *cost_l_2;
extern COST *cost_init_1;
extern COST *cost_init_gr;
extern COST *cost_init_2;
extern val_ore *ora_l_start[5], *ora_l_finis[5];
extern val_ore *ora_l_start_gr[5], *ora_l_finis_gr[5];
extern ORAR_LAB *buffer_lab_l;
extern ORAR_LAB *buffer_lab_f;
extern ORAR_LAB *buffer_seminar_l;
extern ORAR_LAB *buffer_seminar_f;
extern ORAR_LAB *buffer_p_l;
extern ORAR_LAB *buffer_p_f;
extern ORAR_CURS *buffer_curs_l;
extern ORAR_CURS *buffer_curs_f;
extern ORAR ORARE_F[NR_ORARE];
//extern ORAR_P *buffer_p_l;
extern lista_sali Li_lab;
extern lista_sali Li_sem;
extern lista_sali Li_p;
extern lista_sali Li_curs;
extern lista_discipline L_D[NR_ORARE];
extern char* nume_orare[NR_ORARE];
int cost_t_l_1;
int cost_t_l_gr;
int cost_t_l_2;
extern int cost_t_o_1;
extern int cost_t_o_gr;
extern int cost_t_o_2;


void test_extern()
{
	for(int i = 0; i < NR_ORARE; i++)
	{
		printf("orar %d nr grupe total %d numar grupe optionale %d\n", i, v_grupe[i].nr_grupe, v_grupe[i].nr_optionale);
	}
}

//fctie care verifica daca semigrupa este in lista de grupa a unei oc (fara grupa intreaga)
int exista_doar_semigrupa(ocupare *oc, int semi_grupa, int paritate)
{
	nod_grupa *ngr, *naux;

	int pp; //va retine paritatea materiei care ocupa oc
	pp = oc->paritate;
	//daca e curs, are toate semigrupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei si ce semigrupa e
	int indice_orar, nr, semigrupa;
	int aux = semi_grupa;
	//orar
	int j = 0;
	while(aux >= (2 * v_grupe[j].nr_grupe))
	{
		aux -= (2 * v_grupe[j].nr_grupe);
		j++;
	}// j o sa indice nr de orar, aux va retine ce a mai ramas
	indice_orar = j;
	nr = 1;
	while(aux >= 2)
	{
		aux -= 2;
		nr++;
	}
	if (aux == 0)
		semigrupa = 1;
	if (aux == 1)
		semigrupa = 2;

	ngr = generare_nume_grupa(indice_orar, nr, semigrupa);
	//ngr2 = generare_nume_grupa(indice_orar, nr, 0);//pentru grupa totala
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( (strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		naux = naux->pnext;
	}
	return 0;
}

//fctie care vede daca exista grupa intr-o casuta de ocupare din orarul mare
int exista_semigrupa(ocupare *oc, int semi_grupa, int paritate)
{
	nod_grupa *ngr, *ngr2, *naux;

	int pp; //va retine paritatea materiei care ocupa oc
	pp = oc->paritate;
	//daca e curs, are toate semigrupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei si ce semigrupa e
	int indice_orar, nr, semigrupa;
	int aux = semi_grupa;
	//orar
	int j = 0;
	while(aux >= (2 * v_grupe[j].nr_grupe))
	{
		aux -= (2 * v_grupe[j].nr_grupe);
		j++;
	}// j o sa indice nr de orar, aux va retine ce a mai ramas
	indice_orar = j;
	nr = 1;
	while(aux >= 2)
	{
		aux -= 2;
		nr++;
	}
	if (aux == 0)
		semigrupa = 1;
	if (aux == 1)
		semigrupa = 2;

	ngr = generare_nume_grupa(indice_orar, nr, semigrupa);
	ngr2 = generare_nume_grupa(indice_orar, nr, 0);//pentru grupa totala
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( (strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		if( (strcmp( naux->nume, ngr2->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		naux = naux->pnext;
	}
	return 0;
}

int verif_grupa(ocupare *oc, int grupa, int paritate)
{
	nod_grupa *ngr;//grupa mare
	nod_grupa *ngr1;//prima semigrupa
	nod_grupa *ngr2; //a 2a semigrupa
	nod_grupa *naux;

	int pp; //va retine paritatea materiei care ocupa oc
	pp = oc->paritate;
	//daca e curs, are toate grupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei
	int indice_orar, nr;
	int aux = grupa;
	//orar
	int j = 0;
	while(aux >=  v_grupe[j].nr_grupe)
	{
		aux -= v_grupe[j].nr_grupe;
		j++;
	}// j o sa indice nr de orar, aux va retine ce a mai ramas
	indice_orar = j;
	nr = 1;
	while(aux >= 1)
	{
		aux--;
		nr++;
	}
	
	ngr = generare_nume_grupa(indice_orar, nr, 0);//pentru grupa totala
	ngr1 = generare_nume_grupa(indice_orar, nr, 1);//pentru prima semigrupa
	ngr2 = generare_nume_grupa(indice_orar, nr, 2);//pentru a 2a semigrupa
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( (strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		if( (strcmp( naux->nume, ngr1->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		if( (strcmp( naux->nume, ngr2->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		naux = naux->pnext;
	}
	return 0;
}

//sa existe DOAR grupa, fara semigrupe
int verif_grupa2(ocupare *oc, int grupa, int paritate)
{
	nod_grupa *ngr;//grupa mares
	nod_grupa *naux;

	int pp; //va retine paritatea materiei care ocupa oc
	pp = oc->paritate;
	//daca e curs, are toate grupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei
	int indice_orar, nr;
	int aux = grupa;
	//orar
	int j = 0;
	while(aux >=  v_grupe[j].nr_grupe)
	{
		aux -= v_grupe[j].nr_grupe;
		j++;
	}// j o sa indice nr de orar, aux va retine ce a mai ramas
	
	indice_orar = j;
	nr = 1;
	while(aux >= 1)
	{
		aux--;
		nr++;
	}
	
	ngr = generare_nume_grupa(indice_orar, nr, 0);//pentru grupa totala
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( (strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		
		naux = naux->pnext;
	}
	return 0;
}

int verif_grupa_master(ocupare *oc, int grupa, int paritate)
{
	nod_grupa *ngr;//grupa mare
	nod_grupa *naux;

	int pp; //va retine paritatea materiei care ocupa oc
	pp = oc->paritate;
	//daca e curs, are toate grupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei
	int indice_orar, nr;
	int aux = grupa;
	//orar
	int j = 0;
	
	aux -= suma_nr_grupe_l;
	j = 14;
	while(aux >=  v_grupe[j].nr_grupe)
		{
			aux -= v_grupe[j].nr_grupe;
			j++;
		}
	while(aux >=  v_grupe[j].nr_grupe)
		{
			aux -= v_grupe[j].nr_grupe;
			j++;
		}
	indice_orar = j;
	nr = 1;
	while(aux >= 1)
	{
		aux--;
		nr++;
	}
	
	ngr = generare_nume_grupa(indice_orar, nr, 0);//pentru grupa totala
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( (strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0 || paritate == 0) )
			return 1;
		
		naux = naux->pnext;
	}
	return 0;
}

void gaseste_goluri(ORAR O, int semi_grupa, int paritate, GOL coada_goluri[20], int &nr_goluri)
{
	//printf("++a intrat in fctie++\n");
	int a_inceput;
	int durata_gol = 0;
	int inceput_gol = 0;
	int sf_gol = 0;
	ocupare *oc;
	int ora_stop;

	
	for(int z = 0; z < 5; z++)
	{
		if( paritate == 1)
			ora_stop = ora_l_finis[z][semi_grupa].val_impar;
		if (paritate == 2)
			ora_stop = ora_l_finis[z][semi_grupa].val_par;
		if (paritate == 0)
		{
			if(ora_l_finis[z][semi_grupa].val_impar > ora_l_finis[z][semi_grupa].val_par )
				ora_stop = ora_l_finis[z][semi_grupa].val_impar;
			else
				ora_stop = ora_l_finis[z][semi_grupa].val_par;
		}
		//printf("ora_stop %d zi %d\n", ora_stop, z);
		a_inceput = 0;
		for(int o = 8; o < 22; o++)
			if( O[z][o].nume_materie != NULL)
			{
				int gasit = 0;//sa vad daca semigrupa cautata e in lista de materii de la acea ora
				oc = &O[z][o];
				while(oc != NULL)
				{
					if(exista_semigrupa(oc, semi_grupa, paritate) > 0)
					{
						gasit = 1;
						a_inceput = 1;
						durata_gol = 0;
						inceput_gol = 0;
						sf_gol = 0;
					}
					oc = oc->pnext;
				}
				if (gasit == 0)
					goto casuta_libera;
			}
			else//am casuta libera
			{
				
casuta_libera:	if(a_inceput == 1 && o < ora_stop )
				{
					//printf("-a inceput un gol zi %d ora %d-\n", z, o);
					durata_gol++;
					if(inceput_gol == 0)
					{
						inceput_gol = o;
						sf_gol = o + 1;
					}
					else//aveam deja un gol inceput
					{
						if(durata_gol == 2)//am un gol complet
						{
							sf_gol++;
							//adaugam golul
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
						if(durata_gol == 1)
							sf_gol++;
						if(durata_gol > 2)
						{
							inceput_gol++; //decalez cu o pozitie
							sf_gol++;
							durata_gol--; 
							//adaug noul gol
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
					}//gol inceput
					

					
				}//if a_inceput
			}//casuta libera
	}//for zile
	//printf("\n");
}

void gaseste_goluri_gr(ORAR O, int grupa, int paritate, GOL coada_goluri[20], int &nr_goluri)
{
	//printf("++a intrat in fctie++\n");
	int a_inceput;
	int durata_gol = 0;
	int inceput_gol = 0;
	int sf_gol = 0;
	ocupare *oc;
	int ora_stop;

	
	for(int z = 0; z < 5; z++)
	{
		if( paritate == 1)
			ora_stop = ora_l_finis_gr[z][grupa].val_impar;
		if (paritate == 2)
			ora_stop = ora_l_finis_gr[z][grupa].val_par;
		if (paritate == 0)
		{
			if(ora_l_finis_gr[z][grupa].val_impar > ora_l_finis_gr[z][grupa].val_par )
				ora_stop = ora_l_finis_gr[z][grupa].val_impar;
			else
				ora_stop = ora_l_finis_gr[z][grupa].val_par;
		}
		//printf("ora_stop %d zi %d\n", ora_stop, z);
		a_inceput = 0;
		for(int o = 8; o < 22; o++)
			if( O[z][o].nume_materie != NULL)
			{
				int gasit = 0;//sa vad daca grupa cautata e in lista de materii de la acea ora
				oc = &O[z][o];
				while(oc != NULL)
				{
					if(verif_grupa(oc, grupa, paritate) > 0)
					{
						gasit = 1;
						a_inceput = 1;
						durata_gol = 0;
						inceput_gol = 0;
						sf_gol = 0;
					}
					oc = oc->pnext;
				}
				if (gasit == 0)
					goto casuta_libera;
			}
			else//am casuta libera
			{
				
casuta_libera:	if(a_inceput == 1 && o < ora_stop )
				{
					//printf("-a inceput un gol zi %d ora %d-\n", z, o);
					durata_gol++;
					if(inceput_gol == 0)
					{
						inceput_gol = o;
						sf_gol = o + 1;
					}
					else//aveam deja un gol inceput
					{
						if(durata_gol == 2)//am un gol complet
						{
							sf_gol++;
							//adaugam golul
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
						if(durata_gol == 1)
							sf_gol++;
						if(durata_gol > 2)
						{
							inceput_gol++; //decalez cu o pozitie
							sf_gol++;
							durata_gol--; 
							//adaug noul gol
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
					}//gol inceput
					

					
				}//if a_inceput
			}//casuta libera
	}//for zile
	//printf("\n");
}

void gaseste_goluri_2(ORAR O, int grupa, int paritate, GOL coada_goluri[20], int &nr_goluri)
{
	//printf("++a intrat in fctie++\n");
	int a_inceput;
	int durata_gol = 0;
	int inceput_gol = 0;
	int sf_gol = 0;
	ocupare *oc;
	int ora_stop;

	
	for(int z = 0; z < 5; z++)
	{
		if( paritate == 1)
			ora_stop = ora_l_finis[z][grupa].val_impar;
		if (paritate == 2)
			ora_stop = ora_l_finis[z][grupa].val_par;
		if (paritate == 0)
		{
			if(ora_l_finis[z][grupa].val_impar > ora_l_finis[z][grupa].val_par )
				ora_stop = ora_l_finis[z][grupa].val_impar;
			else
				ora_stop = ora_l_finis[z][grupa].val_par;
		}
		//printf("ora_stop %d zi %d\n", ora_stop, z);
		a_inceput = 0;
		for(int o = 8; o < 22; o++)
			if( O[z][o].nume_materie != NULL)
			{
				int gasit = 0;//sa vad daca grupa cautata e in lista de materii de la acea ora
				oc = &O[z][o];
				while(oc != NULL)
				{
					if(verif_grupa_master(oc, grupa, paritate) > 0)
					{
						gasit = 1;
						a_inceput = 1;
						durata_gol = 0;
						inceput_gol = 0;
						sf_gol = 0;
					}
					oc = oc->pnext;
				}
				if (gasit == 0)
					goto casuta_libera;
			}
			else//am casuta libera
			{
				
casuta_libera:	if(a_inceput == 1 && o < ora_stop )
				{
					//printf("-a inceput un gol zi %d ora %d-\n", z, o);
					durata_gol++;
					if(inceput_gol == 0)
					{
						inceput_gol = o;
						sf_gol = o + 1;
					}
					else//aveam deja un gol inceput
					{
						if(durata_gol == 2)//am un gol complet
						{
							sf_gol++;
							//adaugam golul
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
						if(durata_gol == 1)
							sf_gol++;
						if(durata_gol > 2)
						{
							inceput_gol++; //decalez cu o pozitie
							sf_gol++;
							durata_gol--; 
							//adaug noul gol
							coada_goluri[nr_goluri].z = z;
							coada_goluri[nr_goluri].s = inceput_gol;
							coada_goluri[nr_goluri].f = sf_gol;
							//printf(">>nr_goluri devine %d zi %d ora_s %d ora_f %d<<\n", nr_goluri + 1,
							//	coada_goluri[nr_goluri].z, coada_goluri[nr_goluri].s, coada_goluri[nr_goluri].f);
							nr_goluri++;
							
						}
					}//gol inceput
					

					
				}//if a_inceput
			}//casuta libera
	}//for zile
	//printf("\n");
}


int exista_grupa(ocupare *oc, int grupa, int paritate)
{
	nod_grupa *ngr, *naux;
	int pp;//va retine paritatea
	pp = oc->paritate;
	//daca e curs, are toate semigrupele 
	if( (oc->tip == 0) && ( pp == paritate || pp == 0) )
		return 1;
	//trebuie sa calc numarul de orar, numarul grupei
	int indice_orar, nr;
	int aux = grupa;
	//orar
	int j = 0;
	while( aux >= 2 * v_grupe[j].nr_grupe )//trec de orarele de licenta
	{
		aux -= (2 * v_grupe[j].nr_grupe);
		j++;
	}
	/*//sa sare peste ultimul orar de licenta
	while( aux >= v_grupe[j].nr_grupe)//adaug orare de master
	{
		aux -= v_grupe[j].nr_grupe;
		j++;
	}*/
	// j o sa indice nr de orar, aux va retine ce a mai ramas
	indice_orar = j;
	nr = 1;
	//cat a mai ramas in aux va arata grupa
	while (aux >= 1)
	{
		aux--;
		nr++;
	}
	ngr = generare_nume_grupa(indice_orar, nr, 0);
	naux = oc->li_grupa.pfirst;
	while(naux != NULL)
	{
		if( ( strcmp( naux->nume, ngr->nume) == 0) && ( pp == paritate || pp == 0) )
			return 1;
		naux = naux->pnext;
	}
	return 0;
}

//fctie care calculeaza costul unei semigrupe intr-un orar pe zile
int calc_cost_1(ORAR O, int semi_grupa, val_ore *ora_start[], val_ore *ora_finis[], int paritate)
{
/////# O o sa fie ocupare[5][22]
	int cost = 0;
	int costproba = 0; //nu stiu daca s-a terminat orarul sau nu
	int gasit_sgr; // //daca a fost gasita semigrupa in casuta de ocupare
	ocupare *oc, aux;
	for(int z = 0; z < 5; z++)
	{
		gasit_sgr = 0;//nu am semigrupa la inceputul orelor
		for(int ora = 8; ora < 22; ora++)
		{
			if( O[z][ora].nume_materie != NULL)
			//trebuie sa vad pentru fiecare materie daca grupa sau semigrupa este in lista
			{
				oc = &O[z][ora];
				gasit_sgr = 0;//incep ptr o casuta noua
				while(oc != NULL)
				{
					if( exista_semigrupa(oc, semi_grupa, paritate) != 0 )
					{
						gasit_sgr = 1;
						if (paritate == 1) //impara
						{
							if(ora_start[z][semi_grupa].val_impar == 0)
								ora_start[z][semi_grupa].val_impar = ora;
							if (ora_start[z][semi_grupa].val_impar != 0)
								ora_finis[z][semi_grupa].val_impar = ora + 1;
						}
						if (paritate == 2) //para
						{
							if(ora_start[z][semi_grupa].val_par == 0)
								ora_start[z][semi_grupa].val_par = ora;
							if (ora_start[z][semi_grupa].val_par != 0)
								ora_finis[z][semi_grupa].val_par = ora + 1;
						}
						costproba = 0;
					}
					oc = oc->pnext;
				}
				if(gasit_sgr ==0)//nu apare grupa
				{
					if(paritate == 1)//impara
						if(ora_start[z][semi_grupa].val_impar != 0)
						{
							costproba++;
							cost++;
						}
					if(paritate == 2)//para
						if(ora_start[z][semi_grupa].val_par != 0)
						{
							costproba++;
							cost++;
						}

				}
				
			}//casuta oc
			else//casuta libera
			{
				gasit_sgr = 0;
				if(paritate == 1)
					if(ora_start[z][semi_grupa].val_impar != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}
				if(paritate == 2)
					if(ora_start[z][semi_grupa].val_par != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}

			}//casuta libera
		
		}//for ore
		//la sfarsitul orelor orice scad costul de proba neconfirmat
		cost -= costproba;
		costproba = 0;
		//printf("paritate %d zi %dcost %d\n", paritate, z, cost);
		
		//vad daca am o zi care s-a eliberat complet, sa nu 
		if (paritate == 1)
		{
			if(ora_start[z][semi_grupa].val_impar == 0)
				ora_finis[z][semi_grupa].val_impar = 0;
		}
		if (paritate == 2)
		{
			if(ora_start[z][semi_grupa].val_par == 0)
				ora_finis[z][semi_grupa].val_par = 0;
		}
		/*
		if( tip_apel == 1)	
		{
			if( paritate == 1)
			printf("semi_grupa %d zi %d costul %d ora_p %d ora_sf %d\n",
				semi_grupa, z, cost, ora_start[z][semi_grupa].val_impar, ora_finis[z][semi_grupa].val_impar);
			if( paritate == 2)
			printf("semi_grupa %d zi %d costul %d ora_p %d ora_sf %d\n",
				semi_grupa, z, cost, ora_start[z][semi_grupa].val_par, ora_finis[z][semi_grupa].val_par);
		}*/
		
	}//for zile
	return cost;
}

int calc_cost_gr(ORAR O, int grupa, val_ore *ora_start_gr[], val_ore *ora_finis_gr[], int paritate)
{
/////# O o sa fie ocupare[5][22]
	//printf("Calc_cost_gr %d\n", grupa);
	int cost = 0;
	int costproba = 0; //nu stiu daca s-a terminat orarul sau nu
	int gasit_gr; //daca a fost gasita grupa in casuta de ocupare
	ocupare *oc, aux;
	for(int z = 0; z < 5; z++)
	{
		gasit_gr = 0;//nu am grupa la inceputul orelor
		for(int ora = 8; ora < 22; ora++)
		{
			if( O[z][ora].nume_materie != NULL)
			//trebuie sa vad pentru fiecare materie daca grupa este in lista
			{
				oc = &O[z][ora];
				gasit_gr = 0;//incep ptr o casuta noua
				while(oc != NULL)
				{
					if( verif_grupa(oc, grupa, paritate) != 0 )
					{
						gasit_gr = 1;
						if (paritate == 1) //impara
						{
							if(ora_start_gr[z][grupa].val_impar == 0)
								ora_start_gr[z][grupa].val_impar = ora;
							if (ora_start_gr[z][grupa].val_impar != 0)
								ora_finis_gr[z][grupa].val_impar = ora + 1;
						}
						if (paritate == 2) //para
						{
							if(ora_start_gr[z][grupa].val_par == 0)
								ora_start_gr[z][grupa].val_par = ora;
							if (ora_start_gr[z][grupa].val_par != 0)
								ora_finis_gr[z][grupa].val_par = ora + 1;
						}
						costproba = 0;
					}
					oc = oc->pnext;
				}
				if(gasit_gr ==0)//nu apare grupa
				{
					if(paritate == 1)//impara
						if(ora_start_gr[z][grupa].val_impar != 0)
						{
							costproba++;
							cost++;
						}
					if(paritate == 2)//para
						if(ora_start_gr[z][grupa].val_par != 0)
						{
							costproba++;
							cost++;
						}

				}
				
			}//casuta oc
			else//casuta libera
			{
				gasit_gr = 0;
				if(paritate == 1)
					if(ora_start_gr[z][grupa].val_impar != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}
				if(paritate == 2)
					if(ora_start_gr[z][grupa].val_par != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}

			}//casuta libera
		
		}//for ore
		//la sfarsitul orelor orice scad costul de proba neconfirmat
		cost -= costproba;
		costproba = 0;
		//printf("paritate %d zi %dcost %d\n", paritate, z, cost);
		
		//vad daca am o zi care s-a eliberat complet, sa nu 
		if (paritate == 1)
		{
			if(ora_start_gr[z][grupa].val_impar == 0)
				ora_finis_gr[z][grupa].val_impar = 0;
		}
		if (paritate == 2)
		{
			if(ora_start_gr[z][grupa].val_par == 0)
				ora_finis_gr[z][grupa].val_par = 0;
		}
		/*
		if( tip_apel == 1)	
		{
			if( paritate == 1)
				;
			printf("grupa %d zi %d costul impar %d ora_p %d ora_sf %d\n",
				grupa, z, cost, ora_start_gr[z][grupa].val_impar, ora_finis_gr[z][grupa].val_impar);
			if( paritate == 2)
				
			printf("grupa %d zi %d costul %d par ora_p %d ora_sf %d\n",
				grupa, z, cost, ora_start_gr[z][grupa].val_par, ora_finis_gr[z][grupa].val_par);
		}*/
		
	}//for zile
	//printf("cost ret %d\n", cost);
	return cost;
}

//fctie care calculeaza costul unei grupe intr-un orar pe zile
int calc_cost_2(ORAR O, int grupa, val_ore *ora_start[], val_ore *ora_finis[], int paritate)
{
/////# O o sa fie ocupare[5][22]
	int cost = 0;
	int costproba = 0; //nu stiu daca s-a terminat orarul sau nu
	int gasit_gr; //daca a fost gasita grupa in casuta de ocupare
	ocupare *oc, aux;
	for(int z = 0; z < 5; z++)
	{
		gasit_gr = 0;//nu am grupa la inceputul orelor
		for(int ora = 8; ora < 22; ora++)
		{
			if( O[z][ora].nume_materie != NULL)
			//trebuie sa vad pentru fiecare materie daca grupa este in lista
			{
				oc = &O[z][ora];
				gasit_gr = 0;//incep ptr o casuta noua
				while(oc != NULL)
				{
					if( verif_grupa_master(oc, grupa, paritate) != 0 )
					{
						gasit_gr = 1;
						if(paritate == 1)//impara
						{
							if(ora_start[z][grupa].val_impar == 0)
								ora_start[z][grupa].val_impar = ora;
							if (ora_start[z][grupa].val_impar != 0)
								ora_finis[z][grupa].val_impar = ora + 1;
						}
						if(paritate == 2)
						{
							if(ora_start[z][grupa].val_par == 0)
								ora_start[z][grupa].val_par = ora;
							if (ora_start[z][grupa].val_par != 0)
								ora_finis[z][grupa].val_par = ora + 1;
						}
						costproba = 0;
					}
					oc = oc->pnext;
				}
				if(gasit_gr ==0)//nu apare grupa
				{
					if(paritate == 1)
						if(ora_start[z][grupa].val_impar != 0)
						{
							costproba++;
							cost++;
						}
					if(paritate == 2)
						if(ora_start[z][grupa].val_par != 0)
						{
							costproba++;
							cost++;
						}

				}

			}//inchidere casuta
			else//casuta libera
			{
				gasit_gr = 0;
				if(paritate == 1)
					if(ora_start[z][grupa].val_impar != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}
				if(paritate == 2)
					if(ora_start[z][grupa].val_par != 0)//a inceput programul
					{	
						cost++;
						costproba++;
					}

			}//casuta libera
		}//inchidere ore
		//la sfarsitul orelor orice scad costul de proba neconfirmat
		cost -= costproba;
		costproba = 0;
		//printf("paritate %d zi %dcost %d\n", paritate, z, cost);
		//vad daca am o zi care s-a eliberat complet, sa nu 
		if (paritate == 1)
		{
			if(ora_start[z][grupa].val_impar == 0)
				ora_finis[z][grupa].val_impar = 0;
		}
		if (paritate == 2)
		{
			if(ora_start[z][grupa].val_par == 0)
				ora_finis[z][grupa].val_par = 0;
		}
		//if( paritate == 1)
		//	printf("grupa %d zi %d costul impar %d ora_p %d ora_sf %d\n",
		//		grupa, z, cost, ora_start[z][grupa].val_impar, ora_finis[z][grupa].val_impar);
		//if( paritate == 2)
		//	printf("grupa %d zi %d costul par %d ora_p %d ora_sf %d\n",
		//		grupa, z, cost, ora_start[z][grupa].val_par, ora_finis[z][grupa].val_par);
	}//inchidere zile
	return cost;
}

int este_liber(char *sala, int z, int o)
{
	int indice_sala = getat(Li_lab, sala);
	if(indice_sala != -1)
		if(buffer_lab_l[indice_sala - 1][z][o].nume != NULL)
			return 0;
	indice_sala = getat(Li_sem, sala);
	if(indice_sala != -1)
		if(buffer_seminar_l[indice_sala - 1][z][o].nume != NULL)
			return 0;
	indice_sala = getat(Li_curs, sala);
	if(indice_sala != -1)
		if(buffer_curs_l[indice_sala - 1][z][o].nume_materie!= NULL)
			return 0;
	indice_sala = getat(Li_p, sala);
	if(indice_sala != -1)
		if(buffer_p_l[indice_sala - 1][z][o].nume != NULL)
			return 0;
	return 1;
}

//fctie care verifica daca sunt si alte grupe care au un lab comun libere
int este_liber(ORAR O, char *grupa ,int z, int o)
{
	nod_grupa *ngr;
	if ( O[z][o].nume_materie == NULL)
		return 1;
	ocupare *oc = &O[z][o];
	while( oc != NULL)
	{
		ngr = oc->li_grupa.pfirst;
		while( ngr != NULL)
		{
			if( strcmp( grupa, ngr->nume) == 0)
				return 0;
			ngr = ngr->pnext;
		}
		oc = oc->pnext;
	}
	return 1;

}

//fctie de optimizare pentru semigrupe - lab/sem/proiecte care sunt pe semigrupe
void optimizare_1(ORAR ORARE_L[], int semi_grupa, int mod)
//mod - 0 pe ambele paritati, 1 impara 2 para
{
	call_stack++;
	
	if(apeluri_optimizare >= 2400)
	{
		// 9 printf("IESIRE din optimizare\n");
		call_stack--;
		return;
	}
	//1 printf("##### OPTIMIZARE %d#####stiva %d\n", apeluri_optimizare, call_stack);
	int nr_apel_asta = apeluri_optimizare;
	apeluri_optimizare++;
	
	
	int j = 0; //vor retine orarul, grupa/semigrupa
	int nr_gr;
	int nr_sgr;
	int mod_c = mod;
	ocupare *oc;
	ocupare *oc2;//pentru o + 1
	ocupare *oc_pi;//pentru pasul inapoi
	oc_pi = new ocupare;
	ocupare *aux;//pentru eliminare
	GOL coada_goluri[20];
	int pot_muta = 0; //daca e la inceputul sau sfarsitul orelor
	int nr_goluri = 0;
	//pentru fiecare semigrupa - semi_grupa o sa varieze de la 0 la nr maxim
	if (semi_grupa >= suma_nr_grupe_l)
		printf("$$ EROARE nr grupe_l");
	if( (cost_l_1[semi_grupa].cost_impar == 0) && (cost_l_1[semi_grupa].cost_par == 0) )
	{
	//2	printf("%d grupa optima\n", semi_grupa);
		return;
	}
	if(cost_l_1[semi_grupa].cost_impar == 0)
		mod_c = 2;//lucrez doar pe para
	if(cost_l_1[semi_grupa].cost_par == 0)
		mod_c = 1; //lucrez pe impara de acuma incolo in aceasta parcurgere
	

	int aux_po = semi_grupa;
	while(aux_po >= (2 * v_grupe[j].nr_grupe))
	{
		aux_po -= (2 * v_grupe[j].nr_grupe);
		j++;
	}//j va arata nr orarului, aux_po va contine restul - nr grupei/semigrupei
	
	if(aux_po % 2 == 1)
		nr_sgr = 2;
	if(aux_po % 2 == 0)
		nr_sgr = 1;
	nr_gr = aux_po / 2 + 1;
	//printf("%d-nr de orar %d grupa %d{%d}\n", semi_grupa, j, nr_gr, nr_sgr);
	
	int ora_p = 0;//ora de inceput si sfarsit pentru semigrupa
	int ora_sf = 0;


	//incerc  sa bag unde am gauri - incerc sa umplu cate un gol
	for(int z = 0; z < 5; z++)
	{
		//in fctie de paritate - intervalul pe care voi lucra
		if (mod_c == 1) //caut optimizari pe impar
		{
			ora_p = ora_l_start[z][semi_grupa].val_impar;
			ora_sf = ora_l_finis[z][semi_grupa].val_impar;
		}
		if (mod_c == 2)//caut optimizari pe par
		{
			ora_p = ora_l_start[z][semi_grupa].val_par;
			ora_sf = ora_l_finis[z][semi_grupa].val_par;
		}
		if (mod_c == 0)//caut optimizari pe ambele
		{
			//ora de inceput mai mica
			if (ora_l_start[z][semi_grupa].val_impar >= ora_l_start[z][semi_grupa].val_par)
			{
				if(ora_l_start[z][semi_grupa].val_par != 0 )
					ora_p =	ora_l_start[z][semi_grupa].val_par;
				else
					ora_p = ora_l_start[z][semi_grupa].val_impar;
			}
			else
				ora_p = ora_l_start[z][semi_grupa].val_impar;
			//ora de sfarsit mai mare
			if ( ora_l_finis[z][semi_grupa].val_impar <= ora_l_finis[z][semi_grupa].val_par)
				ora_sf = ora_l_finis[z][semi_grupa].val_par;
			else
				ora_sf = ora_l_finis[z][semi_grupa].val_impar;
		}
		// 3 printf("ORAR %d ZI %d sgrupa %dincepe la %d termina la %d\n", j, z, semi_grupa, ora_p, ora_sf);
		// 4 printf("mod cautare %d\n", mod_c);
		//printf("%d\n", nr_goluri);
		for(int o = ora_p; o < ora_sf; o++)
		{
			//am gasit o casuta ocupata
			if( (ORARE_L[j][z][o].nume_materie != NULL) && (apeluri_optimizare < 2400) )
			{
				
				oc = &ORARE_L[j][z][o];
				int ex_sg;
				while( (oc != NULL && oc->tip != -2) && (apeluri_optimizare < 2400) )
				{
					if( ex_sg = exista_doar_semigrupa(oc, semi_grupa, mod_c) )
					{
						// 5 printf("casuta ocupata zi %dora %d\n",z , o);
						if( oc->tip == 1 || oc->tip == 2 || oc->tip == 3)//daca e lab/sem/proiect
						{
							pot_muta = 0;
							//trebuie sa fie primul sau ultimul lab
							if (o == ora_p || o == (ora_sf - 2) )
								pot_muta = 1;
							else
								goto increment;
							nr_goluri = 0;
							gaseste_goluri(ORARE_L[j], semi_grupa, mod_c, coada_goluri, nr_goluri);
							while( (nr_goluri > 0) && (apeluri_optimizare < 2400) )
							{
								//mutarea laboratorului in ultimul gol
//##############################verific daca se poate face mutare daca e sala libera
							// 6	printf("verific daca am sala libera sa mut\n");
								if( este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s) == 0 
									|| este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, 
											coada_goluri[nr_goluri - 1].s + 1) == 0)
									goto gol_urm;
								/* 7 printf("mutare zi%d ora%d in zi_%d ora_%d materia %s grupa %d\n"
									, z, o, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s,
									oc->nume_materie, semi_grupa);*/
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s], oc);
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + 1], oc);
								init_ocupare(oc_pi);
								copy( *oc_pi, oc);
								//trebuie sa fac modificari si in lista de sali
								int indice_sala;
								nod_disciplina *pdisc;
								char *serie;
								int cs = 0;//cursorul seriei
								serie = (char *)calloc( 15, sizeof( char ) );
								while( !isdigit( nume_orare[j][cs]) )
									cs++;
								//incepe numele seriei
								int s = 0;
								while( nume_orare[j][cs] != '.')
									serie[s++] = nume_orare[j][cs++];
								pdisc = getat(oc->nume_materie, L_D[j], oc->tip);
								nod_grupa* g;
								g = generare_nume_grupa(j, nr_gr, nr_sgr);
								if (oc->tip == 1)
								{	
									indice_sala = getat(Li_lab, oc->sala);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 2)
								{
									indice_sala = getat(Li_sem, oc->sala);
									addsem(buffer_seminar_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 3)
								{							
									indice_sala = getat(Li_p, oc->sala);
									addsem(buffer_p_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								//elimin oc din lista de ocupari la ziua/ora aia din orarul mare
								/* 8 printf("elimin %s cu %d orarul %d ziua %d ora %d\n",
									ORARE_L[j][z][o].nume_materie, ORARE_L[j][z][o].li_grupa.nr_elem, j, z, o);*/
								//printf("%s\n", ORARE_L[0][4][8].nume_materie);
								//printf("%s\n", ORARE_L[0][4][9].nume_materie);
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][z][o + ol];
									if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][z][o + ol]);
									else
									{
										if( ol == 0)
										{

											//oc2 = &ORARE_L[j][z][o + 1];
											if ( egalitate(*aux, *oc))
												 //pun pe urm elem dupa oc pe prima pozitie
												suprascrie(ORARE_L[j][z][o + ol], oc->pnext);
											else
											{
												while( egalitate( *(aux->pnext), *oc) != 1)
												{
													aux = aux->pnext;
													//oc2 = oc2->pnext;
												}
												if(oc->pnext !=NULL)//daca oc nu este ultimul din lista
													aux->pnext = oc->pnext;
												else
													aux->pnext = NULL;
											}
										}//ol == 0
										if ( ol == 1)
										{
											//oc in loc de oc2
											if( egalitate(*aux, *oc_pi) == 1)
												 //pun pe urm elem dupa oc pe prima pozitie
												 suprascrie(ORARE_L[j][z][o + ol], aux->pnext);
											else
											{
												//while(aux->pnext != oc2)
												while( egalitate( *(aux->pnext), *oc_pi) != 1)
													aux = aux->pnext;
												//aux->pnext arata acum catre oc_pi care trebuie eliminat
												oc2 = aux;
												aux = aux->pnext;//acum aux arata catre oc_pi
												if(aux->pnext != NULL)//daca oc_pi nu este ultimul din lista
													oc2->pnext = aux->pnext;
												else
													oc2->pnext = NULL;
											}
										}//if ol == 1
									}//else
								}// for elimin oc din lista de ocupari la ziua/ora aia
								//printf("%s\n", ORARE_L[0][4][8].nume_materie);
								//printf("%s\n", ORARE_L[0][4][9].nume_materie);
								//eliberez in orarul salii la ora aia
								if (oc->tip == 1)
								{
									elibereaza(buffer_lab_l, indice_sala, z, o);
									elibereaza(buffer_lab_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 2)
								{
									elibereaza(buffer_seminar_l, indice_sala, z, o);
									elibereaza(buffer_seminar_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 3)
								{								
									elibereaza(buffer_p_l, indice_sala, z, o);
									elibereaza(buffer_p_l, indice_sala, z, o + 1);
								}
								
								//nr_goluri--;
//###costuri ####				//trebuie reactualizate costurile si valorile orelor de pornire/terminare a orelor
								/*if( o == ora_p)//mutarea s-a facut de pe prima pozitie
								{
									if(mod_c == 1)
										ora_l_start[z][semi_grupa].val_impar += 2;
									if(mod_c == 2)
										ora_l_start[z][semi_grupa].val_par += 2;
									if(mod_c == 0)
									{
										ora_l_start[z][semi_grupa].val_impar += 2;
										ora_l_start[z][semi_grupa].val_par += 2;
									}
								}
								if( o == ora_sf - 2)//mutarea s-a facut de pe ultima pozitie
								{
									if(mod_c == 1 || mod_c == 0)
										ora_l_finis[z][semi_grupa].val_impar -= 2;
									if(mod_c == 2 || mod_c == 0)
										ora_l_finis[z][semi_grupa].val_par -= 2;
									
								}*/
								tip_apel = 1;
								//resetez vechile ore de start si finis inainte de calculul costurilor
								if(mod_c == 1)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start[z][semi_grupa].val_impar = 0;
									cost_l_1[semi_grupa].cost_impar = calc_cost_1(ORARE_L[j], semi_grupa, ora_l_start, ora_l_finis, mod_c);
								}
								if(mod_c == 2)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start[z][semi_grupa].val_par = 0;
									cost_l_1[semi_grupa].cost_par = calc_cost_1(ORARE_L[j], semi_grupa, ora_l_start, ora_l_finis, mod_c);
								}
								if(mod_c == 0)
								{
									for(int z = 0; z < 5; z++)
									{
										ora_l_start[z][semi_grupa].val_impar = 0;
										ora_l_start[z][semi_grupa].val_par = 0;
									}
									cost_l_1[semi_grupa].cost_impar = calc_cost_1(ORARE_L[j], semi_grupa, ora_l_start, ora_l_finis, 1);
									cost_l_1[semi_grupa].cost_par = calc_cost_1(ORARE_L[j], semi_grupa, ora_l_start, ora_l_finis, 2);
								}
								//calculam costul total 
								cost_t_l_1 = 0;
								for(int i = 0; i < suma_nr_grupe_l; i++)
								{
									cost_t_l_1 += cost_l_1[i].cost_impar;
									cost_t_l_1 += cost_l_1[i].cost_par;
								}
								/*for(int i = 0; i < suma_nr_grupe_l; i++)
									{
										int cost_vechi = cost_init_1[i].cost_impar + cost_init_1[i].cost_par;
										int cost_lucru = cost_l_1[i].cost_impar + cost_l_1[i].cost_par;
										printf("semigrupa %d costul init//lucru %d--%d\n", i, cost_vechi, cost_lucru);
									}*/
									
									// 10 printf("costul vechi %d, costul NOU %d\n", cost_t_o_1, cost_t_l_1 );
								if (cost_t_l_1 < cost_t_o_1)//avem o noua solutie optima
								{
									//printf("AM modificat costul optim: vechi %d, cost OPTIMnou %d apel %d\n",
									//	cost_t_o_1, cost_t_l_1, apeluri_optimizare);
									cost_t_o_1 = cost_t_l_1;
									//printf("copiez orarul MARE de lucru in cel final \n");
									
									//initializare_orar(ORARE_F);
									copy_orar(ORARE_F, ORARE_L);
									
									//printf("copiez orarul de lab de lucru in cel final \n");
									
									eliberare_mem(buffer_lab_f, Li_lab.nr_elem);
									buffer_lab_f = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									verifica_copiere_orar(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									
									//printf("copiez orarul de seminar de lucru in cel final \n");
									eliberare_mem(buffer_seminar_f, Li_sem.nr_elem);
									buffer_seminar_f = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									verifica_copiere_orar(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									
									//printf("copiez orarul de proiect de lucru in cel final \n");
									eliberare_mem(buffer_p_f, Li_p.nr_elem);
									buffer_p_f = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									verifica_copiere_orar(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									
									//printf("copiez orarul de curs de lucru in cel final \n");
									eliberare_mem(buffer_curs_f, Li_curs.nr_elem);
									buffer_curs_f = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
									copy_orar_curs(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);
									verifica_copiere_orar(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);

								}
								
								// 12 printf("apel din nou\n");
								//apel din nou
								int apelare = 0;
								if(apeluri_optimizare < 2400)
								{	
									int limit_dr = 0;
									int limit_st = 0;
									int limit_max = 0;
									if(semi_grupa <= 33)
									{
										limit_st = 0;
										limit_dr = 33 + 10;
									}
									if( (semi_grupa > 33) && (semi_grupa <= 63) )
									{
										limit_st = 33;
										limit_dr = 63 + 20;
									}
									if( (semi_grupa > 63) && (semi_grupa <= 87) )
									{
										limit_st = 63 - 10;
										limit_dr = 87 + 10;
									}
									if( (semi_grupa > 87) && (semi_grupa <= 121) )
									{
										limit_st = 87 - 10;
										limit_dr = 121;
									}

										for(int i = limit_st; i < limit_dr; i++)
										{
											optimizare_1(ORARE_L, i, mod_c);
											if(apeluri_optimizare > 2399)
												break;
										}
									

									apelare = 1;
								}//apeluri

//##############################//pas inapoi
								//adaug la loc in orarul mare
								
								//if(apelare == 1)
									call_stack--;
								//else
								//	if(apeluri_optimizare == 4199)
								//		call_stack--;
								// 13 printf("###AM INTRAT IN PAS INAPOI %d stiva %d\n",
								//	nr_apel_asta, call_stack);
								//exit(EXIT_FAILURE);
								
								
								//exit(EXIT_SUCCESS);
								copy( ORARE_L[j][z][o], oc_pi);
								copy( ORARE_L[j][z][o + 1], oc_pi);
								//adaug seminariile la loc in orarul de sali
								if (oc_pi->tip == 1)
								{	
									addsem(buffer_lab_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 2)
								{
									addsem(buffer_seminar_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_seminar_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 3)
								{							
									addsem(buffer_p_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_p_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								//elimin noile pozitii
								//eliberez in orarul salii la ora aia
								//nr_goluri++;//pentru a pune ziua si ora golului inapoi
								if (oc_pi->tip == 1)
								{
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri -1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 2)
								{
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 3)
								{								
									elibereaza(buffer_p_l, 
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_p_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								//elimin din orarul mare - acum oc_pi de unde l-am mutat inainte tre eliminat la loc
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + ol];
									int ultim_gol = nr_goluri - 1;
									//daca e chiar pe prima pozitie
									if( egalitate(*aux, *oc_pi) == 1)
									{
										if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									}
									//if(aux->pnext == NULL)
									//{	
									//	elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									//}
									else
									{
										
										if(ol == 0)
										{
											//oc2 = &ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + 1];
											while( egalitate( *(aux->pnext), *oc_pi) != 1 )
											{
												aux = aux->pnext;
												//oc2 = oc2->pnext;
											}
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
										if( ol == 1)
										{
											while( egalitate( *(aux->pnext), *oc_pi) != 1)
												aux = aux->pnext;
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
									}//else
								}//for eliminare

								//o++;//fiecare lab/sem/p are 2 ore deci sa nu il mut de 2 ori
gol_urm:						nr_goluri--;
							}//while exista goluri
						}//if lab/sem/p

					}//if exista semigrupa
increment:			oc = oc->pnext;
					nr_goluri = 0;
					
				}//while oc
				
			}//if casuta
		}//for ore

	}//for zile

					
}


void optimizare_grupe_licenta(ORAR ORARE_L[], int grupa, int mod)
//mod - 0 pe ambele paritati, 1 impara 2 para
{
	call_stack++;
	if(apeluri_optimizare_grupe_l >= 1500)
	{
		// 9 printf("IESIRE din optimizare\n");
		call_stack--;
		return;
	}
	//printf("##### OPTIMIZARE GRUPE LICENTA %d#####stiva %d\n", apeluri_optimizare_grupe_l, call_stack);
	int nr_apel_asta = apeluri_optimizare_grupe_l;
	apeluri_optimizare_grupe_l++;
	
	
	int j = 0; //vor retine orarul, grupa
	int nr_gr;
	int mod_c = mod;
	ocupare *oc;
	ocupare *oc2;//pentru o + 1
	ocupare *oc_pi;//pentru pasul inapoi
	oc_pi = new ocupare;
	ocupare *aux;//pentru eliminare
	GOL coada_goluri[20];
	int pot_muta = 0; //daca e la inceputul sau sfarsitul orelor
	int nr_goluri = 0;
	//pentru fiecare grupa - semi_grupa o sa varieze de la 0 la nr maxim
	if (grupa >= nr_grupe_l)
		printf("$$ EROARE nr grupe_l");
	if( (cost_l_gr[grupa].cost_impar == 0) && (cost_l_gr[grupa].cost_par == 0) )
	{
	//	printf("%d grupa optima apel %d\n", grupa, apeluri_optimizare_grupe_l - 1);
		return;
	}
	if(cost_l_gr[grupa].cost_impar == 0)
		mod_c = 2;//lucrez doar pe para
	if(cost_l_gr[grupa].cost_par == 0)
		mod_c = 1; //lucrez pe impara de acuma incolo in aceasta parcurgere
	

	int aux_po = grupa;
	while(aux_po >= v_grupe[j].nr_grupe)
	{
		aux_po -= v_grupe[j].nr_grupe;
		j++;
	}//j va arata nr orarului, aux_po va contine restul - nr grupei/semigrupei
	/*
	if(aux_po % 2 == 1)
		nr_sgr = 2;
	if(aux_po % 2 == 0)
		nr_sgr = 1;*/
	nr_gr = aux_po + 1;
	//printf("%d-nr de orar %d grupa %d{%d}\n", semi_grupa, j, nr_gr, nr_sgr);
	
	int ora_p = 0;//ora de inceput si sfarsit pentru semigrupa
	int ora_sf = 0;


	//incerc sa bag unde am gauri - incerc sa umplu cate un gol
	for(int z = 0; z < 5; z++)
	{
		//in fctie de paritate - intervalul pe care voi lucra
		if (mod_c == 1) //caut optimizari pe impar
		{
			ora_p = ora_l_start_gr[z][grupa].val_impar;
			ora_sf = ora_l_finis_gr[z][grupa].val_impar;
		}
		if (mod_c == 2)//caut optimizari pe par
		{
			ora_p = ora_l_start_gr[z][grupa].val_par;
			ora_sf = ora_l_finis_gr[z][grupa].val_par;
		}
		if (mod_c == 0)//caut optimizari pe ambele
		{
			//ora de inceput mai mica
			if (ora_l_start_gr[z][grupa].val_impar >= ora_l_start_gr[z][grupa].val_par)
			{
				if(ora_l_start_gr[z][grupa].val_par != 0 )
					ora_p =	ora_l_start_gr[z][grupa].val_par;
				else
					ora_p = ora_l_start_gr[z][grupa].val_impar;
			}
			else
				ora_p = ora_l_start_gr[z][grupa].val_impar;
			//ora de sfarsit mai mare
			if ( ora_l_finis_gr[z][grupa].val_impar <= ora_l_finis_gr[z][grupa].val_par)
				ora_sf = ora_l_finis_gr[z][grupa].val_par;
			else
				ora_sf = ora_l_finis_gr[z][grupa].val_impar;
		}
		// 2 printf("ORAR %d ZI %d grupa %dincepe la %d termina la %d\n", j, z, grupa, ora_p, ora_sf);
		// 3 printf("mod cautare %d\n", mod_c);
		
		for(int o = ora_p; o < ora_sf; o++)
		{
			//am gasit o casuta ocupata
			if( (ORARE_L[j][z][o].nume_materie != NULL) && (apeluri_optimizare_grupe_l < 1500) )
			{
			// 4	printf("casuta ocupata %d zi %d ora %d\n", j, z, o);
				oc = &ORARE_L[j][z][o];
				//nu mut seminariile de MP si BM
				if ( strcmp( oc->nume_materie, "MP") == 0 && j >= 9 && j <=13 )
					goto increment;
				if ( strcmp( oc->nume_materie, "BM") == 0 && j >= 9 && j <=13 )
					goto increment;
				int ex_g;
				while( (oc != NULL && oc->tip != -2) && (apeluri_optimizare_grupe_l < 1500) )
				{
					//nu mut seminariile de MP si BM
					if ( strcmp( oc->nume_materie, "MP") == 0 && j >= 9 && j <=13 )
						goto increment;
					if ( strcmp( oc->nume_materie, "BM") == 0 && j >= 9 && j <=13 )
						goto increment;
					if( ex_g = verif_grupa2(oc, grupa, mod_c) )
					{
						// 5 printf("casuta ocupata zi %dora %d\n",z , o);
						if( oc->tip == 1 || oc->tip == 2 || oc->tip == 3)//daca e lab/sem/proiect
						{
							pot_muta = 0;
							//trebuie sa fie primul sau ultimul lab
							if (o == ora_p || o == (ora_sf - 2) )
								pot_muta = 1;
							else
								goto increment;
							nr_goluri = 0;
							gaseste_goluri_gr(ORARE_L[j], grupa, mod_c, coada_goluri, nr_goluri);
							while( (nr_goluri > 0) && (apeluri_optimizare_grupe_l< 1500) )
							{
								//mutarea laboratorului in ultimul gol
//##############################verific daca se poate face mutare daca e sala libera
								// 6 printf("verific daca am sala libera sa mut\n");
								if( este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s) == 0 
									|| este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, 
											coada_goluri[nr_goluri - 1].s + 1) == 0)
									goto gol_urm;
								// printf("mutare zi%d ora%d in zi_%d ora_%d materia %s grupa %d\n"
								//	, z, o, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s,
								//	oc->nume_materie, grupa);
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s], oc);
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + 1], oc);
								init_ocupare(oc_pi);
								copy( *oc_pi, oc);
								//trebuie sa fac modificari si in lista de sali
								int indice_sala;
								nod_disciplina *pdisc;
								char *serie;
								int cs = 0;//cursorul seriei
								serie = (char *)calloc( 15, sizeof( char ) );
								while( !isdigit( nume_orare[j][cs]) )
									cs++;
								//incepe numele seriei
								int s = 0;
								while( nume_orare[j][cs] != '.')
									serie[s++] = nume_orare[j][cs++];
								pdisc = getat(oc->nume_materie, L_D[j], oc->tip);
								nod_grupa* g;
								g = generare_nume_grupa(j, nr_gr, 0);
								if (oc->tip == 1)
								{	
									indice_sala = getat(Li_lab, oc->sala);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 2)
								{
									indice_sala = getat(Li_sem, oc->sala);
									addsem(buffer_seminar_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 3)
								{							
									indice_sala = getat(Li_p, oc->sala);
									addsem(buffer_p_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								//elimin oc din lista de ocupari la ziua/ora aia din orarul mare
								// 8 printf("elimin %s cu %d orarul %d ziua %d ora %d\n",
								//	oc->nume_materie, ORARE_L[j][z][o].li_grupa.nr_elem, j, z, o);
								//printf("%s\n", ORARE_L[0][4][8].nume_materie);
								//printf("%s\n", ORARE_L[0][4][9].nume_materie);
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][z][o + ol];
									if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][z][o + ol]);
									else
									{
										if( ol == 0)
										{

											//oc2 = &ORARE_L[j][z][o + 1];
											if ( egalitate(*aux, *oc))
												 //pun pe urm elem dupa oc pe prima pozitie
												suprascrie(ORARE_L[j][z][o + ol], oc->pnext);
											else
											{
												while( egalitate( *(aux->pnext), *oc) != 1)
												{
													aux = aux->pnext;
													//oc2 = oc2->pnext;
												}
												if(oc->pnext !=NULL)//daca oc nu este ultimul din lista
													aux->pnext = oc->pnext;
												else
													aux->pnext = NULL;
											}
										}//ol == 0
										if ( ol == 1)
										{
											//oc in loc de oc2
											if( egalitate(*aux, *oc_pi) == 1)
												 //pun pe urm elem dupa oc pe prima pozitie
												 suprascrie(ORARE_L[j][z][o + ol], aux->pnext);
											else
											{
												//while(aux->pnext != oc2)
												while( egalitate( *(aux->pnext), *oc_pi) != 1)
													aux = aux->pnext;
												//aux->pnext arata acum catre oc_pi care trebuie eliminat
												oc2 = aux;
												aux = aux->pnext;//acum aux arata catre oc_pi
												if(aux->pnext != NULL)//daca oc_pi nu este ultimul din lista
													oc2->pnext = aux->pnext;
												else
													oc2->pnext = NULL;
											}
										}//if ol == 1
									}//else
								}// for elimin oc din lista de ocupari la ziua/ora aia
								
								//eliberez in orarul salii la ora aia
								if (oc->tip == 1)
								{
									elibereaza(buffer_lab_l, indice_sala, z, o);
									elibereaza(buffer_lab_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 2)
								{
									elibereaza(buffer_seminar_l, indice_sala, z, o);
									elibereaza(buffer_seminar_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 3)
								{								
									elibereaza(buffer_p_l, indice_sala, z, o);
									elibereaza(buffer_p_l, indice_sala, z, o + 1);
								}
								
								//nr_goluri--;
//###costuri ####				//trebuie reactualizate costurile si valorile orelor de pornire/terminare a orelor
								if( o == ora_p)//mutarea s-a facut de pe prima pozitie
								{
									if(mod_c == 1)
										ora_l_start_gr[z][grupa].val_impar += 2;
									if(mod_c == 2)
										ora_l_start_gr[z][grupa].val_par += 2;
									if(mod_c == 0)
									{
										ora_l_start_gr[z][grupa].val_impar += 2;
										ora_l_start_gr[z][grupa].val_par += 2;
									}
								}
								if( o == ora_sf - 2)//mutarea s-a facut de pe ultima pozitie
								{
									if(mod_c == 1 || mod_c == 0)
										ora_l_finis_gr[z][grupa].val_impar -= 2;
									if(mod_c == 2 || mod_c == 0)
										ora_l_finis_gr[z][grupa].val_par -= 2;
									
								}
								tip_apel = 1;
								//resetez vechile ore de start si finis inainte de calculul costurilor
								if(mod_c == 1)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start_gr[z][grupa].val_impar = 0;
									cost_l_gr[grupa].cost_impar = calc_cost_gr(ORARE_L[j], grupa, ora_l_start_gr, ora_l_finis_gr, mod_c);
								}
								if(mod_c == 2)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start_gr[z][grupa].val_par = 0;
									cost_l_gr[grupa].cost_par = calc_cost_gr(ORARE_L[j], grupa, ora_l_start_gr, ora_l_finis_gr, mod_c);
								}
								if(mod_c == 0)
								{
									for(int z = 0; z < 5; z++)
									{
										ora_l_start_gr[z][grupa].val_impar = 0;
										ora_l_start_gr[z][grupa].val_par = 0;
									}
									cost_l_gr[grupa].cost_impar = calc_cost_gr(ORARE_L[j], grupa, ora_l_start_gr, ora_l_finis_gr, 1);
									cost_l_gr[grupa].cost_par = calc_cost_gr(ORARE_L[j], grupa, ora_l_start_gr, ora_l_finis_gr, 2);
								}
								//calculam costul total 
								cost_t_l_gr = 0;
								for(int i = 0; i < nr_grupe_l; i++)
								{
									cost_t_l_gr += cost_l_gr[i].cost_impar;
									cost_t_l_gr += cost_l_gr[i].cost_par;
								}
								/*for(int i = 0; i < nr_grupe_l; i++)
									{
										int cost_vechi = cost_init_gr[i].cost_impar + cost_init_gr[i].cost_par;
										int cost_lucru = cost_l_gr[i].cost_impar + cost_l_gr[i].cost_par;
										printf("grupa %d costul init//lucru %d--%d\n", i, cost_vechi, cost_lucru);
									}*/
								
								//printf("costul vechi %d, costul NOU %d\n", cost_t_o_gr, cost_t_l_gr );
								if (cost_t_l_gr < cost_t_o_gr)//avem o noua solutie optima
								{
									//printf("AM modificat costul optim: vechi %d, cost OPTIMnou %d -- apel %d\n",
									//	cost_t_o_gr, cost_t_l_gr, apeluri_optimizare_grupe_l );
									cost_t_o_gr = cost_t_l_gr;
									//printf("copiez orarul MARE de lucru in cel final \n");
									
									//initializare_orar(ORARE_F);
									copy_orar(ORARE_F, ORARE_L);
									
									//printf("copiez orarul de lab de lucru in cel final \n");
									eliberare_mem(buffer_lab_f, Li_lab.nr_elem);
									buffer_lab_f = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									verifica_copiere_orar(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									
									//printf("copiez orarul de seminar de lucru in cel final \n");
									eliberare_mem(buffer_seminar_f, Li_sem.nr_elem);
									buffer_seminar_f = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									verifica_copiere_orar(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									
									//printf("copiez orarul de proiect de lucru in cel final \n");
									eliberare_mem(buffer_p_f, Li_p.nr_elem);
									buffer_p_f = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									verifica_copiere_orar(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									
									//printf("copiez orarul de curs de lucru in cel final \n");
									eliberare_mem(buffer_curs_f, Li_curs.nr_elem);
									buffer_curs_f = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
									copy_orar_curs(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);
									verifica_copiere_orar(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);

								}
								
								// 12 printf("apel din nou\n");
								//apel din nou
								int apelare = 0;
								if(apeluri_optimizare_grupe_l < 1500)
								{
									int limit_dr = 0;
									int limit_st = 0;
									if (grupa <= 16)
									{
										limit_st = 0;
										limit_dr = 16 + 4;
									}
									if ( (grupa > 16) && (grupa <= 31) )
									{
										limit_st = 16 - 3;
										limit_dr = 31 + 10;
									}
									if ( (grupa > 31) && (grupa <= 43) )
									{
										limit_st = 31 - 3;
										limit_dr = 43 + 10;
									}
									if ( (grupa > 43) && (grupa <= 60) )
									{
										limit_st = 43 - 5;
										limit_dr = 60;
									}
									for(int i = limit_st; i < limit_dr + 1; i++)
									{
										optimizare_grupe_licenta(ORARE_L, i, mod_c);
										if(apeluri_optimizare_grupe_l > 1499)
											break;
									}
									apelare = 1;
								}

//##############################//pas inapoi
								//adaug la loc in orarul mare
								
								//if(apelare == 1)
									call_stack--;
								//else
								//	if(apeluri_optimizare == 4199)
								//		call_stack--;
								// 13 printf("###AM INTRAT IN PAS INAPOI %d stiva %d\n",
								//	nr_apel_asta, call_stack);
								//exit(EXIT_FAILURE);
								
								
								//exit(EXIT_SUCCESS);
								copy( ORARE_L[j][z][o], oc_pi);
								copy( ORARE_L[j][z][o + 1], oc_pi);
								//adaug seminariile la loc in orarul de sali
								if (oc_pi->tip == 1)
								{	
									addsem(buffer_lab_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 2)
								{
									addsem(buffer_seminar_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_seminar_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 3)
								{							
									addsem(buffer_p_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_p_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								//elimin noile pozitii
								//eliberez in orarul salii la ora aia
								//nr_goluri++;//pentru a pune ziua si ora golului inapoi
								if (oc_pi->tip == 1)
								{
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri -1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 2)
								{
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 3)
								{								
									elibereaza(buffer_p_l, 
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_p_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								//elimin din orarul mare -acum oc_pi de unde l-am mutat inainte tre eliminat la loc
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + ol];
									int ultim_gol = nr_goluri - 1;
									//daca e chiar pe prima pozitie
									if( egalitate(*aux, *oc_pi) == 1)
									{
										if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									}
									//if(aux->pnext == NULL)
									//{	
									//	elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									//}
									else
									{
										
										if(ol == 0)
										{
											//oc2 = &ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + 1];
											while( egalitate( *(aux->pnext), *oc_pi) != 1 )
											{
												aux = aux->pnext;
												//oc2 = oc2->pnext;
											}
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
										if( ol == 1)
										{
											while( egalitate( *(aux->pnext), *oc_pi) != 1)
												aux = aux->pnext;
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
									}//else
								}//for eliminare

								//o++;//fiecare lab/sem/p are 2 ore deci sa nu il mut de 2 ori
gol_urm:						nr_goluri--;
							}//while exista goluri
						}//if lab/sem/p

					}//if exista grupa
increment:			oc = oc->pnext;
					nr_goluri = 0;
					
				}//while oc
				
			}//if casuta
		}//for ore

	}//for zile

					
}


void optimizare2(ORAR ORARE_L[], int grupa, int mod)
//mod - 0 pe ambele paritati, 1 impara 2 para
{
	call_stack++;
	if(apeluri_optimizare_2 >= 1500)
	{
		 printf("IESIRE din optimizare\n");
		call_stack--;
		return;
	}
	// 1 printf("##### OPTIMIZARE GRUPE MASTER %d#####stiva %d\n", apeluri_optimizare_2, call_stack);
	int nr_apel_asta = apeluri_optimizare_2;
	apeluri_optimizare_2++;
		
	int j = 0; //vor retine orarul, grupa
	int nr_gr;
	int mod_c = mod;
	ocupare *oc;
	ocupare *oc2;//pentru o + 1
	ocupare *oc_pi;//pentru pasul inapoi
	oc_pi = new ocupare;
	ocupare *aux;//pentru eliminare
	GOL coada_goluri[20];
	int pot_muta = 0; //daca e la inceputul sau sfarsitul orelor
	int nr_goluri = 0;
	//pentru fiecare grupa o sa varieze de la 0 la nr maxim
	if (grupa >= suma_nr_grupe_l + suma_nr_grupe_m)
		printf("$$ EROARE nr grupe_l");
	if( (cost_l_2[grupa - suma_nr_grupe_l].cost_impar == 0) && (cost_l_2[grupa - suma_nr_grupe_l].cost_par == 0) )
	{
	//	2 printf("%d grupa optima apel %d\n", grupa, apeluri_optimizare_2 - 1);
		return;
	}
	if(cost_l_2[grupa - suma_nr_grupe_l].cost_impar == 0)
		mod_c = 2;//lucrez doar pe para
	if(cost_l_2[grupa - suma_nr_grupe_l].cost_par == 0)
		mod_c = 1; //lucrez pe impara de acuma incolo in aceasta parcurgere
	

	int aux_po = grupa;
	//pentru master
	//while (j <= 13)
	//	while(aux_po >= 2 * v_grupe[j].nr_grupe)
	//	{
	//		aux_po -= 2 * v_grupe[j].nr_grupe;
	//		j++;
	//	}//j va arata nr orarului, aux_po va contine restul - nr grupei/semigrupei
	aux_po -= suma_nr_grupe_l;
	j = 14;
	while(aux_po >=  v_grupe[j].nr_grupe)
		{
			aux_po -= v_grupe[j].nr_grupe;
			j++;
		}

	nr_gr = aux_po + 1;
	// 3 printf("calc %d- nr de orar %d grupa %d\n", grupa, j, nr_gr);
	
	int ora_p = 0;//ora de inceput si sfarsit pentru semigrupa
	int ora_sf = 0;


	//incerc sa bag unde am gauri - incerc sa umplu cate un gol
	for(int z = 0; z < 5; z++)
	{
		//in fctie de paritate - intervalul pe care voi lucra
		if (mod_c == 1) //caut optimizari pe impar
		{
			ora_p = ora_l_start[z][grupa].val_impar;
			ora_sf = ora_l_finis[z][grupa].val_impar;
		}
		if (mod_c == 2)//caut optimizari pe par
		{
			ora_p = ora_l_start[z][grupa].val_par;
			ora_sf = ora_l_finis[z][grupa].val_par;
		}
		if (mod_c == 0)//caut optimizari pe ambele
		{
			//ora de inceput mai mica
			if (ora_l_start[z][grupa].val_impar >= ora_l_start[z][grupa].val_par)
			{
				if(ora_l_start[z][grupa].val_par != 0 )
					ora_p =	ora_l_start[z][grupa].val_par;
				else
					ora_p = ora_l_start[z][grupa].val_impar;
			}
			else
				ora_p = ora_l_start[z][grupa].val_impar;
			//ora de sfarsit mai mare
			if ( ora_l_finis[z][grupa].val_impar <= ora_l_finis[z][grupa].val_par)
				ora_sf = ora_l_finis[z][grupa].val_par;
			else
				ora_sf = ora_l_finis[z][grupa].val_impar;
		}
		// 4 printf("ORAR %d ZI %d grupa %dincepe la %d termina la %d\n", j, z, grupa, ora_p, ora_sf);
		// 5 printf("mod cautare %d\n", mod_c);
		
		for(int o = ora_p; o < ora_sf; o++)
		{
			//am gasit o casuta ocupata
			if( (ORARE_L[j][z][o].nume_materie != NULL) && (apeluri_optimizare_2 < 1500) )
			{
				// 6 printf("casuta ocupata %d zi %d ora %d\n", j, z, o);
				oc = &ORARE_L[j][z][o];
				
				int ex_g;
				while( (oc != NULL && oc->tip != -2) && (apeluri_optimizare_2 < 1500) )
				{
										
					if( ex_g = verif_grupa_master(oc, grupa, mod_c) )
					{
						// 7 printf("casuta ocupata zi %dora %d\n",z , o);
						if( oc->tip == 1 || oc->tip == 2 || oc->tip == 3)//daca e lab/sem/proiect
						{
							pot_muta = 0;
							//trebuie sa fie primul sau ultimul lab
							if (o == ora_p || o == (ora_sf - 2) )
								pot_muta = 1;
							else
								goto increment;
							nr_goluri = 0;
							gaseste_goluri_2(ORARE_L[j], grupa, mod_c, coada_goluri, nr_goluri);
							while( (nr_goluri > 0) && (apeluri_optimizare_2< 1500) )
							{
								//mutarea laboratorului in ultimul gol
//##############################verific daca se poate face mutare daca e sala libera
								// 8 printf("verific daca am sala libera sa mut\n");
								if( este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s) == 0 
									|| este_liber(oc->sala, coada_goluri[nr_goluri - 1].z, 
											coada_goluri[nr_goluri - 1].s + 1) == 0)
									goto gol_urm;
								//verific daca am mai multe semigrupe, tre sa fie libere si acele grupe
								if(oc->li_grupa.nr_elem > 1)
								{
									nod_grupa *ngr;
									ngr = oc->li_grupa.pfirst;
									while( ngr != NULL)
									{
										if (este_liber(ORARE_L[j], ngr->nume, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s) == 0
											|| este_liber(ORARE_L[j], ngr->nume, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1) == 0)
											goto gol_urm;
										ngr = ngr->pnext;
									}
								}
								// 8 printf("mutare zi%d ora%d in zi_%d ora_%d materia %s grupa %d\n"
								//	, z, o, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s,
								//	oc->nume_materie, grupa);
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s], oc);
								copy( ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + 1], oc);
								init_ocupare(oc_pi);
								copy( *oc_pi, oc);
								//trebuie sa fac modificari si in lista de sali
								int indice_sala;
								nod_disciplina *pdisc;
								char *serie;
								int cs = 0;//cursorul seriei
								serie = (char *)calloc( 15, sizeof( char ) );
								while( !isdigit( nume_orare[j][cs]) )
									cs++;
								//incepe numele seriei
								int s = 0;
								while( nume_orare[j][cs] != '.')
									serie[s++] = nume_orare[j][cs++];
								pdisc = getat(oc->nume_materie, L_D[j], oc->tip);
								nod_grupa* g;
								g = generare_nume_grupa(j, nr_gr, 0);
								if (oc->tip == 1)
								{	
									indice_sala = getat(Li_lab, oc->sala);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 2)
								{
									indice_sala = getat(Li_sem, oc->sala);
									addsem(buffer_seminar_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								if (oc->tip == 3)
								{							
									indice_sala = getat(Li_p, oc->sala);
									addsem(buffer_p_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s, pdisc, oc->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, coada_goluri[nr_goluri - 1].z,
										coada_goluri[nr_goluri - 1].s + 1, pdisc, oc->tip, serie, g);
								}
								//elimin oc din lista de ocupari la ziua/ora aia din orarul mare
								// 10 printf("elimin %s cu %d orarul %d ziua %d ora %d\n",
								//	oc->nume_materie, oc->li_grupa.nr_elem, j, z, o);
								//printf("%s\n", ORARE_L[0][4][8].nume_materie);
								//printf("%s\n", ORARE_L[0][4][9].nume_materie);
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][z][o + ol];
									if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][z][o + ol]);
									else
									{
										if( ol == 0)
										{

											//oc2 = &ORARE_L[j][z][o + 1];
											if ( egalitate(*aux, *oc))
												 //pun pe urm elem dupa oc pe prima pozitie
												suprascrie(ORARE_L[j][z][o + ol], oc->pnext);
											else
											{
												while( egalitate( *(aux->pnext), *oc) != 1)
												{
													aux = aux->pnext;
													//oc2 = oc2->pnext;
												}
												if(oc->pnext !=NULL)//daca oc nu este ultimul din lista
													aux->pnext = oc->pnext;
												else
													aux->pnext = NULL;
											}
										}//ol == 0
										if ( ol == 1)
										{
											//oc in loc de oc2
											if( egalitate(*aux, *oc_pi) == 1)
												 //pun pe urm elem dupa oc pe prima pozitie
												 suprascrie(ORARE_L[j][z][o + ol], aux->pnext);
											else
											{
												//while(aux->pnext != oc2)
												while( egalitate( *(aux->pnext), *oc_pi) != 1)
													aux = aux->pnext;
												//aux->pnext arata acum catre oc_pi care trebuie eliminat
												oc2 = aux;
												aux = aux->pnext;//acum aux arata catre oc_pi
												if(aux->pnext != NULL)//daca oc_pi nu este ultimul din lista
													oc2->pnext = aux->pnext;
												else
													oc2->pnext = NULL;
											}
										}//if ol == 1
									}//else
								}// for elimin oc din lista de ocupari la ziua/ora aia
								
								//eliberez in orarul salii la ora aia
								if (oc->tip == 1)
								{
									elibereaza(buffer_lab_l, indice_sala, z, o);
									elibereaza(buffer_lab_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 2)
								{
									elibereaza(buffer_seminar_l, indice_sala, z, o);
									elibereaza(buffer_seminar_l, indice_sala, z, o + 1);
								}
								if (oc->tip == 3)
								{								
									elibereaza(buffer_p_l, indice_sala, z, o);
									elibereaza(buffer_p_l, indice_sala, z, o + 1);
								}
								
								//nr_goluri--;
//###costuri ####				//trebuie reactualizate costurile si valorile orelor de pornire/terminare a orelor
								if( o == ora_p)//mutarea s-a facut de pe prima pozitie
								{
									if(mod_c == 1)
										ora_l_start[z][grupa].val_impar += 2;
									if(mod_c == 2)
										ora_l_start[z][grupa].val_par += 2;
									if(mod_c == 0)
									{
										ora_l_start[z][grupa].val_impar += 2;
										ora_l_start[z][grupa].val_par += 2;
									}
								}
								if( o == ora_sf - 2)//mutarea s-a facut de pe ultima pozitie
								{
									if(mod_c == 1 || mod_c == 0)
										ora_l_finis[z][grupa].val_impar -= 2;
									if(mod_c == 2 || mod_c == 0)
										ora_l_finis[z][grupa].val_par -= 2;
									
								}
								tip_apel = 1;
								//resetez vechile ore de start si finis inainte de calculul costurilor
								int lim_s = 0;//limitele in cazul in care am mai multe grupe la care modific 
								int lim_d = 0;//costurile
								if(oc_pi->li_grupa.nr_elem > 1)
								{
									lim_s = grupa - oc->li_grupa.nr_elem;// pentru ca sunt 3 grupe pe orarele de master
									//vad daca limita stanga este in acelasi orar
									if(lim_s < grupa - aux_po )//mai mic ca prima grupa din orar
										lim_s = grupa - aux_po;
									lim_d = grupa + oc_pi->li_grupa.nr_elem;
									//vad daca limita dreapta este in acelasi orar
									if(lim_d > grupa - aux_po + v_grupe[j].nr_grupe - 1)
										lim_d = grupa - aux_po + v_grupe[j].nr_grupe - 1;
								}
								if(mod_c == 1)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start[z][grupa].val_impar = 0;
									if(oc_pi->li_grupa.nr_elem == 1)
										cost_l_2[grupa - suma_nr_grupe_l].cost_impar = calc_cost_2(ORARE_L[j], grupa, ora_l_start, ora_l_finis, mod_c);
									if(oc_pi->li_grupa.nr_elem > 1)
										for(int i = lim_s; i < lim_d; i++)
										{
											for(int z = 0; z < 5; z++)
												ora_l_start[z][i].val_impar = 0;
											cost_l_2[i - suma_nr_grupe_l].cost_impar = calc_cost_2(ORARE_L[j], i, ora_l_start, ora_l_finis, mod_c);
										}
									
								}
								if(mod_c == 2)
								{
									for(int z = 0; z < 5; z++)
										ora_l_start[z][grupa].val_par = 0;
									if(oc_pi->li_grupa.nr_elem == 1)
										cost_l_2[grupa - suma_nr_grupe_l].cost_par = calc_cost_2(ORARE_L[j], grupa, ora_l_start, ora_l_finis, mod_c);
									if(oc_pi->li_grupa.nr_elem > 1)
										for(int i = lim_s; i < lim_d; i++)
										{
											for(int z = 0; z < 5; z++)
												ora_l_start[z][i].val_par = 0;
											cost_l_2[i - suma_nr_grupe_l].cost_par = calc_cost_2(ORARE_L[j], i, ora_l_start, ora_l_finis, mod_c);
										}
								}
								if(mod_c == 0)
								{
									for(int z = 0; z < 5; z++)
									{
										ora_l_start[z][grupa].val_impar = 0;
										ora_l_start[z][grupa].val_par = 0;
									}
									if(oc_pi->li_grupa.nr_elem == 1)
									{
										cost_l_2[grupa - suma_nr_grupe_l].cost_impar = calc_cost_2(ORARE_L[j], grupa, ora_l_start, ora_l_finis, 1);
										cost_l_2[grupa - suma_nr_grupe_l].cost_par = calc_cost_2(ORARE_L[j], grupa, ora_l_start, ora_l_finis, 2);
									}
									if(oc_pi->li_grupa.nr_elem > 1)
										for(int i = lim_s; i <= lim_d; i++)
										{	for(int z = 0; z < 5; z++)
											{
												ora_l_start[z][i].val_impar = 0;
												ora_l_start[z][i].val_par = 0;
											}
											cost_l_2[i - suma_nr_grupe_l].cost_impar = calc_cost_2(ORARE_L[j], i, ora_l_start, ora_l_finis, 1);
											cost_l_2[i - suma_nr_grupe_l].cost_par = calc_cost_2(ORARE_L[j], i, ora_l_start, ora_l_finis, 2);

										}
								}
								//calculam costul total 
								cost_t_l_2 = 0;
								for(int i = 0; i < suma_nr_grupe_m; i++)
								{
									cost_t_l_2 += cost_l_2[i].cost_impar;
									cost_t_l_2 += cost_l_2[i].cost_par;
								}
								/*for(int i = 0; i < nr_grupe_l; i++)
									{
										int cost_vechi = cost_init_gr[i].cost_impar + cost_init_gr[i].cost_par;
										int cost_lucru = cost_l_gr[i].cost_impar + cost_l_gr[i].cost_par;
										printf("grupa %d costul init//lucru %d--%d\n", i, cost_vechi, cost_lucru);
									}*/
								
								//printf("costul vechi %d, costul NOU %d\n", cost_t_o_gr, cost_t_l_gr );
								if (cost_t_l_2 < cost_t_o_2)//avem o noua solutie optima
								{
								// 10 printf("AM modificat costul optim: vechi %d, cost OPTIMnou %d apel --%d\n",
								//		cost_t_o_2, cost_t_l_2, apeluri_optimizare_2);
									cost_t_o_2 = cost_t_l_2;
									//printf("copiez orarul MARE de lucru in cel final \n");
									
									initializare_orar(ORARE_F);
									copy_orar(ORARE_F, ORARE_L);
									
									//printf("copiez orarul de lab de lucru in cel final \n");
									free(buffer_lab_f);
									buffer_lab_f = (ORAR_LAB *)calloc( Li_lab.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									verifica_copiere_orar(buffer_lab_f, buffer_lab_l, Li_lab.nr_elem);
									
									//printf("copiez orarul de seminar de lucru in cel final \n");
									free(buffer_seminar_f);
									buffer_seminar_f = (ORAR_LAB *)calloc( Li_sem.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									verifica_copiere_orar(buffer_seminar_f, buffer_seminar_l, Li_sem.nr_elem);
									
									//printf("copiez orarul de proiect de lucru in cel final \n");
									free(buffer_p_f);
									buffer_p_f = (ORAR_LAB *)calloc( Li_p.nr_elem, sizeof( ORAR_LAB ) );
									copy_orar_lab(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									verifica_copiere_orar(buffer_p_f, buffer_p_l, Li_p.nr_elem);
									
									//printf("copiez orarul de curs de lucru in cel final \n");
									free(buffer_curs_f);
									buffer_curs_f = (ORAR_CURS *)calloc( Li_curs.nr_elem, sizeof( ORAR_CURS ) );
									copy_orar_curs(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);
									verifica_copiere_orar(buffer_curs_f, buffer_curs_l, Li_curs.nr_elem);

								}
								
								// 13 printf("apel din nou\n");
								//apel din nou
								int apelare = 0;
								if(apeluri_optimizare_2 < 1500)
								{
									int limit_dr = 0;
									int limit_st = 0;
									if (grupa <= 140)
									{
										limit_st = 122;
										limit_dr = 140 + 30;
									}
									if ( (grupa > 140) && (grupa <= 155) )
									{
										limit_st = 140 - 18;
										limit_dr = 155 + 30;
									}
									if ( (grupa > 155) && (grupa <= 170) )
									{
										limit_st = 155 - 30;
										limit_dr = 170 + 15;
									}
									if ( (grupa > 170) && (grupa <= 185) )
									{
										limit_st = 170 - 30;
										limit_dr = 185;
									}
									for(int i = limit_st; i < limit_dr + 1; i++)
									{
										optimizare2(ORARE_L, i, mod_c);
										if(apeluri_optimizare_2 > 1499)
											break;
									}
									apelare = 1;
								}

//##############################//pas inapoi
								//adaug la loc in orarul mare
								
								//if(apelare == 1)
									call_stack--;
								//else
								//	if(apeluri_optimizare == 4199)
								//		call_stack--;
								// 13 printf("###AM INTRAT IN PAS INAPOI %d stiva %d\n",
								//	nr_apel_asta, call_stack);
								//exit(EXIT_FAILURE);
								
								
								//exit(EXIT_SUCCESS);
								copy( ORARE_L[j][z][o], oc_pi);
								copy( ORARE_L[j][z][o + 1], oc_pi);
								//adaug seminariile la loc in orarul de sali
								if (oc_pi->tip == 1)
								{	
									addsem(buffer_lab_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_lab_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 2)
								{
									addsem(buffer_seminar_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_seminar_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								if (oc_pi->tip == 3)
								{							
									addsem(buffer_p_l, indice_sala - 1, z, o, pdisc, oc_pi->tip, serie, g);
									addsem(buffer_p_l, indice_sala - 1, z, o + 1, pdisc, oc_pi->tip, serie, g);
								}
								//elimin noile pozitii
								//eliberez in orarul salii la ora aia
								//nr_goluri++;//pentru a pune ziua si ora golului inapoi
								if (oc_pi->tip == 1)
								{
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_lab_l,
										indice_sala, coada_goluri[nr_goluri -1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 2)
								{
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_seminar_l,
										indice_sala - 1, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								if (oc_pi->tip == 3)
								{								
									elibereaza(buffer_p_l, 
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s);
									elibereaza(buffer_p_l,
										indice_sala, coada_goluri[nr_goluri - 1].z, coada_goluri[nr_goluri - 1].s + 1);
								}
								//elimin din orarul mare -acum oc_pi de unde l-am mutat inainte tre eliminat la loc
								for(int ol = 0; ol < 2; ol++)
								{
									aux = &ORARE_L[j][coada_goluri[nr_goluri - 1].z][coada_goluri[nr_goluri - 1].s + ol];
									int ultim_gol = nr_goluri - 1;
									//daca e chiar pe prima pozitie
									if( egalitate(*aux, *oc_pi) == 1)
									{
										if(aux->pnext == NULL)
										elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									}
									//if(aux->pnext == NULL)
									//{	
									//	elibereaza(ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + ol]);
									//}
									else
									{
										
										if(ol == 0)
										{
											//oc2 = &ORARE_L[j][coada_goluri[ultim_gol].z][coada_goluri[ultim_gol].s + 1];
											while( egalitate( *(aux->pnext), *oc_pi) != 1 )
											{
												aux = aux->pnext;
												//oc2 = oc2->pnext;
											}
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
										if( ol == 1)
										{
											while( egalitate( *(aux->pnext), *oc_pi) != 1)
												aux = aux->pnext;
											if(oc_pi->pnext !=NULL)
												aux->pnext = oc_pi->pnext;
											else
												aux->pnext = NULL;
										}
									}//else
								}//for eliminare

								//o++;//fiecare lab/sem/p are 2 ore deci sa nu il mut de 2 ori
gol_urm:						nr_goluri--;
							}//while exista goluri
						}//if lab/sem/p

					}//if exista grupa
increment:			oc = oc->pnext;
					nr_goluri = 0;
					
				}//while oc
				
			}//if casuta
		}//for ore

	}//for zile

					
}