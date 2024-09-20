#include "main.hxx"
#include <amx.h>
#include <amxaux.h>

#define DEFAULT_BOT_PLATFORM_TAG "BOT PLATFORM"

AMX_NATIVE_INFO bstring_Natives[] = {
	{NULL, NULL}};

int main()
{
	configController::testEasyJson();

	AMX amx;
	cell ret = 0;
	int err;

	err = aux_LoadProgram(&amx, "./test.amx", NULL);
	if (err != AMX_ERR_NONE)
	{
		LOGE(DEFAULT_BOT_PLATFORM_TAG, "Error load AMX file: %d", err);
		return err;
	}

	err = amx_Register(&amx, bstring_Natives, -1);
	if (err != AMX_ERR_NONE)
	{
		LOGE(DEFAULT_BOT_PLATFORM_TAG, "Error register native: %d", err);
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
		LOGE(DEFAULT_BOT_PLATFORM_TAG, "Error execute: %d", err);
	}

	aux_FreeProgram(&amx);

	return 0;
}
