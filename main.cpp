#include <iostream>
#include <amx.h>
#include <amxaux.h>

AMX_NATIVE_INFO bstring_Natives[] = {
	{NULL, NULL}};

int main()
{
	AMX amx;
	cell ret = 0;
	int err;

	err = aux_LoadProgram(&amx, "./test.amx", NULL);
	if (err != AMX_ERR_NONE)
	{
		std::cerr << "Error load program: " << err << std::endl;
		return err;
	}

	err = amx_Register(&amx, bstring_Natives, -1);
	if (err != AMX_ERR_NONE)
	{
		std::cerr << "Error register native: " << err << std::endl;
		aux_FreeProgram(&amx);
		return err;
	}

	err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	while (err == AMX_ERR_SLEEP)
	{
		err = amx_Exec(&amx, &ret, AMX_EXEC_CONT);
	}

	if (err != AMX_ERR_NONE)
	{
		std::cerr << "Error execute: " << err << std::endl;
	}

	aux_FreeProgram(&amx);

	return 0;
}
