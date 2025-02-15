#pragma once

#include <string>
#include <cstddef>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <functional>

#include "glm/glm.hpp"
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/string_cast.hpp"
#include "json/json.h"
#include "GameClock.h"

namespace Fiea::GameEngine
{
	class Scope;
	using ScopePtr = Scope*;
	using ConstScopePtr = const Scope*;
	class RTTI;
	using RTTIPtr = RTTI*;
	using ConstRTTIPtr = const RTTI*;

	using JsonPair = std::pair<const std::string, Json::Value>;

	//using GameTime = size_t;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Quaternion = glm::vec4;
	using Matrix4x4 = glm::mat4;

	// Signature is: void MyFunction(implicit this, RTTI*, const EventArgs*);
	class EventArgs;
	using DelegateFunction = std::function<void(RTTI*, const EventArgs*)>;
	//using DelegateFunction = std::function<void(RTTI&, RTTI*, const EventArgs*)>;

	// Commented out constexpr because weird warnings
	//constexpr static std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
	//constexpr static float operator"" _f(unsigned long long n) { return float(n); }
	static size_t operator "" _z(unsigned long long n) { return size_t(n); }
	static float operator"" _f(unsigned long long n) { return float(n); }
	static std::string operator"" _s(const char charArray[], size_t size) { return std::string(charArray); }

	// Custom string helper functions
	/*
	const static int StringToInt(const std::string&);
	const static float StringToFloat(const std::string&);
	const static glm::vec4 StringToVec4(const std::string&);
	const static glm::mat4 StringToMat4(const std::string&);
	const static std::string IntToString(int);
	const static std::string FloatToString(float);
	const static std::string Vec4ToString(const glm::vec4&);
	const static std::string Mat4ToString(const glm::mat4&);
	*/
	inline const static int StringToInt(const std::string& str) { return std::stoi(str); }
	inline const static float StringToFloat(const std::string& str) { return std::stof(str); }
	inline const static glm::vec4 StringToVec4(const std::string& str)
	{
		//vec4(0.000000, 1.000000, 2.000000, 3.000000)
		float f0, f1, f2, f3;
		int success = sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
		if (success != 4) { throw std::invalid_argument("Invalid input in StringToVec4()"); }
		return glm::vec4(f0, f1, f2, f3);
	}
	inline const static glm::mat4 StringToMat4(const std::string& str)
	{
		//mat4x4((0.000000, 1.000000, 2.000000, 3.000000), 
		//(10.000000, 11.000000, 12.000000, 13.000000), 
		//(20.000000, 21.000000, 22.000000, 23.000000), 
		//(30.000000, 31.000000, 32.000000, 33.000000))
		float f00, f01, f02, f03;
		float f10, f11, f12, f13;
		float f20, f21, f22, f23;
		float f30, f31, f32, f33;
		int success = sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",\
			&f00, &f01, &f02, &f03, &f10, &f11, &f12, &f13, &f20, &f21, &f22, &f23, &f30, &f31, &f32, &f33);
		if (success != 16) { throw std::invalid_argument("Invalid input in StringToMat4"); }
		return glm::mat4(f00, f01, f02, f03, f10, f11, f12, f13, f20, f21, f22, f23, f30, f31, f32, f33);
	}
	inline const static std::string IntToString(int i) { return std::to_string(i); }
	inline const static std::string FloatToString(float f) { return std::to_string(f); }
	inline const static std::string Vec4ToString(const glm::vec4& vec) { return glm::to_string(vec); }
	inline const static std::string Mat4ToString(const glm::mat4& mat) { return glm::to_string(mat); }
	
	inline const static bool StringStartsWith(const std::string& str, const std::string& substring)
	{
		return (str.rfind(substring, 0) == 0);
	}

	// public enum for the various types the Datum can store
	enum class DatumType
	{
		Integer,
		Float,
		Vector4,
		Matrix4,
		String,
		Table,		// Scope*
		Pointer,	// RTTI*
		Unknown
	};

	// public struct for Transforms
	struct Transform;
	struct Transform4;
	struct Transform
	{
		Transform();
		Transform(Transform4);
		Transform& operator=(Transform4);
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
	struct Transform4
	{
		Transform4();
		Transform4(Transform);
		Transform4& operator=(Transform);
		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec4 scale;
	};
	// Default constructor for Transform
	inline Transform::Transform() : position(Vector3(0, 0, 0)), rotation(Vector3(0, 0, 0)), scale(Vector3(1, 1, 1))
	{
	}
	// Converting constructor from Transform4 to regular Transform
	inline Transform::Transform(Transform4 other) : position(other.position), 
		rotation(other.rotation), 
		scale(other.scale)
	{
	}
	// Converting operator= from Transform4 to regular Transform
	inline Transform& Transform::operator=(Transform4 other)
	{
		position = other.position; rotation = other.rotation; scale = other.scale;
		return *this;
	}
	// Default constructor for Transform4
	inline Transform4::Transform4() : position(Vector4(0, 0, 0, 1)), rotation(Vector4(0, 0, 0, 0)), scale(Vector4(1, 1, 1, 0))
	{
	}
	// Converting constructor from regular Transform to Transform4
	inline Transform4::Transform4(Transform other) : position({ other.position, 1 }), 
		rotation({ other.rotation, 0 }), 
		scale({ other.scale, 0 })
	{
	}
	// Converting operator= from regular Transform to Transform4
	inline Transform4& Transform4::operator=(Transform other)
	{
		position = { other.position, 1 };
		rotation = { other.rotation, 0 };
		scale = { other.scale, 0 };
		return *this;
	}

	// public struct for Attributed
	struct Signature
	{
		std::string name;
		DatumType type;
		size_t sizeOfArray;
		size_t offset;
	};
	inline bool operator==(const Signature& lhs, const Signature& rhs)
	{
		return (lhs.name == rhs.name) && (lhs.type == rhs.type) && (lhs.sizeOfArray == rhs.sizeOfArray) && (lhs.offset == rhs.offset);
	}
	inline bool operator!=(const Signature& lhs, const Signature& rhs)
	{
		return operator==(lhs, rhs);
	}
}
