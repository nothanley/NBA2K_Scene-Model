#include <common_json.h>
#include <fstream>

std::shared_ptr<JSON> commonjson::loadFileJson(const char* path)
{
	try
	{
		std::ifstream file(path);
		auto json = std::make_shared<JSON>(JSON::parse(file));
		return json;
	}
	catch (...)
	{
	}

	return nullptr;
}


//Checks if string in vector
inline static bool isStringInVector(const std::vector<std::string>& list, const std::string& target)
{
	return std::find(list.begin(), list.end(), target) != list.end();
}

void commonjson::findAllMatchingJsonValues(JSON& obj, const char* target_key, std::vector<std::string>& list)
{
	/* Recursive scan through json for all 'string' value types with matching input key */
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_string() && it.key() == target_key)
		{
			// push to list
			if (!::isStringInVector(list, it.value()))
				list.push_back(it.value());
		}
		else if (it.value().is_object())
		{
			commonjson::findAllMatchingJsonValues(it.value(), target_key, list);
		}
	}
}

