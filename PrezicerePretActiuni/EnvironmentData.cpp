#include "EnvironmentData.h"
#include <nlohmann/json.hpp>
#include <iostream>

EnvironmentData* EnvironmentData::instance;

std::string EnvironmentData::toJson() const
{
	nlohmann::json jsonData;
	jsonData["numar_maxim_antrenari"] = numar_maxim_antrenari;
	jsonData["procent_precizie_antrenament"] = procent_precizie_antrenament;
	jsonData["lambda"] = lambda;
	jsonData["normalisationStyle"] = normalisationStyle;
	jsonData["lossFunctionStyle"] = lossFunctionStyle;

	return jsonData.dump(4);
}

EnvironmentData* EnvironmentData::fromJson(const std::string& json)
{
	nlohmann::json jsonData = nlohmann::json::parse(json);
	EnvironmentData* envData = EnvironmentData::getInstance(0, 0, 0, DataNormalisationStyle::Logaritm, LossFunctionStyle::MSE);
	envData->numar_maxim_antrenari = jsonData["numar_maxim_antrenari"];
	envData->procent_precizie_antrenament = jsonData["procent_precizie_antrenament"];
	envData->lambda = jsonData["lambda"];
	envData->normalisationStyle = jsonData["normalisationStyle"];
	envData->lossFunctionStyle = jsonData["lossFunctionStyle"];

	return envData;
}
