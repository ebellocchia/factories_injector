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

#ifndef _EXAMPLE_REAL_OBJ_HPP_
#define _EXAMPLE_REAL_OBJ_HPP_

/*
 * Includes
 */
#include <obj.hpp>

/*
 * Classes
 */

// Real object implementing IObj, used in real code
class RealObj : public IObj
{
    public:
        RealObj(const int cValue) :
          mValue(cValue)
        {}

        void SayHi(void) const override
        {
            std::cout << "Hi from real object! My value is: " << (mValue * 10) << std::endl;
        }

    private:
      int mValue;
};

// Concrete factory that creates RealObj instances
class RealObjFactory : public IObjFactory
{
    public:
      tObjectPtr Create(const int cValue) const override
      {
          return std::make_unique<RealObj>(cValue);
      }
};

#endif  // _EXAMPLE_REAL_OBJ_HPP_
