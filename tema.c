//***NUME Mandru, PRENUME Cosmina***
//****GRUPA 313CB, SERIA CB*********

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdint.h>


typedef struct celulag
{	
	void *info;
	struct celulag *urm;
}TCelulaG, *TLG, **ALG;

typedef	struct 
{
	TLG ic, sc;
}TCoada;

typedef struct 
{	
	TLG vf;
}TStiva;

typedef struct
{
	int PID;
	int prioritate;
	int timp;
	int executed_time;
	int timp_ramas_cuanta;
	int start_mem;
	long int dim_mem;
}proces;

typedef struct 
{
	int PID;
	int start_mem;
	int nr_crt_stiva;
	long int dim_mem;
	TStiva *memorie;
}mem_OS;

//functie de alocare celula lista generica
TLG AlocCelulaG(void* x) 
{
	TLG aux;
	
	aux = (TLG)malloc(sizeof(TCelulaG));
	if (!aux) 
	{
		return NULL;
	}
	aux->info = x;
	aux->urm = NULL;

	return aux;
}

//functie de alocare stiva
TStiva* AlocaS()
{	
	TStiva *S;
	S = (TStiva*)malloc(sizeof(TStiva));
	if(!S)
	{
		return NULL;
	}
	S->vf = NULL;

	return S;
}

//functie de alocare coada
TCoada* AlocaC()
{
	TCoada *C;

	C = malloc(sizeof(TCoada));
	if(!C)
	{
		return NULL;
	}
	C->sc = NULL;
	C->ic = NULL;

	return C;
}

//functie de introduce in stiva cu alocare
int Push(TStiva *s, void *x)
{
	TLG aux = AlocCelulaG(x);
	if (!aux) 
	{
		return 0;
	}
	aux->urm =s->vf;
	s->vf = aux;
	
	return 0;
}

//functie de introducere in stiva fara alocare
//folosita in refacerea stivei intiale dupa parcurgere
int Push2(TStiva *s, TLG x)
{
	x->urm = s->vf;
	s->vf = x;
	return 0;
}

//functie de extragere element din stiva
TLG Pop(TStiva *S)
{
	TLG aux;
	
	if(!S->vf)
	{
		return NULL;
	}
	aux = S->vf;
	S->vf = aux->urm;

	return aux;
}

//functie de extragere element din coada
TLG ExtrC(TCoada* C)
{
	TLG aux;
	
	if(!C->ic)
	{
		return NULL;
	}
	aux = C->ic;
	C->ic = aux->urm;

	return aux;
}

//functie de inserare in coada cu alocare
int InsC(TCoada *C, void *x) 
{
	TLG u = NULL, aux; 
	TLG l = C->ic;

	if (l != NULL) 
	{
		u = l;
		while(u->urm != NULL) 
		{
			u = u->urm;
		}
	}
	
	if (u == NULL) 
	{
		l = AlocCelulaG(x);
		C->sc = l;
		C->ic = l;
	}
	else 
	{
		aux = AlocCelulaG(x);
		u->urm = aux;
		C->sc = aux;
	}

	return 0;
}

//functie de inserare in coada fara alocare
//folosita in refacerea cozii initiale dupa parcurgere
int InsC2(TCoada *C, void *x) 
{	
	TLG u = NULL, aux; 
	TLG l = C->ic;
	
	if (l != NULL) 
	{
		u = l;
		while(u->urm != NULL)
		{
			u = u->urm;
		}
	}

	if (u == NULL) 
	{
		l = (TLG)x;
		l->urm = NULL;
		C->sc = l;
		C->ic = l;
	}
	else 
	{
		aux = (TLG)x;
		aux->urm = NULL;
		u->urm = aux;
		C->sc = aux;
	}

	return 0;
}

//functie de inserare la inceput in lista generica
int InsInc_TLG(ALG L, void *x) 
{
	TLG aux = AlocCelulaG(x);
	if(!aux)
	{
		return 0;
	}	
	aux->urm = *L;
	*L = aux;

	return 1;
}

//funtie de inserare la sfasit in lista generica
int Ins_TLG(ALG L, void *x) 
{
	TLG u = NULL;
	
	if (*L != NULL) 
	{
		u = *L;
		while(u->urm != NULL) 
		{
			u = u->urm;
		}
	}
	
	if (u == NULL) 
	{
		*L = AlocCelulaG(x);
	}
	else 
	{
		u->urm = AlocCelulaG(x);
	}
	
	return 1;
}

//functie de distrugere stiva
void free_S(TStiva *S)
{
	TLG aux = S->vf, sters;
	
	while(aux)
	{
		sters = aux;
		aux = aux->urm;
		if(sters->info != NULL)
		{
			free(sters->info);
		}
		if(sters != NULL)
		{
			free(sters);	
		}
	}
	if(S != NULL)
	{
		free(S);
	}
}

