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
 * @file  factory_injector.hpp
 * @brief Declaration and definition of FactoryInjector class
 *
 */

#ifndef _FACTORY_INJECTOR_FACTORY_INJECTOR_HPP_
#define _FACTORY_INJECTOR_FACTORY_INJECTOR_HPP_

/*
 * Includes
 */

// Standard
#include <exception>
#include <memory>
#include <unordered_map>
#include <typeindex>
// Project
#include "factory_traits.hpp"
#include "not_copyable_movable.hpp"

/*
 * Namespaces
 */
namespace factory_injector
{

/* Internal namespace, shall not be used */
namespace injector_details
{

/**
 * @brief Abstract instance class.
 *        Generic container for a generic type.
 */
class AnyInstance
{
    /*
     * Public methods
     */
    public:
        /**
         * @brief Destructor
         */
        virtual ~AnyInstance(void) = default;

        /**
         * @brief  Get instance pointer
         * @return Instance pointer
         */
        virtual void *GetPtr(void) = 0;
};

/**
 * @brief  Real instance class.
 *         Container for the real class instance unique_ptr.
 * @tparam TInstance Instance type
 */
template<class TInstance>
class RealInstance final : public AnyInstance
{
    /*
     * Types
     */
    private:
        /** Instance pointer type definition */
        using tInstancePtr = std::unique_ptr<TInstance>;

    /*
     * Public methods
     */
    public:
        /**
         * @brief     Constructor
         * @param[in] rrArgs Argument lists for constructing class
         * @tparam    TArgs Variadic parameter types
         */
        template<class ... TArgs>
        RealInstance(TArgs&& ... rrArgs) :
            mInstancePtr(std::make_unique<TInstance>(std::forward<TArgs>(rrArgs)...))
        {}

        /**
         * @brief  Get instance pointer
         * @return Instance pointer
         */
        void *GetPtr(void) override
        {
            return mInstancePtr.get();
        }

    /*
     * Members
     */
    private:
        tInstancePtr mInstancePtr;      /**< Instance pointer */
};

/**
 * @brief     Helper function for creating a unique_ptr of AnyInstance from a RealInstance<T>
 * @param[in] rrArgs Argument lists for constructing RealInstance class
 * @tparam    TInstance Instance type
 * @tparam    TArgs     Variadic parameter types
 */
template<class TInstance, class ... TArgs>
std::unique_ptr<AnyInstance> MakeUniqueAnyInstance(TArgs&& ... rrArgs)
{
    return std::make_unique<RealInstance<TInstance>>(std::forward<TArgs>(rrArgs)...);
}

}   // namespace injector_details

/**
 * @brief Custom exception in case a factory is already existent
 */
class FactoryAlreadyRegisteredEx : public std::runtime_error
{
    /*
     * Public methods
     */
    public:
        /**
         * @brief     Constructor
         * @param[in] rcFactoryName Factory name
         */
        FactoryAlreadyRegisteredEx(const std::string& rcFactoryName) :
            std::runtime_error("The requested factory " + rcFactoryName + " is already registered")
        {}
};

/**
 * @brief Custom exception in case a factory is not found
 */
class FactoryNotRegisteredEx : public std::runtime_error
{
    /*
     * Public methods
     */
    public:
        /**
         * @brief     Constructor
         * @param[in] rcFactoryName Factory name
         */
        FactoryNotRegisteredEx(const std::string& rcFactoryName) :
            std::runtime_error("Unable to find the requested factory " + rcFactoryName)
        {}
};

/**
 * @brief Factory injector class.
 *        It allows the registering and getting for factory instances, allowing ruin-time injection of fake or mock factories.
 */
class FactoryInjector final : public NotCopyMovable
{
    /*
     * Types
     */
    private:
        /** Any instance pointer type definition */
        using tAnyInstancePtr  = std::unique_ptr<injector_details::AnyInstance>;
        /** Instance container type definition */
        using tInstanceCont    = std::unordered_map<std::type_index, tAnyInstancePtr>;
        /** Instance container constant iterator type definition */
        using tInstanceContItr = tInstanceCont::const_iterator;

    /**
     * Public methods
     */
    public:
        /**
         * @brief Constructor
         */
        FactoryInjector(void) = default;

