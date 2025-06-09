#pragma once
#include <string>
class IJsonSerializable
{
	virtual std::string toJson() const = 0;
	static IJsonSerializable* fromJson(const std::string& json) = delete;
};

