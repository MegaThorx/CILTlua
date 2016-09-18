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

#include "../CILT/src/cilt.hpp"
#include "LuaProcessor.hpp"

using namespace std;

int main()
{
	const path inputFilepath = PATH_STR("testInputFile");

	unordered_map<string, string> methods =
	{
		make_pair("MTASAClient.Chat.output", "outputChatBox"),
		make_pair("MTASAClient.Console.output", "outputConsole"),
		make_pair("MTASAShared.Fps.getLimit", "getFPSLimit"),
		make_pair("MTASAShared.Fps.setLimit", "setFPSLimit"),
	};
	LuaProcessor luaProcessor(methods);
	
	CompileResult compileResult = CILT::compile(inputFilepath, &luaProcessor);
	if(compileResult.fail())
	{
		printf("Failed to compile, reason: %s\n", compileResult.getErrorMessage());
		return (int)compileResult.getErrorCode();
	}

	FILE *outputFile = fileOpen(PATH_STR("testOutputFile.lua"), PATH_STR("wb"));
	fwrite(compileResult.getLuaOutput().c_str(), 1, compileResult.getLuaOutput().size(), outputFile);
	fclose(outputFile);
	
	printf("Successfully compiled!\n");
	return 0;
}
