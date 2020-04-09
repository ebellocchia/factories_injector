# C++ Factories injector

## Introduction

This is a template-based utility library composed of just a couple of header files, hence it doesn't need to be compiled and linked.\
It allows run-time injection of factories, making the code completely independent from specific factory types and only dependent on factory interfaces.\
In this way, you can use a real factory producing real objects in the production code, while in the testing code you can dynamically inject a factory producing test objects (e.g. stubs or mocks) without modifying a single line.\
For being injected by the library, the factories shall follow the [https://en.wikipedia.org/wiki/Abstract_factory_pattern](abstract factory pattern) and inherit from the *FactoryTraits* structure defined by the library.

## Installation

As already said, you just have to include the header files. There is no dependency and nothing to compile, since the entire code is based on templates.\
The code is compliant to C++14 standard, so you need a compiler supporting C++14.\

To compile the example and the unit tests, just run:

    cmake .
    make

In the folder where the *CMakeLists.txt* file is located.\
A *bin* folder will be created inside with the compiled files.

**NOTE:** for compiling unit tests, you need *googletest* to be installed.

## Usage

To inject a factory, you have to create a factory interface (*abstract factory*) that inherits from the *FactoryTraits* structure, by specifying its type and the interface of the object to be created.

**Example**

    // Object interface
    class IObj
    {};

    // Abstract factory for IObj
    class IObjFactory : public factory_injector::FactoryTraits<IObjFactory, IObj>
    {
        public:
            virtual ~IObjFactory(void) = default;

            // tObjectPtr is defined by FactoryTraits as std::unique_ptr<IObj>
            virtual tObjectPtr Create(/* Some parameters */) const = 0;
    };

Then you can create your real factory classes (*concrete factories*) inheriting from the abstract factory and creating real objects.

**Example**

    // A real object, used for production
    class MyRealObj : public IObj
    {};

    // A test object, used for testing
    class MyTestObj : public IObj
    {};

    // Another test object, used for some other types of tests
    class AnotherTestObj : public IObj
    {};

    // Concrete factory producing MyRealObj
    class MyRealObjFactory : public IObjFactory
    {
        public:
            tObjectPtr Create(/* Some parameters */) const override
            {
                return std::make_unique<MyRealObj>(/* Some parameters */);
            }
    };

    // Concrete factory producing MyTestObj
    class MyTestObjFactory : public IObjFactory
    {
        public:
            tObjectPtr Create(/* Some parameters */) const override
            {
                return std::make_unique<MyTestObj>(/* Some parameters */);
            }
    };

    // Concrete factory producing AnotherTestObj
    class AnotherTestObjFactory : public IObjFactory
    {
        public:
            tObjectPtr Create(/* Some parameters */) const override
            {
                return std::make_unique<AnotherTestObj>(/* Some parameters */);
            }
    };

Finally, you have to register the concrete factory you want to use with one of the following methods:
- *FactoryInjector::RegisterFactory<FactoryType>()* will register the factory type only if not already registered, otherwise a *FactoryAlreadyRegisteredEx* exception is thrown
- *FactoryInjector::OverwriteFactory<FactoryType>()* will register the factory type in any case, overwriting it if already registered

To get back a factory, you can use the *FactoryInjector::GetFactory<FactoryType>()* method. Here you can either specify the concrete or the abstract factory type, but it's suggested to use the abstract one so that the code is completely independent from concrete factories.\
In case you try to get a factory type that is not registered, a *FactoryNotRegisteredEx* will be thrown.

**Example**

    // You can wrap the factory injector in a singleton to make it globally accessible, or pass it around the classes that need it
    factory_injector::FactoryInjector fi;

    // Register MyRealObjFactory. From now on, all the code now will use MyRealObjFactory when calling GetFactory, this producing MyRealObj
    // OverwriteFactory can also be used here, it's the same
    fi.RegisterFactory<MyRealObjFactory>();

    //
    // Somewhere else in the production code...
    //

    // Get the factory by the abstract type
    // It will get MyRealObjFactory, MyTestObjFactory or AnotherTestObjFactory depending on what it's registered
    auto& obj_factory = fi.GetFactory<IObjFactory>();
    // Let's produce some object instances. MyRealObj, MyTestObj or AnotherTestObj will be produced depending on the registered factory.
    // In this way, the code is independent from the concrete factory, which can be injected dynamically.
    for (int i = 0; i < 10; i++)
    {
        // obj is of type std::unique_ptr<IObj>
        auto obj = obj_factory.Create(/* Some parameters */);
        // Do something with the object
    }

    // Alternatively, there is a helper method called CreateObject. It simply calls internally the factory Create method, if defined.
    // It's just a shortcut, if you a different name for the method or you need to use the factory multiple times (e.g. in a loop),
    // just use the GetFactory as described previously
    auto obj = fi.CreateObject<IObjFactory>(/* Some parameters */);

    //
    // In a test code, we need to use MyTestObj
    //

    // Register MyTestObjFactory (MyRealObjFactory will be overwritten if registered)
    fi.OverwriteFactory<MyTestObjFactory>();
    // Now, the previous code will use MyTestObj objects instead of MyRealObj

    //
    // In another test code, we need to use AnotherTestObj
    //

    // Register AnotherTestObjFactory
    fi.OverwriteFactory<AnotherTestObjFactory>();
    // Now, the previous code will use AnotherTestObj objects instead of MyRealObj

Of course, you can register as many factory types as you want, as long as they inherit from a different interface.

## How it works

The base concept is quite simple. The *FactoryInjector* class is keeping track of the registered types by means of a hash table.\
Using the *FactoryTraits*, the class deducts the interface type of the concrete factory. Then, it creates an instance of the concrete factory and inserts it in the hash table, associating it with the *typeid* of the abstract factory. Therefore, if you register another concrete factory inheriting from the same abstract factory, it will overwrite the existent one because the *typeid* of the abstract factory is always the same and it is used as a key of the hash table.\
When you get a factory, a reference to the concrete instance is simply returned by searching for the *typeid* of the abstract factory in the hash table.\
The library takes advantage of smart pointers, move semantics, perfect forwarding and variadic templates to be as most generic as possible.

## License

This software is available under the MIT license.
