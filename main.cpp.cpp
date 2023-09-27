#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;
//1
struct Odgovor {
	string answ;
	int rating = 0;
	string tab = "\t";
	vector<Odgovor*> com;

	Odgovor(string odgovor, int rating = 0) :answ(odgovor), rating(rating), com() {};
	Odgovor() = default;
};

struct Pitanje {
	string q;
	vector<Odgovor*> a;

	Pitanje(string pitanje) :q(pitanje), a() {};
	Pitanje() = default;
};
//2
void dodajPitanje(string pitanje, vector<Pitanje*>& pitanja) {
	Pitanje* p = new Pitanje(pitanje);
	pitanja.push_back(p);
}
//4
Pitanje* pretraziPitanje(string pitanje, vector<Pitanje*> questions) {
	Pitanje* p = new Pitanje(pitanje);
	for (int i = 0; i < questions.size(); i++) {
		if (p->q == questions[i]->q) {
			return questions[i];
		}
	}
	return nullptr;
}
int pretraziPitanjeIndeks(string pitanje, vector<Pitanje*> questions) {
	Pitanje* p = new Pitanje(pitanje);
	for (int i = 0; i < questions.size(); i++) {
		if (p->q == questions[i]->q) {
			return i;
		}
	}
	return -1;
}
//5
Odgovor* pretraziOdgovor(string p, string o, vector<Pitanje*> pitanja) {
	Pitanje* Pitanje1 = pretraziPitanje(p, pitanja);
	if (Pitanje1 == nullptr) {
		cout << "Uneli ste nepostojece pitanje." << endl;
		return nullptr;
	}
	stack<Odgovor*> stack;
	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	int i = 0;

	while (i != Pitanje1->a.size() - 1) {
		stack.push(Pitanje1->a[i]);
		i++;
	}

	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	stack.push(Pitanje1->a.front());

	while (!stack.empty()) {
		Odgovor* next = stack.top();
		stack.pop();
		bool flag = true;
		while (flag) {
			//obrada
			if (next->answ == o)
				return next;
			if ((*next).com.size()) {
				reverse(next->com.begin(), next->com.end());
				int i = 0;
				while (i != next->com.size() - 1) {
					stack.push(next->com[i]);
					i++;
				}
				reverse(next->com.begin(), next->com.end());
				next = next->com.front();
			}
			else
				flag = false;
		}
	}
	cout << "Uneli ste nepostojeci odgovor" << endl;
	return nullptr;
}
int pretraziOdgovorIndeks(string p, string o, vector<Pitanje*> pitanja) {
	Pitanje* Pitanje1 = pretraziPitanje(p, pitanja);
	if (Pitanje1 == nullptr) {
		cout << "Uneli ste nepostojece pitanje." << endl;
		return -1;
	}
	if (Pitanje1->a.size() == 0) {
		cout << "Pitanje nema odgovore" << endl;
		return -1;
	}
	stack<Odgovor*> stack;
	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	int i = 0;

	while (i != Pitanje1->a.size() - 1) {
		stack.push(Pitanje1->a[i]);
		i++;
	}
	int j = -1;
	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	stack.push(Pitanje1->a.front());
	while (!stack.empty()) {
		Odgovor* next = stack.top();
		stack.pop();
		bool flag = true;
		while (flag) {
			//obrada
			j++;
			if (next->answ == o)
				return j;
			if ((*next).com.size()) {
				reverse(next->com.begin(), next->com.end());
				int i = 0;
				while (i != next->com.size() - 1) {
					stack.push(next->com[i]);
					i++;
				}
				reverse(next->com.begin(), next->com.end());
				next = next->com.front();
			}
			else
				flag = false;
		}
	}
	cout << "Uneli ste nepostojeci odgovor" << endl;
	return -1;
}
//3
void dodajOdgovor(string pitanje, string odgovor1, string odgovor2, vector<Pitanje*>& pitanja) {

	Odgovor* o2 = new Odgovor(odgovor2);
	if (odgovor1 == "") {
		Odgovor* o2 = new Odgovor(odgovor2);
		Pitanje* p = pretraziPitanje(pitanje, pitanja);
		if (p != nullptr) {
			if (p->a.size() >= 10) {
				cout << "Nije dozvoljeno dodavanje više odgovora!" << endl;
				return;
			}
			else
				p->a.push_back(o2);
		}
		else {
			cout << "Uneto pitanje ne postoji!" << endl;
			return;
		}
	}
	else {
		Pitanje* p = pretraziPitanje(pitanje, pitanja);
		if (p->a.size() > 0) {
			Odgovor* o = pretraziOdgovor(pitanje, odgovor1, pitanja);
			if (o != nullptr) {
				if (o->com.size() >= 10)
					cout << "Nije dozvoljeno dodavanje više odgovora!" << endl;
				else {
					o2->tab = o->tab + "\t";
					o->com.push_back(o2);
				}
			}
			else {
				cout << "Dati odgovor nije pronadjen" << endl;
				return;
			}
		}
		else
			cout << "Ne postoje odgovori za zadato pitanje" << endl;
	}
}
//6
void upVote(string pitanje, string odgovor, vector<Pitanje*> pitanja) {
	Odgovor* o = pretraziOdgovor(pitanje, odgovor, pitanja);
	if (o != nullptr)
		o->rating++;
}
//7
void sortirajOdgovore(vector<Pitanje*>& pitanja) {
	for (int i = 0; i < pitanja.size(); i++) {
		vector<Odgovor*>& odgovori = pitanja[i]->a;
		sort(odgovori.begin(), odgovori.end(), [](const Odgovor* o1, const Odgovor* o2) {
			return o1->rating > o2->rating;
			});
		for (int i = 0; i < odgovori.size(); i++) {
			Odgovor* node = odgovori[i];
			queue<Odgovor*> queue;
			queue.push(node);
			while (!queue.empty()) {
				Odgovor* next = queue.front();
				queue.pop();
				sort(next->com.begin(), next->com.end(), [](const Odgovor* o1, const Odgovor* o2) {
					return o1->rating > o2->rating;
					});
				for (int j = 0; j < next->com.size(); j++)
					queue.push(next->com[j]);
			}
		}

	}
}
//8
void ispisiSvaPitanja(vector<Pitanje*> pitanja) {
	for (int i = 0; i < pitanja.size(); i++)
		cout << pitanja[i]->q << endl;
}
//9
void ispisOdredjenogPitanja(string pitanje, vector<Pitanje*> pitanja) {
	Pitanje* Pitanje1 = pretraziPitanje(pitanje, pitanja);
	if (Pitanje1 == nullptr) {
		cout << "Nepostojece pitanje" << endl;
		return;
	}
	cout << Pitanje1->q << endl;
	stack<Odgovor*> stack;
	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	int i = 0;

	while (i != Pitanje1->a.size() - 1) {
		stack.push(Pitanje1->a[i]);
		i++;
	}
	reverse(Pitanje1->a.begin(), Pitanje1->a.end());
	stack.push(Pitanje1->a.front());

	while (!stack.empty()) {
		Odgovor* next = stack.top();
		stack.pop();
		bool flag = true;
		while (flag) {
			cout << next->tab << next->answ << " " << next->rating << endl;
			if ((*next).com.size()) {
				reverse(next->com.begin(), next->com.end());
				int i = 0;
				while (i != next->com.size() - 1) {
					stack.push(next->com[i]);
					i++;
				}
				reverse(next->com.begin(), next->com.end());
				next = next->com.front();
			}
			else {
				flag = false;
			}
		}
	}

}
//10
void obrisiOdgovor(string pitanje, string odgovor, vector<Pitanje*>& pitanja) {
	int i = pretraziOdgovorIndeks(pitanje, odgovor, pitanja);
	Pitanje* p = pretraziPitanje(pitanje, pitanja);
	if (p != nullptr) {
		if (p->a.size() > 0) {
			if (i != -1) {
				vector<Odgovor*>::iterator it;
				it = p->a.begin() + i;
				p->a.erase(it);
				cout << "Uspesno brisanje" << endl;
			}
			else
				cout << "Neuspesno brisanje" << endl;
		}
		else
			cout << "Neuspesno brisanje" << endl;
	}

}
//11
void obrisiPitanje(string pitanje, vector<Pitanje*>& pitanja) {
	int i = pretraziPitanjeIndeks(pitanje, pitanja);
	if (i != -1) {
		vector<Pitanje*>::iterator it;
		it = pitanja.begin() + i;
		pitanja.erase(it);
		cout << "Uspesno brisanje" << endl;
	}
	else
		cout << "Neuspesno brisanje" << endl;
}
//dodatno
Pitanje* maxOdg(vector<Pitanje*> pitanja) {
	vector<int> nizMax;
	int k = 0;
	for (int j = 0; j < pitanja.size(); j++) {
		Pitanje* Pitanje1 = pitanja[j];
		nizMax.push_back(0);
		stack<Odgovor*> stack;
		reverse(Pitanje1->a.begin(), Pitanje1->a.end());
		int i = 0;
		if (Pitanje1->a.size() == 0)
			nizMax[j] = 0;
		else {
			while (i != Pitanje1->a.size() - 1) {
				stack.push(Pitanje1->a[i]);
				i++;
			}

			reverse(Pitanje1->a.begin(), Pitanje1->a.end());
			stack.push(Pitanje1->a.front());

			while (!stack.empty()) {
				Odgovor* next = stack.top();
				stack.pop();
				bool flag = true;
				while (flag) {
					//obrada
					nizMax[j]++;
					if ((*next).com.size()) {
						reverse(next->com.begin(), next->com.end());
						int i = 0;
						while (i != next->com.size() - 1) {
							stack.push(next->com[i]);
							i++;
						}
						reverse(next->com.begin(), next->com.end());
						next = next->com.front();
					}
					else
						flag = false;
				}
			}
		}
	}
	int max = nizMax[0];
	for (int j = 0; j < pitanja.size(); j++)
		if (nizMax[j] > max)
			k = j;
	cout << "Broj odgovora na pronadjeno pitanje: " << max << endl;
	return pitanja[k];
}

