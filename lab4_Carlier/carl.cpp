#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>
#include <ctime>
#include <string>
#include <math.h>
#define MAX 99999999

int Heap[MAX], pos = 0;  //kopiec i zmienna do operacji na kopcu
int strat=0;             //do wyboru strategii
int testy=0;             //do wyboru testów
int strategia=0;         //wybór strategii
int tab_kopc[500];       //aby ukazac z ktorej strony zostaly dane do kopca przekazane
int war=0;               //zmienna pomocnicza/indeks do kopca

using namespace std;

struct Zadanie{
	int nr,r,p,q,czasmaszyna;//nr-indeks zadania, czasmaszyna- czas po ktorym zadanie znajduje sie na maszynie
};

class obliczenia{
private:
    int start=MAX;
public:
    int r=start,p=0,q=r,suma=0;
    int pr=0,pq=0;

    void wartosci(const int&start, const int&koniec, const vector<Zadanie>& pi);
    };


///////////////////////////////////////////////////////////////////////////////////////////////////kopiec
///zeruje kopiec
void Czysckopiec(){for(int i=0;i<MAX;i++)Heap[i]=0;}

///zwraca indeks rodzica, v- indeks wierzcholka
int Parent(int v){return v-1-strategia;}

///zwraca indeks lewego dziecka, v- indeks wierzcholka
int LeftSon(int v){return v+strategia;}

///zwraca indeks prawego dziecka, v- indeks wierzcholka
int RightSon(int v){return v+strategia + 1;}

///wypisuje wierzcholek kopca, jesli istnieje
void Top (void){if (pos != 0)cout << Heap[1] << "\n";}

///wstawia podana wartosc do kopca
void Insert (int x)
{
    Heap[pos+1] = x;                           //wstaw na koniec
    pos++;                                     //zwieksz liczbe elementow kopca
    int v = pos;
    while (v != 1 and Heap[Parent(v)] < Heap[v]) //przywroc wlasciwosc kopca
        {
            swap (Heap[Parent(v)], Heap[v]);
            v=Parent(v);
        }
}

