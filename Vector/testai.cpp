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

// v3.0 versijos testavimai

TEST_CASE("Vector konstruktorius", "[vector]")
{
	vector<int> vektorius;
	REQUIRE(vektorius.size() == 0);
	REQUIRE(vektorius.capacity() == 0);
	REQUIRE(vektorius.empty());
}

TEST_CASE("Fill konstrutorius", "[vector]")
{
	vector<int> vektorius1(5, 42);
	REQUIRE(vektorius1.size() == 5);
	for (int i = 0; i < 5; ++i)
		REQUIRE(vektorius1[i] == 42);
}

TEST_CASE("Copy konstruktorius", "[vector]")
{
	vector<int> original(3, 7);
	vector<int> copy(original);
	REQUIRE(copy.size() == original.size());
	for (int i = 0; i < copy.size(); ++i)
		REQUIRE(copy[i] == 7);
}

TEST_CASE("Move konstruktorius", "[vector]")
{
	vector<int> laikinas(3, 8);
	vector<int> moved(std::move(laikinas));
	REQUIRE(moved.size() == 3);
	for (int i = 0; i < 3; ++i)
		REQUIRE(moved[i] == 8);
}

TEST_CASE("Initializer list konstruktorius", "[vector]")
{
	vector<int> vektorius2{1, 2, 3};
	REQUIRE(vektorius2.size() == 3);
	REQUIRE(vektorius2[0] == 1);
	REQUIRE(vektorius2[1] == 2);
	REQUIRE(vektorius2[2] == 3);
}

TEST_CASE("Assigment operatorius", "[vector]")
{
	vector<int> v1(4, 10);
	vector<int> v2;
	v2 = v1;
	REQUIRE(v2.size() == 4);
	for (int i = 0; i < 4; ++i)
		REQUIRE(v2[i] == 10);

	vector<int> v3(2, 5);
	v3 = std::move(v2);
	REQUIRE(v3.size() == 4);
}

// perdaryt

TEST_CASE("Assignment Operators", "[vector]")
{
	vector<int> v1(4, 10);
	vector<int> v2;
	v2 = v1;
	REQUIRE(v2.size() == 4);
	for (int i = 0; i < 4; ++i)
		REQUIRE(v2[i] == 10);

	vector<int> v3(2, 5);
	v3 = std::move(v2);
	REQUIRE(v3.size() == 4);
}

TEST_CASE("Element Access", "[vector]")
{
	vector<int> v{5, 6, 7};
	REQUIRE(v[0] == 5);
	REQUIRE(v.at(1) == 6);
	REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
	REQUIRE(v.front() == 5);
	REQUIRE(v.back() == 7);
}

TEST_CASE("Data Pointer", "[vector]")
{
	vector<int> v{1, 2, 3};
	int *ptr = v.data();
	REQUIRE(*ptr == 1);
}

TEST_CASE("Iterators", "[vector]")
{
	vector<int> v{1, 2, 3};
	REQUIRE(*v.begin() == 1);
	REQUIRE(*(v.end() - 1) == 3);
	REQUIRE(*v.cbegin() == 1);
	REQUIRE(*(v.cend() - 1) == 3);
}

TEST_CASE("Reverse Iterators", "[vector]")
{
	vector<int> v{1, 2, 3};
	REQUIRE(*v.rbegin() == 3);
	REQUIRE(*(v.rend() - 1) == 1);
	REQUIRE(*v.crbegin() == 3);
	REQUIRE(*(v.crend() - 1) == 1);
}

TEST_CASE("Size, Capacity, Empty", "[vector]")
{
	vector<int> v;
	REQUIRE(v.size() == 0);
	REQUIRE(v.capacity() == 0);
	REQUIRE(v.empty());
	v.reserve(10);
	REQUIRE(v.capacity() >= 10);
}

TEST_CASE("Resize", "[vector]")
{
	vector<int> v(5, 1);
	v.resize(10);
	REQUIRE(v.size() == 10);
	v.resize(3);
	REQUIRE(v.size() == 3);
	v.resize(6, 7);
	REQUIRE(v[3] == 7);
	REQUIRE(v[5] == 7);
}

TEST_CASE("Shrink to Fit", "[vector]")
{
	vector<int> v(100, 1);
	v.resize(50);
	v.shrink_to_fit();
	REQUIRE(v.capacity() == v.size());
}

TEST_CASE("Push and Pop", "[vector]")
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	REQUIRE(v.back() == 20);
	v.pop_back();
	REQUIRE(v.back() == 10);
}

TEST_CASE("Insert and Erase", "[vector]")
{
	vector<int> v{1, 2, 4};
	v.insert(v.begin() + 2, 3);
	REQUIRE(v[2] == 3);
	v.erase(v.begin() + 2);
	REQUIRE(v[2] == 4);
}

TEST_CASE("Insert Multiple Elements", "[vector]")
{
	vector<int> v{1, 4};
	v.insert(v.begin() + 1, 2);
	v.insert(v.begin() + 2, 3);
	REQUIRE(v[1] == 2);
	REQUIRE(v[2] == 3);
	REQUIRE(v[3] == 4);
}

TEST_CASE("Erase Range", "[vector]")
{
	vector<int> v{1, 2, 3, 4, 5};
	v.erase(v.begin() + 1, v.begin() + 4);
	REQUIRE(v.size() == 2);
	REQUIRE(v[0] == 1);
	REQUIRE(v[1] == 5);
}

TEST_CASE("Assign with Range and List", "[vector]")
{
	vector<int> source{7, 8, 9};
	vector<int> v;
	v.assign(source.begin(), source.end());
	REQUIRE(v.size() == 3);
	REQUIRE(v[0] == 7);

	v.assign({1, 2, 3});
	REQUIRE(v.size() == 3);
	REQUIRE(v[2] == 3);
}

TEST_CASE("Clear and Assign", "[vector]")
{
	vector<int> v{1, 2, 3};
	v.clear();
	REQUIRE(v.empty());
	v.assign(5, 9);
	for (int i = 0; i < 5; ++i)
		REQUIRE(v[i] == 9);
}

TEST_CASE("Relational Operators", "[vector]")
{
	vector<int> a{1, 2, 3};
	vector<int> b{1, 2, 3};
	vector<int> c{1, 2, 4};
	REQUIRE(a == b);
	REQUIRE(a != c);
	REQUIRE(a < c);
	REQUIRE(c > a);
	REQUIRE(a <= b);
	REQUIRE(c >= b);
}

TEST_CASE("Swap", "[vector]")
{
	vector<int> a{1, 2}, b{3, 4};
	a.swap(b);
	REQUIRE(a[0] == 3);
	REQUIRE(b[0] == 1);
}
