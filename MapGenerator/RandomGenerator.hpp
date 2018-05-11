#ifndef _RANDOM_GENERATOR_HPP
#define _RANDOM_GENERATOR_HPP 1

class RandomGenerator
{
public:
	RandomGenerator(unsigned int seed);

	void mt_Set_Seed(unsigned int seed);
	unsigned int mt_Get_Seed(void) const;
	unsigned int mt_Get_Generation_Count(void) const;

	int mt_Generate(void);
	int mt_Generate(int min, int max);


private:
	unsigned int m_seed;
	unsigned int m_generation_count;
};

#endif // _RANDOM_GENERATOR_HPP
