#include "pch.h"
#include "GameObject.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(GameObject);

#pragma region InheritedFromAttributed
GameObject::GameObject() : Attributed(GameObject::TypeIdClass()), mName("GameObject")
{
}

std::vector<Signature> GameObject::get_signatures()
{
	return std::vector<Signature>
	{
		{ "mChildren", DatumType::Table, 0, 0 },
		{ "mActions", DatumType::Table, 0, 0 },
		{ "mName", DatumType::String, 1, offsetof(GameObject, mName) },
		{ "mPosition", DatumType::Vector4, 1, offsetof(GameObject, mTransform.position) },
		{ "mRotation", DatumType::Vector4, 1, offsetof(GameObject, mTransform.rotation) },
		{ "mScale", DatumType::Vector4, 1, offsetof(GameObject, mTransform.scale) }
	};
}

GameObject* GameObject::Clone() const
{
	return new GameObject(*this);
}

GameObject::GameObject(size_t idOfChild) : Attributed(idOfChild), mName("ChildOfGameObject")
{
}
#pragma endregion

#pragma region FieaGameEngineMethods
Datum* GameObject::get_children_datum()
{
	Datum* result = get_datum_by_key("mChildren");
	assert(result != nullptr); // ERROR: Could not find children Datum
	return result;
}
Datum* GameObject::get_actions_datum()
{
	Datum* result = get_datum_by_key("mActions");
	assert(result != nullptr); // ERROR: Could not find actions Datum
	return result;
}
/*
void GameObject::create_action(const std::string& className, const Action& action)
{
}
*/
void GameObject::adopt_action(Action* action)
{
	adopt_scope(action, "mActions");
}
#pragma endregion

#pragma region TransformProperties
/// @brief childCount()
/// @return Returns the number of children of this GameObject
const size_t GameObject::childCount() const
{
	return get_datum_by_key("mChildren")->size();
}

/// @brief localEulerAngles
/// @return Returns the local rotation of this GameObject in Euler Angles
const Vector3 GameObject::localEulerAngles() const
{
	return mTransform.rotation; // Note: automatically converts to Vec3
}

/// @brief setLocalEulerAngles
/// Sets the local rotation of this GameObject using Euler Angles
/// @param newRotation - the rotation to set to
void GameObject::setLocalEulerAngles(const Vector3& newRotation)
{
	mTransform.rotation = { newRotation, 0 };
}

/// @brief localPosition
/// @return Returns the local position of this GameObject
const Vector3 GameObject::localPosition() const
{
	return mTransform.position; // Note: automatically converts to Vec3
}

/// @brief setLocalPosition
/// Sets the local position of this GameObject
/// @param newPosition - the position to set to
void GameObject::setLocalPosition(const Vector3& newPosition)
{
	mTransform.position = { newPosition, 1 };
}

/// @brief localScale
/// @return Returns the local scale of this GameObject
const Vector3 GameObject::localScale() const
{
	return mTransform.scale; // Note: automatically converts to Vec3
}

/// @brief setLocalScale
/// Sets the local scale of this GameObject
/// @param newScale - the scale to set to
void GameObject::setLocalScale(const Vector3& newScale)
{
	mTransform.scale = { newScale, 0 };
}

/// @brief parent
/// Note: Asserts that the parent is always a GameObject if it exists, not just a Scope
/// @return Returns the parent GameObject, or nullptr if none
GameObject* const GameObject::parent() const
{
	// Get the parent scope. Check if nullptr
	Scope* parentScope = parent_scope();
	if (parentScope == nullptr) { return nullptr; }
	// cast to GameObject. It should ALWAYS be a GameObject. assert false if something bad has happened
	GameObject* result = parent_scope()->As<GameObject>();
	assert(result != nullptr); /* ERROR: parent of GameObject was not a GameObject */
	// Return it as a const pointer to a mutable GameObject
	return result;
}

