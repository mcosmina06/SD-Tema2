***NUME Mandru, PRENUME Cosmina***
****GRUPA 313CB, SERIA CB**********
 
							Readme Tema 2

		Pentru reaizarea task-urilor am creat 3 cozi: running(cu un proces), 
	waiting, finish, pentru fiecare proces cate o stiva, iar pentru memoria
	sistemului de operare o lista generica ce contine stivele de memorie ale
	proceselor si inca cateva informatii necesare. Astfel, exista structura 
	proces cu informatiile despre proces si structura mem_Os ce are drept
	campuri stiva de memorie si inca cateva informatii despre proces.
		De asemeni, am facut functii de inserare cu alocare, pentru atunci 
	cand un proces este inserat prima dat in sistem, si fara alocare pentru
	atunci cand procesele sun mutate dintr-o coada intr-alta dar si pentru 
	parcurgea cozilor/stivelor.
		1.	Functia add: Are ca scop adaugarea unui proces in sistem. Intai se
	verifica daca procesului i se poate aloca memorie in sistem.Se cauta
	cel mai mic pid disponibil in sistem, si pozitia de start a memoriei 
	impreuna cu inserarea pocesului in memorie. In cazul in care nu exista 
	niciun proces in coada de running atunci procesul este inserat in acea 
	coada, daca nu este inserat in coada de waiting dupa criteriile ceurte 
	cu ajutorul functiei Ins_Waiting. In cazul incare in functia de find_start
	nu s-a gasit niciun loc suficient de mare intre procesele existente si 
	procesul nu are loc nici la final se incearca o defragmentare a memoriei.
	Astfel fiecare elemnt al listei este reinserat ordonat crescator dupa pid
	intr-o alta lista generica auxiliara si pozitiile de start ale memoriei
	sunt recalculate. Daca nici dupa aceasta actiune procesul nu are loc la 
	sfarsit se va afisa un mesaj de eroare.
		2.	Functia get: Cauta dupa pid procesul din sistem activ sau procesele
	terminate dupa pid. Am folosit trei functii auxiliare, una ce verifica daca
	procesul se afla in starea running, si altele 2 ce cauat in cozile de 
	waiting si finish. Daca procesul/procesele sun gasite functiile intorc 
	valoarea 1, altfel 0. In acest mod stiu daca un proces a fost sau nu gasit.
		3.	Functia push: Cauta dupa pid, procesul in lista de memorie a
	sistemului. Odata gasit, se veriica daca exista loc in stiva, daca da 
	atunci elementul este adaugat in stiva. In final se verifica si daca 
	procesul a fost gasit sau nu.
		4.	Functia pop: Este asemanatoare cu functia push doar ca aceasta va
	verifica daca stiva este goala, daca nu se extrage primul elemnt din stiva,
	si se face free.
		5.	Functia print stack: Cauta in lista de memorie a sistemului procesul
	dupa pidul dat parametru. Daca a fost gasit se verifica daca stiva este goala
	sau nu. Fiecare elemt din stiva este inserat intr-o stiva auxiliara, urmand o 
	alta parcurgere a listei, nu prin iterare ci prin push-pop, iar fiecare 
	element este afisat si reinodus in stiva initiala, astfel ordinea va fi
	neschimbata.
		6.	Functiile print waiting si print finish, difera doar prin textul ce 
	trebuie afisat. Sunt tratate cazurile pentru pimul si ultimul elemnt din
	coada iar elemtele intermediate sunt etrase si reinserate dupa ce au fost
	afisate. Cozile sunt parcurse pana cand urmatorul elemnt extras din coada
	este adresa primului element din coada salvat la inceput.
		7.	Functia run: Initial functia se ocupa de procesul ce se afla in 
	starea de running. Se verifica urmatoare: daca procesul ia mai ramas sa 
	ruleze din cuanta, daca procesul are timpul mai mic decat cuanta ceea ce 
	inseamna ca se termina si este inserat in finish; daca nici unul din aceste
	cazuri nu a fost atins i se scade din timpul procesului cuanta de timp, 
	apoi se face extragerea din coada de waiting iar procesul de running este 
	inserat in waiting in mod ordonat. Urmatorul pas este rularea proceselor 
	din waiting pana cand timpul de rulare devine 0. Am determinat doua cazuri:
		->Timpul este mai mare decat cuanta: unde exista alte doua cazuri
			->timpul procesului este mai mare decat cuanta ceea ce inseamna 
			ca se va scadea atat din timp cat si din timpul procesului cuanta
			dupa care are loc o extrage si reinserare procesului din running in
			coada de waiting;
			->timpul procesului este mai mic decat cuanta cea ce inseamna ca 
			procesul acum aflat in rulare se va termina, din timp se va scadea
			valoarea timpului procesului, iar procesul este inserat in coada de
			finish si sters din memoria sistemului de operare;
		->Timpul este mai mic decat cuant: si aici sunt alte doua cazuri
			->timpul procesului este mai mic decat timpul ce trebuie rulat,
			atunci procesul se termina, este inserat in coada de finish si 
			sters din memoria sistemului de operare, iar din timp este scazuta 
			valoarea timpului procesului;
			->timpul este mai mic decat timpul procesului ceea ce inseamna ca
			rularea se va sfarsi. Se verifica daca s-a rulat toata cuanta, daca
			daca atunci in running va trece primul sistem din waiting, daca nu
			procesul va ramane in running cu cuanta ramasa de timp.
	Am tratat si cazurile exceptate cand numai exista niciun proces pentru 
	rulare in sistem.
		8.	Functia finish: termina toate procesele active in sistem, adica
	cele din starea running si din coada de waiting. Sunt sterse din lista
	sistemului de operare si inserate in coada de finish. Timpul total necesar
	se calculeaza insumand timpurile proceselor.
