#include <material/effect.h>
#include <common_json.h>


CNSEffect::CNSEffect()
{

}

void
CNSEffect::loadShaderResources()
{
	if (path.empty())
		return;

	commonjson::findAllMatchingJsonValues(*data.get(), "Binary", shaders);
}

