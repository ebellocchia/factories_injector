/**
 * @copyright Copyright (c) 2020 Emanuele Bellocchia
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
 *
 * @file  not_copyable_movable.hpp
 * @brief Declaration of base classes for disabling move/copy operations
 *
 */

#ifndef _FACTORY_INJECTOR_NOT_COPYABLE_MOVABLE_HPP_
#define _FACTORY_INJECTOR_NOT_COPYABLE_MOVABLE_HPP_

/*
 * Namespaces
 */
namespace factory_injector
{

/**
 * @brief Base class for disabling copy operations
 */
class NotCopyable
{
    /*
     * Protected methods
     */
    protected:
        // Default constructor is needed
        NotCopyable(void)                          = default;
        // Forbid class copy
        NotCopyable(const NotCopyable&)            = delete;
        NotCopyable& operator=(const NotCopyable&) = delete;
};

/**
 * @brief Base class for disabling move operations
 */
class NotMovable
{
    /*
     * Protected methods
     */
    protected:
        // Default constructor is needed
        NotMovable(void)                    = default;
        // Forbid class move
        NotMovable(NotMovable&&)            = delete;
        NotMovable& operator=(NotMovable&&) = delete;
};

/**
 * @brief Base class for disabling copy/move operations
 */
class NotCopyMovable : public NotCopyable,
                       public NotMovable
{
    /*
     * Protected methods
     */
    protected:
        // Default constructor
        NotCopyMovable(void) = default;
};

}   // namespace factory_injector

#endif  // _FACTORY_INJECTOR_NOT_COPYABLE_MOVABLE_HPP_
