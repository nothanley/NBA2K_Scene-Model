#include <material/material.h>
#pragma once

class CNSMaterialReader
{

public:
	CNSMaterialReader(JSON* json);

public:
	void parse();

private:
	JSON* m_json;
};