/*
	Bu program Abdullah Var�c� taraf�ndan yaz�lm��t�r.
	Benim ve program hakk�nda detayl� bilgi i�in:
	abdullahvarici.blogspot.com
	Bu program� ki�isel ihtiyac�n�za uygun olarak d�zenleyip kullanabilirsiniz.
	
	Program sizden altyaz�.srt dosyas�n� al�r, i�erisinden kelimeleri �eker ve 
	tekrar say�s�yla birlikte s�ral� bir liste haline getirir. Bir de sizden 
	"bildiklerim.txt" isminde anlam�n� bildi�iniz kelimelerin i�inde bulundu�u 
	bir nevi sizin �ngilizce kelime haznenizi yans�tan bir dosyay� al�yor ayn� 
	i�lemleri buna da uyguluyor. Daha sonra bu iki dosyadaki kelimeleri kar��la�t�r�yor. 
	�ki listedeki kelimeleri fark, kesi�im ve birle�im k�mesi olarak farkl� 
	belgelere yaz�yor. Sonras�nda altyaz�da bulunan ama anlam�n� bilmedi�iniz 
	kelimeleri teker teker herhangi bir �ekim eki ald�ysa yal�n hale �eviriyor 
	(bunu hem veritaban� kullanarak hem de grammer kurallar�n� kullanarak yap�yor) 
	ve bu kelimelerin anlamlar�n� s�zl�k veritaban�ndan kontrol edip yeni bir 
	liste olu�turuyor. Son haldeki listemiz dizide ge�en anlam�n� bilmedi�imiz 
	kelimelerini, bu kelimelerin yal�n hallerini, dizide tekrar etme miktar�n� ve 
	s�zl�kteki anlamlar�n� i�eriyor. Bu program� diziye ba�lamadan �nce �al��t�r�p, 
	anlam�n� bilmedi�iniz kelimelerin bir k�sm�na �al��t�ktan sonra �al��t�rarak 
	�ngilizcemizi, en az�ndan �ngilizce kelime bilgimizi rahat�a geli�tirebiliriz. 
	Bu program�n hedefi dizi film ve altyaz� ile k�s�tl� de�il farkl� ama�lar 
	i�in de kullan�labilir. Mesela iki farkl� �ngilizce metni kar��la�t�rmak, 
	kelimelerin tekrarl� listesini ��kar�p anlamlar�na ula�mak i�in de rahat�a kullan�labilir.
*/

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;

#define UZUNLUK 30			// Max. kelime uzunlu�u

struct Kelime{				// Kelimelere ait struct yap�s� olu�turuluyor.
	string kelime_ekli;		// Yal�n olmayan, ek bulundurabilen kelime. �r: goes, books, book;
	string kelime_yalin;	// kelime_ekli'deki kelimenin yal�n hali. �r: go, book, book;
	string anlam;			// Kelimenin s�zl�kteki anlam�. �r: f. (went, gone) 1. gitmek. 2. -e ��kmak: She�s gone... 
	int tekrar;				// Kelimenin altyaz�daki tekrar miktar�.
	int toplam_tekrar;		// �ki metin kar��la�t�r�ld���ndaki kelimenin toplam tekrar miktar�.
	bool kiyas_durumu;		// �ki metini kar��la�t�r�rken ihtiya� duyuyoruz. 0 -> default ve ortak de�il. 1 -> Kelime iki metinde de mevcut.
	bool sozluk_durumu;		// Kelime s�zl�kte varsa 1, yoksa 0.
	bool cekim_durumu;		// Kelime conjuge ve �o�ullar listesinde varsa 1, yoksa 0
};

void vector_oku(char adres[100], vector <Kelime>* kelimeler);	
void vector_alfabetik_sirala(vector <Kelime>* kelimeler);
void vector_ekrana_yaz_anlamsiz(vector <Kelime> kelimeler);
void vector_dosyaya_yaz_anlamsiz(char adres[100], vector <Kelime> kelimeler);
void vector_tekrari_sil(vector <Kelime>* kelimeler);
void vector_tekrara_gore_sirala(vector <Kelime>* kelimeler);
void vector_karsilastir(vector <Kelime>* kelimeler_1, vector <Kelime>* kelimeler_2, vector <Kelime>* kelimeler_3, 
						vector <Kelime>* kelimeler_4, vector <Kelime>* kelimeler_5, vector <Kelime>* kelimeler_6);
