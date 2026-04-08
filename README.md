# Advanced-CPP-HW12



*Problem 1:*
⚫ Extend Tuple2 (on canvas) so that you can
get elements by type as well as index
⚫ Tuple2<int, double> t2id(4, 5.6);
cout << "get<1>(t2id) = " << get<1>(t2id) << endl; // Works now
cout << "get<int>(t2id) = " << get<int>(t2id) << endl; // HW



*Problem 2:*
⚫ Someone might expect std::sort to work on
std::list
⚫ Is this reasonable?
⚫ If so, how would you implement it?



*Problem 3*
⚫ This exercise is to practice using factories
⚫ Use the classes in factory.h to create
⚫ an abstract factory train_factory for building train cars like
Locomotive, FreightCar and Caboose
⚫ The car classes don’t need to have any particularly train-like
functionality, so don’t worry too much about how many
methods you define (if you give them any methods at all).
⚫ concrete factories model_train_factory and
real_train_factory
⚫ Use these factories to create cars for a model train
⚫ You can model your solution off of factory.cpp in Canvas



*Problem 4 (EC)*
⚫ Often factory methods need to be called with
specific arguments.
⚫ Create a flexible_abstract_factory template with a
corresponding concrete factory template that that
can take signatures
⚫ If you just give it a class instead of a signature, then it calls
the default constructor like our original factory
⚫ flexible_abstract_factory
<Locomotive(double /* horsepower */),
FreightCar(long /* capacity */),
Caboose>
Redo 12-3 with this class template
**Due in 2 weeks**



*Problem 5 (EC)*
⚫ Instead of having a ModelLocomotive class,
etc., define them as template specializations
Model<Locomotive>, etc.
⚫ Now change the concrete_factory class so
you can create a model train factory like:
using parameterized_model_train_factory
parameterized_factory<train_factory, Model>;
⚫ Redo 12-3 with this class
**Due in 2 weeks**