//functie de distrugere coada
void free_C(TCoada *C)
{
	TLG aux = C->ic, sters;
	
	while(aux)
	{
		sters = aux;
		aux = aux->urm;		
		if(sters->info != NULL)
		{
			free(sters->info);
		}
		if(sters != NULL)
		{
			free(sters);
		}
	}
	if(C != NULL)
	{
		free(C);
	}
}

//functie de distrugere lista generica ce reprezinta
//memoria sistemului de operare
void free_Mem_OS(ALG L) 
{
	TLG aux;	
	
	while(*L) 
	{
		aux = *L;
		*L = aux->urm;
		free_S(((mem_OS*)aux->info)->memorie);
		if(aux->info != NULL)
		{
			free(aux->info);
		}
		if(aux != NULL)
		{
			free(aux);
		}
	}
}

//functie de eliminare din lista generica, din memoria
//sistemului de operare
void delete_TLG(ALG Mem_OS, int pid)
{    
  	TLG l, ant;; 
	
	for(l = *Mem_OS, ant = NULL; l != NULL; ant = l, l = l->urm) 
	{
		if(((mem_OS*)l->info)->PID == pid) 
		{
			break;
		}
	}
	if (l == NULL) 
	{
		return;
	}
	if(ant == NULL) 
	{
		*Mem_OS = l->urm;
	}
	else 
	{
		ant->urm = l->urm;
	}

	free_S(((mem_OS*)l->info)->memorie);
	free(l->info);
	free(l);
}

//functie de inserare ordonata in coada de waiting cu alocare folosiata
//doar atunci cand un proces este nou in sistem si trebuie introdus/alocat
int Ins_Waiting(TCoada *C_w, proces *Q)
{	
	int ok = 0;
	TCoada *C1;
	TLG aux;
	proces *P2, *P1;

	//in caz de coada vida se insereaza fara nicio restrictie
	if(C_w->ic == NULL)
	{
		InsC(C_w,(void*)Q);
		return 1;
	}
	
	P1 = (proces*)C_w->ic->info;
	P2 = (proces*)C_w->sc->info;
	//in coada C1 se retin elementele extrase din coada de waiting
	//cat si procesul de inserat
	C1 = AlocaC();

	//daca prioitate procesului de inserat este mai mare decat ultima
	//prioritate din coada, atunci i se cauta pozitia de inserat in coada
	if(P2->prioritate <= Q->prioritate)
	{
		// se extrag elementele din coada pana cand se intalneste un element
		//cu prioritatea mai mare sau egala cua cea a procesului de inserat
		//sau in cazul in care exista mai multe procese cu aceasi prioritate
		//se va extrage pana cand ok nu a luat valoarea 1, adica nu i s-a 
		//gasit un loc procesuli in coada
		while(P1->prioritate >= Q->prioritate && C_w->ic != NULL && ok == 0)
		{	
			aux = ExtrC(C_w);
			if(P1->prioritate == Q->prioritate)
			{
				if(P1->timp >= Q->timp)
				{	
					if(P1->timp == Q->timp)
					{	
						if(P1->PID > Q->PID)
						{	
							InsC(C1,(void*)Q);
							ok = 1;
						}
					}
					else
					{
						InsC(C1,(void*)Q);
						ok = 1;
					}
				}
			}
			InsC2(C1, (void*)aux);
			if(C_w->ic != NULL)
			{
				//se actualizeazza mereu inceputul de coada
				P1 = (proces*)C_w->ic->info;
			}
		}
		if(ok == 0)
		{	
			//in cazul in care conditiile din while-ul anterior au fost 
			//indeplinte insa procesului de inserat nu i s-a gasit locul
			//se va insera la sfarsit in coada C1
			InsC(C1,(void*)Q);
		}
		//in final are loc concatenarea celor 2 cozi
		C1->sc->urm = C_w->ic;
		C1->sc = C_w->sc;
		C_w->ic = C1->ic;
		C_w->sc = C1->sc;
		if(C1 != NULL)
		{
			free(C1);
		}
		return 1;
	}

	//in cazul in care prioritatea procesului de inserat este strict mai mica
	//decat ultima priotitat din coada se va insera direct la sfarsitul cozii
	if(P2->prioritate > Q->prioritate)
	{
		InsC(C_w,(void*)Q);
		if(C1 != NULL)
		{
			free(C1);
		}
		return 1;
	}
	return 0;
}

