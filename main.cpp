#include "stdafx.h"

int main(int argc, char* argv[])
{
	try
	{
		RiddleSolver rs(9);
		rs.initWithRows({
				{8,6,0,0,2,0,0,0,0},
				{0,0,0,7,0,0,0,5,9},
				{0,0,0,0,0,0,0,0,0},
				{0,0,0,0,6,0,8,0,0},
				{0,4,0,0,0,0,0,0,0},
				{0,0,5,3,0,0,0,0,7},
				{0,0,0,0,0,0,0,0,0},
				{0,2,0,0,0,0,6,0,0},
				{0,0,7,5,0,9,0,0,0}
				});
		RiddleSolver::printRiddle(rs.solve());
	}
	catch (const std::exception& exc)
	{
		std::cout << "Error occurred: " << exc.what() << std::endl;
	}

	return 0;
}
