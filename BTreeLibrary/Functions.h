#pragma once

/*
	EXTRACTORS
*/
// Basic key extractor (returns plain object)
template<typename T>
struct PlaintKeyExtractor
{
	T operator()(T& elem)
	{
		return elem;
	}
};

/*
	COMPARATORS

	You should pass key extractor when you are
	dealling with objects and not primitives.
*/
// Less comparator
template<typename T, typename Extract = PlaintKeyExtractor<T>>
struct Less
{
	bool operator()(T& firstArg, T& secondArg)
	{
		return Extract(firstArg) < Extract(secondArg);
	}
};