//functie de inserare in coada de waiting la fel ca cea de mai sus dar
//fara alocare
int Ins_Waiting2(TCoada *C_w, TLG x)
{	
	int ok = 0;
	TCoada *C1;
	TLG aux;
	proces *P2, *P1, *Q;
	
	if(C_w->ic == NULL)
	{
		InsC2(C_w,(void*)x);
		return 1;
	}
	
	P2 = (proces*)C_w->sc->info;
	P1 = (proces*)C_w->ic->info;
	Q = (proces*)x->info;
	C1 = AlocaC();
	if(P2->prioritate <= Q->prioritate)
	{
		while(P1->prioritate >= Q->prioritate && C_w->ic != NULL && ok == 0) 
		{	
			aux = ExtrC(C_w);
			if(P1->prioritate == Q->prioritate)
			{	
				if(P1->timp == Q->timp)
				{	
					if(P1->PID > Q->PID)
					{	
						InsC2(C1,(void*)x);
						ok = 1;
					}
				}
				else if(P1->timp > Q->timp) 
				{
					InsC2(C1,(void*)x);
					ok = 1;
				}
			}
			InsC2(C1, (void*)aux);
			if(C_w->ic != NULL)
			{
				P1 = (proces*)C_w->ic->info;
			}
		}
		if(ok == 0)
		{
			InsC2(C1,(void*)x);
		}
		C1->sc->urm = C_w->ic;
		C1->sc = C_w->sc;
		C_w->ic = C1->ic;
		C_w->sc = C1->sc;
		if(C1 != NULL)
		{
			free(C1);
		}
		return 1;
	}
	if(P2->prioritate > Q->prioritate)
	{
		InsC2(C_w,(void*)x);
		if(C1 != NULL)
		{
			free(C1);
		}
		return 1;
	}

	return 0;
}

//fucntie de gasire a celui mai mic pid disponibil din sistemul de operare
//primeste ca parametru un pid si il cauta in memoria sistemului, in caz 
//afirmativ intoarce 1, invers 0
int find_PID(TLG Mem_OS, int pid)
{	
	int ok = 0;	
	TLG L = Mem_OS;
	mem_OS* P;

	//in Mem_OS se gasesc toate procesele active in sistem
	//care se afla fie in coada de waiting fie in cea de running
  	while(L)
  	{	
  		P = (mem_OS*)L->info;
		if(P->PID == pid)
		{
			ok = 1;
		}
 		L = L->urm;
    }
	if(ok == 1)
	{
		return 1;
	}
	return 0;
}


int Ins_Ord(ALG L, TLG l)
{  
	int ok = 0;
  	TLG p, ant, sf;
  	mem_OS *S, *S1, *A;

  	if(*L == NULL)
  	{
   		l->urm = *L;
		*L = l;
		S = (mem_OS*)l->info;
    	S->start_mem = 0;
    	return 0;
	}
  	S = (mem_OS*)l->info;
  	for(ant = NULL, p = *L; p != NULL ; ant = p, p = p->urm)
  	{	
	  	S1 = (mem_OS*)p->info;
	  	if(p->urm == NULL)
	  	{
	  		sf = p;
	  	}
	    if( S->PID < S1->PID)
	    {	
	        //inserez inaintea lui p pe l
	        l->urm = p->urm;
			p->urm = l;
			//recalculez poziitile de start a memoriei
			if(ant == NULL)
			{
				S->start_mem = 0;
			}
			else
			{
		    	A = (mem_OS*)ant->info;
				S->start_mem = A->start_mem + A->dim_mem;

			}
			S1->start_mem = S->start_mem + S->dim_mem;
	        ok = 1;
	        return 1;
	    }
    }
    if(ok == 0 )
    {	
    	S1 = (mem_OS*)sf->info;
    	sf->urm = l;
    	l->urm = NULL;
    	S->start_mem = S1->start_mem + S1->dim_mem;
     	return 1;
    }

  return 0;
}

//functie de defragmentare a memorie
int defragmentare(ALG Mem_OS, proces *P, mem_OS *S, FILE *f)
{
	TLG L = NULL, l, p;
	proces *Q;

	//se parcurge lista de memorie curente, fiecare element di ea este
	//reinserat ordonat dupa pid intr - o noua lista
	for(l = *Mem_OS; l != NULL; l = l->urm)
	{
		Ins_Ord(&L, l);
		if(l->urm == NULL)
		{
			p = l;
		}
	}
	Q = (proces*)p->info;
	if(Q->start_mem + Q->dim_mem + P->dim_mem > 3145728)
	{
		fprintf(f, "Cannot reserve memory for PID %d.\n", P->PID);
	}
	else
	{
		P->start_mem = Q->start_mem + Q->dim_mem;
		S->start_mem = P->start_mem;
		Ins_TLG(Mem_OS, (void*)S);
	}

	*Mem_OS = NULL;
	*Mem_OS = L;
	return 0;
}

