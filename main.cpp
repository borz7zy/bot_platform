#include "main.hxx"
#include <stdexcept>
#include <sstream>
#include <amx.h>
#include <amxaux.h>
#include <easy_json.hxx>
#include <logprint.hxx>

#define DEFAULT_BOT_PLATFORM_TAG "BOT PLATFORM"
#define EASY_JSON_TAG " / EASY JSON"
#define CONFIG_MANAGER_TAG " / CONFIG MANAGER"

#define CONFIG_PATH "./configCore.json"

config_controller::ConfigManager &config_controller::ConfigManager::getInstance()
{
	static ConfigManager instance;
	return instance;
}

template <typename T>
void config_controller::ConfigManager::setConfig(const std::string &key, const T &value, AccessType access)
{
	if (configData.find(key) != configData.end() && configData[key].access == AccessType::RO)
	{
		throw std::runtime_error("Attempt to write to read-only parameter: " + key);
	}

	configData[key] = {toString(value), access};
}

std::string config_controller::ConfigManager::getConfig(const std::string &key) const
{
	auto it = configData.find(key);
	if (it != configData.end())
	{
		return it->second.value;
	}
	throw std::runtime_error("Parameter not found: " + key);
}

template <typename T>
std::string config_controller::ConfigManager::toString(const T &value) const
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template <>
std::string config_controller::ConfigManager::toString(const bool &value) const
{
	return value ? "true" : "false";
}

template void config_controller::ConfigManager::setConfig<bool>(const std::string &key, const bool &value, AccessType access);
template void config_controller::ConfigManager::setConfig<int>(const std::string &key, const int &value, AccessType access);
template void config_controller::ConfigManager::setConfig<std::string>(const std::string &key, const std::string &value, AccessType access);

void clearString(std::string &s)
{
	std::fill(s.begin(), s.end(), '\0');
	s.clear();
}

AMX_NATIVE_INFO bstring_Natives[] = {
	{NULL, NULL}};

int main()
{
	std::string configVers,
		pluginsPath,
		mainScript,
		plugins;
	bool hotReloadAMX;
	try
	{
		config_controller::ConfigManager &config = config_controller::ConfigManager::getInstance();

		int response_ej = easy_json::file_get_value_string(CONFIG_PATH, "configVersion", configVers);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (configVersion): %d", response_ej);
		}
		else
			config.setConfig("configVersion", configVers, config_controller::ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "pluginsPath", pluginsPath);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (pluginsPath): %d", response_ej);
		}
		else
			config.setConfig("pluginsPath", pluginsPath, config_controller::ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "mainScript", mainScript);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (mainScript): %d", response_ej);
		}
		else
			config.setConfig("mainScript", mainScript, config_controller::ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_string(CONFIG_PATH, "plugins", mainScript);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (plugins): %d", response_ej);
		}
		else
			config.setConfig("plugins", plugins, config_controller::ConfigManager::AccessType::RO);

		response_ej = easy_json::file_get_value_bool(CONFIG_PATH, "hotReloadAMX", hotReloadAMX);
		if (response_ej != 0)
		{
			LOGE(DEFAULT_BOT_PLATFORM_TAG EASY_JSON_TAG, "Error extracting config path (hotReloadAMX): %d", response_ej);
		}
		else
			config.setConfig("hotReloadAMX", hotReloadAMX, config_controller::ConfigManager::AccessType::RO);
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
