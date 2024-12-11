#pragma once

//##########################################
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
//##########################################

#define STACK_SIZE 16
#define MEMORY_SIZE 4096
#define FONT_BYTE_ARRAY_SIZE 80

#define STACK_UPPER_BOUND 0x0
#define STACK_LOWER_BOUND STACK_SIZE
#define FONT_START_ADDRESS 0x050
#define FONT_END_ADDRESS (FONT_START_ADDRESS + 79)
#define MEMORY_START_ADDRESS 0x200
#define MEMORY_END_ADDRESS 0xFFF

#define CHARACTER_FONT_LENGTH 5
//##########################################


enum StackOperationReturnCode {
	STACK_OVERFLOW,
	STACK_UNDERFLOW,
	STACK_OPERATION_SUCCESS
};
//##########################################

class Memory
{
private:
	//Points to first empty place inside stack
	uint16_t m_StackPointer;
	uint16_t m_StackContent[STACK_SIZE];

	uint8_t m_MemoryContent[MEMORY_SIZE];
	uint8_t m_FontBytes[FONT_BYTE_ARRAY_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
public:
	enum MemoryReturnCode : uint8_t
	{
		MEMORY_OK, MEMORY_OVERFLOW, INVALID_ROM_FILE_NAME
	};
public:
	Memory();
	uint8_t GetStackPointer();
	//@param value The Value To be pushed onto stack
	StackOperationReturnCode PushOntoStack(uint16_t value);
	//@param popedValue A Refrence to a variable that will contain the value after it's popped from top of stack
	StackOperationReturnCode PopFromStack(uint16_t& popedValue);
	//@param programBinary the program binary that will be run
	MemoryReturnCode LoadMemory(const std::string& binaryFileName);
	//@param address the address location of word you want to read
	uint16_t ReadWord(uint16_t address);
	//@param address the address location of half_word you want to read
	uint8_t ReadHalfWord(uint16_t address);
	//@param address the address location inside the memory you want to write to
	//@param data the word you want to write to that address
	void WriteWord(uint16_t address , uint16_t data);
	//@param address the address location inside the memory you want to write to
	//@param data the half_word you want to write to that address
	void WriteHalfWord(uint16_t address , uint8_t data);
};

