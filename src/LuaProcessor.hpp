/*
This file is part of CILTlua (https://github.com/DEC05EBA/CILTlua).

CILTlua is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CILTlua is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CILTlua.If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LUA_PROCESSOR_HPP
#define LUA_PROCESSOR_HPP

#include "../CILT/src/Processor.hpp"
#include <unordered_map>
#include <utility> // std::make_pair

class LuaProcessor : public cilt::Processor
{
public:
	/*
	"predefinedMethods" should be a map of methods where the key is the "full method name" in CIL and the value is the lua function name.
	A "full method name" is formatted like this: NamespaceName.ClassName.MethodName, for example:
	namespace MTASAClient
	{
		class Chat
		{
			static void output(string msg) {}
		}
	}

	A "full method name" for the "output" method above would be: MTASAClient.Chat.output
	*/
	LuaProcessor(const std::unordered_map<std::string, std::string> &predefinedMethods) : m_predefinedMethods(predefinedMethods) 
	{
		addTransCompData("-- This file was generated by translating CIL to lua using CILTlua (https://github.com/DEC05EBA/CILTlua)\n");
	}

	void onMethodBegin(const std::string &namespaceName, const std::string &className, const std::string &methodName, const std::vector<cil::Value> &params)
	{
		addTransCompData("function " + namespaceName + "_" + className + "_" + methodName + "(");

		for(uint32 i = 0; i < params.size(); ++i)
		{
			addTransCompData(params[i].toString());
			if(i != params.size() - 1)
				addTransCompData(", ");
		}

		addTransCompData(")\n");
	}

	void onMethodEnd(const std::string &namespaceName, const std::string &className, const std::string &methodName)
	{
		addTransCompData("end\n");
	}

	// This implementation currently only works with static methods, but it will be fixed later.
	// At this stage, we are just testing small things at a time
	void onMethodCall(cil::Value lhsVar, const std::string &namespaceName, const std::string &className, const std::string &methodName, const std::vector<cil::Value> &args)
	{
		if(lhsVar.isVariable())
		{
			addTransCompData("local ");
			addTransCompData(lhsVar.toString() + " = ");
		}

		if(namespaceName == "System" && className == "String" && methodName == "Concat")
		{
			for(uint32 i = 0; i < args.size(); ++i)
			{
				if(args[i].isVariable() || args[i].getType() != SignatureType::STRING)
				{
					addTransCompData("tostring(");
					addTransCompData(args[i].toString());
					addTransCompData(")");
				}
				else
				{
					addTransCompData(args[i].toString());
				}

				if(i != args.size() - 1)
					addTransCompData("..");
			}
			addTransCompData("\n");
		}
		else
		{
			const std::string *predefinedFunction = getPredefinedMethod(namespaceName + "." + className + "." + methodName);
			if(predefinedFunction)
				addTransCompData(*predefinedFunction + "(");
			else
				addTransCompData(namespaceName + "_" + className + "_" + methodName + "(");

			for(uint32 i = 0; i < args.size(); ++i)
			{
				addTransCompData(args[i].toString());
				if(i != args.size() - 1)
					addTransCompData(", ");
			}

			addTransCompData(")\n");
		}
	}

	void onMethodReturn(cil::Value value)
	{
		addTransCompData("return ");
		if(!value.isNone()) addTransCompData(value.toString());
		addTransCompData("\n");
	}

	void onMainMethodCall(const std::string &namespaceName, const std::string &className, const std::string &methodName, const std::vector<cil::Value> &args)
	{
		addTransCompData(namespaceName + "_" + className + "_" + methodName + "()\n");
	}

	void onVariableDeclaration(cil::Type type, const std::string &name)
	{
		addTransCompData("local ");
		addTransCompData(name + "\n");
	}

	void onVariableAssignment(const std::string &name, cil::Value rhsValue)
	{
		addTransCompData(name + " = " + rhsValue.toString() + "\n");
	}

	void onIfStatementBegin(const cilt::Condition &condition)
	{
		addTransCompData("if(");
		addTransCompData(condition.lhs.toString());

		switch(condition.getType())
		{
			case cilt::Condition::Type::LESS:
				addTransCompData("<");
				break;
			case cilt::Condition::Type::LESS_EQUAL:
				addTransCompData("<=");
				break;
			case cilt::Condition::Type::GREATER:
				addTransCompData(">");
				break;
			case cilt::Condition::Type::GREATER_EQUAL:
				addTransCompData(">=");
				break;
			case cilt::Condition::Type::EQUALS:
				addTransCompData("==");
				break;
			case cilt::Condition::Type::NOT_EQUALS:
				addTransCompData("~=");
				break;
		}

		addTransCompData(condition.rhs.toString());
		addTransCompData(")then\n");
	}

	void onIfStatementEnd()
	{
		addTransCompData("end\n");
	}

	void onElseStatementBegin()
	{
		addTransCompData("else\n");
	}

	void onElseStatementEnd()
	{
		addTransCompData("end\n");
	}

	void onNewObject(cil::Value lhsVar, const std::string &namespaceName, const std::string &className, const std::string &methodName, const std::vector<cil::Value> &args)
	{
		if(lhsVar.isVariable())
		{
			addTransCompData("local ");
			addTransCompData(lhsVar.toString() + " = ");
		}

		const std::string *predefinedFunction = getPredefinedMethod(namespaceName + "." + className + "." + methodName);
		if(predefinedFunction)
			addTransCompData(*predefinedFunction + "(");
		else
			addTransCompData(namespaceName + "_" + className + "_" + methodName + "(");

		for(uint32 i = 0; i < args.size(); ++i)
		{
			addTransCompData(args[i].toString());
			if(i != args.size() - 1)
				addTransCompData(", ");
		}

		addTransCompData(")\n");
	}

	void onMemberMethodCall(cil::Value lhsVar, const std::string &namespaceName, const std::string &className, const std::string &methodName, const std::vector<cil::Value> &args)
	{
		if(lhsVar.isVariable())
		{
			addTransCompData("local ");
			addTransCompData(lhsVar.toString() + " = ");
		}

		const std::string *predefinedFunction = getPredefinedMethod(namespaceName + "." + className + "." + methodName);
		if(predefinedFunction)
			addTransCompData(*predefinedFunction + "(");
		else
			addTransCompData(namespaceName + "_" + className + "_" + methodName + "(");

		for(uint32 i = 0; i < args.size(); ++i)
		{
			addTransCompData(args[i].toString());
			if(i != args.size() - 1)
				addTransCompData(", ");
		}

		addTransCompData(")\n");
	}

	void onLoadField(cil::Value lhs, cil::Value obj, cil::Value field)
	{
		addTransCompData("local " + lhs.toString() + " = " + obj.toString() + "." + field.toString() + "\n");
	}

	void onStoreField(cil::Value obj, cil::Value field, cil::Value rhs)
	{
		addTransCompData(obj.toString() + "." + field.toString() + " = " + rhs.toString() + "\n");
	}
private:
	const std::string* getPredefinedMethod(const std::string &key)
	{
		std::unordered_map<std::string, std::string>::const_iterator it = m_predefinedMethods.find(key);
		if(it != m_predefinedMethods.cend())
			return &it->second;

		return nullptr;
	}
private:
	std::unordered_map<std::string, std::string> m_predefinedMethods;
};

#endif // LUA_PROCESSOR_HPP