//functie ce determina pozitia de start a unui proces si il si insereaza
//pe aceasta in lista de memorie pe pozitia corespunzatoare
int find_start(proces *P, ALG Mem_OS, mem_OS *S, FILE *f)
{	
	TLG l = *Mem_OS, in_l = l, aux;
	mem_OS *Q, *Q_urm;

	//in cazul in care nu exista niciun proces in sistem se insereaza
	//la inceput in lista, pozitia de start fiind desigur 0
	if(*Mem_OS == NULL)
	{	
		P->start_mem = 0;
		S->start_mem = 0;
		InsInc_TLG(Mem_OS, (void*)S);
		return 1;
	}

	Q = (mem_OS*)in_l->info;
	//este tratat cazul exceptat in care primul element din sistem nu are 
	//pozitia de start 0, adica exista loc de introducere a unui nou proces
	if(Q->start_mem != 0)
	{	
		if(P->dim_mem <= Q->start_mem)
		{	
			P->start_mem = 0;
			S->start_mem = 0;
			InsInc_TLG(Mem_OS, (void*)S);
			return 1;
		}
	}

	//se parcurge memoria sistemului de operare
	while(l)
	{	
		Q = (mem_OS*)l->info;
		//in cazul in care nu s-a ajuns la sfarsitul listei
		//se verifica daca exista spatiu suficient, intre procese,
		//pentru introducerea unui nou proces, in caz afirmativ se va 
		//calcula pozitia de start a procesului si va fi inserat in memorie 
		//pe pozitia corespunzatoare
		if(l->urm != NULL)
		{
			Q_urm = (mem_OS*)l->urm->info;
			if(Q_urm->start_mem - (Q->dim_mem + Q->start_mem) >= P->dim_mem) 
			{	
				P->start_mem = Q->start_mem + Q->dim_mem;
				S->start_mem = P->start_mem;
				aux = AlocCelulaG((void*)S);
				if(!aux)
					return 0;
				aux->urm = l->urm;
				l->urm = aux;
				return 1;
			}
		}
		else 
		{	
			//in cazul in care nu s-a gasit niciun spatiu liber intre procesele
			//existente se va adauga la sfarsit in lista daca exista spatiu
			//suficient, altfel se apeleaza defragmentare
			if(Q->start_mem + Q->dim_mem + P->dim_mem > 3145728)
			{
				defragmentare(Mem_OS, P, S, f);
				return 1;
			}
			P->start_mem = Q->start_mem + Q->dim_mem;
			S->start_mem = P->start_mem;
			Ins_TLG(Mem_OS, (void*)S);
			return 1;
		}
		l = l->urm;
	}

	return 0;
}

//functia ce adauga un proces in sistem
int add(long int dim_mem, int timp, int prioritate, TCoada *C_w, 
	TCoada *C_r, ALG Mem_OS, FILE *f)
{
	int pid = 1;
	long int mem_sistem = 0;
	proces *Q;
	mem_OS *S;
	TLG l = *Mem_OS;

	//se cauata cel mai mic pid posibil
	while(find_PID(*Mem_OS, pid) == 1)
	{
		pid++;
	}

	//se calculeaza cata memorie este ocupata in sistem 
	while(l)
	{	
		mem_sistem += ((mem_OS*)l->info)->dim_mem;
		l = l->urm;
	}

	//se verifica daca procesul are loc in sistem
	if (mem_sistem + dim_mem > 3145728)
	{
		fprintf(f, "Cannot reserve memory for PID %d.\n", pid);
		return 0;
	}

	Q = (proces*)malloc(sizeof(proces));
	if(!Q)
	{
		return 0;
	}
	Q->dim_mem = dim_mem;
	Q->timp = timp;
	Q->executed_time = timp;
	Q->prioritate = prioritate;
	Q->timp_ramas_cuanta = 0;
	Q->start_mem = 0;
	Q->PID = 0;
	Q->PID = pid;
	
	S = (mem_OS*)malloc(sizeof(mem_OS));
	if(!S)
	{
		return 0;
	}
	S->PID = 0;
	S->dim_mem = dim_mem;
	S->nr_crt_stiva = 0;
	S->start_mem = 0;
	S->memorie = AlocaS();
	S->PID = pid;

	//se cauta startul pentru noul proces
	find_start(Q, Mem_OS, S, f);
	fprintf(f, "Process created successfully: PID: %d, Memory starts at 0x%0x.\n", 
		Q->PID, Q->start_mem);
	// in cazul in care nu exista niciun proces in sistem disponibil de rulare
	//procesul se va insera direct in coada de running, daca nu se insereaza
	//in coada de waiting conform criteriilor date	
	if(C_r->ic == NULL)
	{	
		InsC(C_r,(void*)Q);
	}	
	else
	{	
		Ins_Waiting(C_w, Q);
	}

	return 1;
}

//functie ce verifica daca procesul conform pidului dat ca parametru 
//se afla in running
int get_running(int pid, TCoada *C_r, FILE *f)
{
	proces *P;

	if(C_r->ic != NULL)
	{	
		if(((proces*)C_r->ic->info)->PID == pid)
		{	
			P = (proces*)C_r->ic->info;
			fprintf(f, "Process %d is running (remaining_time: %d).\n",
			 P->PID, P->timp);
			return 1;
		}
	}
	return 0;
}