void sozluk_oku(char adres[100], vector <Kelime>* kelimeler);
void cekimli_oku(char adres[100], vector <Kelime>* kelimeler);
int anlamlari_bul(vector <Kelime> kelimeler, vector <Kelime> sozluk_kelimeler, vector <Kelime> cekimli_kelimeler, vector <Kelime>* anlamli_kelimeler);
void anlamlari_duzenle(vector <Kelime>* anlamli_kelimeler);
void vector_ekrana_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler);
void vector_dosyaya_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler);


int main(){
	setlocale(LC_ALL, "Turkish");
		
	vector <Kelime> sozluk_vector;
	vector <Kelime> cekimli_vector;	
	vector <Kelime> altyazi_vector;
	vector <Kelime> bildiklerim_vector;
	vector <Kelime> afb;
	vector <Kelime> bfa;
	vector <Kelime> anb;
	vector <Kelime> aub;
	vector <Kelime> anlamli_vector;
	int bulunan;
		
	cout << "S�zl�kteki kelimeler taran�yor..." << endl;
	sozluk_oku("s�zl�k.txt", &sozluk_vector);
	cout << "S�zl�kte  <" << sozluk_vector.size() << ">  adet kelime bulundu."<< endl << endl;
	
	cout << "�ekimli kelimeler listesi taran�yor..." << endl;
	cekimli_oku("�ekimli kelimeler.txt", &cekimli_vector);
	cout << "�ekimli kelimeler listesinde  <" << cekimli_vector.size() << ">  adet kelime bulundu."<< endl << endl;
	
	cout << "altyaz�.srt'deki kelimeler al�n�yor..." << endl;
	vector_oku("altyaz�.srt", &altyazi_vector);
	cout << "altyaz�.srt'den toplam  <" << altyazi_vector.size() << ">  adet kelime al�nd�." << endl << endl;
	
	cout << "bildiklerim.txt'deki kelimeler al�n�yor..." << endl;
	vector_oku("bildiklerim.txt", &bildiklerim_vector);
	cout << "bildiklerim.txt'den toplam  <" << bildiklerim_vector.size() << ">  adet kelime al�nd�." << endl << endl;
	
	cout << "Listeler alfabetik s�ralan�yor..." << endl;
	vector_alfabetik_sirala(&altyazi_vector);
	vector_alfabetik_sirala(&bildiklerim_vector);
	
	cout << "Tekrars�z listeler olu�turuluyor..." << endl << endl;
	vector_tekrari_sil(&altyazi_vector);
	vector_tekrari_sil(&bildiklerim_vector);
	
	cout << "altyaz�.srt'de  <" << altyazi_vector.size() << ">  adet 'farkl�' kelime var." << endl;
	cout << "bildiklerim.txt'de  <" << bildiklerim_vector.size() << ">  adet 'farkl�' kelime var." << endl << endl; 
	
	cout << "altyaz�.srt'den alfabetik liste a_alf.txt dosyas�na yazd�r�l�yor..." << endl;
	vector_dosyaya_yaz_anlamsiz("a_alf.txt", altyazi_vector);
	cout << "bildiklerim.txt'den alfabetik liste b_alf.txt dosyas�na yazd�r�l�yor..." << endl << endl;
	vector_dosyaya_yaz_anlamsiz("b_alf.txt", bildiklerim_vector);
	
	cout << "Listeler tekrara g�re s�ralan�yor..." << endl;
	vector_tekrara_gore_sirala(&altyazi_vector);
	vector_tekrara_gore_sirala(&bildiklerim_vector);
	
	cout << "altyaz�.srt'den tekrar s�ral� liste a_tek.txt dosyas�na yazd�r�l�yor..." << endl;
	vector_dosyaya_yaz_anlamsiz("a_tek.txt", altyazi_vector);
	cout << "bildiklerim.txt'den tekrar s�ral� liste b_tek.txt dosyas�na yazd�r�l�yor..." << endl << endl;
	vector_dosyaya_yaz_anlamsiz("b_tek.txt", bildiklerim_vector);
	
	vector_alfabetik_sirala(&altyazi_vector);
	vector_alfabetik_sirala(&bildiklerim_vector);
	
	cout << "a_alf.txt ve b_alf.txt'deki kelimeler kar��la�t�r�l�yor..." << endl;
	vector_karsilastir(&altyazi_vector, &bildiklerim_vector, &afb, &bfa, &anb, &aub);	
	vector_dosyaya_yaz_anlamsiz("afb.txt", afb);
	vector_dosyaya_yaz_anlamsiz("bfa.txt", bfa);
	vector_dosyaya_yaz_anlamsiz("anb.txt", anb);
	vector_dosyaya_yaz_anlamsiz("aub.txt", aub);
	cout << "Listeler kar��la�t�r�ld� ve dosyalara yaz�ld�." << endl;
	cout << "afb.txt=a fark b | bfa.txt=b fark a | anb.txt=a kesi�im b | aub.txt=a birle�im b" << endl;
	cout << "Son durumda a, b, afb, bfa, anb, aub listelerindeki kelime say�lar�:" << endl;
	cout <<"a: "<<altyazi_vector.size() <<"   "<<"b: "<< bildiklerim_vector.size()<<"   "<<"afb: "<< afb.size();
	cout <<"   "<<"bfa: "<< bfa.size()<<"   "<<"anb: "<< anb.size()<<"   "<<"aub: "<< aub.size() << endl << endl;
	cout << "altyaz�.srt'deki kelimelerin  %" << (float)anb.size()/altyazi_vector.size()*100 << "'�n� zaten biliyorsun." << endl <<endl;
	
	cout << "afb.txt'deki kelimelerin anlamlar� al�n�yor..." << endl;
	bulunan = anlamlari_bul(afb, sozluk_vector, cekimli_vector, &anlamli_vector);
	cout << "S�zl�kten   " << bulunan << "  adet kelimenin anlam�na bak�ld�."<< endl;
	cout << afb.size()-bulunan << " adet kelime s�zl�kte ve �ekimli kelime listesinde bulunamad�. Oran: "; 
	cout << "%"<< (float)bulunan/afb.size()*100 << endl << endl;
	
	cout << "Kelimeler ve anlamlar� listesi olu�turuluyor..." << endl;
	anlamlari_duzenle(&anlamli_vector);
	vector_dosyaya_yaz_anlamli("afb_anlamlar_alf.txt", anlamli_vector);
	cout << "afb_anlamlar_alf.txt dosyas�nda alfabetik liste olu�turuldu." << endl << endl;

	cout << "Tekrara dayal� liste olu�turuluyor..." << endl;
	vector_tekrara_gore_sirala(&anlamli_vector);
	vector_dosyaya_yaz_anlamli("afb_anlamlar_tek.txt", anlamli_vector);
	cout << "afb_anlamlar_tek.txt dosyas�nda tekrara g�re s�ral� liste olu�turuldu." << endl << endl;
	
	cout <<endl << endl << "Burada bitti i�imiz. �yi �al��malar..." << endl;
		
	getchar();
}

