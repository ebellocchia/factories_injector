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
#include <iostream>
#include <real_obj.hpp>
#include <test_obj.hpp>

/*
 * Functions
 */

// Main function
int main(int argc, char *argv[])
{
    // Create factory injector instance.
    // It can be wrapped in a singleton to make it globally accessible.
    factory_injector::FactoryInjector fi;


    // Register RealObjFactory for IObjFactory
    // OverwriteFactory can also be used here, it's the same
    fi.RegisterFactory<RealObjFactory>();
    // Create object from the factory interface type, we'll get a RealObj
    // The CreateObject method internally calls the Create method of the factory, so it's equivalent to: GetFactory<FactoryType>().Create(...)
    auto obj = fi.CreateObject<IObjFactory>(10);
    obj->SayHi();

    // Alternative way for creating object: use GetFactory and call the factory method explicitly.
    // This is also better if you need to use the factory multiple times, like in a loop.
    auto& factory = fi.GetFactory<IObjFactory>();
    // Let's create some objects
    for (int i = 0; i < 5; i++)
    {
        obj = factory.Create(i);
        obj->SayHi();
    }


    // Now use the TestObjFactory for IObjFactory
    // This should be in a test code, but it's here just for making an example
    fi.OverwriteFactory<TestObjFactory>();
    // Create again the object from the factory interface type
    // We'll get a TestObj
    obj = fi.CreateObject<IObjFactory>(10);
    obj->SayHi();

    return 0;
}