//functie ce verifica daca procesul conform pidului dat ca parametru 
//se afla in waiting
int get_waiting(int pid, TCoada *C_w, FILE *f)
{
	int ok = 0;
	proces *P;
	TLG aux, aux_inc;

	if(C_w->ic != NULL)
	{	
		//se stocheza adresa primului element de coada si se si verifica 
		// daca nu cumva acesta este procesul cautat, in caz afirmtiv
		//cautarea se termina si inceputul cozii este reinserat in coada
		aux_inc = ExtrC(C_w);
		P = (proces*)aux_inc->info;
		if(P->PID == pid)
		{
			fprintf(f, "Process %d is waiting (remaining_time: %d).\n",
				P->PID, P->timp);  
			Ins_Waiting2(C_w, (void*)aux_inc);
			return 1;
		}
		//daca coada are un singur element cautarea ia sfarsit
		if(C_w->ic == NULL)
		{
			InsC2(C_w, (void*)aux_inc);
			return 0;			
		}
		InsC2(C_w, (void*)aux_inc);
		aux = ExtrC(C_w);
		P = (proces*)aux->info;
		//altfel se parcurge coada prin extragere si inserare pana la intalnirea
		//adresei de inceput salvata anterior in aux_inc
		while(aux->urm != aux_inc)
		{
			InsC2(C_w, (void*)aux);
			P = (proces*)aux->info;
			if(P->PID == pid)
			{
				fprintf(f, "Process %d is waiting (remaining_time: %d).\n",
				 P->PID, P->timp);    
				ok = 1;
			}
			aux = ExtrC(C_w);
		}
		//se trateaza cazul pentru ultimul element din coada intrucat acesta
		//este extras in cadrul buclei while dar nu si reinserat
		InsC2(C_w, (void*)aux);
		P = (proces*)aux->info;
		if(P->PID == pid)
		{
			fprintf(f, "Process %d is waiting (remaining_time: %d).\n",
				P->PID, P->timp);    
			ok = 1;
		}		
	}

	//in cazul in care procesul a fost gasit se intoarce 1, altfel 0
	if(ok == 1)
	{
		return 1;
	}
	return 0;
}

//functie ce cauta procesul/procesele in coada de finish pe acelasi principiu
//ca functia anterioara cu exceptia faptului ca odata ce un proces cu pid-ul 
//dat a fost gasit cautarea nu se opreste
int get_finished(int pid, TCoada *C_f, FILE *f)
{
	int ok = 0;
	proces *P;
	TLG aux, aux_inc;

	if(C_f->ic != NULL)
	{	
		aux_inc = ExtrC(C_f);
		P = (proces*)aux_inc->info;
		if(P->PID == pid)
		{
			fprintf(f, "Process %d is finished.\n", P->PID); 
			ok = 1;
		}
		if(C_f->ic == NULL)
		{
			InsC2(C_f, (void*)aux_inc);
			return 0;			
		}
		InsC2(C_f, (void*)aux_inc);
		aux = ExtrC(C_f);
		P = (proces*)aux->info;
		while(aux->urm != aux_inc)
		{
			InsC2(C_f, (void*)aux);
			P = (proces*)aux->info;
			if(P->PID == pid)
			{
				fprintf(f, "Process %d is finished.\n", P->PID);  
				ok = 1;
			}
			aux = ExtrC(C_f);
		}
		InsC2(C_f, (void*)aux);
		P = (proces*)aux->info;
		if(P->PID == pid)
		{
			fprintf(f, "Process %d is finished.\n", P->PID);   
			ok = 1;
		}
	}

	if(ok == 1)
	{
		return 1;
	}
	return 0;
}

//functia get apeleaza cele trei functii anterioare
int get(int pid, TCoada *C_w, TCoada *C_r, TCoada *C_f, FILE *f)
{	
	int ok = 0, running, waiting, finished;

	running = get_running(pid, C_r, f);
	waiting = get_waiting(pid, C_w, f);
	finished = get_finished(pid, C_f, f);

	if(running == 1 || waiting == 1 || finished == 1)
	{
		ok = 1;
	}
	//daca procesul nu a fost gasit in niciuna din cozi, running, waiting,
	//finish, atunci se intoarce procesul urmator
	if(ok == 0)
	{
		fprintf(f, "Process %d not found.\n", pid);
		return 0;
	}

	return 1;
} 

//functie ce adauga 4 octeti pe stiva unui proces, in functie de pid-ul dat
int push_process(int pid, long int x, TLG Mem_OS, FILE *f)
{	
	int ok = 0;
	int *p;
	TLG l = Mem_OS;
	mem_OS *P;

	//se cauta in lista memoriei sistemului de operare stiva
	//procesului corespunzator dupa pid, odata gasit se verifica
	//daca exista loc in stiva, daca da atunci elemtul este inserat
	//in stiva, daca nu se afizeaza un mesaj
	//totodata se incremeteaza si variabila nr_crt_stiva, ce retine
	//numarul de int-uri introduse in stiva
  	while(l)
  	{
    	P = (mem_OS*)l->info;
    	if(P->PID == pid)
    	{
	    	if(P->nr_crt_stiva == (P->dim_mem / 4))
	    	{
	    		fprintf(f, "Stack overflow PID %d.\n", pid);
	    		ok = 1;
	    	}
	    	else	
	    	{	
	    		p = (int*)malloc(sizeof(int));
	    		if(!p)
	    		{
	    			return 0;
	    		}
	    		*p = x;
	    		Push(P->memorie, (void*)p);
	    		P->nr_crt_stiva++;
	    		ok = 1;
	    	}
    	}
    	l = l->urm;
    }

    //in caz ca piul nu a fost gasit se afiseaza urmatorul mesaj
    if(ok == 0)
    {	
    	fprintf(f, "PID %d not found.\n", pid);
    	return 0;
    }
    return 1;
}

