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

// Dummy object's comparator
struct DummyComparator : public std::binary_function<Dummy*, Dummy*, bool>
{
	bool operator()(const Dummy* d1, const Dummy*  d2) const
	{
		return d1->value < d2->value;
	}
};

#endif