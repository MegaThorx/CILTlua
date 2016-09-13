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

class LuaProcessor : public CILTProcessor
{
public:
	void onMethodBegin(const std::string &methodName)
	{

	}

	void onMethodEnd(const std::string &methodName)
	{

	}

	void onMethodCall(const std::string &namespaceName, const std::string &className, const std::string &methodName)
	{
		addTransCompData(namespaceName + "_" + className + "_" + methodName);
	}
};

#endif // LUA_PROCESSOR_HPP