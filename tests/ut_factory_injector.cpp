/**
 * Copyright (c) 2020 Emanuele Bellocchia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Includes
 */

// Google test
#include "gtest/gtest.h"
// Utils
#include "ut_utils.hpp"
// Class under test
#include "factory_injector.hpp"


/*
 * Using directives
 */
using namespace factory_injector;

/*
 * Classes
 */

// Dummy class interface
class IDummyClass
{
    public:
      virtual ~IDummyClass(void) = default;
};

// Dummy class 1
class DummyClass1 : public IDummyClass
{};

// Dummy class 2
class DummyClass2 : public IDummyClass
{};

// Dummy class factory interface
class IDummyClassFactory : public FactoryTraits<IDummyClassFactory, IDummyClass>
{
    public:
      virtual ~IDummyClassFactory(void)     = default;
      virtual tObjectPtr Create(void) const = 0;
};

// Dummy class 1 factory
class DummyClass1Factory : public IDummyClassFactory
{
    public:
      tObjectPtr Create(void) const override
      {
          return std::make_unique<DummyClass1>();
      }
};

// Dummy class 2 factory
class DummyClass2Factory : public IDummyClassFactory
{
    public:
      tObjectPtr Create(void) const override
      {
          return std::make_unique<DummyClass2>();
      }
};

// Other class factory interface
class IOtherClassFactory : public FactoryTraits<IOtherClassFactory, IDummyClass>
{
    public:
      virtual ~IOtherClassFactory(void) = default;
};

/*
 * Test fixture
 */

// Fixture for factory injector
class UTFactoryInjector : public ::testing::Test
{
    /*
     * Public methods
     */
    public:
        // Constructor
        UTFactoryInjector(void)
        {}

    /*
     * Members
     */
    protected:
        FactoryInjector mFactoryInjector;
};

/*
 * Tests
 */

// Test for RegisterFactory and GetFactory
TEST_F(UTFactoryInjector, RegisterAndGetFactory)
{
    // Register factory
    mFactoryInjector.RegisterFactory<DummyClass1Factory>();

    // Get factory from class type
    auto& factory = mFactoryInjector.GetFactory<DummyClass1Factory>();
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass1Factory>(factory))  << "Wrong factory type when getting from class type";
    EXPECT_FALSE(ut_utils::IsOfType<DummyClass2Factory>(factory)) << "Wrong factory type when getting from class type";

    // Get factory from interface type
    factory = mFactoryInjector.GetFactory<IDummyClassFactory>();
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass1Factory>(factory))  << "Wrong factory type when getting from interface type";
    EXPECT_FALSE(ut_utils::IsOfType<DummyClass2Factory>(factory)) << "Wrong factory type when getting from interface type";
}

// Test for overwriting an existent factory
TEST_F(UTFactoryInjector, OverwriteAndGetFactory)
{
    // Register factory
    mFactoryInjector.OverwriteFactory<DummyClass1Factory>();

    // Get factory from interface
    auto& factory = mFactoryInjector.GetFactory<IDummyClassFactory>();
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass1Factory>(factory))  << "Wrong factory type after registering";
    EXPECT_FALSE(ut_utils::IsOfType<DummyClass2Factory>(factory)) << "Wrong factory type after registering";

    // Overwrite the previous factory
    mFactoryInjector.OverwriteFactory<DummyClass2Factory>();

    // Get factory from interface
    auto& new_factory = mFactoryInjector.GetFactory<DummyClass2Factory>();
    EXPECT_FALSE(ut_utils::IsOfType<DummyClass1Factory>(new_factory)) << "Wrong factory type after overwriting";
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass2Factory>(new_factory))  << "Wrong factory type after overwriting";
}

// Test for registering a factory that is already existent
TEST_F(UTFactoryInjector, RegisterAlreadyExistentFactory)
{
    // Register factory
    mFactoryInjector.RegisterFactory<DummyClass1Factory>();

    // Register a factory that is already registered, shall throw exception
    EXPECT_THROW(mFactoryInjector.RegisterFactory<DummyClass1Factory>(), FactoryAlreadyRegisteredEx) << "Exception not thrown when registering an already existent factory";
}

// Test for getting a not-existent factory
TEST_F(UTFactoryInjector, GetNotExistentFactory)
{
    // Register factory
    mFactoryInjector.RegisterFactory<DummyClass1Factory>();

    // Get a factory that is not existent, shall throw exception
    EXPECT_THROW(mFactoryInjector.GetFactory<IOtherClassFactory>(), FactoryNotRegisteredEx) << "Exception not thrown when getting a not existent factory";
}

// Test for CreateObject
TEST_F(UTFactoryInjector, CreateObject)
{
    // Register factory
    mFactoryInjector.RegisterFactory<DummyClass1Factory>();

    // Create object from class type
    auto obj_ptr = mFactoryInjector.CreateObject<DummyClass1Factory>();
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass1>(*obj_ptr)) << "Wrong object type when getting from class type";

    // Create object from interface type
    obj_ptr = mFactoryInjector.CreateObject<IDummyClassFactory>();
    EXPECT_TRUE(ut_utils::IsOfType<DummyClass1>(*obj_ptr)) << "Wrong object type when getting from interface type";
}