// Altyaz� dosyas�n� a�ar. ��inden 2 ila 30 karakter b�y�kl���ndeki kelimeleri vector'e atar. 
// B�t�n harfleri k���k harfe d�n��t�r�r ve ingilizcedeki 26 karakter haricindeki karakterleri almaz; 
// denk geldi�inde kelimenin o karaktere kadar olan k�sm�n� al�r.
// Kelimeye ait tekrar ba�lang�� de�erini 1; cekim_durumu ve sozluk_durumu de�erlerini 0 olarak atar.
void vector_oku(char adres[100], vector <Kelime>* kelimeler){		
	FILE *pRead;
	if((pRead = fopen(adres, "r")) == NULL){
		cout << adres <<" a��lamad�.";
	}
	
	int k=0;
	char harf, temp[UZUNLUK];
	Kelime tempStruct;
	while(!feof(pRead)){
		memset(temp, 0, UZUNLUK);
		fscanf(pRead, "%c", &harf);
		
		if((harf<='z' && harf>='a') || (harf<='Z' && harf>='A')){
			for(int i=0;i<UZUNLUK;i++){
				if(harf<='z' && harf>='a')
					temp[i]=harf;
				else if(harf<='Z' && harf>='A')
					temp[i]=harf+32;
				else{
					if(strlen(temp)>2){
						tempStruct.kelime_ekli = temp;
						tempStruct.tekrar = 1;
						tempStruct.cekim_durumu = 0;
						tempStruct.sozluk_durumu = 0;
						kelimeler->push_back(tempStruct);
					}
					break;
				}
				fscanf(pRead, "%c", &harf);
			}
		}
	}
	fclose(pRead);
}

