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
 * @file  factory_traits.hpp
 * @brief Type traits for factories that need to be injected
 *
 */

#ifndef _FACTORY_INJECTOR_FACTORY_TRAITS_HPP_
#define _FACTORY_INJECTOR_FACTORY_TRAITS_HPP_

/*
 * Namespaces
 */
namespace factory_injector
{

/* Internal namespace, shall not be used */
namespace traits_details
{

/**
 * @brief  Helper struct for adding const reference to a type
 * @tparam T Class type
 */
template<class T>
struct add_const_ref
{
    /** Type with constant reference */
    using type = typename std::add_const<typename std::add_lvalue_reference<T>::type>::type;
};

/**
 * @brief  Helper struct for removing const reference from a type
 * @tparam T Class type
 */
template<class T>
struct remove_const_ref
{
    /** Type without constant reference */
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

/**
 * @brief  Helper struct for getting the pure factory type from a factory type
 * @tparam TFactory Factory type
 */
template<class TFactory>
struct get_factory
{
    /** Cleaned factory type */
    using type = typename remove_const_ref<TFactory>::type;
};

/**
 * @brief  Helper struct for getting the interface type from a factory type
 * @tparam TFactory Factory type
 */
template<class TFactory>
struct get_interface
{
    /** Interface type */
    using type = typename get_factory<TFactory>::type::tInterface;
};

/**
 * @brief  Helper struct for getting constant reference interface type from a factory type
 * @tparam TFactory Factory type
 */
template<class TFactory>
struct get_interface_const_ref
{
    /** Factory constant reference interface type */
    using type = typename add_const_ref<typename get_interface<TFactory>::type>::type;
};

}

/**
 * @brief  Type traits for factories.
 *         It defines the needed types for factories that need to be injected.
 *         The interface of these factories shall inherit from this structure.
 * @tparam TInterface Interface type
 * @tparam TObject    Object type
 */
template<class TInterface, class TObject>
struct FactoryTraits
{
    /*
     * Types
     */

    using tInterface = typename traits_details::remove_const_ref<TInterface>::type;  /**< Interface class type */
    using tObject    = typename traits_details::remove_const_ref<TObject>::type;     /**< Object type          */
    using tObjectPtr = std::unique_ptr<tObject>;                                     /**< Object pointer type  */
};

}   // namespace factory_injector

#endif  // _FACTORY_INJECTOR_FACTORY_TRAITS_HPP_
