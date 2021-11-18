# SISTEME-DE-FISIERE
**Implementare-comenzi-pt-lucrul-cu-SISTEME-DE-FISERE.**

**UN DIRECTOR CONTINE REFERINTE ATAT PENTRU SUBDIRECTOARELE LUI, CAT SI PENTRU
SUBFISIERELE SALE.**

typedef struct Dir{
	char *name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director
 
typedef struct File {
	char *name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul pe
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

**Ne dorim ca cel mai de baza nivel al sistemului de fisiere sa fie un director
numit home in interiorul caruia o sa ne desfasuram toate task-urile.**

In momentul rularii aplicatiei, aceasta va astepta inputuri de la stdin. Fiecare comanda va fi apelata sub forma 
<nume_comanda> + argumente, argumentele variind in functie de comanda data. Exemple de comenzi: touch f1, mkdir d1,
rm f2, rmdir d2, ls, mv nume_vechi nume_nou etc. Aplicatia se va opri la comanda stop. Comenzile vor fi explicate in
cele ce urmeaza.


**1. touch**
void touch (Dir* parent, char* name);
Functia creaza un fisier in directorul curent. Acesta va fi adaugat la finalul listei de fisiere. Aceasta primeste
ca argument numele fisierului. Numele fisierului nu contine ”/”(nu va reprezenta o cale). In cazul in care exista 
deja un fisier/director cu acest nume, se va afisa urmatorul mesaj de eroare: File already exists\n.

**2. mkdir**
void mkdir (Dir* parent, char* name);
Functia creaza un director in directorul curent. Acesta va fi adaugat la finalul listei de fisiere. Aceasta primeste
ca argument numele directorului. Numele directorului nu contine ”/”(nu va reprezenta o cale). In cazul in care exista deja un
fisier/director cu acest nume, se va afisa urmatorul mesaj de eroare: Directory already exists\n.

**3. ls**
void ls (Dir* parent);
Functia afiseaza toate directoarele, respectiv fisierele din directorul curent. Mai intai vor fi afisate numele
directoarelor, apoi a fisierelor, fiecare pe un rand nou.

**4. rm**
void rm (Dir* parent, char* name);
Functia elimina fisierul cu numele name din directorul curent. Daca nu a fost gasit niciun fisier cu numele dat,
se va afisa mesajul de eroare Could not find the file\n. Numele fisierului nu contine ”/”(nu va reprezenta o cale).

Exemplu de rulare:

touch f1
touch f2
ls
rm f1
ls
stop
Rezultat:

f1
f2
f2
**5. rmdir**
void rmdir (Dir* parent, char* name);
Functia elimina directorul cu numele name din directorul curent si toate fisierele care se gasesc in acesta. Daca nu 
a fost gasit niciun director cu numele dat, se va afisa mesajul de eroare Could not find the dir\n. Numele directorului
nu contine ”/”(nu va reprezenta o cale).

Exemplu de rulare:

mkdir d1
mkdir d2
ls
rmdir d2
ls
stop
Rezultat:

d1
d2
d1
Comportamentul functiei rmdir difera de comportamentul utilitarului rmdir. Functionalitatea acestei 
functii este echivalenta cu folosirea comenzii rm -r <director>

**6. cd **
void cd(Dir** target, char* name);
Functia schimba directorul curent in directorul cu numele name, din lista de directoare a directorului curent. 
Daca nu este gasit, se va afisa mesajul de eroare No directories found!\n. Pentru a schimba directorul 
curent in directorul parinte al celui curent se va folosi sirul de caractere special ”..”. Daca directorul
curent nu are parinte, ,atunci nu se va schimba directorul curent. Numele directorului nu contine ”/”(nu va reprezenta o cale).

**7. tree**
void tree(Dir* target, int level);
Functia tree afiseaza interiorul directorului curent intr-o forma arborescenta si al fiecarului director din interior.

Formatul de output este urmatorul: 4 spatii(” ”) x level + nume file/director. Antetul functiei estescris pentru a
permite o rezolvare recursiva, insa cerinta poate fi rezolvata iterativ.

Ordinea de afisare este urmtoarea:

Se vor afisa mai intai numele directoarele si apoi numele fisierele
Numele directoarelor si al fisierelor vor fi afisate in ordinea in care au fost create
Dupa afisarea numelui unui director, se va afisa tot ceea ce contine acel director
Exemplu de rulare:

mkdir d1
mkdir d2
cd d1
touch f1
mkdir d3
cd d3
touch f2
mkdir d4
cd ..
cd ..
tree
stop
Rezultat:

d1
    d3
        d4
        f2
    f1
d2
8. pwd - 10p
char *pwd (Dir* target);
Functia intoarce un sir de caractere ce reprezinta calea de la directorul radacina(home) pana la directorum
curent, cu numele target. Formatul aplicat pentru afisare este: ”/home/(parent_name)/…./(target-name)”. Rezultatul
functiei trebuie afisat la stdout, in afara functiei pwd.

Exemplu de rulare:

mkdir d1
cd d1
pwd
stop
Rezultat:

/home/d1
**9. stop**
void stop (Dir* target);
Aceasta functie opreste aplicatia.

**10. memory management**
O alta parte foarte importanta a temei este intelegerea si lucrul cu memoria. Pentru asta, acest task va
consta din doua parti:

1) Alocarea corecta de memorie: memoria va fi alocata dinamic pentru troate string-urile si structurile folosite.
Pentru un fisier cu numele “Ana”, se vor aloca dinamic 4 octeti (3 pentru fiecare litera + unul pentru terminatorul 
de sir). Declaratiile statice de tipul char a[50] trebuie modificate in char *a = malloc(size_of_string);.

2) Dezalocarea corecta a memoriei: memoria va fi dezalocata corect si complet. Astfel, in functie de task, vor trebui
sa se faca urmatoarele operatii:

