#pragma once

#include "Definitions.h"
#include "IFactory.h"
#include "Attributed.h"
#include "Action.h"

namespace Fiea::GameEngine
{
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		// Rule of 5
		GameObject();
		~GameObject() = default;
		GameObject(const GameObject&) = default;
		GameObject(GameObject&&) = default;
		GameObject& operator=(const GameObject&) = default;
		GameObject& operator=(GameObject&&) = default;
	protected:
		GameObject(size_t);
	public:
		// methods for Fiea::GameEngine
		Datum* get_children_datum();
		Datum* get_actions_datum();
		// I don't like the fact that GameObject should be allowed to create its own actions. IMO GameObject shouldn't need to talk to the FactoryManager
		// I'm keeping this method commented out for now, replacing it with adopt_action
		//void create_action(const std::string&, const Action&);
		void adopt_action(Action*);

		// methods emulating Unity's Transform Properties
		// Note: many of these return const copies, to prevent the use from using them as C#-style properties
		const size_t childCount() const;
		//const Vector3 eulerAngles() const;
		//void setEulerAngles(const Vector3&);
		//const Vector3 forward() const;
		//void setForward(const Vector3&);
		//const bool hasChanged() const;
		//void setHasChanged(bool);
		//const size_t hierarchyCapacity() const;
		//const size_t hierarchyCount() const;
		const Vector3 localEulerAngles() const;
		void setLocalEulerAngles(const Vector3&);
		const Vector3 localPosition() const;
		void setLocalPosition(const Vector3&);
		const Vector3 localScale() const;
		void setLocalScale(const Vector3&);
		//const Vector3 lossyScale() const;
		//void setLossyScale(const Vector3&) const;
		GameObject* const parent() const;
		//const Vector3 position() const;
		//void setPosition(const Vector3&);
		//const Vector3 right() const;
		GameObject* const root() const;
		//const Quaternion rotation() const;
		//void setRotation(const Quaternion&);
		//const Vector3 up() const;
		const Transform localTransform() const;
		void setLocalTransform(const Transform&);
		//const Transform worldTransform() const;
		//void setWorldTransform(const Transform&);
		const std::string name() const;
		void setName(const std::string&);

		// methods emulating Unity's Transform Public Methods
		//void DetatchChildren() const;
		//GameObject* Find(const std::string&) const;
		const GameObject* GetChild(size_t) const;
		GameObject* GetChild(size_t);
		//void GetLocalPositionAndRotation(Vector3&, Vector3&) const;
		//void GetPositionAndRotation(Vector3&, Vector3&) const;
		//size_t GetSiblingIndex() const;
		//Vector3 InverseTransformDirection(const Vector3&) const;
		//Vector3 InverseTransformPoint(const Vector3&) const;
		//Vector3 InverseTransformVector(const Vector3&) const;
		//bool IsChildOf(GameObject*) const;
		//void LookAt(GameObject*, const Vector3& up = { 0,1,0 });
		//void Rotate(const Vector3&);
		//void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		//void SetAsFirstSibling();
		//void SetAsLastSibling();
		//void SetLocalPositionAndRotation(const Vector3&, const Quaternion&);
		void SetParent(GameObject*);
		//void SetPositionAndRotation(const Vector3&, const Quaternion&);
		//void SetSiblingIndex(size_t);
		//Vector3 TransformDirection(const Vector3&) const;
		//Vector3 TransformPoint(const Vector3&) const;
		//Vector3 TransformVector(const Vector3&) const;
		//void Translate(const Vector3&);

		// methods emulating Unity's Object static methods
		//static void Destroy(GameObject*);
		//static GameObject* Instantiate(GameObject* original);
		//static GameObject* Instantiate(GameObject* original, GameObject* parent);
		//static GameObject* Instantiate(GameObject* original, const Vector3& position, const Quaternion& rotation);
		//static GameObject* Instantiate(GameObject* original, const Vector3& position, const Quaternion& rotation, GameObject* parent);

		// methods emulated Unity's MonoBehaviour messages
		//virtual void Awake();
		//virtual void Start();
		virtual void Update(const GameTime&);
		//virtual void OnDestroy();

		// Inherited
		static std::vector<Signature> get_signatures();
		[[nodiscard]] virtual GameObject* Clone() const override;

	private:
		// list of children in internal "mChildren" Datum
		// list of actions in internal "mActions" Datum
		std::string mName;
		Transform4 mTransform;
	};

	MAKE_CONCRETE_FACTORY(GameObject);
}