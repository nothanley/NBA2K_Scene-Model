#include <material/material_reader.h>


CNSMaterialReader::CNSMaterialReader(JSON* json)
	:
	m_json(json)
{
}

void CNSMaterialReader::parse()
{
	if (!m_json)
		return;

	// ...

}