rm: se va dezaloca structura in sine
rmdir: se va dezaloca atat structura in sine, cat si fiecare structura din interiorul directorului (atat directoarele
cat si fisierele)
pwd: se va dezaloca sirul intors de acesta, in afara functiei pwd (dupa ce a fost afisat)
stop: la inchiderea aplicatiei, memoria alocata pentru directoarele si fisierele va fi eliberata
Pentru testare, vom folosi valgrind si ca punct de referinta, programul nu trebuie sa afiseze niciun read invalid sau
orice leak de memorie.

valgrind –leak-check=full –show-leak-kinds=all –track-origins=yes ./tema.

Punctajul pe aceasta cerinta va fi oferit doar daca cel putin 50p au fost obtinute din alte cerinte.
**11. mv **
void mv(Dir* parent, char* oldname, char* newname);
Functia va schimba numele directorului/fisierului oldname din directorul curent in newname. Se va verifica, mai intai, 
daca numele oldname exista deja, in caz contrar se va afisa mesajul File/Director not found\n, iar apoi daca numele newname
nu este folosit deja, in caz contrar se va afisa mesajul de eroare File/Director already exists\n. Schimbarea se va face 
prin readaugare directorului/fisierului in lista de directoare/fisiere. In cazul directoarelor, interiorul va ramane neschimbat.
Numele directorului/fisierului(atat cel vechi, cat si cel nou) nu contine ”/”(nu va reprezenta o cale).
int exists(Dir* parent, char* name) = VERIFICA DC EXISTA UN FILE/DIR CU 
NUMELE PRIMIT(char* name) = verificare se face in toate subdirectoarele
mod recusiv.

FUNCTII IMPEMENTATE DE MINE, CATEVA EXPLICATII LEGATE DE FUNCTIONALITATEA LOR:

void touch (Dir* parent, char* name) = se aloca memorie pt o structura 
de tip File si se adauga in mod corespunzator.

void mkdir (Dir* parent, char* name) = se aloca memorie pt o structura 
de tip Dir si se adauga in mod corespunzator.

void ls (Dir* parent) = se parcurge lista de directoare continute de parent
si se afiseaza numele, la fel si pentru fisiere.

void free_file(File* file) = goleste o structura de tipul File.

void free_dir(Dir* dir) = Goleste recursiv un director. Se va elimina pentru
fiecare director incepand din |dir|: toate fisierele lui, si se va apela 
recursiv pt subdirectoarele lui.

void rm (Dir* parent, char* name) = cauta dc exista un fisier cu acest nume,
in caz afirmativ se elibereaza structura(cu free_file) si se refac legaturile de la nivelul
listei inlantuita de fisiere in mod corespunzator(cum e specificat in enunt).

void rmdir (Dir* parent, char* name) = cauta dc exista un director cu acest nume,
in caz afirmativ se elibereaza structura(si restul directorului cu ajutorul lui
free_dir) . Se refac legaturile de la nivelul listei inlantuita de fisiere
in mod corespunzator(cum e specificat in enunt).

void cd(Dir** target, char *name) = imi schimba directorul in care ma aflu.
Se executa schimbarea efectiva numai in situatia in care exista un director
cu numele primit ca parametru.

char *pwd (Dir* target) = afiseaza calea pe care trebuie sa o parcurg pentru
a ajunge in directorul "target", evident dc acesta exista.
Se concateneaza la inceputul directorului curent parintele acestului pana se
ajunge la root(parintele e null in acest caz). Se afiseaza efetiv stringul,
care ulterior va fi eliberat in main().

void stop (Dir* target) = cand se ajunge la executia lui stop programului meu
nu ii vor mai fi neccesare structurile de directoare si fisiere alocate in
procedurile precedent(mkdir, touch); deci se va dezaloca tot acest sistem de fisiere
prin apelare free_dir pe root. (aici greseam initial pt ca nu pastram o copie
a rootului si eu dezalocam de la punctul(directorul) la care ramasesem
-dir se schimbase in urma apelului functiei cd).