// Ald��� vector'deki kelimelerin kelime_ekli de�i�kenini baz alarak alfabetik olarak s�ralama yapar. 
void vector_alfabetik_sirala(vector <Kelime>* kelimeler){
	Kelime tempStruct;
	for(int i=0; i<kelimeler->size(); i++){
		for(int j=0; j<kelimeler->size()-1; j++){
			if((*kelimeler)[j].kelime_ekli > (*kelimeler)[j+1].kelime_ekli){
				tempStruct = (*kelimeler)[j];
				(*kelimeler)[j] = (*kelimeler)[j+1];
				(*kelimeler)[j+1] = tempStruct;
			}
		}
	}
}

// Ald��� vector'deki s�ralamaya uygun olarak (alfabetik - tekrara dayal� - hi�biri) tekrar� ve kelimeyi konsol ekran�na yazar.
void vector_ekrana_yaz_anlamsiz(vector <Kelime> kelimeler){
	for(int i=0; i<kelimeler.size(); i++)
		cout <<"("<< kelimeler[i].tekrar <<")\t"<< kelimeler[i].kelime_ekli << endl;	
}

// Ald��� vector'deki s�ralamaya uygun olarak (alfabetik - tekrara dayal� - hi�biri) tekrar� ve kelimeyi dosyaya yazar.
void vector_dosyaya_yaz_anlamsiz(char adres[100], vector <Kelime> kelimeler){
	ofstream outFile(adres);
	for(int i=0; i<kelimeler.size(); i++)
		outFile <<"("<< kelimeler[i].tekrar <<")\t"<< kelimeler[i].kelime_ekli << endl;
	outFile.close();
}

// Ald��� vector'deki tekrar eden kelimelerin tekrar say�s�n� hesaplar ve tekrars�z bir liste olu�turur. 
void vector_tekrari_sil(vector <Kelime>* kelimeler){
	vector_alfabetik_sirala(kelimeler);
	for(int i=0; i<kelimeler->size()-1; i++){
		if((*kelimeler)[i].kelime_ekli == (*kelimeler)[i+1].kelime_ekli){
			(*kelimeler)[i].tekrar++;
			kelimeler->erase(kelimeler->begin()+i+1);
			i--;
		}
	}
}

// Ald��� vector'u kelimelerin tekrar say�s�na g�re �oktan aza do�ru s�ralar.
// Vector'un tekrar etmeyen ve tekrar say�s� hesaplanm�� kelimelerden olu�tu�una emin olunuz. 
void vector_tekrara_gore_sirala(vector <Kelime>* kelimeler){
	Kelime tempStruct;
	for(int i=0; i<kelimeler->size(); i++){
		for(int j=0; j<kelimeler->size()-1; j++){
			if((*kelimeler)[j].tekrar < (*kelimeler)[j+1].tekrar){		// "<" yerine ">" kullanarak azdan �o�a do�ru s�ramlam�� olursun
				tempStruct = (*kelimeler)[j];
				(*kelimeler)[j] = (*kelimeler)[j+1];
				(*kelimeler)[j+1] = tempStruct;
			}
		}
	}	
}

