#pragma once
#include <stdint.h>
#include <iostream>
//#####################################################
#define UINT16_NUMBER_OF_BYTES       (sizeof(uint16_t))
#define UINT16_NUMBER_OF_NIBBLES	 (sizeof(uint16_t) *  2U)
#define UINT16_NUMBER_OF_BITS		 (sizeof(uint16_t) *  8U)
#define UINT8_NUMBER_OF_NIBBLES		 (sizeof(uint8_t)  *  2U) 
#define UINT8_NUMBER_OF_BITS		 (sizeof(uint8_t)  *  8U) 
//#####################################################

struct ByteManipulator
{
private:
	ByteManipulator();
public:

	/*
		@param value		the value from which to extract the nibble
		@param nibbleNumber the position of that nibble (most significant nibble has position 1)
	*/
	static uint8_t ExtractNibble(uint16_t value, uint8_t nibblePosition);

	/*
		@param value		the value from which to extract the nibble
		@param nibbleNumber the position of that nibble (most significant nibble has position 1)
	*/
	static uint8_t ExtractNibble(uint8_t value, uint8_t nibblePosition);

	/*
		@param value		the value from which to extract the byte
		@param nibbleNumber the position of that byte (most significant nibble has position 1)
	*/
	static uint8_t ExtractByte(uint16_t value, uint8_t bytePosition);

	static uint16_t Extract12LSBits(uint16_t value);

	/*
		@param value		the value from which to extract the bit
		@param nibbleNumber the position of that bit (most significant bit has position 1)
	*/
	static uint8_t GetBit(uint8_t value, uint8_t bitPosition);

	/*
		@param value		the value from which to extract the bit
		@param nibbleNumber the position of that bit (most significant bit has position 1)
	*/
	static uint8_t GetBit(uint16_t value, uint8_t bitPosition);

};

