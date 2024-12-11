//#####################################################
#include "ByteManipulator.h"
//#####################################################

/*
	@param value		the value from which to extract the nibble
	@param nibbleNumber the position of that nibble (most significant nibble has position 1)
*/
uint8_t ByteManipulator::ExtractNibble(uint16_t value, uint8_t nibblePosition)
{
	if (nibblePosition > UINT16_NUMBER_OF_NIBBLES)
	{
		std::cout << "[ERROR] Invalid nibble number a 16-bit number contains only " <<
			UINT16_NUMBER_OF_NIBBLES << " nibbles" << std::endl;
	}
	return (value >> ((sizeof(value) * 2 - nibblePosition) * 4)) & 0xFU;
}

/*
	@param value		the value from which to extract the nibble
	@param nibbleNumber the position of that nibble (most significant nibble has position 1)
*/
uint8_t ByteManipulator::ExtractNibble(uint8_t value, uint8_t nibblePosition)
{
	if (nibblePosition > UINT8_NUMBER_OF_NIBBLES)
	{
		std::cout << "[ERROR] Invalid nibble number an 8-bit number contains only " <<
			UINT8_NUMBER_OF_NIBBLES << " nibbles" << std::endl;
	}
	return (value >> ((sizeof(value) * 2 - nibblePosition) * 4)) & 0xFU;
}

/*
	@param value		the value from which to extract the byte
	@param nibbleNumber the position of that byte (most significant nibble has position 1)
*/
uint8_t ByteManipulator::ExtractByte(uint16_t value, uint8_t bytePosition)
{
	if (bytePosition > UINT16_NUMBER_OF_BYTES)
	{
		std::cout << "[ERROR] Invalid nibble number a 16-bit number contains only " <<
			UINT16_NUMBER_OF_BYTES << " bytes" << std::endl;
	}
	return (value >> ((2 - bytePosition) * 8)) & 0xFFU;
}

uint16_t ByteManipulator::Extract12LSBits(uint16_t value)
{
	return value & 0xFFFu;
}

uint8_t ByteManipulator::GetBit(uint8_t value, uint8_t bitPosition)
{
	if (bitPosition > UINT8_NUMBER_OF_BITS)
	{
		std::cout << "[ERROR] Invalid bit number an 8-bit number contains only " <<
			UINT8_NUMBER_OF_BITS << " bits" << std::endl;
	}
	return (value >> (UINT8_NUMBER_OF_BITS - bitPosition )) & 0x1U;
}

uint8_t ByteManipulator::GetBit(uint16_t value, uint8_t bitPosition)
{
	if (bitPosition > UINT16_NUMBER_OF_BITS)
	{
		std::cout << "[ERROR] Invalid bit number a 16-bit number contains only " <<
			UINT16_NUMBER_OF_BITS << " bits" << std::endl;
	}
	return (value >> (UINT16_NUMBER_OF_BITS - bitPosition)) & 0x1U;
}


