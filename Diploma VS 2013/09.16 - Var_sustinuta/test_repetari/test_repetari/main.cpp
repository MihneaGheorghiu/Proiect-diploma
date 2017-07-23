#include<stdio.h>
#include<malloc.h>
#include<string.h>

void main()
{
	FILE *f_lab, *f_curs, *f_sem, *f_p;
	f_lab = fopen("sali_lab.in", "r");
	f_curs = fopen("sali_curs.in", "r");
	f_sem = fopen("sali_seminar.in", "r");
	f_p = fopen("sali_p.in", "r");
	char *pagina_lab[100];
	char *pagina_curs[100];
	char *pagina_sem[100];
	char *pagina_p[100];
	int nr_lab, nr_curs, nr_sem, nr_p;
	nr_lab = nr_curs = nr_sem = nr_p = 0;

	printf("######## Program de analizare fisiere de intrare ##############\n");

	for(int i = 0; i < 100; i++)
	{
		pagina_lab[i] = (char*)calloc( 10, sizeof(char) );
		pagina_curs[i] = (char*)calloc( 10, sizeof(char) );
		pagina_sem[i] = (char*)calloc( 10, sizeof(char) );
		pagina_p[i] = (char*)calloc( 10, sizeof(char) );

	}
	char line[100];
	int i = 0;
	while(fgets(line, 100, f_lab))
	{
		fgets(line, 100, f_lab);//scap de index si trec la nume sala
		strcpy(pagina_lab[i], line);
		//printf("%s", pagina_lab[i]);
		nr_lab++;
		i++;
	}

	i = 0;
	while(fgets(line, 100, f_curs))
	{
		fgets(line, 100, f_curs);//scap de index si trec la nume sala
		strcpy(pagina_curs[i], line);
		//printf("%s", pagina_curs[i]);
		nr_curs++;
		i++;
	}

	i = 0;
	while(fgets(line, 100, f_sem))
	{
		fgets(line, 100, f_sem);//scap de index si trec la nume sala
		strcpy(pagina_sem[i], line);
		//printf("%s", pagina_sem[i]);
		nr_sem++;
		i++;
	}

	i = 0;
	while(fgets(line, 100, f_p))
	{
		fgets(line, 100, f_p);//scap de index si trec la nume sala
		strcpy(pagina_p[i], line);
		//printf("%s", pagina_p[i]);
		nr_p++;
		i++;
	}
	
/////comparare daca se repeta vreun nume de sala in acelasi fisier
	
	for(int i = 0; i < nr_curs; i++)
		for(int j = i + 1; j < nr_curs; j++)
			if( strcmp(pagina_curs[i], pagina_curs[j]) == 0)
				printf("se repeta sala %s\n", pagina_curs[i]);

	for(int i = 0; i < nr_sem; i++)
		for(int j = i + 1; j < nr_sem; j++)
			if( strcmp(pagina_sem[i], pagina_sem[j]) == 0)
				printf("se repeta sala %s\n", pagina_sem[i]);

	for(int i = 0; i < nr_p; i++)
		for(int j = i + 1; j < nr_p; j++)
			if( strcmp(pagina_p[i], pagina_p[j]) == 0)
				printf("se repeta sala %s\n", pagina_p[i]);
	
	for(int i = 0; i < nr_lab; i++)
		for(int j = i + 1; j < nr_lab; j++)
			if( strcmp(pagina_lab[i], pagina_lab[j]) == 0)
				printf("se repeta sala %s\n", pagina_lab[i]);
	

///comparare daca se repeta ceva in toate cele 4 fisiere
	//compar sali de lab cu restul
	 for(int i = 0; i < nr_lab; i++)
	 {
		 for(int j = 0; j <nr_curs; j++)
			 if(strcmp(pagina_lab[i], pagina_curs[j]) == 0)
				 printf("se repeta sala de lab %d cu sala de curs %d %s\n", i, j, pagina_lab[i]);
		 for(int j = 0; j <nr_sem; j++)
			 if(strcmp(pagina_lab[i], pagina_sem[j]) == 0)
				 printf("se repeta sala de lab %d cu sala de seminar %d %s\n",
				 i, j, pagina_lab[i]);
		 for(int j = 0; j <nr_p; j++)
			 if(strcmp(pagina_lab[i], pagina_p[j]) == 0)
				 printf("se repeta sala de lab %d cu sala de proiect %d %s\n",
				 i, j, pagina_lab[i]);
	 }
	 //compara sali de seminar cu restul -curs si p
	 for(int i = 0; i < nr_sem; i++)
	 {
		for(int j = 0; j < nr_curs; j++)
			if(strcmp(pagina_sem[i], pagina_curs[j]) == 0)
				printf("se repeta sala de seminar %d cu sala de curs %d %s\n",
				i, j, pagina_sem[i]);
		for(int j = 0; j < nr_p; j++)
			if(strcmp(pagina_sem[i], pagina_p[j]) == 0)
				printf("se repeta sala de seminar %d cu sala de proiect %d %s\n",
				 i, j, pagina_sem[i]);
	} 
	//compara sali de curs cu p
	for(int i = 0; i < nr_curs; i++)
		for(int j = 0; j < nr_p; j++)
			if(strcmp(pagina_curs[i], pagina_p[j]) == 0)
				printf("se repeta sala de curs %d cu sala de proiect %d %s\n",
				 i, j, pagina_curs[i]);
	
}