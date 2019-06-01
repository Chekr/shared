#ifndef _DISCRETE_LIST_H_
#define _DISCRETE_LIST_H_

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <random>
#include <string>
using std::string;

#include "math-helpers.h"

template<typename T>
class DiscreteBspList
{
public:
	DiscreteBspList(map<T, float> items) : gen(rd())
	{
		for(auto item : m_items)
		{
			m_values.push_back(item.first);
			m_weights.push_back(item.second);
		}

		m_d = new std::discrete_distribution<int>(m_weights.begin(), weights.end());
	}

	~DiscreteBspList()
	{
		delete m_d;
	}


	T GetRandomItem()
	{
		int index = m_d->operator()(m_gen);
		return m_values[index];
	}

public:


private:
	std::random_device m_rd;
	std::mt19937 m_gen;

	std::discrete_distribution<int>* m_d;

	vector<float> m_weights;
	vector<T> m_values;
};

template<typename T>
class DiscreteList
{
public:
	DiscreteList()
	{
		m_sumOfWeight = 0.0f;
	}

	void AddItem(T item, float weight)
	{
		if(weight < 0.0f)
		{
			return;
		}
		m_items[item] = weight;
		m_sumOfWeight += weight;
	}

	string GetRandomItem()
	{
		float rnd = RandInRange(0.0f, m_sumOfWeight);

		for(auto item : m_items)
		{
			if(rnd < item.second)
			{
				return item.first;
			}
			rnd -= item.second;
		}

		return nullptr;
	}

private:
	map<T, float> m_items;
	float m_sumOfWeight;
};


#endif
