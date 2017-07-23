//pot baga #define sau tipuri de date
#define NR_ORARE 35
//#define NR_ORARE2 38
//#define NR_SALI_CURS 21
//#define NR_SALI_LAB 12
//#define NR_SALI_SEM 10 
//#define NR_SALI_P 5
//#define NR_SALI 36
#define LUNGIME_NUME 100
#define ANI_LIC 14 //numarul de serii de licenta
#define ANI_MASTER 21 //numarul de orare pe serii la master

//grupe
typedef struct gr
{
	char *nume;
	int nr_grupa;
	struct gr *pnext;
} nod_grupa, *pnod_grupa;

typedef struct lgr
{
	int nr_elem;
	nod_grupa *pfirst;
	nod_grupa *plast;
} lista_grupe;

//orar o sa  fie o matrice[5][22] cu zile/ore de struct
typedef struct oc
{
	int index_titular;
	char* sala;
	char *nume_materie;
	lista_grupe li_grupa;
	int paritate;
	int semigrupa;
	int afisat;//la printare, il trec o singura data
	int tip; //0 curs, 1 lab, 2 seminar, 3 proiect
	struct oc* pnext;//pentru a crea o mica lista inlantuita cu 2 elemente in caz de suprapunere
} ocupare, *pocupare;

typedef ocupare ORAR[5][22];

//orar profesori
typedef struct oc1
{
	char *nume_materie;
	char *serie;
	int index_sala;
	lista_grupe li_grupa;
	int paritate;
	struct oc1 *pnext;//pentru suprapunere
} oc_profesor;
typedef oc_profesor ORAR_PF[5][22];

//orar pe sali de laborator/proiect/seminar
typedef struct oc2
{
	char *nume;
	char serie[15];
	lista_grupe li_grupa;
	int paritate;
	struct oc2* pnext; //suprapunere la paritate
} oc_lab;
typedef oc_lab ORAR_LAB[5][22];


//orar pe sali de curs
typedef struct oc4
{
	char *nume_materie;
	char *acronim;
	char *serie;
	int paritate;
	struct oc4 *pnext;//pentru suprapunere
	lista_grupe li_grupa;
} oc_curs;
typedef oc_curs ORAR_CURS[5][22];


//lista inlantuita profesori
typedef struct nodt
{
	char *nume;
	int index;
	struct nodt *pnext;
} nod_titulari, *pnod_titulari;

typedef struct li
{
	int nr; //nr de intrari
	nod_titulari *pfirst;
	nod_titulari *plast;//pentru adaugare pe ultima pozitie
} lista_titulari;

//lista de sali 
typedef struct ns
{
	int index;
	char *nume;
	struct ns *pnext;
} nod_sali, *pnod_sali;

typedef struct lns
{
	int nr_elem;
	nod_sali *pfirst;
	nod_sali *plast; //pentru adaugare pe ultima pozitie
} lista_sali;

//structura disciplina
typedef struct disc
{
	int index;
	char *nume;//nume disciplina
	char *acronim;//acronim
	char *an;//anul in care se preda
	int sem;//semestru
	int tip[4];//tip curs/laborator/seminar/ proiect
	int durata[4];
	int paritate[4]; //0 - normal, 1-impar, 2-par, 3- in una din ele dep de grupe
	nod_titulari *titular;//titular
	int index_titular;
	int nr_lab; //daca are lab care poate fi pe semigrupe se pune un '2'
	struct disc *pnext;
} nod_disciplina, *pnod_disciplina;

typedef struct ld
{
	int nr;
	nod_disciplina *pfirst;
	nod_disciplina *plast;
} lista_discipline;

//structuri pentru nr de grupe pe an
typedef struct n_r
{
	int nr_grupe;
	int nr_optionale;
} numar_grupe;

//structuri pentru retinerea costurilor si a inceputului/sf de orar pe paritati

typedef struct cst
{
	int cost_par;
	int cost_impar;
} COST;

typedef struct val_z
{
	int val_par;
	int val_impar;
} val_ore;

typedef struct ggol
{
	int s;//ora start
	int f;//ora final?
	int z;//zi
} GOL;

//fctii din main
lista_grupe generare_nume_grupe(int i);

//fctii liste
void addlist(lista_titulari &Li, int index, char name[]);
void addlist(lista_sali &Li, int index, char name[]);
void addlist(lista_discipline &li, int index, char nume[], char acronim[], int tip[], 
			 int durata[], int paritate[], int index_titular);
void addlist(lista_grupe &li, char* nume);

nod_titulari* getat(lista_titulari li, int index);
nod_disciplina* getat(char *cuv, lista_discipline Li, int tip);
int getat(lista_titulari li, char *nume_cautat);
int getat(lista_sali li, char *nume_cautat);

