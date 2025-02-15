#pragma once
#include <unordered_map>
#include <stdexcept>

#include "Definitions.h"

namespace Fiea::GameEngine
{

	class TypeRegistrySingleton
	{
		typedef std::vector<Signature> SigList;

	public:
		/// @brief A pointer to the singleton instance of the TypeRegistrySingleton
		inline static TypeRegistrySingleton* Instance;

		/// @brief initialize_singleton
		/// Initializes the singleton by created a new instance
		/// Throws an exception if it has already been initialized
		static void initialize_singleton()
		{
			// if already initialized, then throw an exception
			if (Instance != nullptr) { throw std::invalid_argument("Tried to initialize TypeRegistrySingleton when it has already been intialized."); }
			// Otherwise, create a new instance and set it as the singleton instance
			Instance = new TypeRegistrySingleton();
		}

		/// @brief destroy_singleton
		/// Frees the memory of the singleton instance
		static void destroy_singleton()
		{
			delete Instance;
			Instance = nullptr;
		}

		/// @brief get_signatures_of_class
		/// Returns a vector of Signatures corresponding to the RTTI class whose ID is passed in
		/// Throws an exception if the singleton has not been initialized yet
		/// Throws an exception if the ID has not been set yet
		/// @param typeId: the typeID of the RTTI class, represented as a size_t
		/// @return Returns the vector of Signatures corresponding to that class
		static SigList get_signatures_of_class(size_t typeId)
		{
			// If instance is still nullptr, then has not been initialized. throw exception
			if (Instance == nullptr) { throw std::out_of_range("Tried to get from TypeRegistrySingleton before initializing."); }
			// Try to find the key of typeId
			auto it = Instance->mTypeRegistry.find(typeId);
			// if the key is not there yet, throw exception
			if (it == Instance->mTypeRegistry.end()) { throw std::out_of_range("Could not find the specified typeId in TypeRegistrySingleton."); }
			// else we're good. Compile a list of signatures and return them.
			SigList result;
			// first check if this class has a parent
			auto parentIt = Instance->mParentAssociation.find(typeId);
			if (parentIt != Instance->mParentAssociation.end())
			{
				// recursively get parent signatures
				SigList inheritedSigs = get_signatures_of_class(Instance->mParentAssociation[typeId]);
				// Insert them into our result
				result.insert(result.end(), inheritedSigs.begin(), inheritedSigs.end());
			}
			// Then insert our own signatures
			SigList ourSignautres = Instance->mTypeRegistry[typeId];
			result.insert(result.end(), ourSignautres.begin(), ourSignautres.end());
			return result;
			/*
			std::vector<Signature> listOfMySignatures;
			std::vector<size_t> listOfMyParents = Instance->mParentAssociation[typeId];
			// Recursively get a list of my parents' signatures
			for (auto it : listOfMyParents)
			{
				std::vector<Signature> parentSignatures = get_signatures_of_class(it);
				listOfMySignatures.insert(listOfMySignatures.end(), parentSignatures.begin(), parentSignatures.end());
			}
			// Then add on my own signatures
			std::vector<Signature> onlyMySignatures = Instance->mTypeRegistry[typeId];
			listOfMySignatures.insert(listOfMySignatures.end(), onlyMySignatures.begin(), onlyMySignatures.end());
			return listOfMySignatures;
			*/
		}

		/*
		/// @brief set_signatures_of_class
		/// Given a typeId and a vector of Signatures, associates the two together and adds it to the registry
		/// Will early return if the key has already been set
		/// Throws an exception if the singleton has not been initialized yet
		/// @param typeId: the typeID of the RTTI class, represented as a size_t
		/// @param listOfSignatures: a vector of Signatures that corresponds to the RTTI class
		static void set_signatures_of_class(size_t typeId, const std::vector<Signature>& listOfSignatures)
		{
			// If instance is still nullptr, then has not been initialized. throw exception
			if (Instance == nullptr) { throw std::out_of_range("Tried to set to TypeRegistrySingleton before initializing."); }
			// If the key has already been set, then early return
			if (Instance->mTypeRegistry.find(typeId) != Instance->mTypeRegistry.end()) { return; }
			// Otherwise, add a new type/signatures association
			Instance->mTypeRegistry[typeId] = listOfSignatures;
		}
		*/
		static void set_signatures_of_class(size_t myTypeId, const SigList& listOfSignatures, size_t myParent = 0)
		{
			// If instance is still nullptr, then has not been initialized. throw exception
			if (Instance == nullptr) { throw std::out_of_range("Tried to set to TypeRegistrySingleton before initializing."); }
			// If the key has already been set, then early return
			if (Instance->mTypeRegistry.find(myTypeId) != Instance->mTypeRegistry.end()) { return; }
			// Otherwise, add a new type/signatures association
			Instance->mTypeRegistry[myTypeId] = listOfSignatures;
			// Then, if this class has parents, go ahead and put them in the parent association
			if (myParent != 0) {
				Instance->mParentAssociation[myTypeId] = myParent;
			}
		}


	private:
		TypeRegistrySingleton() = default;
		~TypeRegistrySingleton() = default;
		std::unordered_map<size_t, SigList> mTypeRegistry;
		std::unordered_map<size_t, size_t> mParentAssociation;
	};
}