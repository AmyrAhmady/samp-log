#pragma once

#include <samplog/samplog.hpp>
#include "Singleton.hpp"

#include <fmt/format.h>

using samplog::PluginLogger_t;
using samplog::LogLevel;
using samplog::AmxFuncCallInfo;

class PluginLog : public Singleton<PluginLog>
{
	friend class Singleton<PluginLog>;
private:
	PluginLog() :
		m_Logger("log-plugin")
	{ }
	~PluginLog() = default;

public:
	inline bool IsLogLevel(LogLevel level)
	{
		return m_Logger.IsLogLevel(level);
	}

	template<typename... Args>
	inline void Log(LogLevel level, const char *msg)
	{
		m_Logger.Log(level, msg);
	}

	template<typename... Args>
	inline void Log(LogLevel level, const char *format, Args &&...args)
	{
		auto str = fmt::format(format, std::forward<Args>(args)...);
		m_Logger.Log(level, str.c_str());
	}

	// should only be called in native functions
	template<typename... Args>
	void LogNative(LogLevel level, const char *fmt, Args &&...args)
	{
		if (!IsLogLevel(level))
			return;

		std::string msg = fmt::format("{:s}", fmt::format(fmt, std::forward<Args>(args)...));

		Log(level, msg.c_str());
	}

private:
	PluginLogger_t m_Logger;
};
