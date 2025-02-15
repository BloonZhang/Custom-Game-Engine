#pragma once

#include <unordered_map>

#include "Definitions.h"
#include "IFactory.h"

namespace Fiea::GameEngine 
{
	class FactoryManagerSingleton
	{
	public:
		/// @brief A pointer to the singleton instance of the FactoryManagerSingleton
		inline static FactoryManagerSingleton* Instance;

		/// @brief initialize_singleton
		/// Initializes the singleton by created a new instance
		/// Throws an exception if it has already been initialized
		static void initialize_singleton()
		{
			// if already initialized, then throw an exception
			if (Instance != nullptr) { throw std::invalid_argument("Tried to initialize FactoryManagerSingleton when it has already been intialized."); }
			// Otherwise, create a new instance and set it as the singleton instance
			Instance = new FactoryManagerSingleton();
		}

		/// @brief destroy_singleton
		/// Frees the memory of the singleton instance
		static void destroy_singleton()
		{
			// For each factory, delete it.
			// TODO: use unique_ptr instead of raw pointer
			for (auto it : Instance->mFactories) { delete it.second; }
			delete Instance;
			Instance = nullptr;
		}

		/// @brief find_factory
		/// Given a class name, returns the associated concrete factory.
		/// @param className - A string representing the name of the class whose factory to get
		/// @return Returns an IFactory pointer to the concrete factory
		static IFactory* find_factory(const std::string& className)
		{
			if (Instance->mFactories.find(className) != Instance->mFactories.end())
			{
				return Instance->mFactories[className];
			}
			else 
			{ 
				return nullptr; 
			}
		}

		/// @brief create_instance_of
		/// Given a class name, return a new instance of that class
		/// Marked as nodiscard because it returns newly allocated memory
		/// @param className - A string representing the name of the class to get an instance of
		/// @return Returns a new instance of the class
		[[nodiscard]] static Scope* create_instance_of(const std::string& className)
		{
			IFactory* factory = find_factory(className);
			if (factory == nullptr) { return nullptr; }
			else { return factory->create(); }
		}

		/// @brief add_factory
		/// Adds a concrete factory to the list of factories that the Manager can use
		/// @param concreteFactory - An IFactory pointer to the concrete factory to add
		static void add_factory(IFactory* concreteFactory)
		{
			Instance->mFactories.insert({ concreteFactory->get_class_name(), concreteFactory });
		}


	private:
		FactoryManagerSingleton() = default;
		~FactoryManagerSingleton() = default;
		std::unordered_map<std::string, IFactory*> mFactories;
	};
}