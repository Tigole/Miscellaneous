#include "RandomGenerator.hpp"

#include <cstdlib>

RandomGenerator::RandomGenerator(unsigned int seed)
 :	m_seed(seed),
    m_generation_count(0)
{
	std::srand(seed);
}

void RandomGenerator::mt_Set_Seed(unsigned int seed)
{
	m_seed = seed;
	std::srand(seed);
	m_generation_count = 0;
}

unsigned int RandomGenerator::mt_Get_Seed(void) const
{
	return m_seed;
}

unsigned int RandomGenerator::mt_Get_Generation_Count(void) const
{
    return m_generation_count;
}


int RandomGenerator::mt_Generate(void)
{
    m_generation_count++;
	return std::rand();
}

int RandomGenerator::mt_Generate(int min, int max)
{
	int l_dist(std::abs(max - min));
	int l_ret(min);

	if (l_dist != 0)
    {
        m_generation_count++;
		l_ret = ((std::rand() % l_dist) + min);
    }

	return l_ret;
}
