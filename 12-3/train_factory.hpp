#ifndef TRAIN_FACTORY_H
#define TRAIN_FACTORY_H
#include<tuple>
#include<memory>
#include<chrono>
#include<string>
#include<typeinfo>
#include<vector>
#include<iostream>
#include<ctime>
using std::tuple;
using std::unique_ptr;
using std::make_unique;
#pragma warning(disable : 4250)

namespace cspp51045 {

// Empty tag struct for carrying a type T as a function arg in order to overload doCreate by type
template<typename T>
struct TT {};

// A single-type abstract factory interface with virtual doCreate
template<typename T>
struct abstract_creator {
    virtual unique_ptr<T> doCreate(TT<T> &&) = 0;
};

// Log entry storing the type name and timestamp of each creation
struct CreationLogEntry 
{
    std::string typeName;
    std::chrono::system_clock::time_point timestamp;
};

// Derived abstract_factory 
// Variadic template which requires overload for all Ts, example:
// struct abstract_factory<A, B, C>
//     : public abstract_creator<A>,
//       public abstract_creator<B>,
//       public abstract_creator<C>
// {
// };

template<typename... Ts>
struct train_factory : public abstract_creator<Ts>... {

    // allows us to call factory.create<U>() where factory is a factory<A, B..., U> instance
	template<class U> 
    unique_ptr<U> create() {
        log.push_back({ typeid(U).name(), std::chrono::system_clock::now() }); 
		abstract_creator<U> &creator = *this; // Creates a reference to the abstract_creator<U> base subobject
		return creator.doCreate(TT<U>());  // Return a unique_ptr<U> of type U (using the empty TT object to determine type U)
	}

    void printLog() const 
    {
        for(auto &entry : log)
        {
            std::time_t t = std::chrono::system_clock::to_time_t(entry.timestamp);
            std::cout << "Created: " << entry.typeName << " at " << std::ctime(&t);
        }
    }

	virtual ~train_factory() = default;

private:

    std::vector<CreationLogEntry> log;
};

// Ties specific Abstract type to a Concrete implementation within a given AbstractFactory
// By inheriting from AbstractFactory, it provides an override of doCreate(TT<Abstract>) that returns a concrete object
template<typename AbstractFactory, typename Abstract, typename Concrete>
struct concrete_creator : virtual public AbstractFactory {
	unique_ptr<Abstract> doCreate(TT<Abstract> &&) override {
		return make_unique<Concrete>(); // Return Concrete object as an Abstract unique_ptr 
	}
};

// Tells the compiler that the template exists with those parameters but deliberately prevents direct instantiation
template<typename AbstractFactory, typename... ConcreteTypes>
struct concrete_factory;

// i.e. concrete_factory<train_factory<Car, Truck>, Sedan, Pickup> inherits from:
// concrete_creator<..., Car, Sedan> and concrete_creator<..., Truck, Pickup>
template<typename... AbstractTypes, typename... ConcreteTypes>
struct concrete_factory<train_factory<AbstractTypes...>, ConcreteTypes...> 
  : public concrete_creator<train_factory<AbstractTypes...>, AbstractTypes, ConcreteTypes>... {};
}

// Abstract train component types




#endif
