#include<iostream>
#include <fstream>
#include <vector>
using namespace std;
class Amunicja
{
	public:
		Amunicja();
		~Amunicja();
		int Kaliber;
		int Ilosc_sztuk;
		string Rodzaj_zaplonu;
		string Typ_broni;
		void Dodaj_amunicje(int ilosc);
		void Usun_zuzyta_amunicje(int ilosc);
		void Generuj_raport(vector<Amunicja>amunicja);
};
class Amunicja_do_broni_krotkiej: public Amunicja
{
	public:
		Amunicja_do_broni_krotkiej(int a,string zaplon);
		~Amunicja_do_broni_krotkiej();
};
class Amunicja_do_broni_dlugiej: public Amunicja
{
	public:
		Amunicja_do_broni_dlugiej(int a,string zaplon);
		~Amunicja_do_broni_dlugiej();
};
class Amunicja_do_broni_gladkolufowej: public Amunicja
{
	public:
		Amunicja_do_broni_gladkolufowej(int a,string zaplon);
		~Amunicja_do_broni_gladkolufowej();
};
Amunicja::Amunicja()
{
}
Amunicja::~Amunicja()
{
}
Amunicja_do_broni_krotkiej::Amunicja_do_broni_krotkiej(int a,string zaplon)
{
	Kaliber=a;
	Rodzaj_zaplonu=zaplon;
	Ilosc_sztuk=0;
	Typ_broni="Krotka";
	cout<<"dodano kaliber "<<Kaliber<<" Zaplon "<<Rodzaj_zaplonu<<endl;
}
Amunicja_do_broni_krotkiej::~Amunicja_do_broni_krotkiej()
{
}
Amunicja_do_broni_dlugiej::Amunicja_do_broni_dlugiej(int a,string zaplon)
{
	Kaliber=a;
	Rodzaj_zaplonu=zaplon;
	Ilosc_sztuk=0;
	Typ_broni="Dluga";
	cout<<"dodano kaliber "<<Kaliber<<" Zaplon "<<Rodzaj_zaplonu<<endl;
}
Amunicja_do_broni_dlugiej::~Amunicja_do_broni_dlugiej()
{
}
Amunicja_do_broni_gladkolufowej::Amunicja_do_broni_gladkolufowej(int a,string zaplon)
{
	Kaliber=a;
	Rodzaj_zaplonu=zaplon;
	Ilosc_sztuk=0;
	Typ_broni="Gladkolufowa";
	cout<<"dodano kaliber "<<Kaliber<<" Zaplon "<<Rodzaj_zaplonu<<endl;
}
Amunicja_do_broni_gladkolufowej::~Amunicja_do_broni_gladkolufowej()
{
}
void Amunicja::Dodaj_amunicje(int ilosc)
{
	Ilosc_sztuk=Ilosc_sztuk+ilosc;
	cout<<"dodano Amunicje"<<endl;
}
void Amunicja::Usun_zuzyta_amunicje(int ilosc)
{
	Ilosc_sztuk=Ilosc_sztuk-ilosc;
	cout<<"usunieto Amunicje"<<endl;
}
void Amunicja::Generuj_raport(vector<Amunicja>amunicja)
{
	ofstream zapisz("Raport.txt");
	for( int i = 0; i < amunicja.size(); i++ )
    {
    	zapisz<<"Bron "<<amunicja[i].Typ_broni<<" Kaliber: "<<amunicja[i].Kaliber<<" Zaplon: "<<amunicja[i].Rodzaj_zaplonu<<" Ilosc sztuk: "<<amunicja[i].Ilosc_sztuk<<endl;
    }
	zapisz.close();
}
main()
{
	vector<Amunicja>testowa;
	ifstream plik("Wejscie.txt");
	string rodzaj,zaplon;
	int kaliber;
	while(!plik.eof())
	{
		plik>>rodzaj;
		plik>>kaliber;
		plik>>zaplon;
		if(rodzaj=="krotka")
		{
			testowa.push_back( Amunicja_do_broni_krotkiej(kaliber, zaplon) );
		}
		else if(rodzaj=="dluga")
		{
			testowa.push_back( Amunicja_do_broni_dlugiej( kaliber,zaplon ) );
		}
		else if(rodzaj=="gladkolufowa")
		{
			testowa.push_back( Amunicja_do_broni_gladkolufowej(kaliber, zaplon) );
		}
		else
		{
			cout<<" bledna nazwa";
		}
	}
	plik.close();
	testowa[0].Generuj_raport(testowa);
}
