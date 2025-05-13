#ifndef ZMOGUS_H
#define ZMOGUS_H
#include "../santrauka.h"
#include "vector.h"

class Zmogus
{
protected:
	std::string vardas_;
	std::string pavarde_;

public:
	Zmogus() = default;
	Zmogus(const std::string &vardas, const std::string &pavarde)
		: vardas_(vardas), pavarde_(pavarde) {}

	virtual void setVardas(const std::string &vardas) = 0;
	virtual std::string getVardas() const = 0;

	virtual void setPavarde(const std::string &pavarde) = 0;
	virtual std::string getPavarde() const = 0;

	virtual ~Zmogus()
	{
		vardas_ = "";
		pavarde_ = "";
	}
};
#endif