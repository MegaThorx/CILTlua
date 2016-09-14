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

#include "../CILT/src/ciltProcessor.hpp"
#include <unordered_map>
#include <utility> // std::make_pair

class LuaProcessor : public CILTProcessor
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
	LuaProcessor(const StringKeyValueMap &predefinedMethods) : m_predefinedMethods(predefinedMethods) {}

	void onMethodBegin(const std::string &namespaceName, const std::string &className, const std::string &methodName)
	{
		addTransCompData("function " + namespaceName + "_" + className + "_" + methodName + "()\n");
	}

	void onMethodEnd(const std::string &namespaceName, const std::string &className, const std::string &methodName)
	{
		addTransCompData("end\n");
	}

	// This implementation currently only works with static methods, but it will be fixed later.
	// At this stage, we are just testing small things at a time
	void onMethodCall(const std::string &namespaceName, const std::string &className, const std::string &methodName)
	{
		const std::string *predefinedFunction = getPredefinedMethod(namespaceName + "." + className + "." + methodName);
		if(predefinedFunction)
			addTransCompData(*predefinedFunction + "()\n");
		else
			addTransCompData(namespaceName + "_" + className + "_" + methodName + "()\n");
	}

	void onMethodReturn()
	{
		addTransCompData("return\n");
	}
private:
	const std::string* getPredefinedMethod(const std::string &key)
	{
		StringKeyValueMap::const_iterator it = m_predefinedMethods.find(key);
		if(it != m_predefinedMethods.cend())
			return &it->second;

		return nullptr;
	}
private:
	StringKeyValueMap m_predefinedMethods;
};

#endif // LUA_PROCESSOR_HPP