#include <iostream>
#include <fstream>
#include <vector>
#include "webserv.hpp"

int main(int argc, char **argv)
{
	std::string a(argv[1]);
	ConfigGroup obj(a, 20);
	std::vector<Config> test1 = obj.getConfig(0);

	return 0;
}


//#include <iostream>

//int main(void)
//{
//	int array[10] = {4, 5, 2, 7, 9, 1, 8, 3, 6, 10};
//	int i, j, temp;
//	for(i=0; i<10; i++)
//	{
//		for(j=0; j<9-i; j++)
//		{
//			std::cout << "i : " << i << std::endl;
//			std::cout << "j : " << j << std::endl;


//			if(array[j] > array[j+1])
//			{

//				temp = array[j];
//				array[j] = array[j+1];
//				array[j+1] = temp;
//			}
//		}
//	} // 결과 확인
//	for(i=0; i<10; i++)
//	{
//		std::cout << array[i] << std::endl;
//	}
//	return 0;
//}
