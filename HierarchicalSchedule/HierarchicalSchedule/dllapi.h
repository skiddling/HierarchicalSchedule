#pragma once

#include <string>

#ifdef DLLAPI_EXPORTS
#define DLLAPIEXPORT __declspec(dllexport)
#else
#define DLLAPIEXPORT __declspec(dllimport)
#endif // DLLAPI_EXPORTS

extern "C" {
	DLLAPIEXPORT std::string GetSchedule(std::string s);
}

