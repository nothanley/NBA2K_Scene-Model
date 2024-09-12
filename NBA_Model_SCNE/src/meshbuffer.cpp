#include <meshbuffer.h>
#include <nbamodel.h>
#include <common.h>

DataBuffer::DataBuffer()
	:
	m_index(0)
{
}

void DataBuffer::loadBinary()
{

}

int DataBuffer::getStreamIdx()
{
	return m_index;
}

void DataBuffer::parse(JSON& json)
{
	for (JSON::iterator it = json.begin(); it != json.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
			case enPropertyTag::FORMAT:
				m_format = it.value();
				break;
			case enPropertyTag::STREAM:
				m_index  = it.value();
				break;
			case enPropertyTag::OFFSET:
				m_offset = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SCALE:
				m_scale  = { it.value()[0], it.value()[1], it.value()[2], it.value()[3] };
				break;
			case enPropertyTag::SIZE:
				m_size = it.value();
				break;
			case enPropertyTag::BINARY:
				m_path = it.value();
				break;
			case enPropertyTag::STRIDE:
				m_stride = it.value();
				break;
			default:
				break;
		};
	}
}


