#ifndef TUPLE_H
#  define TUPLE_H

#include <type_traits>


namespace mpcs {
// First try at tuple
template<typename ...Ts> struct Tuple1; // primary struct for storing Tuples

template<> struct Tuple1<> {}; // explicit specialization for empty tuples

template<typename T, typename ...Ts> 
struct Tuple1<T, Ts...>
{
	Tuple1(T const &t, Ts const &... ts) : val(t), restOfVals(ts...) {}
	T val;
	Tuple1<Ts...> restOfVals;
}; // template struct for creating a tuple with the first val seperated from restOfVals

// Indirect through a class because can't partially specialize template function

template<int i, typename ...Ts> // Template for getter which gets<i>
struct Getter1;


// Simplified: Recurses on tuple without first element until i = 0 i.e. element i is the first element

// If the first argument is zero, then return the first value in the Tuple
template<typename T,  typename ...Ts>
struct Getter1<0, T, Ts...> {
	static auto &get(Tuple1<T, Ts...> &tup) {
		return tup.val; // returns the val of the first value in the tuple (T)
	}
};

// If the Getter1 first arg is "i" (not zero), then recurses on a tuple of restOfVals
// Decrements i for the next call
template<int i, typename T, typename ...Ts>
struct Getter1<i, T, Ts...> {
	static auto &get(Tuple1<T, Ts...> &tup) {
		return Getter1<i-1, Ts...>::get(tup.restOfVals);
	}
};

// Allows us to call Getter1<i, Ts...>::get(tup) -> The interface for get
// i.e. get<1>(myTuple)
template<int i, typename ...Ts>
auto & get(Tuple1<Ts...> &tup) {
	return Getter1<i, Ts...>::get(tup);
}

/*
// Here is an example of what the template instantiations generate
struct Tuple1<int, float> {
	int val;
	Tuple1<float> restofVals;
};

struct Tuple1<float> {
	float val;
	Tuple1<> restOfVals;
};

*/
// Tuple2 is almost identical to tuple1 except it uses the empty base optimization
// for better space utilization as demonstrated in Tuple.cpp
template<typename ...Ts> struct Tuple2;

template<> struct Tuple2<> {};

template<typename T, typename ...Ts>
struct Tuple2<T, Ts...> : public Tuple2<Ts...>  // Inheritance Optimization 
{
	Tuple2(T const &t, Ts const &... ts) : Tuple2<Ts...>(ts...), val(t) {}
	T val;
};
// This optimization works because instead of storing restOfVals as a data member
// (which forces Tuple1<> to occupy at least 1 byte even when empty),
// Tuple2 inherits from Tuple2<Ts...> as a base class.
//
// C++ allows empty base classes to have zero size (Empty Base Optimization),
// so Tuple2<> contributes no wasted bytes to the layout.
//
// Example: Tuple2<int, float> layout:
// [ float val | int val ]  -- no wasted padding byte
// vs Tuple1<int, float>:
// [ int val | float val | 1 byte (empty Tuple1<>) ] + alignment padding


// --------------------------------------------------------------------------------------------------

// Indirect through a class because can't partially specialize template arguments
// 
// auto Getter2<0, int, float>::get(Tuple2<int, float> &t2) { return t2.val }
// auto Getter2<1, int, float>::get(Tuple2<int, float> &t2) { 
//   Tuple2<float> &restOfVals = t2;
//   return Getter2<0>(restOfVals);
// }


template<int i, typename ...Ts>
struct Getter2;

// Base case (return the first val)
template<typename T, typename ...Ts>
struct Getter2<0, T, Ts...> {
	static auto &get(Tuple2<T, Ts...> &tup) {
		return tup.val;
	}
};

// Recursive Case (drops first element)
template<int i, typename T, typename ...Ts>
struct Getter2<i, T, Ts...> {
	static auto &get(Tuple2<T, Ts...> &tup) {
		Tuple2<Ts...> &restOfVals = tup;
		return Getter2<i - 1, Ts...>::get(restOfVals);
	}
};

// Alias template
template<int i, typename ...Ts>
auto & get(Tuple2<Ts...> &tup) {
	return Getter2<i, Ts...>::get(tup);
}




// Revised Implementation using type reference:

template<typename Expected, typename ...Elements>
struct getByType;

// Base Case
template<typename Expected, typename ...Elements>
struct getByType<Expected, Expected, Elements...> {
	static auto &get(Tuple2<Expected, Elements...> &tup) {
		return tup.val;
	}
};

// Recursive Case
template<typename Expected, typename T, typename ...Elements>
struct getByType<Expected, T, Elements...> {
	static auto &get(Tuple2<Expected, Elements...> &tup)
	{
		Tuple2<Elements...> &restOfVals = tup;
		return getByType<Expected, Elements...>::get(tup);
	}
};

// Alis Template
template<typename Expected, typename ...Elements>
auto &get(Tuple2<Elements...> &tup) {
	return getByType<Expected, Elements...>::get(tup);
}

}

#endif