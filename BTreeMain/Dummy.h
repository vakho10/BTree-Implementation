#ifndef DUMMY_H
#define DUMMY_H

#pragma once

#include <string>

class Dummy
{
public:
	const std::string name;
	const int value;
	Dummy(std::string _name, int value) : name(_name), value(value) {}

	// Helper function which returns Dummy objects 
	// printable value (which will be used by traverse functions)
	static std::string printableValueFunc(Dummy* d)
	{
		return d->name;
	}
};

// Dummy key extractor
struct DummyExtractor
{
	int operator()(Dummy& arg)
	{
		return arg.value;
	}
};

// Dummy comparator
struct DummyComparator
{
	bool operator()(Dummy& d1, Dummy& d2)
	{
		return d1.value < d2.value;
	}
};

#endif