#include "LogManager.hpp"
#include "PluginLog.hpp"


bool Logger::Log(samplog::LogLevel level, std::string &&msg, AMX *amx)
{
	PluginLog::Get()->Log(samplog::LogLevel::DEBUG, "Logger::Log(level={}, msg='{}', amx={})",
		level, msg, static_cast<const void *>(amx));

	return m_Logger->Log(level, msg);
}


Logger::Id LogManager::Create(std::string logname, bool debuginfo)
{
	PluginLog::Get()->Log(samplog::LogLevel::DEBUG, 
		"LogManager::Create(logname='{}', debuginfo={})",
		logname, debuginfo);

	if (logname.empty())
	{
		PluginLog::Get()->Log(samplog::LogLevel::ERROR, 
			"LogManager::Create - empty log name");
		return 0;
	}

	if (logname.find("plugins/") == 0)
	{
		PluginLog::Get()->Log(samplog::LogLevel::ERROR, 
			"LogManager::Create - invalid log name");
		return 0;
	}

	if (logname.find("log-core") == 0 || logname.find("samp-server") == 0)
	{
		PluginLog::Get()->Log(samplog::LogLevel::ERROR,
			"LogManager::Create - invalid log name");
		return 0;
	}

	Logger::Id id = 1;
	while (m_Logs.find(id) != m_Logs.end())
		++id;

	PluginLog::Get()->Log(samplog::LogLevel::INFO, 
		"created logger '{}' with id {}", logname, id);

	m_Logs.emplace(id, Logger(samplog::Api::Get()->CreateLogger(logname.c_str()), debuginfo));
	return id;
}