//functie ce elimina primii patru octeti din stiva unui proces dat
//principiul functiei este asemanator cu functia anterioara
int pop_process(int pid, TLG Mem_OS, FILE *f)
{	
	int ok = 0;
	TLG l = Mem_OS, p;
	mem_OS *P;
  	while(l)
  	{
    	P = (mem_OS*)l->info;
    	if(P->PID == pid)
    	{	
    		//daca stiva procesului este goala se afizeaza mesajul
	    	if(P->memorie->vf == NULL)
	    	{
	    		fprintf(f, "Empty stack PID %d.\n", pid);
	    	}
	    	else	
	    	{	
	    		//altfel se elimina primul element din stiva si se 
	    		//face free la elemntul extras; se decrementeaza si contorul
	    		//stivei
	    		p = Pop(P->memorie);
				free(p->info);
				free(p);
	    		P->nr_crt_stiva--;
	    	}
	    	ok = 1;
    	}
    	l = l->urm;
    }

    if(ok == 0)
    {	
    	fprintf(f, "PID %d not found.\n", pid);
    	return 0;
    }

    return 1;
}

//functi de afisare a stivei unui proces
int print_stack(TLG Mem_OS, int pid, FILE *f) 
{   
	int ok = 0;
  	TLG l = Mem_OS, aux;
  	TStiva *aux_S = AlocaS();
  	mem_OS *S;

	while(l)
	{	
		S = (mem_OS*)l->info;
		if(S->PID == pid)
		{
			//in caz de stiva goala se afiseaza:
			if(S->memorie->vf == NULL)
			{
				fprintf(f, "Empty stack PID %d.\n", pid);
				free_S(aux_S);
				return 1;
			}
			else
			{	
				//se extrag toate elementele din stiva si se introduc
				//intr-una auxiliara, in acest moment aux_S contine toate
				//elemtele din stiva procesului doar ca rasturnate
				aux = Pop(S->memorie);
				while(aux)
				{	
					Push2(aux_S, aux);
		    		aux = Pop(S->memorie);
		    	}
				fprintf(f, "Stack of PID %d: ", pid);
				//sunt din nou extrase pe rand elementele din aux_S
				//sun afisate si reintroduse in stiva de memorie a procesului
				//in ordinea initiala
				aux = Pop(aux_S);
				while(aux->urm)
				{	
					Push2(S->memorie, aux);
					fprintf(f, "%d ", *(int*)aux->info);
		    		aux = Pop(aux_S);
		    	}
		    	//se trateaza cazul pentru ultimul element
				Push2(S->memorie, aux);
				fprintf(f, "%d.\n", *(int*)aux->info);					
		    }   	
		    ok = 1;
    	}
    	l = l->urm;
    }

    free_S(aux_S);

    //in cazul in care procesul nu a fost gasit se afiseaza
    if(ok == 0)
    {
    	fprintf(f, "PID %d not found.\n", pid);
    	return 0;
    }
    return 1;
}

//functie de afisare a cozii de asteptare se parcurge coada ca in cazul 
//functiei de get_waiting sau get_finish, doar ca la fiecare proces extras
//se afizeaza informatiile despre acesta
int print_waiting(TCoada *C_w, FILE *f)
{	
	proces *P;
	TLG aux, aux_inc;    
  	
  	fprintf(f, "Waiting queue:\n");
  	if(C_w->ic == NULL)
  	{
  		fprintf(f, "[]\n");
  		return 1;
   	}
  	fprintf(f, "[");
	aux_inc = ExtrC(C_w);
	P = (proces*)aux_inc->info;
	if(C_w->ic == NULL)
	{
	    fprintf(f, "(%d: priority = %d, remaining_time = %d)]\n", P->PID, 
	    	P->prioritate, P->timp); 
	    InsC2(C_w, (void*)aux_inc);  
	    return 1;
	}
	else
	{
	    fprintf(f, "(%d: priority = %d, remaining_time = %d),\n", P->PID, 
	    	P->prioritate, P->timp);   
	} 					
	InsC2(C_w, (void*)aux_inc);
	
	aux = ExtrC(C_w);
	P = (proces*)aux_inc->info;
	while(aux->urm != aux_inc)
	{
    	InsC2(C_w, (void*)aux);
		P = (proces*)aux->info;
    	
    	fprintf(f, "(%d: priority = %d, remaining_time = %d),\n", P->PID, 
    		P->prioritate, P->timp);
		
		aux = ExtrC(C_w);
    }
	
	InsC2(C_w, (void*)aux);
	P = (proces*)aux->info;
	fprintf(f, "(%d: priority = %d, remaining_time = %d)]\n", P->PID, 
	    P->prioritate, P->timp);   

	return 1;
}