///Usuwa wierzcholek kopca
void Remove()
{
    if (pos==0)           //jezeli kopiec jest pusty zakoncz bez zadnej akcji
        return;
    Heap[1]=Heap[pos];    //wstaw ostatni element do wierzcholka
    pos--;                //zmniejsz liczbe elementow kopca
    int v = 1;
    while(true)           //przywroc wlasciwosc kopca
    {
        if (LeftSon(v) > pos)
            break;
        int a = Heap[LeftSon(v)];
        if (RightSon(v) > pos)
        {
            if (Heap[v] >= Heap[LeftSon(v)])
                break;
            swap (Heap[v], Heap[LeftSon(v)]);
            v=LeftSon(v);
        }
        else if (Heap[LeftSon(v)] > Heap[RightSon(v)])
        {
            if (Heap[v] >= Heap[LeftSon(v)])
                break;
            swap (Heap[v], Heap[LeftSon(v)]);
            v=LeftSon(v);
        }
        else
        {
            if (Heap[v] >= Heap[RightSon(v)])
                break;
            swap (Heap[v], Heap[RightSon(v)]);
            v=RightSon(v);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////kopiec koniec

///
void obliczenia::wartosci(const int& poczatek, const int& koniec, const vector<Zadanie>& pi)
{
    for(int i = poczatek; i <= koniec; ++i){
        if(pi[i].r < r){
            r = pi[i].r;
        }
        if(pi[i].q<q)
        {
            q=pi[i].q;
        }
        p = p + pi[i].p;
    }
    suma=p+q+r;
    pr=p+r;
    pq=p+q;
}
///porownywanie dwoch wartosci r
struct porownanieR{
bool operator() (const Zadanie & a, const Zadanie & b){return a.r < b.r;};
}porownanieR;
///porownywanie dwoch wartosci q
struct porownanieQ{
    bool operator() (const Zadanie & a, const Zadanie & b){return a.q > b.q;}
}porownanieQ;

vector < Zadanie > zadania,G,pigwiazdka;//lista wczytanych zadan, G-schrage i schrege z podzialem/skrypt, pigwiazdka- najlepsza permutacja/skrypt

///Schrage/skrypt:
///Struktury pomocnicze:
///t-chwila czasowa
///N-zbiór zadan nieuszeregowanych
///G-zbiór zadan gotowych do realizacji
///Cmax-maksymalny z terminów dostarczenia
///pi- permutacja wykonania zadan na maszynie
int shrage(vector <Zadanie> & pi)
{
	vector <Zadanie> N = zadania;
	pi.clear();
	sort(N.begin(), N.end(),porownanieR);
	Zadanie e;
	int t = 0, Cmax = 0;
	G.clear();
  	while(!G.empty() || !N.empty())
        {
            while(!N.empty() && N[0].r <= t)
                {
                    e = N[0];
                    G.push_back(e);
                    N.erase(N.begin());
                }
            if(G.empty())
                {
                    t = N[0].r;
                }
            else
                {
                    sort(G.begin(), G.end(),porownanieQ);
                    e = G[0];
                    e.czasmaszyna = t;
                    G.erase(G.begin());
                    pi.push_back(e);
                    t += e.p;
                    Cmax = max(Cmax, t + e.q);
                }
        }
  	return Cmax;
}
///Schrage z podzialem/skrypt:
///Struktury pomocnicze:
///t-chwila czasowa
///N-zbiór zadan nieuszeregowanych
///G-zbiór zadan gotowych do realizacji
///Cmax-maksymalny z terminów dostarczenia
///l-aktualnie wykonywane zadanie
///pi- permutacja wykonania zadan na maszynie
int shragePodz()
{
    vector <Zadanie> N = zadania;
	sort(N.begin(), N.end(),porownanieR);
	Zadanie e, l;               //
	l.q = l.p = l.r = 0;
	int t = 0, Cmax = 0;
	G.clear();
  	while(!G.empty() || !N.empty())
        {
            while(!N.empty() && N[0].r <= t)
                {
                    e = N[0];
                    G.push_back(e);
                    N.erase(N.begin());
                    if(e.q >= l.q)
                        {
                            l.p = t - e.r;
                            t = e.r;
                            if(l.p > 0)
                            G.push_back(l);
                        }
                }
            if(G.empty())
                {
                    t = N[0].r;
                }
            else
                {
                    sort(G.begin(), G.end(),porownanieQ);
                    e = G[0];
                    G.erase(G.begin());
                    l = e;
                    t += e.p;
                    Cmax = max(Cmax, t + e.q);
                }
        }
  	return Cmax;
}
///Funkcja dzielaca na blok(a,b) i wyznaczajaca zadanie referencyjne
void blok(int & a, int & b, int & j, const vector <Zadanie> & pi,int ilosczadan)
{
	int maksymalny = 0;
	j=-1;
	for(int i = 0; i < ilosczadan; ++i)
        {
            if(pi[i].czasmaszyna + pi[i].p + pi[i].q > maksymalny)
                {
                    b = i;
                    maksymalny = pi[i].czasmaszyna + pi[i].p + pi[i].q;
                }
        }
    for(a = b - 1; a >= 0; --a)
        {
            if(pi[a].czasmaszyna + pi[a].p < pi[a + 1].czasmaszyna)
                {
                    break;
                }
        }
    ///c- najwieksze a<=i<=b takie, ze q(i)<q(b)
	for(int i = b - 1; i >= a; --i)
        {
            if(pi[i].q < pi[b].q)
                {
                    j = i;
                    break;
                }
        }
}
///testy eliminacyjne
///UB-górne oszacowanie wartoœci funkcji celu
///pi- permutacja wykonania zadan na maszynie, w skrypcie PI
///n- ilosc zadan
int testyElim(const int&start, const int&koniec, const vector<Zadanie>& pi, int &UB,int n)
{
    int tmp;
    obliczenia poczatek;
    poczatek.wartosci(start,koniec,pi);
    for(int i=0;i<n;i++)
    {
        if(i<start)
        {
           // cout<<"i<start"<<endl;
            if( pi[i].p > UB - poczatek.suma )
            {
                tmp=pi[i].nr;
               // cout<<pi[i].r<<" "<<pi[i].p<<" "<<poczatek.p<<" "<<pi[koniec].q<<" "<<endl;
               // cout<<"ub "<<UB<<endl;

                  //  cout<<"1st if "<<pi[i].r + pi[i].p + poczatek.p + pi[koniec].q<<endl;
                   // cout<<"2nd if "<<poczatek.pr+pi[i].p+pi[i].q<<endl;
				if( pi[i].r + pi[i].p + poczatek.p + pi[koniec].q >= UB )
                    zadania[tmp].r=max(pi[i].r,poczatek.pr);
                    //cout<<"indeks zadania "<<zadania[tmp].r<<endl;}
                else if(poczatek.pr+pi[i].p+pi[i].q >= UB )
					zadania[tmp].q=max(pi[i].q,poczatek.pq);
					//cout<<"indeks konca" <<zadania[tmp].q<<endl;}
			}
        }
        else if(i>koniec)
        {
            //cout<<"i>koniec"<<endl;
            if( pi[i].p > UB - poczatek.suma )
            {
                tmp=pi[i].nr;
				if( pi[i].r + pi[i].p + poczatek.p + pi[koniec].q >= UB )

                    zadania[tmp].r=max(pi[i].r,poczatek.pr);
                else if(poczatek.pr+pi[i].p+pi[i].q >= UB )
					zadania[tmp].q=max(pi[i].q,poczatek.pq);
			}
        }
    }
}
///wybor strategii
///UB-górne oszacowanie wartoœci funkcji celu
///DB-dolne oszacowanie wartoœci funkcji celu
///pi- permutacja wykonania zadan na maszynie, w skrypcie PI
int wyborStrategii(const int&c, const int&b, vector<Zadanie>pi, int&DB, int UB){
	int DB1, DB2;
	obliczenia strateg;
	strateg.wartosci(c+1,b,pi);

	zadania[pi[c].nr].r = max(pi[c].r, strateg.pr);
	DB1 = shragePodz();
	if(DB1<UB) {tab_kopc[war]=1; war++; Insert(DB1);}
	zadania[pi[c].nr].r  = pi[c].r;

	zadania[pi[c].nr].q = max(pi[c].q, strateg.pq);
	DB2 = shragePodz();
	if(DB2<UB) {tab_kopc[war]=2; war++; Insert(DB2);}
	zadania[pi[c].nr].q = pi[c].q;

	//cout<<"DB1 "<<DB1<<" DB2 "<<DB2<<" UB "<<UB<<endl;

	if(testy==0){
	if(DB1 < DB2 && DB1<UB){
        DB = DB1;
        //Insert(DB);
        return 1;
	}
	else if(DB2 < UB){
        DB = DB2;
        return 2;
        //Insert(DB);
    }
    else
		return 0;
	}
}





///Algorytm Carlier
///Struktury pomocnicze:
///UB-górne oszacowanie wartoœci funkcji celu
///LB-dolne oszacowanie wartoœci funkcji celu
///Cmax-maksymalny z terminów dostarczenia
///U- wartosc funkcji celu
///pi- permutacja wykonania zadan na maszynie, w skrypcie PI
///pigwiazdka- optymalna pi zadan na maszynie, w skrypcie PI*
///c- zadanie referencyjne, jak w skrypcie
///n- ilosc zadan
void carlier(int & UB,int n)
{

	int U, a, b, c, Rpri, Qpri, Ppri, r0, q0, LB, index;
	vector <Zadanie> pi;
	obliczenia poczatek;
	obliczenia zmiana;
	U = shrage(pi);
	if( U < UB )
        {
            UB = U;
            pigwiazdka = pi;
        }



	///wyznaczanie bloku(a,b) oraz zadania referencyjnego c
	blok(a, b, c, pi,n);
	if( c == -1 )
		return;
    ///wyznaczanie r' q' i p'
    ///////////////////////////////////////
    poczatek.wartosci(c+1,b,pi);
	/////////////////////////////////////
	///gdy wybrano testy eliminacyjne
	if(testy==0) testyElim(c+1, b, pi, UB,n);
	///gdy wybrano testy strategie
	strategia = wyborStrategii(c, b, pi, LB, UB);

	//cout<<strategia<<endl;
	//cout<<strategia;
	/////////////////////////////////////
	index = pi[c].nr;
	///jesli brak strategii i testow, to normalny Carlier
	if(strategia==1 || strat==1)
    {
	///podstawiamy pod r max z r(pi) i r'+p'
    zadania[index].r = max(pi[c].r, poczatek.r+poczatek.p);
    carlier(UB,n);
    ///odtworzenie oryginalnego r
    zadania[index].r  = pi[c].r;
    ///podstawiamy pod q max z q(pi) i q'+p'
    zadania[index].q = max(pi[c].q, poczatek.q + poczatek.p);
    ////////////////////////////////////
    ///wyznaczanie nowszych r' q' i p'
    zmiana.wartosci(c,b,pi);
    ///wyznaczenie dolnego oszacowania
	LB=max(zmiana.suma,poczatek.suma);
	/////////////////////////////////////
    if( LB < UB )
        {
            carlier(UB,n);
        }
    ///odtworzenie oryginalnego q
    zadania[index].q = pi[c].q;
    }
    else if(strategia==2)
    {
        	///podstawiamy pod r max z r(pi) i r'+p'
    zadania[index].q = max(pi[c].q, poczatek.pq);
    carlier(UB,n);
    ///odtworzenie oryginalnego r
    zadania[index].q  = pi[c].q;
    ///podstawiamy pod q max z q(pi) i q'+p'
    zadania[index].r = max(pi[c].q, poczatek.pr);
    ////////////////////////////////////
    ///wyznaczanie nowszych r' q' i p'
    zmiana.wartosci(c,b,pi);
    ///wyznaczenie dolnego oszacowania
	LB=max(zmiana.suma,poczatek.suma);
	/////////////////////////////////////
    if( LB < UB )
        {
            carlier(UB,n);
        }
    ///odtworzenie oryginalnego q
    zadania[index].q = pi[c].q;
    }



}


int main(){
	int maxi,ilosczadan;
	fstream plik;
	ofstream plik2;
	plik.open("Data000.txt");
	plik2.open("Dataoutput2.txt");//"Dataoutput.txt",
	cout<<"Wybierz metode strategii\n 0-wlasciwa \n 1-wide:left"<<endl;
	cin>>strat;
	cout<<"uwzglednienie testow eliminacji:\n0-uwzglednienie\n1-brak testow"<<endl;
	cin>>testy;
	for(int ilosc = 0; ilosc <= 8; ++ilosc)
        {
            Czysckopiec();
            clock_t start = clock();
            cout<<endl<<"data."<<ilosc<<endl;
            plik2<<endl<<"data."<<ilosc<<endl;
            maxi = MAX;
			Zadanie zad;
            plik >> ilosczadan;
            for(int i = 0; i < ilosczadan; ++i)
                {
                    zad.nr = i;
                    plik >> zad.r >> zad.p >> zad.q;
                    zadania.push_back(zad);
                }
            carlier(maxi,ilosczadan);
            cout << "carl: \n" << maxi << endl;
            plik2 << "carl: \n" << maxi << endl;
            for(int i = 0; i < ilosczadan; i++){
                cout << pigwiazdka[i].nr << " ";
                plik2 << pigwiazdka[i].nr << " ";
            }
            cout<<endl;
            plik2<<endl;
            zadania.clear();
            G.clear();
            pigwiazdka.clear();
            printf( "\nCzas wykonywania ogolem: %lu ms\n", clock() - start );
            cout<<"Kopc"<<endl;
            for(int i=0;i<100;i++)
            {
                if(i>0)
                    if(Heap[i]==0)
                        break;
                        if(tab_kopc[i]==1) cout<<" left: ";
                        else if(tab_kopc[i]==2) cout<<" right: ";
                    cout<<Heap[i]<<" ";
                    Remove();
            }
        }

	plik.close();
	plik2.close();
	return 0;
}