// S�ras�yla a b afb bfa anb aub vector'lerini ald���n� d���nelim. 
// afb->a fark b	bfa->b fark a	anb->a kesi�im b	aub->a birle�im b
// a ve b vector'leri herhangi iki farkl� altyaz�dan al�nan kelimelerden olu�mu� ve s�ralanm�� olmal�d�r.
// a ve b vector'leri kar��la�t�r�l�r. a'da bulunan ama b'de bulunmayan kelimelerin listesi afb'ye atan�r.
// b'de bulunan ama a'da bulunmayan kelimelerin listesi bfa'ya atan�r. Kelimelerdeki tekrar say�lar� korunur ve atan�r.
// Hem a'da hem b'de bulunan kelimelerin listesi anb'ye atan�r ve anb'deki kelimelerin tekrar say�s� a'daki ve b'deki tekrar say�s�n�n toplam�na e�itlenir.
// afb, anb ve bfa vector'leri oldu�u gibi birle�tirilip aub vector'�ne atan�r.
void vector_karsilastir(vector <Kelime>* kelimeler_1, vector <Kelime>* kelimeler_2, vector <Kelime>* kelimeler_3, 
					vector <Kelime>* kelimeler_4, vector <Kelime>* kelimeler_5, vector <Kelime>* kelimeler_6){
	for(int i=0; i<kelimeler_1->size(); i++){
		(*kelimeler_1)[i].kiyas_durumu = 0;
	}
	for(int i=0; i<kelimeler_2->size(); i++){
		(*kelimeler_2)[i].kiyas_durumu = 0;
	}
	
	for(int i=0; i<kelimeler_1->size(); i++){
		for(int j=0; j<kelimeler_2->size(); j++){
			if((*kelimeler_1)[i].kelime_ekli == (*kelimeler_2)[j].kelime_ekli){
				(*kelimeler_1)[i].kiyas_durumu = 1;
				(*kelimeler_2)[j].kiyas_durumu = 1;
				(*kelimeler_1)[i].toplam_tekrar = (*kelimeler_1)[i].tekrar + (*kelimeler_2)[j].tekrar;
				(*kelimeler_2)[j].toplam_tekrar = (*kelimeler_1)[i].toplam_tekrar;
				break;
			}
		}
	}
	
	int k=0;
	for(int i=0; i<kelimeler_1->size(); i++){
		if((*kelimeler_1)[i].kiyas_durumu == 0){
			kelimeler_3->push_back((*kelimeler_1)[i]);
		}
		else{
			kelimeler_5->push_back((*kelimeler_1)[i]);
			(*kelimeler_5)[k].tekrar = (*kelimeler_5)[k].toplam_tekrar;
			k++;
		}
	}
		
	for(int i=0; i<kelimeler_2->size(); i++){
		if((*kelimeler_2)[i].kiyas_durumu == 0){
			kelimeler_4->push_back((*kelimeler_2)[i]);
		}
	}
	
	for(int i=0; i<kelimeler_3->size(); i++){
		kelimeler_6->push_back((*kelimeler_3)[i]);
	}
	for(int i=0; i<kelimeler_4->size(); i++){
		kelimeler_6->push_back((*kelimeler_4)[i]);
	}
	for(int i=0; i<kelimeler_5->size(); i++){
		kelimeler_6->push_back((*kelimeler_5)[i]);
	}
	
}

// S�zl�k metnini a�ar ve kelimeleri kelime_yalin'e anlamlar�n� anlam'e atar.
void sozluk_oku(char adres[100], vector <Kelime>* kelimeler){
	ifstream input(adres);
	Kelime temp;
	while(getline(input, temp.kelime_yalin)){
		getline(input, temp.anlam);
		kelimeler->push_back(temp);
	}
}

// Conjuge ve �o�ullar metnini a�ar. Kelimelerin yal�n hallerini kelime_yalin'e, ek bulundaran halini kelime_ekli'e atar.
void cekimli_oku(char adres[100], vector <Kelime>* kelimeler){
	ifstream input(adres);
	Kelime temp;
	while(getline(input, temp.kelime_yalin)){
		getline(input, temp.kelime_ekli);
		kelimeler->push_back(temp);
	}
}