//functie de afisare a cozii de finish, creata pe acelasi principiu
//ca functia anteriora
int print_finished(TCoada *C_f, FILE *f)
{	
	proces *P;
	TLG aux, aux_inc;     
  	
  	fprintf(f, "Finished queue:\n");
  	if(C_f->ic == NULL)
  	{
  		fprintf(f, "[]\n");
  		return 1;
   	}
  	fprintf(f, "[");
	aux_inc = ExtrC(C_f);
  	P = (proces*)aux_inc->info;
	if(C_f->ic == NULL)
	{
	    fprintf(f, "(%d: priority = %d, executed_time = %d)]\n", P->PID, 
    		P->prioritate, P->executed_time); 	
	   	InsC2(C_f, (void*)aux_inc);
	    return 1;
	}
    else
    {
    	fprintf(f, "(%d: priority = %d, executed_time = %d),\n", P->PID, 
    		P->prioritate, P->executed_time); 	 			
    }
	InsC2(C_f, (void*)aux_inc);

	aux = ExtrC(C_f);
	P = (proces*)aux->info;
	while(aux->urm != aux_inc)
	{
    	InsC2(C_f, (void*)aux);
		P = (proces*)aux->info;

    	fprintf(f, "(%d: priority = %d, executed_time = %d),\n", P->PID, 
    			P->prioritate, P->executed_time);
		aux = ExtrC(C_f);
	}

	InsC2(C_f, (void*)aux);
	P = (proces*)aux->info;
    fprintf(f, "(%d: priority = %d, executed_time = %d)]\n", P->PID, 
    	P->prioritate, P->executed_time); 	

	return 1;
}

//functie ce verifica daca coada de waiting este goala sau nu
//in caz afirmativ mai intai ezte introdus elementul x si apoi
//are loc extragerea; daca coada nu este vida atunci prima data 
//are loc exragerea si apoi introducerea
TLG verifica_null(TCoada *C_w, TLG x)
{
	TLG aux;

	if(C_w->ic == NULL)
	{	
		Ins_Waiting2(C_w, (void*)x);
		aux = ExtrC(C_w);
	}
	else
	{	
		aux = ExtrC(C_w);	
		Ins_Waiting2(C_w, (void*)x);	
	}
	return aux;
}

//functia run trateaza mai intai procesul ce se afla in running, se verifica
//daca acesta mai are de rulat din cuanta sau daca nu se verifica daca dupa 
//rularea ce urmeaza se va termina. Daca se termina se introduce in coada de 
//finish si este scost si din lista de memorie a sistemului de operare; daca 
//nu este introdus in coada de waiting
int run(TCoada *C_r, TCoada *C_f, TCoada *C_w, ALG Mem_OS, int time, 
	int cuanta_timp)
{	
	proces *P; 
	if(C_r->ic == NULL)
		return 0;
	TLG aux = ExtrC(C_r);  
	P = (proces*)aux->info;
	if(P->timp_ramas_cuanta != 0)
	{	
		time -= P->timp_ramas_cuanta;
		P->timp -= P->timp_ramas_cuanta;
		P->timp_ramas_cuanta = 0;
		aux = verifica_null(C_w, aux);
	}
	else
	{	
		if(P->timp <= cuanta_timp)
		{	
			time -= P->timp;
			P->timp = 0;
			InsC2(C_f, (void*)aux);
			delete_TLG(Mem_OS, P->PID);	
			if(C_w == NULL)
				return 0;	
			aux = ExtrC(C_w);
		}
		else
		{	
			time -= cuanta_timp;
			P->timp -= cuanta_timp;
			aux = verifica_null(C_w, aux);
		}
	}
	//Cat timp timpul de rulare nu a ajuns la null se ruleaza procesele
	//din running in functie de ordinea din coada. Am dedus 2 cazuri principale
	//fiecare avand alte 2 cazuri(le-am detaliat in README)
	while(time != 0)
	{	
		P = (proces*)aux->info;
		if(time > cuanta_timp)
		{	
		   	if(P->timp > cuanta_timp)
			{
			    time -= cuanta_timp;
			    P->timp -= cuanta_timp;
				aux = verifica_null(C_w, aux);
			}
			else 
			{	
			    time -= P->timp;
			    P->timp = 0;
		    	InsC2(C_f, (void*)aux);
		    	delete_TLG(Mem_OS, P->PID);	
		    	if(C_w->ic == NULL)
		    		return 0;
		    	aux = ExtrC(C_w);
			}
		}
		else if(time <= cuanta_timp)
		{	
		   	if(time > P->timp)
		    {	
		    	time -= P->timp;
		    	P->timp = 0;
		    	InsC2(C_f, (void*)aux);
			    delete_TLG(Mem_OS, P->PID);	
			    if(C_w->ic == NULL)
			    	return 0;
		    	aux = ExtrC(C_w);	
		    }
		    else if(time <= P->timp)
		    {	
		    	P->timp -= time;
		    	P->timp_ramas_cuanta = cuanta_timp - time;
		    	time = 0;
		    	if(P->timp_ramas_cuanta == 0)
		    	{
		    		aux = verifica_null(C_w, aux);
		    		P = (proces*)aux->info;
		    	}
		    	InsC2(C_r, (void*)aux);
		    }
		}
	}
	return 1;
}

