#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "vector.h"
#include "studentas.h"
#include "../santrauka.h"

TEST_CASE("Konstruktorius", "[constructor]")
{
	Studentas studentas;
	studentas.galutinis_pazymys_vid();
	studentas.galutinis_pazymys_med();
	REQUIRE(studentas == Studentas("Vardenis", "Pavardenis", {}, 0));
}

TEST_CASE("Destruktorius", "[destructor]")
{
	Studentas *studentas = new Studentas("Vardenis", "Pavardenis", {1, 2, 3}, 8);
	studentas->~Studentas();
	REQUIRE(studentas->getEgzaminoPazymys() == 0);
	REQUIRE(studentas->getPazymiai().empty());
	operator delete(studentas);
}

TEST_CASE("Copy constructor", "[copy constructor]")
{
	Studentas studentas1("Jonas", "Jonaitis", {8, 9, 10}, 7);
	Studentas studentas2(studentas1);
	REQUIRE(studentas2.getVardas() == "Jonas");
	REQUIRE(studentas2.getPavarde() == "Jonaitis");
	REQUIRE(studentas2.getEgzaminoPazymys() == 7);
	REQUIRE(studentas2.getPazymiai() == vector<int>{8, 9, 10});
}

TEST_CASE("Copy assignment", "[copy assignment]")
{
	Studentas studentas1("Jonas", "Jonaitis", {8, 9, 10}, 7), studentas2;
	studentas2 = studentas1;
	REQUIRE(studentas2.getVardas() == "Jonas");
	REQUIRE(studentas2.getPavarde() == "Jonaitis");
	REQUIRE(studentas2.getEgzaminoPazymys() == 7);
	REQUIRE(studentas2.getPazymiai() == vector<int>{8, 9, 10});
}

TEST_CASE("Move constructor", "[move constructor]")
{
	Studentas studentas1("Jonas", "Jonaitis", {8, 9, 10}, 7);
	Studentas studentas2(std::move(studentas1));
	REQUIRE(studentas2.getVardas() == "Jonas");
	REQUIRE(studentas2.getPavarde() == "Jonaitis");
	REQUIRE(studentas2.getEgzaminoPazymys() == 7);
	REQUIRE(studentas2.getPazymiai() == vector<int>{8, 9, 10});
	REQUIRE_FALSE(studentas1 == studentas2);
}

TEST_CASE("Move assignment", "[move assignment]")
{
	Studentas studentas1("Jonas", "Jonaitis", {8, 9, 10}, 7), studentas2;
	studentas2 = std::move(studentas1);
	REQUIRE(studentas2.getVardas() == "Jonas");
	REQUIRE(studentas2.getPavarde() == "Jonaitis");
	REQUIRE(studentas2.getEgzaminoPazymys() == 7);
	REQUIRE(studentas2.getPazymiai() == vector<int>{8, 9, 10});
	REQUIRE_FALSE(studentas1 == studentas2);
}

TEST_CASE("Getters", "[getters]")
{
	Studentas s1("Jonas", "Jonaitis", {8, 9, 10}, 7);
	REQUIRE(s1.getVardas() == "Jonas");
	REQUIRE(s1.getPavarde() == "Jonaitis");
	REQUIRE_FALSE(s1.getPazymiai().empty());
}
