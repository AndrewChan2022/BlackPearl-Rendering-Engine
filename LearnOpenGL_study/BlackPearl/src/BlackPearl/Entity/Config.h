#pragma once
#include<bitset>
namespace BlackPearl {
	//һ��Entity���32��Component
	static const unsigned int MaxComponents = 32;
	typedef std::bitset<MaxComponents> ComponentMask;

}