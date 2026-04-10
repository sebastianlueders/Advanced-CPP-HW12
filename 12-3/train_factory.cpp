#include "train_factory.hpp"
#include <iostream>
#include <memory>
using namespace std;
using namespace cspp51045;

// Abstract train car types
struct Locomotive {virtual void describe() = 0; virtual ~Locomotive() = default; };
struct FreightCar {virtual void describe() = 0; virtual ~FreightCar() = default; };
struct Caboose {virtual void describe() = 0; virtual ~Caboose() = default; };

// Concrete model train implementations
struct ModelLocomotive : public Locomotive { void describe() { cout << "Model Locomotive\n"; }};
struct ModelFreightCar : public FreightCar { void describe() { cout << "Model FreightCar\n"; }};
struct ModelCaboose : public Caboose { void describe() { cout << "Model Caboose\n";}};

// Concrete real train implementations
struct RealLocomotive : public Locomotive { void describe() { cout << "Real Locomotive\n"; }};
struct RealFreightCar : public FreightCar { void describe() { cout << "Real FreightCar\n"; }};
struct RealCaboose : public Caboose { void describe() { cout << "Real Caboose"; }};

// Factory type aliases
using AbstractTrainFactory = train_factory<Locomotive, FreightCar, Caboose>;
using model_train_factory = concrete_factory<AbstractTrainFactory, ModelLocomotive, ModelFreightCar, ModelCaboose>;
using real_train_factory = concrete_factory<AbstractTrainFactory, RealLocomotive, RealFreightCar, RealCaboose>;


int main() 
{
    // Build model train
    auto factory = make_unique<model_train_factory>(); // Swap to real_train_factory for real trains!

    auto loco = factory->create<Locomotive>();
    auto fc = factory->create<FreightCar>();
    auto cab = factory->create<Caboose>();

    loco->describe();
    fc->describe();
    cab->describe();

    factory->printLog();

    return 0;
}