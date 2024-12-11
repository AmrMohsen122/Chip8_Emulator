#include "CPU.h"

int main(int argc, char* argv[])
{
	std::string romFileName{};
	int executionSpeed = 1; //Measured in instructions per second
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-f") == 0)
		{
			romFileName = argv[i + 1];
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			executionSpeed = std::stoi(argv[i + 1]);
		}
	}
	if (executionSpeed == 0)
	{
		std::cout << "[ERROR] execution speed can't be 0" << std::endl;
		return 0;
	}
	if (romFileName.empty())
	{
		std::cout << "[ERROR] rom file name can't be empty" << std::endl;
		return 0;
	}
	CPU* cpu = CPU::GetInstance(executionSpeed);
	cpu->Run(romFileName);
	return 0;
}