int main() {
	vector<Pitanje*> questions;
	/*
	dodajPitanje("A", questions);
	dodajPitanje("S", questions);
	dodajPitanje("K", questions);

	dodajOdgovor("A", "", "B", questions);
	dodajOdgovor("A", "", "C", questions);
	dodajOdgovor("A", "", "D", questions);
	dodajOdgovor("A", "", "E", questions);

	dodajOdgovor("A", "B", "F", questions);
	dodajOdgovor("A", "C", "G", questions);
	dodajOdgovor("A", "B", "H", questions);

	dodajOdgovor("S", "", "T", questions);
	dodajOdgovor("S", "", "M", questions);
	*/
	int n = 1;

	while (n != 0) {

		cout << "1. Dodaj pitanje" << endl;
		cout << "2. Dodaj odgovor na pitanje ili odgovor" << endl;
		cout << "3. Pretrazi pitanje" << endl;
		cout << "4. Pretrazi odgovor" << endl;
		cout << "5. Povecaj ocenu odgovora" << endl;
		cout << "6. Uredi sve odgovore" << endl;
		cout << "7. Ispisi pitanja" << endl;
		cout << "8. Ispisi odredjeno pitanje i njegove odgovore i komentare" << endl;
		cout << "9. Pronadji pitanje za koje postoji najvise odgovora i komentara" << endl;
		cout << "10. Obrisi odgovor ili komentar i sve njegove potomke" << endl;
		cout << "11. Obrisi pitanje i sve odgovore" << endl;
		cout << "0. Prekini interakciju sa programom" << endl;

		cin >> n;

		string s1, s2, s3;
		Odgovor* o;
		Pitanje* p;

		switch (n) {
		case 1:
			cout << "Upisite pitanje koje zelite da dodate: ";
			cin.ignore();
			getline(cin, s1);
			dodajPitanje(s1, questions);
			break;
		case 2:
			cout << "Upisite pitanje na koje zelite da dodate odgovor: ";
			cin.ignore();
			getline(cin, s1);
			cout << "Upisite odgovor koje zelite da dodate odgovor, ili ostavite ? ukoliko dodajte odgovor na pitanje: ";
			getline(cin, s2);
			cout << "Upisite odgovor koji zelite da dodate: ";
			getline(cin, s3);
			s2 = (s2 == "?" ? "" : s2);
			dodajOdgovor(s1, s2, s3, questions);
			break;
		case 3:
			cout << "Upisite pitanje koje zelite da pretrazite: ";
			cin.ignore();
			getline(cin, s1);
			p = pretraziPitanje(s1, questions);
			if (p != nullptr)
				cout << "Uspesna pretraga" << endl;
			else
				cout << "Neuspesna pretraga" << endl;
			break;
		case 4:
			cout << "Upisite pitanje za koje zelite da obavite pretragu: ";
			cin.ignore();
			getline(cin, s1);
			cout << "Upisite odgovor koji zelite da pretrazite: ";
			getline(cin, s2);
			o = pretraziOdgovor(s1, s2, questions);
			if (o != nullptr)
				cout << "Uspesna pretraga" << endl;
			else
				cout << "Neuspesna pretraga" << endl;
			break;
		case 5:
			cout << "Upisite pitanje za koje zelite da ocenite odgovor: ";
			cin.ignore();
			getline(cin, s1);
			cout << "Upisite odgovor koji zelite da ocenite: ";
			getline(cin, s2);
			upVote(s1, s2, questions);
			break;
		case 6:
			sortirajOdgovore(questions);
			break;
		case 7:
			ispisiSvaPitanja(questions);
			break;
		case 8:
			cout << "Upisite pitanje za koje zelite da izvrsite ispis: ";
			cin.ignore();
			getline(cin, s1);
			ispisOdredjenogPitanja(s1, questions);
			break;
		case 9:
			ispisOdredjenogPitanja(maxOdg(questions)->q, questions);
			break;
		case 10:
			cout << "Upisite pitanje za koje zelite da obrisete odgovor: ";
			cin.ignore();
			getline(cin, s1);
			cout << "Upisite odgovor koji zelite da obrisete: ";
			getline(cin, s2);
			obrisiOdgovor(s1, s2, questions);
			break;
		case 11:
			cout << "Upisite pitanje koje zelite da obrisete: ";
			cin.ignore();
			getline(cin, s1);
			obrisiPitanje(s1, questions);
			break;
		}
	}
}