        /**
         * @brief     Register a factory by overwriting it. The factory type is represented by the template parameter.
         *            A factory is registered using its interface class type, allowing to register a mock or fake factory
         *            that inherits from the same interface.
         *            If called multiple times, the new factory overwrites the existent one, allowing a test application
         *            to overwrite the existent factories with its own ones.
         * @param[in] rrArgs   Argument lists for constructing factory
         * @tparam    TFactory Factory type
         * @tparam    TArgs    Variadic parameter types
         * @return    void
         */
        template<class TFactory, class ... TArgs>
        void OverwriteFactory(TArgs&& ... rrArgs)
        {
            // Get interface type index
            auto type_idx = GetInterfaceTypeIndex<TFactory>();

            // Helper type for shortening
            using tFactory = typename traits_details::get_factory<TFactory>::type;

            // Register or overwrite instance
            mInstanceCont[type_idx] = injector_details::MakeUniqueAnyInstance<tFactory>(std::forward<TArgs>(rrArgs)...);
        }

        /**
         * @brief     Same of OverwriteFactory method but, if the factory is already existent, a FactoryAlreadyRegisteredEx exception is thrown.
         * @param[in] rrArgs   Argument lists for constructing factory
         * @tparam    TFactory Factory type
         * @tparam    TArgs    Variadic parameter types
         * @return    void
         */
        template<class TFactory, class ... TArgs>
        void RegisterFactory(TArgs&& ... rrArgs)
        {
            // Find factory
            auto instance_itr = FindFactory<TFactory>();

            // Register if not found
            if (instance_itr == std::cend(mInstanceCont))
            {
                OverwriteFactory<TFactory>(std::forward<TArgs>(rrArgs)...);
            }
            else
            {
                throw FactoryAlreadyRegisteredEx(typeid(TFactory).name());
            }
        }

        /**
         * @brief  Get a factory instance. The factory type is represented by the template parameter.
         *         FactoryNotRegisteredEx is thrown if the factory is not existent.
         * @tparam TFactory Factory type
         * @return Constant reference to factory interface class
         */
        template<class TFactory>
        auto GetFactory(void) const
            -> typename traits_details::get_interface_const_ref<TFactory>::type
        {
            // Find factory
            auto instance_itr = FindFactory<TFactory>();

            // Return it if found
            if (instance_itr != std::cend(mInstanceCont))
            {
                using tInterface = typename traits_details::get_interface<TFactory>::type;

                return *static_cast<tInterface *>(instance_itr->second->GetPtr());
            }
            else
            {
                throw FactoryNotRegisteredEx(typeid(TFactory).name());
            }
        }

        /**
         * @brief     Helper method for creating an object from a factory. The factory type is represented by the
         *            template parameter. It requires the factory interface to define the following method:
         *                virtual tObjectPtr Create( some_parameters... ) const = 0;
         *            otherwise the compilation will fail.
         *            If the factory interface defines different methods, you can neglect this method and just use GetFactory
         *            by calling your method directly, e.g. GetFactory<TFactory>().YourMethod(params...)
         * @param[in] rrArgs   Argument lists for constructing object
         * @tparam    TFactory Factory type
         * @tparam    TArgs    Variadic parameter types
         * @return    Object pointer
         */
        template<class TFactory, class ... TArgs>
        auto CreateObject(TArgs&& ... rrArgs) const
            -> typename traits_details::get_factory<TFactory>::type::tObjectPtr
        {
            auto& factory = GetFactory<TFactory>();

            return factory.Create(std::forward<TArgs>(rrArgs)...);
        }

    /*
     * Private methods
     */
    private:
        /**
         * @brief  Find the specified factory type.
         * @tparam TFactory Factory type
         * @return Constant iterator to the found factory
         */
        template<class TFactory>
        tInstanceContItr FindFactory(void) const
        {
            // Get interface type index
            auto type_idx = GetInterfaceTypeIndex<TFactory>();

            // Find instance
            return mInstanceCont.find(type_idx);
        }

        /**
         * @brief  Get the type index of the factory interface.
         * @tparam TFactory Factory type
         * @return Type index of the factory interface
         */
        template<class TFactory>
        std::type_index GetInterfaceTypeIndex(void) const
        {
            // Helper types for shortening
            using tInterface = typename traits_details::get_interface<TFactory>::type;

            // Get type index
            return std::type_index(typeid(tInterface));
        }

    /*
     * Members
     */
    private:
        tInstanceCont mInstanceCont;    /**< Instance container */
};

}   // namespace factory_injector

#endif  // _FACTORY_INJECTOR_FACTORY_INJECTOR_HPP_
