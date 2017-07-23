Programul contine urmatoarele fisiere:
Fisiere de intrare:
orare.in
titulari.in
sali_curs.in
sali_lab.in
sali_seminar.in
sali_p.in -pentru proiecte

grupe_in -numarul de grupe pentru fiecare orar + grupe optionale
si in disc 35 de fisiere pentru discipline aferente fiecarui orar

lab_semigrupe.in - lista de materii care au laboratoare pe semigrupe

supliment_lab_in - lista cu seminarii care trebuie adaugate la unele grupe( din .csv-uri nu se
poate vedea cand un seminar/lab/proiect apare la mai multe grupe

-am scos diacriticile pentru toate fisierele de intrare

Fisierele de intrare pentru discipline contin:
index numeric(nr inregistrare)
nume disciplina
acronim
daca are curs/laborator/seminar/proiect
durata celor 4
paritate
index profesor

Formatul de intrare trebuie respectat pentru fiecare tip de fisiere, fara caractere de 
spatiere in plus. Fiecare fisier lab_semigrupe.in se va termina cu un '\n', pentru ca fctia
strcmp nu merge

in /CSV am orarele din anul precedent in format .csv, uniformizate (pentru a putea fi citite)
csv trebuie modificate de mana pentru a dauga de exemplu paritatea[] sau semigrupa{} si a 
aranja cursurile in format comun. Daca de exemplu sunt 2 seminarii/cursuri in acelasi timp,
se vor pune cu '#'
lab/seminariile/proiectele sunt de forma : acronim(l/s/p) - sala[paritate]{semigrupa}
cursurile sunt de forma: "NUME CURS (c), titular (sala)[paritate]"
(paritate si semigrupe doar unde este cazul)
daca se suprapun 2 cursuri/lab/seminar la aceeasi ora, dar pe parititati diferite: # 
(sau optionale)
Ora trebuie sa fie pe 2 cifre (08 si 09)

citirea se face sub forma:

Surse:
##header1.h
-definire constante si tipuri de date pentru structuri/liste
-lista de fctii ce sunt apelate intre fisiere

##functii_auxiliare.cpp
-fctie de setare a semestrului (I)
-fctie de citire a materiilor care au lab pe semigrupe
-fctie de initializare a unei liste de grupe
-fctie de initializare a unei casute de ocupare
-fctie care verifica daca o grupa se afla in lista de grupe a unei casute ocupate in orarul mare
-fctie de copiere a vectorului de orare mari(adica orarul total)
-fctie de suprascriere pentru a pune o casuta in locul alteia
-fctie de eliberare memorie pentru un vector de orare sali de lab
-fctie de eliberare memorie pentru un vector de orare sali de curs

##functii_algoritm
-test_extern - testeaza functionarea variabilelor din alte fisiere
-calc_cost_1 - calculeaza costul golurilor pentru o semigrupa
-calc_cost_gr - calculeaza costul golurilor pentru o grupa
-calc_cost_2 - calculeaza costul golurilor pentru o grupa (master)
-exista_doar_semigrupa - vede daca o semigrupa este intr-o casuta de ocupare ptr orar mare, fara grupa mai mare!!
-exista_semigrupa - vede daca o semi_grupa este sau nu intr-o casuta de ocupare ptr orar mare
-exista_grupa -vede daca o grupa este sau nu intr-o casuta ocupare ptr orar mare
-verif_grupa - vede daca o grupa este sau nu intr-o casuta ocupare ptr orar mare( si prin una din semigrupe)
-verif_grupa2 -vede daca o grupa este sau nu intr-o casuta ocupare ptr orar mare ptr mutare!
-verif_grupa_master - vede daca o grupa este sau nu intr-o casuta ptr orar mare (master)
-optimizare_1 -optimizare pentru semigrupe - lab/proiecte cu o singura semigrupa
-optimizare_grupe_licenta - optimizare pentru grupe - lab/proiecte cu o singura grupa!! 
-optimizare_grupe2 - optimizarea pentru grupele de masterat
-fctie care gaseste golurile ptr fiecare semigrupa
-fctie care gaseste golurile ptr fiecare grupa
-este_liber - vede daca o sala este libera la o anumita zi/ora
-este-liber - vede daca o grupa care are un lab comun cu alta are altceva la ora aia


##functii_citire.cpp
-fctie de citire pentru lista de titulari
-fctie de citire pentru lista de sali
-fctie de cititre pentru nr de grupe per orar
-fctie de cititre pentru discipline
 Este cea mai mare fctie de citire
 pentru fiecare orar generez intai numele fisierului de intrare pentru discipline
 fiecare orar va avea o lista de discipline
 apoi citesc din fiecare fisier fiecare disciplina si o adaug in lista

-fctiile de citire folosesc fctiile de adaugare lista 
-fctie de initializare pentru vectorul de orare

##functii_liste.cpp
-fctii de adaugare lista pentru lista de titulari si pentru listele de sali
-fctie de getat(pozitie) pentru lista de titulari curs dupa (indice sau nume)
-fctie de getat pentru discipline din lista de discipline cu numele pentru adaugare in orar
-fctie de getat pentru pozitia unei sali in lista de sali
-fctie de getat

##functii_orar.cpp
-fctii de adaugare in orare (pentru orarul vechi de exemplu)
-3 fctii de afisare pentru orare mari, pentru orar titulari si pentru orare sali - vor genera fisiere .csv
-fctie ce copiaza o casuta de ocupare in alta
-fctie care copiaza orarele pentru salile de lab
-fctie care copiaza orarele pentru salilele de curs
-fctie care elibereaza o casuta de materie cand o sterg
-fctie care elibereaza un loc din vectorul de orare sali cand fac cate o mutare
-fctie care verifica egalitatea intre 2 casute ocupare

##main.cpp
-declaratii variabile
-citeste nume de orare din orare.in si apeleaza fctiile de citire date intrare pentru orarele initiale + completarile 
necesare
-aplica algoritmul de optimizare a orarelor
-face afisarea (in fisiere de iesire a orarelor rezultate)
alte fctii in main:
adaugare_grupe_lab_sup - din supliment_lab.in lab/sem care sunt la mai multe grupe si din .csv nu se poate vedea
generare_nume_grupa - generare nume grupa pentru o singura grupa(posibil subgrupa) pentru seminarii/lab pentru un orar
generare_nume_grupe - lista de grupe pentru un orar de serie, pentru antet la afisare sau pentru a le adauga la un curs

+am inclus si programul utilitar test_repetari care va afisa repetarile unei sali in 
interiorul unui fisier de intrare sau repetarile unei sali in mai multe fisiere de intrare
(adica apare la mai multe tipuri de sali)

+ReadMe

#####Greseli gasite in orarele vechi(cand am pus sa afiseze ce apare la suprapuneri)
1#Reprezentarea cunostintelor cu Knowledge Representation and Reasoning de la masterat anul 5 IA
si ISI (oricum cred ca sunt aceeasi materie si ar trebui sa bag doar una)

2#Sisteme de operare(5 CM SCPD) si RETELE LOCALE (3CC) miercuri 18:00

3#FILOZOFIE COGNITIVISTA de la 2CA joi de la 14-16 in A02Leu coincide cu matematica 1 de la 1CA

4# CLOUD COMPUTING la anu 5 cu C. Dobre se suprapune in EC102 INTEGRAREA SISTEMELOR INFORMATICE
de la 4c5 luni 10-12

5#am scos seminarul de pedagogie de la 2CB pentru ca nu are sala specificata si EC002 care apare la alte serii e ocupat la
aceea ora ( Sisteme Incorporate Seria4C2)

6#cursul de marti 8:00 Categorii si Computer Science (curs) (CCS) de la 5CM-G1 se suprapune cu sem de LFA 3CA in AN210

7#seminarul de pedagogie 2CA de joi 18 EC002 coincide cu Politici în sisteme distribuite 6CM-SSA si E-GOV
#Exceptii
la master sunt si laboratoare de la 17 la 19 (ore impare)

8# vineri la ora 8 laboratorul de Progr in EG105 coincide la 1CA si 1CB, las asa?

9#luni la ora 16 in EG405 coinci POO (2CB) si MPS(4C5)

10#luni la ora 10 in AN213 coincid sem de IF de la 1CC cu franceza 2CC


BSIM in sala electronic arts ed321
Ed321 apare ca sala de lab la KESE an6ABD

A04Leu la 1CB vineri ca sala de seminar

la anul 4C4 am luat cele 2 optionale in una singura cum era la celalalte serii din anul 4

la anul 6 ABD am 2 sali la seminarul de APPI ED 112/AN 217 ?? 

la 1CC luni ora 12 se suprapune II curs cu seminarul de IF la una din semigrupe

BN033 ca sala de la nu contine nici un laborator

linii de cod program: 6172


