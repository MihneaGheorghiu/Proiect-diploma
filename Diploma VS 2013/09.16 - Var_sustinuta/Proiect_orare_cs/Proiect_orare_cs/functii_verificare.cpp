#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include "header1.h"

//buffer 2 a fost sursa copierii, buffer 1 destinatia
void verifica_copiere_orar(ORAR_LAB *buffer_1, ORAR_LAB *buffer_2, int nr_elem)
{
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				if(buffer_2[i][z][o].nume != NULL)
				{
					if( strcmp(buffer_1[i][z][o].nume, buffer_2[i][z][o].nume) != 0)
					{
						printf("FCTIA NU MERGE BINE #1\n");
						exit(EXIT_FAILURE);
					}
					if( buffer_1[i][z][o].paritate != buffer_2[i][z][o].paritate)
					{
						printf("FCTIA NU MERGE BINE #2\n");
						exit(EXIT_FAILURE);
					}
					//verificat grupele
					nod_grupa *aux1, *aux2;
					aux1 = buffer_1[i][z][o].li_grupa.pfirst;
					aux2 = buffer_2[i][z][o].li_grupa.pfirst;
					while(aux2 != NULL)
					{
						if( strcmp(aux1->nume, aux2->nume) != 0 )
						{
							printf("FCTIA NU MERGE BINE #3\n");
							exit(EXIT_FAILURE);
						}
						aux1 = aux1->pnext;
						aux2 = aux2->pnext;
					}
					if( buffer_2[i][z][o].pnext != NULL)
					{
						oc_lab *nod1, *nod2;
						nod2 = &buffer_2[i][z][o];
						nod1 = &buffer_1[i][z][o];
						while(nod2 != NULL)
						{
							if(strcmp(nod1->nume, nod2->nume) != 0 )
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							if(nod1->paritate != nod2->paritate)
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							//verificat grupele
							nod_grupa *aux1, *aux2;
							aux1 = nod1->li_grupa.pfirst;
							aux2 = nod2->li_grupa.pfirst;
							while(aux2 != NULL)
							{
								if( strcmp(aux1->nume, aux2->nume) != 0 )
								{
									printf("FCTIA NU MERGE BINE (NODURI) %d %d %d\n", i, z, o);
									printf("aux_dest->%s aux->sursa %s \n", aux1->nume, aux2->nume);
									exit(EXIT_FAILURE);
								}
								aux1 = aux1->pnext;
								aux2 = aux2->pnext;
							}
							nod2 = nod2->pnext;
							nod1 = nod1->pnext;
						}//while nod2
					}
				}//if casuta plina
			}//for-uri
}

//buffer 2 a fost sursa copierii, buffer 1 destinatia
void verifica_copiere_orar(ORAR_CURS *buffer_1, ORAR_CURS *buffer_2, int nr_elem)
{
	for(int i = 0; i < nr_elem; i++)
		for(int z = 0; z < 5; z++)
			for(int o = 8; o < 22; o++)
			{
				if(buffer_2[i][z][o].nume_materie != NULL)
				{
					if( strcmp(buffer_1[i][z][o].nume_materie, buffer_2[i][z][o].nume_materie) != 0)
					{
						printf("FCTIA NU MERGE BINE #C1\n");
						exit(EXIT_FAILURE);
					}
					if( strcmp(buffer_1[i][z][o].acronim, buffer_2[i][z][o].acronim) != 0)
					{
						printf("FCTIA NU MERGE BINE #C2\n");
						exit(EXIT_FAILURE);
					}
					if( strcmp(buffer_1[i][z][o].serie, buffer_2[i][z][o].serie) != 0)
					{
						printf("FCTIA NU MERGE BINE #C3\n");
						printf("orar %d zi %d ora %d serie1 %s serie2 %s \n",
							i, z, o, buffer_1[i][z][o].serie, buffer_2[i][z][o].serie);
						exit(EXIT_FAILURE);
					}
					if( buffer_1[i][z][o].paritate != buffer_2[i][z][o].paritate)
					{
						printf("FCTIA NU MERGE BINE #C4\n");
						exit(EXIT_FAILURE);
					}
					

					if( buffer_2[i][z][o].pnext != NULL)
					{
						oc_curs *nod1, *nod2;
						nod2 = &buffer_2[i][z][o];
						nod1 = &buffer_1[i][z][o];
						while(nod2 != NULL)
						{
							if(strcmp(nod1->nume_materie, nod2->nume_materie) != 0 )
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							if(strcmp(nod1->acronim, nod2->acronim) != 0 )
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							if(strcmp(nod1->serie, nod2->serie) != 0 )
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							if(nod1->paritate != nod2->paritate)
							{
								printf("FCTIA NU MERGE BINE (NODURI)\n");
								exit(EXIT_FAILURE);
							}
							
							nod2 = nod2->pnext;
							nod1 = nod1->pnext;
						}//while nod2
					}
				}//if casuta plina
			}//for-uri
}