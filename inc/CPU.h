#pragma once
//##########################################
#include <stdint.h>
#include <random>
#include <thread>
#include <functional>
#include "Memory.h"
#include "Timer.h"
#include "Display.h"
#include "ByteManipulator.h"
#include "InputHandler.h"
//##########################################

#define NUMBER_OF_GENERAL_PURPOSE_REGS (16)
#define CHIP_8_TIMERS_DECREMENT_RATE_HZ (60)

#define RESERVED_INTERPRETER_MEMORY_START_ADDRESS (0x000)
#define RESERVED_INTERPRETER_MEMORY_END_ADDRESS (0x1FF)
#define CHIP_8_PC_START_ADDRESS (RESERVED_INTERPRETER_MEMORY_END_ADDRESS + 1)

#define SPRITE_WIDTH 8
#define CHIP8_INSTRUCTION_WIDTH_IN_BYTES 2

//##########################################

class CPU final
{
private:
	uint16_t m_ProgramCounter;
	uint16_t m_IndexRegister;
	Timer m_DelayTimer;
	Timer m_SoundDelayTimer;
	uint8_t m_GeneralPurposeRegisters[NUMBER_OF_GENERAL_PURPOSE_REGS];
	Memory m_Memory;
	Display m_Display;
	static CPU* m_CPU_6502InstancePtr;
	std::uniform_int_distribution<> m_GeneratorDistribution;
	std::mt19937 m_Generator; 
	uint32_t m_InstructionExecutionCyclePeriod;
	CPU(uint32_t executionSpeed);
public:
	enum FlagRegisterValue : uint8_t
	{
		NOT_RAISED = 0x00,
		RAISED = 0x01,
	};
	enum InstructionOpCodes : uint8_t
	{
		CLEAR_DISPLAY_AND_RETURN_PROCEDURE = 0x0,
		JUMP = 0x1,
		CALL_SUBROUTINE = 0x2,
		SKIP_CONDITIONAL_IMMEDIATE_EQUALITY = 0x3,
		SKIP_CONDITIONAL_IMMEDIATE_INEQUALITY = 0x4,
		SKIP_CONDITIONAL_REG_EQUALITY = 0x5,
		SET_REG_WITH_IMMEDIATE_VALUE = 0x6,
		ADD_VALUE_TO_REG = 0x7,
		LOGICAL_AND_ARETHMETIC_INSTRUCTIONS = 0x8,
		SKIP_CONDITIONAL_REG_INEQUALITY = 0x9,
		SET_INDEX_REG = 0xA,
		JUMP_WITH_OFFSET = 0xB,
		GENERATE_RANDOM = 0xC,
		DISPLAY = 0xD, 
		SKIP_IF_KEY = 0xE,
		MISC_INSTRUCTIONS = 0xF,
	};
	enum MiscInstructionCodes : uint8_t
	{
		READ_DELAY_TIMER_INTO_VX = 0x07,
		SET_DELAY_TIMER_WITH_VX = 0x15,
		SET_SOUND_TIMER_WITH_VX = 0x18,
		ADD_VX_TO_I_REG = 0x1E,
		GET_KEYBOARD_KEY_PRESSED_BLOCKING = 0x0A,
		SET_I_REG_TO_CHAR_FONT_ADDRESS = 0x29,
		CONVERT_VX_TO_BCD = 0x33,
		SAVE_REGISTER_RANGE_TO_MEMORY = 0x55,
		LOAD_REGISTER_RANGE_FROM_MEMORY = 0x65,
	};
	enum LogicalAndArithmeticInstructionCodes : uint8_t
	{
		SET_REG_WITH_REG_VALUE = 0x0,
		BITWISE_OR = 0x1,
		BITWISE_AND = 0x2,
		BITWISE_XOR = 0x3,
		ADD_REGISTERS = 0x4,
		SUBTRACT_REGISTERS_NORMAL = 0x5,
		SHIFT_REGISTER_RIGHT = 0x6,
		SUBTRACT_REGISTERS_REVERSED = 0x7,
		SHIFT_REGISTER_LEFT = 0xE,
	};
	enum KeyEvent : uint8_t
	{
		KeyPressed = 0xE,
		KeyNotPressed = 0x1,
	};
	enum GeneralPurposeRegisterNames : uint8_t
	{
		V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF
	};
private:
	uint16_t Fetch();
	void Execute(uint16_t instruction);
	void JumpToAddress(uint16_t address);
	bool CallSubRoutine(uint16_t subroutineAddress);
	bool ReturnFromSubroutine();
	void SkipInstructionOnCondition(uint8_t value1, uint8_t value2, std::function<bool(uint8_t, uint8_t)> comparator);
	void AddRegisters(GeneralPurposeRegisterNames operandReg1 , GeneralPurposeRegisterNames operandReg2);
	void SubtractRegisters(GeneralPurposeRegisterNames operandReg1, GeneralPurposeRegisterNames operandReg2, bool reversed);
	void ShiftRegister(GeneralPurposeRegisterNames operandReg1 , GeneralPurposeRegisterNames operandReg2 , bool right);
	void DisplaySprite(uint16_t spriteStartAddress, uint8_t initialXCoordiante, uint8_t initialYCoordinate, uint8_t spriteLength);
	void StoreRegistersToMemory(GeneralPurposeRegisterNames endRegister);
	void LoadRegistersFromMemory(GeneralPurposeRegisterNames endRegsiter);
	void ConvertToBCD(uint8_t valueToConvert);

	void GetKeyPressed(GeneralPurposeRegisterNames operandRegister);

public:
	//delete both copy assignment operator and consturctor to avoid creating more instances of this singleton
	CPU(const CPU& other) = delete;
	CPU(CPU&& other) = delete;
	CPU& operator=(const CPU& other) = delete;
	CPU& operator=(CPU&& other) = delete;
	static CPU* GetInstance(uint32_t executionSpeed);
	~CPU();
	void Run(const std::string& binaryFileName);
};

