#include "Memory.h"

Memory::Memory() : m_StackContent{ 0 }, m_MemoryContent{0}
{
	m_StackPointer = STACK_LOWER_BOUND - 1;
}

uint8_t Memory::GetStackPointer()
{
	return m_StackPointer;
}

StackOperationReturnCode Memory::PushOntoStack(uint16_t value) {
	if (m_StackPointer >= STACK_UPPER_BOUND)
	{
		m_StackContent[m_StackPointer--] = value;
		return 	STACK_OPERATION_SUCCESS;
	}
	return STACK_OVERFLOW;
}

StackOperationReturnCode Memory::PopFromStack(uint16_t& poppedValue)
{
	if (m_StackPointer < STACK_LOWER_BOUND)
	{
		poppedValue = m_StackContent[++m_StackPointer];
		return STACK_OPERATION_SUCCESS;
	}
	return STACK_UNDERFLOW;
}

Memory::MemoryReturnCode Memory::LoadMemory(const std::string& binaryFileName)
{
	uint16_t currentAddress = MEMORY_START_ADDRESS;
	char currentReadByte;
	std::ifstream binaryFile(binaryFileName , std::ifstream::in | std::ifstream::binary);
	if (!binaryFile.is_open())
	{
		return INVALID_ROM_FILE_NAME;
	}

	while (binaryFile.get(currentReadByte)) {
		WriteHalfWord(currentAddress, currentReadByte);
		++currentAddress;
		if (currentAddress > MEMORY_END_ADDRESS)
		{
			return MEMORY_OVERFLOW;
		}
	}
	binaryFile.close();
	return MEMORY_OK;
}

uint16_t Memory::ReadWord(uint16_t address)
{
	//This assumes a big endian 
	return ((ReadHalfWord(address) << 8) | ReadHalfWord(address + 1));
}

uint8_t Memory::ReadHalfWord(uint16_t address)
{
	if (address >= FONT_START_ADDRESS && address <= FONT_END_ADDRESS)
		return m_FontBytes[(address - FONT_START_ADDRESS)];
	return m_MemoryContent[(address - MEMORY_START_ADDRESS)];
}

void Memory::WriteWord(uint16_t address, uint16_t data)
{
	//This assumes a big endian 
	WriteHalfWord(address , static_cast<uint8_t>(data >> 8));
	WriteHalfWord(address + 1 , static_cast<uint8_t>(data));
}

void Memory::WriteHalfWord(uint16_t address, uint8_t data)
{
	m_MemoryContent[address - MEMORY_START_ADDRESS] = data;
}