// Bu fonksiyonun ilk arg�menti tarama yapaca�� alfabetik s�ral� ve tekrars�z kelimelerin bulundu�u altyaz� ya da kar��la�t�r�lm�� vector'lerden biridir.
// ikinci argument s�zl�k vector'�; ���nc�s� conjuge ve �o�ullar vector'� ve sonuncusu da sonu� vector'�d�r.
// Bu fonksiyondan �nce sozluk_oku ve cekimli_oku fonksiyonlar� �a��r�lmal�d�r.
// Biraz anlamli ve kar���k olan bu fonksiyon �zet olarak �nce kelimelerin kelime_ekli'lerini s�zl�kteki kelime_yalin'lerle kar��la�t�r�p;
// bulduklar�n� anlamlar�yla beraber sonu� vector'�ne yazar. 
// Ayn� zamanda s�zc�klerin ek alm�� olma olas�l��a kar��n conjuge ve �o�ullar listesinde aray�p tekrar s�zl�kte arar ve 
// yine bu kelimelere ait yal�n hallerini ve anlamlar�n� sonu� vector'�ne atar.
// Fonksiyon kelimelerin ka��n� s�zl�k veya conjuge listesinde bulduysa sonucu return eder.
int anlamlari_bul(vector <Kelime> kelimeler, vector <Kelime> sozluk_kelimeler, vector <Kelime> cekimli_kelimeler, vector <Kelime>* anlamli_kelimeler){
	Kelime temp;
	int bulunan = kelimeler.size();
	int flag = 0;
	int flag1 = 0;
	int flag2 = 0;
	int flag3 = 0;
	string kelime;
	
	for(int i=0;i<kelimeler.size(); i++){
		for(int j=0; j<sozluk_kelimeler.size(); j++){
			if(kelimeler[i].kelime_ekli == sozluk_kelimeler[j].kelime_yalin){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = kelimeler[i].kelime_ekli;
				temp.tekrar = kelimeler[i].tekrar;
				temp.anlam = sozluk_kelimeler[j].anlam;
				anlamli_kelimeler->push_back(temp);
				flag1 = 1;
				break;
			}
		}
		
		for(int k=0; k<cekimli_kelimeler.size(); k++){
			if(kelimeler[i].kelime_ekli == cekimli_kelimeler[k].kelime_ekli){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = cekimli_kelimeler[k].kelime_yalin;
				temp.tekrar = kelimeler[i].tekrar;
				
				for(int m=0; m<sozluk_kelimeler.size(); m++){
					if(temp.kelime_yalin == sozluk_kelimeler[m].kelime_yalin){
						temp.anlam = sozluk_kelimeler[m].anlam;
						flag = 1;
						flag2 = 1;
					}
				}
				
				if(flag == 0)
					temp.anlam = "??????????";
					
				flag = 0;	
				anlamli_kelimeler->push_back(temp);
				break;
			}
		}
		
		if(flag1==0 && flag2==0){
			for(int j=0; j<sozluk_kelimeler.size(); j++){
				if(kelimeler[i].kelime_ekli == sozluk_kelimeler[j].kelime_yalin+"s"){
					temp.kelime_ekli = kelimeler[i].kelime_ekli;
					temp.kelime_yalin = sozluk_kelimeler[j].kelime_yalin;
					temp.tekrar = kelimeler[i].tekrar;
					temp.anlam = sozluk_kelimeler[j].anlam;
					anlamli_kelimeler->push_back(temp);
					flag3=1;
					break;
				}
			}
			
			if(flag3==0){
				temp.kelime_ekli = kelimeler[i].kelime_ekli;
				temp.kelime_yalin = "???";
				temp.tekrar = kelimeler[i].tekrar;
				temp.anlam = "??????????";
				anlamli_kelimeler->push_back(temp);
				bulunan--;
			}
		}
		flag1=0;
		flag2=0;
		flag3=0;
	}
	return bulunan;
}

// anlamli_kelimeler vector'undeki tekrar eden ve anlamlar� ayn� olan kelimelerin tekrarlar� silinir.
void anlamlari_duzenle(vector <Kelime>* anlamli_kelimeler){
	for(int i=0; i<anlamli_kelimeler->size()-1; i++){
		if(((*anlamli_kelimeler)[i].kelime_ekli == (*anlamli_kelimeler)[i+1].kelime_ekli) && ((*anlamli_kelimeler)[i].anlam == (*anlamli_kelimeler)[i+1].anlam)){
			anlamli_kelimeler->erase(anlamli_kelimeler->begin()+i+1);
		}
	}
}

// anlamli_kelimeler vector'unu s�zl�kten bulunmu� anlamlar� ve altyaz�daki tekrar miktarlar�yla ekrana yazar.
void vector_ekrana_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler){
	for(int i=0; i<anlamli_kelimeler.size(); i++)
		cout <<"("<< anlamli_kelimeler[i].tekrar <<")  "<< anlamli_kelimeler[i].kelime_ekli <<" -> "<< anlamli_kelimeler[i].kelime_yalin <<" = "<< anlamli_kelimeler[i].anlam << endl << endl;
}

// anlamli_kelimeler vector'unu s�zl�kten bulunmu� anlamlar� ve altyaz�daki tekrar miktarlar�yla dosyaya yazar.
void vector_dosyaya_yaz_anlamli(char adres[100], vector <Kelime> anlamli_kelimeler){
	ofstream outFile(adres);
	for(int i=0; i<anlamli_kelimeler.size(); i++)
		outFile <<"("<< anlamli_kelimeler[i].tekrar <<")  "<< anlamli_kelimeler[i].kelime_ekli <<" -> "<< anlamli_kelimeler[i].kelime_yalin <<" = "<< anlamli_kelimeler[i].anlam << endl << endl;
	outFile.close();
}


