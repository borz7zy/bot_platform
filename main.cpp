#include "main.hxx"
#include <stdexcept>
#include <amx.h>
#include <amxaux.h>
#include <easy_json.hxx>
#include <logprint.hxx>
#include "config_controller.hxx"
#include "plugin_manager.hxx"

#define DEFAULT_BOT_PLATFORM_TAG "BOT PLATFORM"
#define EASY_JSON_TAG " / EASY JSON"
#define CONFIG_MANAGER_TAG " / CONFIG MANAGER"

#define DEFAULT_AMX_TAG "AMX"

#define CONFIG_PATH "./configCore.json"

void clearString(std::string &s)
{
	std::fill(s.begin(), s.end(), '\0');
	s.clear();
}

namespace amx_natives
{
	cell AMX_NATIVE_CALL LOG_I(AMX *amx, const cell *params)
	{
		const char *str = nullptr;
		amx_StrParam_Type(amx, params[1], str, char *);
		if (str != nullptr)
			LOGI(DEFAULT_AMX_TAG, str);
		return 0;
	}
	cell AMX_NATIVE_CALL LOG_E(AMX *amx, const cell *params)
	{
		const char *str = nullptr;
		amx_StrParam_Type(amx, params[1], str, char *);
		if (str != nullptr)
			LOGE(DEFAULT_AMX_TAG, str);
		return 0;
	}

	AMX_NATIVE_INFO bstring_Natives[] = {
		{"LOGI", amx_natives::LOG_I},
		{"LOGE", amx_natives::LOG_E},
		{NULL, NULL}};
}

int main()
{
	PluginManager pluginManager;

	std::string pluginTestPath = "./libplugin.dylib";

	if (pluginManager.loadPlugin(pluginTestPath))
	{
		pluginManager.executePluginFunction("pluginFunction");
		pluginManager.unloadPlugin();
	}

	std::string configVers,
		pluginsPath,
		mainScript,
		plugins;
	bool hotReloadAMX;
	ConfigManager &config = ConfigManager::getInstance();
	try
	{
		int response_ej = easy_json::file_get_value_string(CONFIG_PATH, "configVersion", configVers);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (configVersion): %d", response_ej);
		}
		else
			config.setConfig("configVersion", configVers, ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "pluginsPath", pluginsPath);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (pluginsPath): %d", response_ej);
		}
		else
			config.setConfig("pluginsPath", pluginsPath, ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "mainScript", mainScript);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (mainScript): %d", response_ej);
		}
		else
			config.setConfig("mainScript", mainScript, ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "plugins", mainScript);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (plugins): %d", response_ej);
		}
		else
			config.setConfig("plugins", plugins, ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_bool(CONFIG_PATH, "hotReloadAMX", hotReloadAMX);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (hotReloadAMX): %d", response_ej);
		}
		else
			config.setConfig("hotReloadAMX", hotReloadAMX, ConfigManager::AccessType::RO);
	}
	catch (const std::exception &e)
	{
		LOGE(DEFAULT_BOT_PLATFORM_TAG CONFIG_MANAGER_TAG, "Error: %s", e.what());
	}

	clearString(configVers);
	clearString(pluginsPath);
	clearString(mainScript);
	clearString(plugins);

	AMX amx;
	cell ret = 0;
	int err;

	try
	{
		err = aux_LoadProgram(&amx, (char *)config.getConfig("mainScript").c_str(), NULL);
		if (err != AMX_ERR_NONE)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG, "Error load AMX file: %d", err);
			return err;
		}
	}
	catch (const std::exception &e)
	{
		LOGE(DEFAULT_BOT_PLATFORM_TAG CONFIG_MANAGER_TAG, "Error: %s", e.what());
		return -1;
	}

	err = amx_Register(&amx, amx_natives::bstring_Natives, -1);
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
