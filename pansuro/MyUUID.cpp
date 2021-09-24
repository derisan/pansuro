#include "pch.h"
#include "MyUUID.h"

#include <random>

static std::random_device s_RandomDevice;
static std::mt19937_64 eng(s_RandomDevice());
static std::uniform_int_distribution<UINT64> s_UniformIntDistribution;

MyUUID::MyUUID()
	: m_UUID(s_UniformIntDistribution(eng))
{

}
