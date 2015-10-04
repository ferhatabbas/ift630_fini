#include <string>
#include <chrono>
#include <iostream>
using namespace std;

class Timer{
  std::chrono::time_point<std::chrono::system_clock> start;
  std::chrono::time_point<std::chrono::system_clock> end;
 public:
  Timer(){
    start = std::chrono::system_clock::now();
  }
  ~Timer(){
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> el_sec = end-start;
    std::cout << "Time " << el_sec.count() << " s.\n";
  }
};


string ADD(string mot, int cle)
{
	int taille;
	taille = mot.length();
	for (int i = 0; i < taille; i++)
		mot[i] = mot[i] + cle * i;
	return mot;
}

string XOR(string mot, string cle)
{
	int taille;
	taille = mot.length();
	for (int i = 0; i < taille; i++)
		mot[i] = mot[i] ^ cle[i];
	return mot;
}
/*
Ce t t e f onc t i on e f f e c t u e un d e c a l a g e c i r c u l a i r e
v e r s l a d r o i t e de "nb" c a r a c t e r e s
*/
string decale(string mot,const int nb)
{
	string t;
	t.resize(nb);
	int max = mot.length()-1;
	for (int i = 0; i < nb; i++)
		t[i] = mot[max-i];
	for (int i = max; i >= nb; i--)
		mot[i] = mot[i-nb];
	for (int i = 0; i < nb; i++)
		mot[i] = t[nb-i-1];
	return mot;
}

/*Ce t t e f onc t i on e f f e c t u e une t r a n s l a t i o n
ent r e l e s c a r a c t e r e s du mot d ' une d i s t anc e
d e d u i t e de l a cle 3
*/
string echange(string mot, int cle)
{
	int taille = mot.length();
	for (int i = 0; i + cle < taille; i++)
	{
		char temp;
		temp = mot[i];
		mot[i] = mot[i + cle];
		mot[i + cle] = temp;
	}
	return mot;
}

/*
Ce t t e f onc t i on genere une cle numerique
*/
int getCle(string mot)
{
	int cle, temp;
	temp = 0;
	for (int i = 0; i < mot.length(); i++)
		temp = temp + (unsigned int)mot[i];
	cle = (temp % 4) + 1;
	return abs(cle);
}

/*
Cette fonction encode le message
*/
string encode(string mot)
{
	string copie;
	int cle;
	copie = mot;
	for (int j = 0; j < 3; j++)
	{
		cle = getCle(mot);
		mot = decale(mot, cle / 2);
		mot = ADD(mot, cle);
		mot = echange(mot, cle);
		mot = XOR(mot, copie);
	}
	return mot;
}