/// @brief root
/// Note: Asserts that the root is always a GameObject, not just a Scope
/// @return Returns the root GameObject, which can be itself
GameObject* const GameObject::root() const
{
	// Keep going up the parent tree until you reach nullptr
	Scope* current = (Scope*)this;
	Scope* parent = current->parent_scope();
	while (parent != nullptr)
	{
		current = parent;
		parent = current->parent_scope();
	}
	// At this point, result should be the top-most Scope* in the tree
	// Cast to GameObject. It should ALWAYS be a GameObject. assert false if something bad has happened
	GameObject* result = current->As<GameObject>();
	assert(result != nullptr); /* ERROR: root of GameObject was not a GameObject */
	// Return it as a const pointer to a mutable GameObject
	return result;
}

/// @brief localTransform
/// @return Returns the local transform of the GameObject as a Transform
const Transform GameObject::localTransform() const
{
	return mTransform; // will call converting constructor Transform(Transform4)
}

/// @brief setLocalTransform
/// Sets the local transform of the GameObject
/// @param newTransform - the Transform that represents the GameObject's new transform
void GameObject::setLocalTransform(const Transform& newTransform)
{
	mTransform = newTransform; // will call converting Transform4::operator=(Transform)
}

/// @brief name
/// @return Returns the name of the GameObject
const std::string GameObject::name() const
{
	return mName;
}

/// @brief setName
/// Sets the name of the GameObject
/// @param newName - a string representing the new name
void GameObject::setName(const std::string& newName)
{
	mName = newName;
}
#pragma endregion

#pragma region TransformMethods
/// @brief GetChild const
/// Throws out_of_range if input is greater than childCount
/// @param index - The index of the child to return
/// @return Returns a pointer to the const child at the specified index
const GameObject* GameObject::GetChild(size_t index) const
{
	// If out of bounds
	if (index > childCount()) { throw std::out_of_range("index in GameObject::GetChild const is greater than number of children"); }
	const GameObject* result = get_datum_by_key("mChildren")->get_scope_at(index)->As<GameObject>();
	assert(result != nullptr);
	return result;
}

/// @brief GetChild
/// Throws out_of_range if input is greater than childCount
/// @param index - The index of the child to return
/// @return Returns a pointer to the child at the specified index
GameObject* GameObject::GetChild(size_t index)
{
	// If out of bounds
	if (index > childCount()) { throw std::out_of_range("index in GameObject::GetChild is greater than number of children"); }
	GameObject* result = get_datum_by_key("mChildren")->get_scope_at(index)->As<GameObject>();
	assert(result != nullptr);
	return result;
}

/// @brief SetParent
/// Sets the parent of this GameObject to another GameObject
/// @param newParent - The GameObject that will be the new parent
void GameObject::SetParent(GameObject* newParent)
{
	newParent->adopt_scope(this, "mChildren");
}
#pragma endregion

#pragma region MonoBehaviour
/// @brief Update()
/// Function that is called every frame.
/// Will also call the Update() of child GOs and child Actions
/// Virtual, so can be overrided by derived classes
/// @param deltaTime - The amount of time since the last frame, represented by a GameTime
void GameObject::Update(const GameTime& deltaTime)
{
	// First update children
	{
		Datum* gameObjectChildrenDatum = get_children_datum();
		size_t numberOfChildren = gameObjectChildrenDatum->size();
		for (size_t i = 0; i < numberOfChildren; ++i)
		{
			GameObject* go = gameObjectChildrenDatum->get_scope_at(i)->As<GameObject>();
			assert(go != nullptr); // ERROR: Children of GameObject was not a GameObject
			go->Update(deltaTime);
		}
	}
	// Then update actions
	{
		Datum* actionChildrenDatum = get_actions_datum();
		size_t numberOfActions = actionChildrenDatum->size();
		for (size_t i = 0; i < numberOfActions; ++i)
		{
			Action* action = actionChildrenDatum->get_scope_at(i)->As<Action>();
			assert(action != nullptr); // ERROR: Children of GameObject was not a GameObject
			action->Update(deltaTime);
		}
	}
}
#pragma endregion