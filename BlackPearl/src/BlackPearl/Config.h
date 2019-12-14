#pragma once
#include<bitset>
namespace BlackPearl {
	//һ��Entity���32��Component
	class Configuration {
	public:
		static const unsigned int MaxComponents = 32;
		typedef std::bitset<MaxComponents> ComponentMask;

		static const unsigned int WindowWidth = 960;
		static const unsigned int WindowHeight = 540;

		//���ز���
		static const unsigned int MSAA_SAMPLES = 0;

	};


}