//fctii addaugare in orar
void addcurs(ORAR ORAREV[], int i, int zi, int ora, nod_disciplina* pdisc, char* nume_sala,
			 int nr_grupe, int nr_optionale, int paritate, lista_grupe li_gr);
void addcurs(ORAR_PF buffer[], int indice_t, int zi, int ora, char* nume_materie, 
			 int index_sala, int durata, int paritate, char* serie);
void addcurs(ORAR_CURS buffer_curs[], int indice_sala, int zi, int ora,
			 nod_disciplina *pdisc, int paritate, char* serie);

void addsem(ORAR ORAREV[], int i ,int zi, int ora, int tip, 
			char* acronim, int durata, int paritate, char* nume_sala, nod_grupa* g);
void addsem(ORAR_LAB buffer[], int indice_sala, int zi, int ora,
			nod_disciplina * pdisc, int tip, char* serie, nod_grupa * g);

//fctii citire fis intrare/initializare
void citire_nume_orare(char* nume_orare[]);
void citire_titulari(lista_titulari &Li);
void citire_sali(lista_sali &Li, char fisier[]);
void citire_discipline(lista_discipline L_D[], char* nume_orare[]);
void citire_nr_grupe(numar_grupe v_grupe[]);
void initializare_orar(ORAR ORAREV[]);

//fctii auxiliare
void init_lista_grupe(lista_grupe &li);
void init_ocupare(ocupare *oc);
void setare_semestru(lista_discipline L_D[], int sem);
void setare_grupelab(lista_discipline L_D[]);
int exista_grupa(ocupare poc, int j, int i);
void copy_orar(ORAR ORARE_L[], ORAR ORAREV[]);
void eliberare_mem(ORAR_LAB *buffer, int nr_elem);
void eliberare_mem(ORAR_CURS *buffer, int nr_elem);
void eliberare_mem(ORAR ORARED[]);

//fctii pentru orare
nod_grupa* generare_nume_grupa(int i, int nr, int semigrupa);
void afisare_orar(ORAR VO[], char* nume_orare[], numar_grupe v_grupe[], lista_titulari Li_titulari);
void afisare_orar(ORAR_PF buffer_titulari[], lista_titulari Li_titulari);
void afisare_orar(lista_sali Li_sali, ORAR VO[], char *tip);
void copy(ocupare &o_dest, ocupare *oc);
void suprascrie(ocupare &o_dest, ocupare *oc);
int egalitate(ocupare oc1, ocupare oc2);

//fctii legate de optimizarea orarelor
int calc_cost_1(ORAR O, int semi_grupa, val_ore *ora_start[], val_ore *ora_finis[], int paritate);
int calc_cost_gr(ORAR O, int grupa, val_ore *ora_start_gr[], val_ore *ora_finis_gr[], int paritate);
int calc_cost_2(ORAR O, int grupa, val_ore *ora_start[], val_ore *ora_finis[], int paritate);
int exista_semigrupa(ocupare *oc, int semi_grupa, int paritate);
int exista_doar_semigrupa(ocupare *oc, int semi_grupa, int paritate);
int exista_grupa(ocupare *oc, int grupa, int paritate);
int verif_grupa(ocupare *oc, int grupa, int paritate);
int verif_grupa2(ocupare *oc, int grupa, int paritate);
int verif_grupa_master(ocupare *oc, int grupa, int paritate);
void test_extern();
void optimizare_1(ORAR ORARE_L[], int semi_grupa, int mod);
void optimizare_grupe_licenta(ORAR ORARE_L[], int grupa, int mod);
void optimizare2(ORAR ORARE_L[], int grupa, int mod);
void gaseste_goluri(ORAR O, int semi_grupa, int paritate, GOL coada_goluri[], int &nr_goluri);
void gaseste_goluri_gr(ORAR O, int grupa, int paritate, GOL coada_goluri[20], int &nr_goluri);
void gaseste_goluri_2(ORAR O, int grupa, int paritate, GOL coada_goluri[20], int &nr_goluri);
int este_liber(char *sala, int z, int o);
void copy_orar_lab(ORAR_LAB *buffer_dest, ORAR_LAB *buffer_s, int nr_elem);
void copy_orar_curs(ORAR_CURS *buffer_dest, ORAR_CURS *buffer_s, int nr_elem);
void elibereaza(ocupare &oc);
void elibereaza(ORAR_LAB* buffer, int indice_sala, int z, int o);

//fctii test
void verifica_copiere_orar(ORAR_LAB *buffer_1, ORAR_LAB *buffer_2, int nr_elem);
void verifica_copiere_orar(ORAR_CURS *buffer_1, ORAR_CURS *buffer_2, int nr_elem);