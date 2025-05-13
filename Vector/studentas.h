#ifndef STUDENTAS_H
#define STUDENTAS_H
#include "../santrauka.h"
#include "zmogus.h"
#include "vector.h"

class Studentas : public Zmogus
{
private:
	std::vector<int> pazymiai_;
	int egzamino_pazymys_ = 0;
	double galutinis_pazymys_vid_ = 0.0;
	double galutinis_pazymys_med_ = 0.0;

public:
	// Default konstruktorius
	Studentas()
		: Zmogus("Vardenis", "Pavardenis"),
		  pazymiai_{},
		  egzamino_pazymys_{0},
		  galutinis_pazymys_vid_{0.0},
		  galutinis_pazymys_med_{0.0} {}

	Studentas(std::string vardas, std::string pavarde, std::vector<int> pazymiai, int egzamino_pazymys)
		: Zmogus(vardas, pavarde),
		  pazymiai_{pazymiai},
		  egzamino_pazymys_{egzamino_pazymys} {}

	void clear()
	{
		pazymiai_.clear();
		egzamino_pazymys_ = 0;
	}
	// Destruktorius
	~Studentas() { clear(); }

	// Copy konstruktorius
	Studentas(const Studentas &studentas)
		: Zmogus(studentas.vardas_, studentas.pavarde_),
		  pazymiai_{studentas.pazymiai_},
		  egzamino_pazymys_{studentas.egzamino_pazymys_},
		  galutinis_pazymys_vid_{studentas.galutinis_pazymys_vid_},
		  galutinis_pazymys_med_{studentas.galutinis_pazymys_med_} {}

	// Copy assigment operatorius
	Studentas &operator=(const Studentas &studentas)
	{
		if (this != &studentas)
		{
			vardas_ = studentas.vardas_;
			pavarde_ = studentas.pavarde_;
			pazymiai_ = studentas.pazymiai_;
			egzamino_pazymys_ = studentas.egzamino_pazymys_;
			galutinis_pazymys_vid_ = studentas.galutinis_pazymys_vid_;
			galutinis_pazymys_med_ = studentas.galutinis_pazymys_med_;
		}
		return *this;
	}

	// Move konstruktorius
	Studentas(Studentas &&studentas) noexcept
		: Zmogus(std::move(studentas.vardas_), std::move(studentas.pavarde_)),
		  pazymiai_{std::move(studentas.pazymiai_)},
		  egzamino_pazymys_{std::move(studentas.egzamino_pazymys_)},
		  galutinis_pazymys_vid_{std::move(studentas.galutinis_pazymys_vid_)},
		  galutinis_pazymys_med_{std::move(studentas.galutinis_pazymys_med_)} {}

	// Move assigment operatorius
	Studentas &operator=(Studentas &&studentas) noexcept
	{
		if (this != &studentas)
		{
			vardas_ = std::move(studentas.vardas_);
			pavarde_ = std::move(studentas.pavarde_);
			pazymiai_ = std::move(studentas.pazymiai_);
			egzamino_pazymys_ = std::move(studentas.egzamino_pazymys_);
			galutinis_pazymys_vid_ = std::move(studentas.galutinis_pazymys_vid_);
			galutinis_pazymys_med_ = std::move(studentas.galutinis_pazymys_med_);
		}
		return *this;
	}

	// IO operatoriai
	friend std::istream &operator>>(std::istream &, Studentas &);
	friend std::ostream &operator<<(std::ostream &, const Studentas &);

	void setVardas(const std::string &vardas) override { vardas_ = vardas; }
	std::string getVardas() const override { return vardas_; }

	void setPavarde(const std::string &pavarde) override { pavarde_ = pavarde; }
	std::string getPavarde() const override { return pavarde_; }

	std::vector<int> getPazymiai() const { return pazymiai_; }
	int getEgzaminoPazymys() const { return egzamino_pazymys_; }
	double getGalutinisPazymysVid() const { return galutinis_pazymys_vid_; }
	double getGalutinisPazymysMed() const { return galutinis_pazymys_med_; }

	void setPazymiai(std::vector<int> pazymiai) { pazymiai_ = pazymiai; }
	void setEgzaminoPazymys(int egzamino_pazymys) { egzamino_pazymys_ = egzamino_pazymys; }
	void addPazymys(int pazymys) { pazymiai_.push_back(pazymys); }
	void removeLastPazymys(int pazymys) { pazymiai_.pop_back(); }

	void galutinis_pazymys_vid();
	void galutinis_pazymys_med();

	bool vardoPaieska() const { return vardas_.find("Vardas") == 0; }
	bool pavardesPaieska() const { return pavarde_.find("Pavarde") == 0; }

	bool operator==(const Studentas &s) const
	{
		if (vardas_ == s.vardas_ && pavarde_ == s.pavarde_ && pazymiai_ == s.pazymiai_ &&
			egzamino_pazymys_ == s.egzamino_pazymys_ && galutinis_pazymys_vid_ == s.galutinis_pazymys_vid_ && galutinis_pazymys_med_ == s.galutinis_pazymys_med_)
		{
			return true;
		}
		else
			return false;
	}

	bool operator!=(const Studentas &s) const
	{
		if (vardas_ != s.vardas_ || pavarde_ != s.pavarde_ || pazymiai_ != s.pazymiai_ ||
			egzamino_pazymys_ != s.egzamino_pazymys_ || galutinis_pazymys_vid_ != s.galutinis_pazymys_vid_ || galutinis_pazymys_med_ != s.galutinis_pazymys_med_)
		{
			return true;
		}
		else
			return false;
	}
};

#endif