//functia finish ia toate procsele active din sistem, adica cele din coada
//de running si waiting si le trece in finish, eliminandu-le si din memoria
//sistemului; totodata se calculeaza si suma timpurilor ramase
void finish(TCoada *C_r, TCoada *C_w, TCoada *C_f, ALG Mem_OS, FILE *f)
{	
	int total_time = 0;
	proces *P;
	TLG aux = ExtrC(C_r); 
	
	if(aux != NULL) 
	{
		P = (proces*)aux->info;
 	 	total_time += P->timp;
  		InsC2(C_f, (void*)aux);
	}
  	
  	aux = ExtrC(C_w);
  	while(aux)
  	{
  		P = (proces*)aux->info;
    	total_time += P->timp;
    	delete_TLG(Mem_OS, P->PID);	
    	InsC2(C_f, (void*)aux);
    	aux = ExtrC(C_w);
    }
    
    fprintf(f, "Total time: %d\n", total_time);
}

int main(int argc, char *argv[])
{
	TCoada *C_r = AlocaC();
	TCoada *C_w = AlocaC();
	TCoada *C_f = AlocaC(); 
	TLG Mem_OS = NULL;
	char line[256], *p;
	int cuanta_timp, timp, prioritate, pid, run_timp;
	long int mem_size, push;
	char* filenamein = strdup(argv[1]);
	char* filenameout = strdup(argv[2]);

	FILE *fin = fopen(filenamein, "r");

	if (fin == NULL) {
        	fprintf(stderr, "ERROR: Can't open file %s", filenamein);
        	return -1;
    	}

	FILE *fout = fopen(filenameout, "a");

	if (fout == NULL) {
		fclose(fin);
        	fprintf(stderr, "ERROR: Can't open file %s", filenameout);
        	return -1;
    	}

	fgets(line, sizeof(line), fin);
	cuanta_timp = atoi(line);
	while(fgets(line, sizeof(line), fin)) 
	{
		p = strtok(line, " ");
		
		if(strcmp(p, "add") == 0) 
		{	
			p = strtok(NULL, " ");
			mem_size = atol(p);
			p = strtok(NULL, " ");
			timp = atoi(p);
			p = strtok(NULL, " ");
			prioritate = atoi(p);
			add(mem_size, timp, prioritate, C_w, C_r, &Mem_OS, fout);
		}
		
		if(strcmp(p, "get") == 0) 
		{
			p = strtok(NULL, " ");
			pid = atoi(p);
			get(pid, C_w, C_r, C_f, fout);
		}
		
		if(strcmp(p, "push") == 0) 
		{
			p = strtok(NULL, " ");
			pid = atoi(p);
			p = strtok(NULL, " ");
			push = atol(p);
			push_process(pid, push, Mem_OS, fout);
		}

		if(strcmp(p, "print") == 0) 
		{
			p = strtok(NULL, " ");
			if(strncmp(p, "waiting", 7) == 0) 
			{	
				print_waiting(C_w, fout);
			}
			if(strcmp(p, "stack") == 0) 
			{	
				p = strtok(NULL, " ");
				pid = atoi(p);
				print_stack(Mem_OS, pid, fout);
			}
			if(strncmp(p, "finished", 8) == 0) 
			{	
				print_finished(C_f, fout);
			}
		}
		if(strcmp(p, "pop") == 0) 
		{
			p = strtok(NULL, " ");
			pid = atoi(p);
			pop_process(pid, Mem_OS, fout);
		}

		if(strcmp(p, "run") == 0) 
		{	
			p = strtok(NULL, " ");
			run_timp = atoi(p);
			run(C_r, C_f, C_w, &Mem_OS, run_timp, cuanta_timp);
		}

		if(strncmp(p, "finished", 8) != 0)
		{
			if(strncmp(p, "finish", 6) == 0) 
			{	
				finish(C_r, C_w, C_f, &Mem_OS, fout);
			}
		}
	}
	if(C_w != NULL)
	{
		free_C(C_w);
	}
	if(C_r != NULL)
	{
		free_C(C_r);
	}
	if(C_f != NULL)
	{
		free_C(C_f);
	}
	if(Mem_OS != NULL)
	{
		free_Mem_OS(&Mem_OS);
	}

	fclose(fin);
	fclose(fout);
	free(filenamein);
	free(filenameout);

	return 0;
}