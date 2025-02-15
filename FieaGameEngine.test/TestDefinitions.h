#pragma once
#include "CppUnitTest.h"

#include <vector>
#include "glm/glm.hpp"

#include "Datum.h"
#include "Scope.h"

#include "Definitions.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum>(const Fiea::GameEngine::Datum& datum) 
	{
		RETURN_WIDE_STRING("i am a datum");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum>(const Fiea::GameEngine::Datum* datum) 
	{
		RETURN_WIDE_STRING("i am a datum");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum>(Fiea::GameEngine::Datum* datum) 
	{
		RETURN_WIDE_STRING("i am a datum");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum*>(Fiea::GameEngine::Datum** datum) 
	{
		RETURN_WIDE_STRING("i am a datumptr");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum*>(Fiea::GameEngine::Datum* const *datum) 
	{
		RETURN_WIDE_STRING("i am a datumptr");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Datum*>(Fiea::GameEngine::Datum* const &datum) 
	{
		RETURN_WIDE_STRING("i am a datumptr");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::DatumType>(const Fiea::GameEngine::DatumType& type)
	{
		std::wstringstream ss;
		switch (type)
		{
		case Fiea::GameEngine::DatumType::Integer:
			ss << "DatumType::Integer";
			break;
		case Fiea::GameEngine::DatumType::Float:
			ss << "DatumType::Float";
			break;
		case Fiea::GameEngine::DatumType::Vector4:
			ss << "DatumType::Vector4";
			break;
		case Fiea::GameEngine::DatumType::Matrix4:
			ss << "DatumType::Matrix4";
			break;
		case Fiea::GameEngine::DatumType::String:
			ss << "DatumType::String";
			break;
		case Fiea::GameEngine::DatumType::Table:
			ss << "DatumType::Table";
			break;
		case Fiea::GameEngine::DatumType::Pointer:
			ss << "DatumType::Pointer";
			break;
		case Fiea::GameEngine::DatumType::Unknown:
			ss << "DatumType::Unknown";
			break;
		default:
			ss << "Something really cringe happened in ToString<DatumType>";
		}
		RETURN_WIDE_STRING(ss.str());
	}
	template<>
	std::wstring ToString<glm::vec3>(const glm::vec3& vec)
	{
		RETURN_WIDE_STRING("i am a vector3");
	}
	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& vec)
	{
		RETURN_WIDE_STRING("i am a vector4");
	}
	template<>
	std::wstring ToString<glm::mat4>(const glm::mat4& vec)
	{
		RETURN_WIDE_STRING("i am a matrix4");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::ScopePtr>(const Fiea::GameEngine::ScopePtr& vec)
	{
		RETURN_WIDE_STRING("i am a scopeptr");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Scope>(const Fiea::GameEngine::Scope& vec)
	{
		RETURN_WIDE_STRING("i am a scope");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Scope>(const Fiea::GameEngine::Scope* vec)
	{
		RETURN_WIDE_STRING("i am a scope");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::Scope>(Fiea::GameEngine::Scope* vec)
	{
		RETURN_WIDE_STRING("i am a scope");
	}
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTI>(const Fiea::GameEngine::RTTI& rtti) { RETURN_WIDE_STRING("i am a rtti"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTI>(const Fiea::GameEngine::RTTI* rtti) { RETURN_WIDE_STRING("i am a rtti"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTI>(Fiea::GameEngine::RTTI* rtti) { RETURN_WIDE_STRING("i am a rtti"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTIPtr>(const Fiea::GameEngine::RTTIPtr& rttiptr) { RETURN_WIDE_STRING("i am a RTTIptr"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTIPtr>(const Fiea::GameEngine::RTTIPtr* rttiptr) { RETURN_WIDE_STRING("i am a RTTIptr"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::RTTIPtr>(Fiea::GameEngine::RTTIPtr* rttiptr) { RETURN_WIDE_STRING("i am a RTTIptr"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::Signature>(const Fiea::GameEngine::Signature& signature) { RETURN_WIDE_STRING("i am a signature"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::Signature>(const Fiea::GameEngine::Signature* signature) { RETURN_WIDE_STRING("i am a signature"); }
	template<>
	std::wstring ToString<Fiea::GameEngine::Signature>(Fiea::GameEngine::Signature* signature) { RETURN_WIDE_STRING("i am a signature"); }
	template<>
	std::wstring ToString<std::vector<Fiea::GameEngine::Signature>>(const std::vector<Fiea::GameEngine::Signature>& signature) { RETURN_WIDE_STRING("i am a list of signatures"); }
	template<>
	std::wstring ToString<std::vector<Fiea::GameEngine::Signature>>(const std::vector<Fiea::GameEngine::Signature>* signature) { RETURN_WIDE_STRING("i am a list of signatures"); }
	template<>
	std::wstring ToString<std::vector<Fiea::GameEngine::Signature>>(std::vector<Fiea::GameEngine::Signature>* signature) { RETURN_WIDE_STRING("i am a list of signatures"); }
}