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

void commonjson::findAllMatchingJsonValues(JSON& obj, const char* target_key, std::vector<std::string>& list)
{
	/* Recursive scan through json for all 'string' value types with matching input key */
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (it.value().is_string() && it.key() == target_key)
		{
			// append to list
			list.push_back(it.value());
		}
		else if (it.value().is_object())
		{
			commonjson::findAllMatchingJsonValues(it.value(), target_key, list);
		}
	}
}

