#include "funkcijos.h"
#include "vector.h"
vector<std::chrono::duration<double>> testuLaikai;
vector<std::chrono::duration<double>> kurimoLaikai;
vector<std::chrono::duration<double>> rusiavimoLaikai;
vector<std::chrono::duration<double>> kietekuLaikai;
vector<std::chrono::duration<double>> vargsiukuLaikai;
vector<std::chrono::duration<double>> vienoIsSesiuRusiavimoLaikai;
int testuSkaicius = 0;
int kurimuSkaicius = 0;
int rusiavimoSkaicius = 0;
int vienoIsSesiuRusiavimoSkaicius = 0;
int kiekYraStudentuFaile;

void aplankalas()
{
	if (!std::filesystem::exists(aplankalo_pavadinimas))
	{
		std::filesystem::create_directory(aplankalo_pavadinimas);
		cout << "Sukurtas aplankalas: " << aplankalo_pavadinimas << endl;
	}
	else
	{
		cout << "Aplankalas '" << aplankalo_pavadinimas << "' jau egzistuoja.\n";
	}
}

int failo_generavimo_pasirinkimas(vector<string> &failai)
{
	int generuoti;
	cout << "Ar norite sugeneruoti naujus failus?\n"
		 << "1 - Taip\n"
		 << "2 - Ne\n"
		 << "Pasirinkite: ";
	while (true)
	{
		try
		{
			cin >> generuoti;

			if ((generuoti != 1 && generuoti != 2) || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama pasirinkima. Iveskite dar karta.");
			}
			break;
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	if (generuoti == 1)
	{
		int failu_kiekis;
		while (true)
		{
			try
			{
				cout << "Kiek failu norite sugeneruoti? ";
				cin >> failu_kiekis;

				if (failu_kiekis <= 0 || cin.fail() || cin.peek() != '\n')
				{
					throw std::invalid_argument("Ivedete netinkama skaiciu. Bandykite dar karta.");
				}
				break;
			}
			catch (const std::invalid_argument &e)
			{
				cout << e.what() << endl;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}

		for (int i = 0; i < failu_kiekis; i++)
		{
			long long studentu_skaicius;
			while (true)
			{
				try
				{
					cout << "Iveskite studentu skaiciu " << i + 1 << "-ajam failui: ";
					cin >> studentu_skaicius;

					if (studentu_skaicius <= 0 || cin.fail() || cin.peek() != '\n')
					{
						throw std::invalid_argument("Ivedete netinkama skaiciu. Bandykite dar karta.");
					}
					break;
				}
				catch (const std::invalid_argument &e)
				{
					cout << e.what() << endl;
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}

			string failo_pavadinimas = "studentai" + to_string(studentu_skaicius) + ".txt";
			failai.push_back(failo_pavadinimas);

			try
			{
				failo_generavimas(failo_pavadinimas, studentu_skaicius);
			}
			catch (const std::exception &e)
			{
				cout << "Nepavyko sugeneruoti failo: " << failo_pavadinimas << ". Klaida: " << e.what() << endl;
			}
		}
		cout << "Failai sekmingai sugeneruoti!\n";
	}
	else
	{
		cout << "Failai nebuvo sugeneruoti.\n";
	}
	return generuoti;
}

void failo_generavimas(const string &failo_pavadinimas, long long studentu_skaicius)
{
	auto start = std::chrono::high_resolution_clock::now();
	aplankalas();
	string sukurti_faila_aplankale = aplankalo_pavadinimas + "/" + failo_pavadinimas;

	ofstream failas(sukurti_faila_aplankale);
	if (!failas)
	{
		throw std::runtime_error("Klaida kuriant faila: " + sukurti_faila_aplankale);
	}

	int pazymiu_kiekis = rand() % 25 + 2;

	cout << "Generuojamas failas: " << sukurti_faila_aplankale
		 << " su " << studentu_skaicius << " studentais ir "
		 << pazymiu_kiekis << " ND.\n";

	failas << setw(20) << left << "Vardas"
		   << setw(20) << left << "Pavarde";
	for (int j = 1; j <= pazymiu_kiekis; j++)
		failas << setw(10) << left << "ND" + to_string(j);
	failas << setw(10) << left << "Egzaminas" << endl;

	for (long long i = 1; i <= studentu_skaicius; i++)
	{
		failas << setw(20) << left << "Vardas" + to_string(i)
			   << setw(20) << left << "Pavarde" + to_string(i);

		for (int j = 0; j < pazymiu_kiekis; j++)
			failas << setw(10) << left << (rand() % 10 + 1);

		failas << setw(10) << left << (rand() % 10 + 1) << endl;

		if (i % 100000 == 0)
			failas.flush();
	}

	failas.close();
	cout << "Failas sukurtas: " << sukurti_faila_aplankale << endl;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = end - start;
	cout << "Faila sukurti uztruko: " << time.count() << " sek." << endl;
	kurimuSkaicius++;
	kurimoLaikai.push_back(time);
}

void parodytiEsamusFailus()
{
	cout << "Esami .txt failai aplanke '" << aplankalo_pavadinimas << "':\n";

	for (const auto &entry : std::filesystem::directory_iterator(aplankalo_pavadinimas))
	{
		if (entry.path().extension() == ".txt")
		{
			string failo_vardas = entry.path().filename().string();
			if (failo_vardas != "rezultatai.txt")
			{
				cout << "- " << failo_vardas << endl;
			}
		}
	}
}

string koki_faila_nuskaityti()
{
	string failo_pavadinimas;
	cout << "Galimi failai aplanke '" << aplankalo_pavadinimas << "':\n";
	for (const auto &entry : std::filesystem::directory_iterator(aplankalo_pavadinimas))
	{
		if (entry.path().extension() == ".txt")
		{
			cout << "- " << entry.path().filename().string() << endl;
		}
	}
	while (true)
	{
		try
		{
			cout << "Iveskite failo pavadinima: ";
			cin >> failo_pavadinimas;

			string sukurti_faila_aplankale = aplankalo_pavadinimas + "/" + failo_pavadinimas;

			if (!std::filesystem::exists(sukurti_faila_aplankale))
			{
				throw std::invalid_argument("Tokio failo nera. Bandykite dar karta.");
			}

			return sukurti_faila_aplankale;
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

void failo_nuskaitymas(vector<Studentas> &studentai, const string &failo_pavadinimas)
{
	try
	{
		auto start = std::chrono::high_resolution_clock::now();
		ifstream failas(failo_pavadinimas);
		stringstream buferis;
		string eilute;
		if (!failas)
		{
			throw std::ios_base::failure("Klaida: nepavyko atidaryti failo!");
		}
		buferis << failas.rdbuf();
		failas.close();
		getline(buferis, eilute);
		while (getline(buferis, eilute))
		{
			istringstream iss(eilute);
			Studentas laikinas;
			string vardas, pavarde;
			iss >> vardas >> pavarde;
			laikinas.setVardas(vardas);
			laikinas.setPavarde(pavarde);
			int pazymys;
			while (iss >> pazymys)
			{
				laikinas.addPazymys(pazymys);
			}
			if (!laikinas.getPazymiai().empty())
			{
				laikinas.setEgzaminoPazymys(laikinas.getPazymiai().back());
				laikinas.removeLastPazymys(pazymys);
			}
			studentai.push_back(std::move(laikinas));
		}
		failas.close();
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time = end - start;
		double laikas = time.count();
		cout << "Faila nuskaityti uztruko: " << laikas << " sek." << endl;
		testuSkaicius++;
		testuLaikai.push_back(time);
	}
	catch (const std::ios_base::failure &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Ivyko klaida: " << e.what() << std::endl;
	}
	kiekYraStudentuFaile = studentai.size();
}

int Meniu()
{
	int pasirinkimas = 0;
	cout << "Meniu\n"
		 << "--------------------------------------------------------------------------------------\n"
		 << "Pasirinkite veiksma ivesdami: \n"
		 << "1 - Jei norite ivesti visa informacija rankiniu budu.\n"
		 << "2 - Jei norite sugeneruoti atsitiktinius pazymius.\n"
		 << "3 - Jei norite atsitiktinai sugeneruoti pazymius ir studentu vardus, pavardes.\n"
		 << "4 - Jei norite nuskaityti informacija is failo.\n"
		 << "5 - Jei norite baigti programa.\n";
	while (pasirinkimas < 1 || pasirinkimas > 5)
	{
		try
		{
			cin >> pasirinkimas;
			if (pasirinkimas < 1 || pasirinkimas > 5 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return pasirinkimas;
}

void pabaiga(int pasirinkimas, int generuoti, int pasirinkimas_rusiavimui, int kaip_surusiuoti)
{
	if (generuoti == 1)
	{
		std::chrono::duration<double> visasLaikas{};
		for (int i = 0; i < kurimoLaikai.size(); i++)
		{
			visasLaikas += kurimoLaikai[i];
		}
		cout << "Bendras failu kurimo laikas: " << visasLaikas.count() << " sek." << endl;

		if (kurimuSkaicius > 0)
		{
			double vidurkis = visasLaikas.count() / kurimuSkaicius;
			cout << "Vidutinis failu kurimo laikas: " << vidurkis << " sek." << endl;
		}
		else
		{
			cout << "Nebuvo sukurti failai." << endl;
		}
	}
	if (pasirinkimas_rusiavimui > 0)
	{
		std::chrono::duration<double> visasLaikas{};

		for (int i = 0; i < rusiavimoLaikai.size(); i++)
		{
			visasLaikas += rusiavimoLaikai[i];
		}
		cout << "Bendras rusiavimo i kietekus ir vargsiukus laikas: " << std::fixed << std::setprecision(6) << visasLaikas.count() << " sek." << endl;
		if (rusiavimoSkaicius > 0)
		{
			double vidurkis = visasLaikas.count() / rusiavimoSkaicius;
			cout << "Vidutinis rusiavimo i kietekus ir vargsiukus laikas: " << std::fixed << std::setprecision(6) << vidurkis << " sek." << endl;
		}
		else
		{
			cout << "Nebuvo surusiuota." << endl;
		}

		// visasLaikas = std::chrono::duration<double>{};
		// for (int i = 0; i < kietekuLaikai.size(); i++)
		// {
		// 	visasLaikas += kietekuLaikai[i];
		// }
		// cout << "Bendras kieteku isvedimo laikas: " << std::fixed << std::setprecision(6) << visasLaikas.count() << " sek." << endl;
		// if (rusiavimoSkaicius > 0)
		// {
		// 	double vidurkis = visasLaikas.count() / rusiavimoSkaicius;
		// 	cout << "Vidutinis kieteku isvedimo laikas: " << std::fixed << std::setprecision(6) << vidurkis << " sek." << endl;
		// }
		// else
		// {
		// 	cout << "Nebuvo isvesti failai." << endl;
		// }

		// visasLaikas = std::chrono::duration<double>{};
		// for (int i = 0; i < vargsiukuLaikai.size(); i++)
		// {
		// 	visasLaikas += vargsiukuLaikai[i];
		// }
		// cout << "Bendras vargsiuku isvedimo laikas: " << std::fixed << std::setprecision(6) << visasLaikas.count() << " sek." << endl;
		// if (rusiavimoSkaicius > 0)
		// {
		// 	double vidurkis = visasLaikas.count() / rusiavimoSkaicius;
		// 	cout << "Vidutinis vargsiuku isvedimo laikas: " << std::fixed << std::setprecision(6) << vidurkis << " sek." << endl;
		// }
		// else
		// {
		// 	cout << "Nebuvo isvesti failai." << endl;
		// }
	}
	if (kaip_surusiuoti > 0)
	{
		std::chrono::duration<double> visasLaikas{};
		for (int i = 0; i < vienoIsSesiuRusiavimoLaikai.size(); i++)
		{
			visasLaikas += vienoIsSesiuRusiavimoLaikai[i];
		}
		cout << "Bendras rusiavimo laikas: " << std::fixed << std::setprecision(6) << visasLaikas.count() << " sek." << endl;
		if (vienoIsSesiuRusiavimoSkaicius > 0)
		{
			double vidurkis = visasLaikas.count() / vienoIsSesiuRusiavimoSkaicius;
			cout << "Vidutinis rusiavimo laikas: " << std::fixed << std::setprecision(6) << vidurkis << " sek." << endl;
		}
		else
		{
			cout << "Nebuvo surusiuota." << endl;
		}
	}
	if (testuSkaicius > 0)
	{
		std::chrono::duration<double> visasLaikas{};
		for (int i = 0; i < testuLaikai.size(); i++)
		{
			visasLaikas += testuLaikai[i];
		}
		cout << "Bendras failo nuskaitymo laikas: " << visasLaikas.count() << " sek." << endl;
		if (testuSkaicius > 0)
		{
			double vidurkis = visasLaikas.count() / testuSkaicius;
			cout << "Vidutinis failo nuskaitymo laikas: " << vidurkis << " sek." << endl;
		}
		else
		{
			cout << "Nera testu." << endl;
		}
	}
	else
	{
		cout << "Programa baigta." << endl;
	}
}

bool ivestiStudenta(Studentas &laikinas)
{
	cout << "Iveskite studento varda ir pavarde (Jei norite baigti, irasykite 'n'.): ";
	string vardas;
	cin >> vardas;
	laikinas.setVardas(vardas);
	if (laikinas.getVardas() == "n")
		return false;
	string pavarde;
	cin >> pavarde;
	laikinas.setPavarde(pavarde);
	return true;
}

void ivestiPazymius(Studentas &laikinas)
{
	int pazymys = 0;
	cout << "Iveskite studento namu darbu pazymius (Jei norite baigti pazymiu irasyma irasykite -1.): ";

	while (true)
	{
		try
		{
			cin >> pazymys;
			if (pazymys == -1)
				break;
			if (pazymys < 1 || pazymys > 10 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
			laikinas.addPazymys(pazymys);
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	cout << "Iveskite egzamino pazymi: ";
	while (true)
	{
		int egzamino_pazymys = 0;
		try
		{
			cin >> egzamino_pazymys;
			laikinas.setEgzaminoPazymys(egzamino_pazymys);
			if (laikinas.getEgzaminoPazymys() >= 1 && laikinas.getEgzaminoPazymys() <= 10)
				break;
			throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

void generuotiPazymius(Studentas &laikinas)
{
	int pazymiu_kiekis = 0;
	cout << "Iveskite kiek pazymiu norite sugeneruoti: ";
	while (pazymiu_kiekis <= 0 || pazymiu_kiekis > 1000)
	{
		try
		{
			cin >> pazymiu_kiekis;
			if (pazymiu_kiekis <= 0 || pazymiu_kiekis > 1000 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	cout << "Sugeneruoti pazymiai: ";
	for (int i = 0; i < pazymiu_kiekis; i++)
	{
		int pazymys = rand() % 10 + 1;
		cout << pazymys << " ";
		laikinas.addPazymys(pazymys);
	}
	cout << endl;

	laikinas.setEgzaminoPazymys(rand() % 10 + 1);
	cout << "Egzamino pazymys: " << laikinas.getEgzaminoPazymys() << endl;
}

void informacijos_ivedimas(vector<Studentas> &studentai, int pasirinkimas, string failo_pavadinimas)
{
	while (true)
	{
		Studentas laikinas;
		string random_vardas[10] = {"Marija", "Arnas", "Tomas", "Greta", "Gabija", "Paulius", "Lukas", "Egle", "Rokas", "Ieva"};
		string random_pavarde[10] = {"Petrauskaite", "Petrauskas", "Tomauskas", "Gretauskaite", "Gabijauskaite", "Paulauskas", "Lukauskaite", "Matuolis", "Rokauskas", "Ievauskaite"};
		int generavimas = 0;
		switch (pasirinkimas)
		{
		case 1:
			if (!ivestiStudenta(laikinas))
				return;
			ivestiPazymius(laikinas);
			break;

		case 2:
			if (!ivestiStudenta(laikinas))
				return;
			generuotiPazymius(laikinas);
			break;

		case 3:
			cout << "Jei norite sugeneruoti varda ir pavarde irasykite 1, jei norite uzbaigti programa irasykite 2: ";
			while (generavimas != 1 && generavimas != 2)
			{
				try
				{
					cin >> generavimas;
					if (generavimas == 2)
					{
						return;
					}
					else if (generavimas == 1)
					{
						laikinas.setVardas(random_vardas[rand() % 10]);
						laikinas.setPavarde(random_pavarde[rand() % 10]);
					}
					else
					{
						throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
					}
				}
				catch (const std::invalid_argument &e)
				{
					cout << e.what() << endl;
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}
			generuotiPazymius(laikinas);
			break;

		case 4:
			failo_nuskaitymas(studentai, failo_pavadinimas);
			return;

		default:
			cout << "Neteisingas pasirinkimas. Bandykite dar kartą." << endl;
			return;
		}

		studentai.push_back(laikinas);
	}
}

int koks_galutinis()
{
	int skaiciavimo_budas = 0;
	cout << "Iveskite kaip norite apskaiciuoti galutini pazymi: \n"
		 << "1 - Jei norite apskaiciuoti vidurki; \n"
		 << "2 - Jei norite apskaiciuoti mediana; \n"
		 << "3 - Jei norite apsaiciuoti abu. \n";

	while (skaiciavimo_budas < 1 || skaiciavimo_budas > 3)
	{
		try
		{
			cin >> skaiciavimo_budas;
			if (skaiciavimo_budas < 1 || skaiciavimo_budas > 3 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return skaiciavimo_budas;
}

int failas_ar_konsole()
{
	int isvedimo_budas = 0;
	cout << "Kaip norite isvesti faila: \n"
		 << "1 - Jei norite isvesti i konsole; \n"
		 << "2 - Jei norite isvesti i faila. \n"
		 << "3 - Jei nenorite isvesti visos informacijos viename faile/konsoleje. \n";
	while (isvedimo_budas < 1 || isvedimo_budas > 3)
	{
		try
		{
			cin >> isvedimo_budas;
			if (isvedimo_budas < 1 || isvedimo_budas > 3 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return isvedimo_budas;
}

int rusiavimas(int skaiciavimo_budas)
{
	int kaip_surusiuoti = 0;

	cout << "Kaip norite surikiuoti studentus faile: \n";

	if (skaiciavimo_budas == 1)
	{
		cout << "1 - Pagal varda\n"
			 << "2 - Pagal pavarde\n"
			 << "3 - Pagal vidurki (nuo maziausio iki didziausio)\n"
			 << "4 - Pagal vidurki (nuo didziausio iki maziausio)\n";
	}
	else if (skaiciavimo_budas == 2)
	{
		cout << "1 - Pagal varda\n"
			 << "2 - Pagal pavarde\n"
			 << "5 - Pagal mediana (nuo maziausio iki didziausio)\n"
			 << "6 - Pagal mediana (nuo didziausio iki maziausio)\n";
	}
	else if (skaiciavimo_budas == 3)
	{
		cout << "1 - Pagal varda\n"
			 << "2 - Pagal pavarde\n"
			 << "3 - Pagal vidurki (nuo maziausio iki didziausio)\n"
			 << "4 - Pagal vidurki (nuo didziausio iki maziausio)\n"
			 << "5 - Pagal mediana (nuo maziausio iki didziausio)\n"
			 << "6 - Pagal mediana (nuo didziausio iki maziausio)\n";
	}

	while (true)
	{
		try
		{
			cin >> kaip_surusiuoti;

			if ((skaiciavimo_budas == 1 && (kaip_surusiuoti < 1 || kaip_surusiuoti > 4)) ||
				(skaiciavimo_budas == 2 && (kaip_surusiuoti < 1 || kaip_surusiuoti > 2 && kaip_surusiuoti < 5)) ||
				(skaiciavimo_budas == 3 && (kaip_surusiuoti < 1 || kaip_surusiuoti > 6)) ||
				cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama pasirinkima. Bandykite dar karta.");
			}
			break;
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return kaip_surusiuoti;
}

bool vardoRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	if (studentas.vardoPaieska() && studentas2.vardoPaieska())
	{
		try
		{
			int num1 = stoi(studentas.getVardas().substr(6));
			int num2 = stoi(studentas2.getVardas().substr(6));
			return num1 < num2;
		}
		catch (const std::invalid_argument &)
		{
			return studentas.getVardas() < studentas2.getVardas();
		}
	}
	else
	{
		return studentas.getVardas() < studentas2.getVardas();
	}
}

bool pavardeRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	if (studentas.pavardesPaieska() && studentas2.pavardesPaieska())
	{
		try
		{
			int num1 = stoi(studentas.getPavarde().substr(7));
			int num2 = stoi(studentas2.getPavarde().substr(7));
			return num1 < num2;
		}
		catch (const std::invalid_argument &)
		{
			return studentas.getPavarde() < studentas2.getPavarde();
		}
	}
	else
	{
		return studentas.getPavarde() < studentas2.getPavarde();
	}
}

bool vidurkioNuoMazRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	return studentas.getGalutinisPazymysVid() < studentas2.getGalutinisPazymysVid();
}

bool vidurkioNuoDidRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	return studentas.getGalutinisPazymysVid() > studentas2.getGalutinisPazymysVid();
}

bool medianosNuoMazRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	return studentas.getGalutinisPazymysMed() < studentas2.getGalutinisPazymysMed();
}

bool medianosNuoDidRusiavimas(const Studentas &studentas, const Studentas &studentas2)
{
	return studentas.getGalutinisPazymysMed() > studentas2.getGalutinisPazymysMed();
}

int papildomas_rusiavimas()
{
	int pasirinkimas_rusiavimui = 0;
	cout << "Ar norite surusiuoti studentus i du atskirus failus (kietekai ir vargsiukai)?\n"
		 << "1 - Taip\n"
		 << "2 - Ne\n";
	while (pasirinkimas_rusiavimui < 1 || pasirinkimas_rusiavimui > 2)
	{

		try
		{
			cin >> pasirinkimas_rusiavimui;
			if (pasirinkimas_rusiavimui < 1 || pasirinkimas_rusiavimui > 2 || cin.fail() || cin.peek() != '\n')
			{
				throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
			}
		}
		catch (const std::invalid_argument &e)
		{
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return pasirinkimas_rusiavimui;
}

int pagal_ka_rusiuoti_studentus_i_failus(vector<Studentas> &studentai, int skaiciavimo_budas)
{
	vector<Studentas> vargsiukai;
	int rusiavimo_budas = 0;

	if (skaiciavimo_budas == 3)
	{
		cout << "Kaip norite surusiuoti studentus: \n"
			 << "1 - Pagal vidurki; \n"
			 << "2 - Pagal mediana. \n";
		while (rusiavimo_budas < 1 || rusiavimo_budas > 2)
		{
			try
			{
				cin >> rusiavimo_budas;
				if (rusiavimo_budas < 1 || rusiavimo_budas > 2 || cin.fail() || cin.peek() != '\n')
				{
					throw std::invalid_argument("Ivedete netinkama simboli. Iveskite dar karta: ");
				}
			}
			catch (const std::invalid_argument &e)
			{
				cout << e.what() << endl;
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}
	return rusiavimo_budas;
}

void studentu_rusiavimas_3strategija(vector<Studentas> &studentai, int skaiciavimo_budas, int kaip_surusiuoti, int rusiavimo_budas)
{
	vector<Studentas> kietekai, vargsiukai;

	auto start = std::chrono::high_resolution_clock::now();
	if (skaiciavimo_budas == 1)
	{
		for (auto &studentas : studentai)
		{
			studentas.galutinis_pazymys_vid();
		}
	}
	else if (skaiciavimo_budas == 2)
	{
		for (auto &studentas : studentai)
		{
			studentas.galutinis_pazymys_med();
		}
	}
	else if (skaiciavimo_budas == 3)
	{
		for (auto &studentas : studentai)
		{
			studentas.galutinis_pazymys_vid();
			studentas.galutinis_pazymys_med();
		}
	}

	auto it = std::stable_partition(studentai.begin(), studentai.end(), [&](const Studentas &s)
									{
        double galutinis_balas = 0;
        if (skaiciavimo_budas == 1 || (skaiciavimo_budas == 3 && rusiavimo_budas == 1))
        {
            galutinis_balas = s.getGalutinisPazymysVid();
        }
        else if (skaiciavimo_budas == 2 || (skaiciavimo_budas == 3 && rusiavimo_budas == 2))
        {
            galutinis_balas = s.getGalutinisPazymysMed();
        }
        return galutinis_balas < 5.0; });

	vargsiukai.assign(studentai.begin(), it);
	kietekai.assign(it, studentai.end());
	studentai.clear();
	kietekai.shrink_to_fit();
	vargsiukai.shrink_to_fit();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = end - start;
	rusiavimoLaikai.push_back(time);
	rusiavimoSkaicius++;
	rusiavimoIf(kietekai, kaip_surusiuoti, skaiciavimo_budas);
	rusiavimoIf(vargsiukai, kaip_surusiuoti, skaiciavimo_budas);
	start = std::chrono::high_resolution_clock::now();
	std::ofstream kietekaifailas("kietekai.txt");
	output(kietekaifailas, kietekai, skaiciavimo_budas, 1);
	kietekaifailas.close();
	end = std::chrono::high_resolution_clock::now();
	time = end - start;
	kietekuLaikai.push_back(time);
	start = std::chrono::high_resolution_clock::now();
	std::ofstream vargsiukaifailas("vargsiukai.txt");
	output(vargsiukaifailas, vargsiukai, skaiciavimo_budas, 1);
	vargsiukaifailas.close();
	end = std::chrono::high_resolution_clock::now();
	time = end - start;
	vargsiukuLaikai.push_back(time);
}

void output(ostream &out, vector<Studentas> studentai, int skaiciavimo_budas, int isvedimo_budas)
{
	stringstream isvestis;

	isvestis << setw(20) << left << "Vardas" << setw(20) << left << "Pavarde";
	switch (skaiciavimo_budas)
	{
	case 1:
		isvestis << setw(20) << left << "Galutinis (Vid.): " << endl;
		break;
	case 2:
		isvestis << setw(20) << left << "Galutinis (Med.): " << endl;
		break;
	case 3:
		isvestis << setw(20) << left << "Galutinis (Vid.)" << setw(20) << left << "Galutinis (Med.)" << endl;
		break;
	}

	isvestis << "--------------------------------------------------------------------------------------\n";

	switch (skaiciavimo_budas)
	{
	case 1:
		for (auto &studentas : studentai)
		{
			isvestis << setw(20) << left << studentas.getVardas() << setw(20) << left << studentas.getPavarde()
					 << setw(20) << left << fixed << setprecision(2) << studentas.getGalutinisPazymysVid()
					 << "\n";
		}
		break;
	case 2:
		for (auto &studentas : studentai)
		{
			isvestis << setw(20) << left << studentas.getVardas() << setw(20) << left << studentas.getPavarde()
					 << setw(20) << left << fixed << setprecision(2) << studentas.getGalutinisPazymysMed() << "\n";
		}
		break;
	case 3:
		for (auto &studentas : studentai)
		{
			isvestis << setw(20) << left << studentas.getVardas() << setw(20) << left << studentas.getPavarde()
					 << setw(20) << left << fixed << setprecision(2) << studentas.getGalutinisPazymysVid()
					 << setw(20) << left << fixed << setprecision(2) << studentas.getGalutinisPazymysMed() << "\n";
		}
		break;
	}
	out << isvestis.str();
}

void rusiavimoIf(vector<Studentas> &studentai, int kaip_surusiuoti, int skaiciavimo_budas)
{
	auto start = std::chrono::high_resolution_clock::now();
	switch (kaip_surusiuoti)
	{
	case 1:
		sort(studentai.begin(), studentai.end(), vardoRusiavimas);
		break;
	case 2:
		sort(studentai.begin(), studentai.end(), pavardeRusiavimas);
		break;
	case 3:
		sort(studentai.begin(), studentai.end(), vidurkioNuoMazRusiavimas);
		break;
	case 4:
		sort(studentai.begin(), studentai.end(), vidurkioNuoDidRusiavimas);
		break;
	case 5:
		sort(studentai.begin(), studentai.end(), medianosNuoMazRusiavimas);
		break;
	case 6:
		sort(studentai.begin(), studentai.end(), medianosNuoDidRusiavimas);
		break;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = end - start;
	double laikas = time.count();
	vienoIsSesiuRusiavimoSkaicius++;
	vienoIsSesiuRusiavimoLaikai.push_back(time);
}

void terminalas(vector<Studentas> &studentai, int kaip_surusiuoti, int skaiciavimo_budas)
{
	rusiavimoIf(studentai, kaip_surusiuoti, skaiciavimo_budas);
	output(cout, studentai, skaiciavimo_budas, 1);
}

void failas(vector<Studentas> &studentai, int kaip_surusiuoti, int skaiciavimo_budas)

{
	rusiavimoIf(studentai, kaip_surusiuoti, skaiciavimo_budas);
	ofstream failas("rezultatai.txt");
	output(failas, studentai, skaiciavimo_budas, 2);
	failas.close();
}

void spartosTestas()
{
	unsigned int sizes[] = {10000, 100000, 1000000, 10000000, 100000000};

	cout << "=== std::vector spartos testas ===" << endl;
	for (unsigned int size : sizes)
	{
		int perskirstymai = 0;
		vector<int> v1;
		auto start = high_resolution_clock::now();

		for (unsigned int i = 0; i < size; ++i)
		{
			v1.push_back(i);
			if (v1.capacity() == v1.size())
				++perskirstymai;
		}

		auto end = high_resolution_clock::now();
		duration<double> trukme = end - start;

		cout << "std::vector[" << size << "] - laikas: " << trukme.count() << " s, perskirstymai: " << perskirstymai << endl;
	}

	cout << "\n=== Tavo Vector spartos testas ===" << endl;
	for (unsigned int size : sizes)
	{
		int perskirstymai = 0;
		vector<int> v2;
		auto start = high_resolution_clock::now();

		for (unsigned int i = 0; i < size; ++i)
		{
			v2.push_back(i);
			if (v2.capacity() == v2.size())
				++perskirstymai;
		}

		auto end = high_resolution_clock::now();
		duration<double> trukme = end - start;

		cout << "Vector[" << size << "] - laikas: " << trukme.count() << " s, perskirstymai: " << perskirstymai << endl;
	}
}

// int ar_paleisti_programa()
// {
// 	int choise;
// 	cout << "Ar paleisti programa?";
// 	cin >> choise;
// 	return choise;
// }