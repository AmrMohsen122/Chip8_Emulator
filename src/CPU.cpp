#include "CPU.h"
//##########################################
CPU* CPU::m_CPU_6502InstancePtr = nullptr;

//##########################################

CPU::CPU(uint32_t executionSpeed) :
	m_DelayTimer(TIMER_MIN_VALUE, CHIP_8_TIMERS_DECREMENT_RATE_HZ),
	m_SoundDelayTimer(TIMER_MIN_VALUE, CHIP_8_TIMERS_DECREMENT_RATE_HZ),
	m_ProgramCounter(CHIP_8_PC_START_ADDRESS),
	m_IndexRegister(0), m_GeneralPurposeRegisters{0}, m_InstructionExecutionCyclePeriod(1000.0 / executionSpeed)
{
	std::random_device device;
	m_Generator = std::mt19937{ device() };
	std::thread delayTimerThread(&Timer::Decrement, &m_DelayTimer);
	delayTimerThread.detach();
}

CPU* CPU::GetInstance(uint32_t executionSpeed)
{
	if (m_CPU_6502InstancePtr == nullptr)
		m_CPU_6502InstancePtr = new CPU(executionSpeed);
	return m_CPU_6502InstancePtr;
}

uint16_t CPU::Fetch()
{
	uint16_t result = m_Memory.ReadWord(m_ProgramCounter);
	m_ProgramCounter += CHIP8_INSTRUCTION_WIDTH_IN_BYTES;
	return result;
}

void CPU::Execute(uint16_t instruction)
{
	InstructionOpCodes currentInstructionOPCode = static_cast<InstructionOpCodes>(ByteManipulator::ExtractNibble(instruction, 1));
	GeneralPurposeRegisterNames operandRegister1 = static_cast<GeneralPurposeRegisterNames>(ByteManipulator::ExtractNibble(instruction, 2));
	GeneralPurposeRegisterNames operandRegister2 = static_cast<GeneralPurposeRegisterNames>(ByteManipulator::ExtractNibble(instruction, 3));
	switch (currentInstructionOPCode)
	{
	case CLEAR_DISPLAY_AND_RETURN_PROCEDURE:
		if (ByteManipulator::ExtractNibble(instruction, 4) == 0x0)
		{
			m_Display.Clear();
		}
		else if (ByteManipulator::ExtractNibble(instruction, 4) == 0xE)
		{
			if (!ReturnFromSubroutine())
			{
				std::cout << "[CPU] stack empty failed to return from subroutine" << std::endl;
			}
		}
		break;
	case JUMP:
		JumpToAddress(ByteManipulator::Extract12LSBits(instruction));
		break;
	case CALL_SUBROUTINE:
		if (!CallSubRoutine(ByteManipulator::Extract12LSBits(instruction)))
		{
			std::cout << "[CPU] failed to call Subroutine with address " << std::hex <<
				(uint16_t)ByteManipulator::Extract12LSBits(instruction) << std::dec << std::endl;
		}
		break;
	case SKIP_CONDITIONAL_IMMEDIATE_EQUALITY:
		SkipInstructionOnCondition(m_GeneralPurposeRegisters[operandRegister1], ByteManipulator::ExtractByte(instruction , 2)
			, [](int x, int y) -> bool { return x == y; }
		);
		break;
	case SKIP_CONDITIONAL_IMMEDIATE_INEQUALITY:
		SkipInstructionOnCondition(m_GeneralPurposeRegisters[operandRegister1], ByteManipulator::ExtractByte(instruction, 2)
			, [](int x, int y) -> bool {return x != y; }
		);
		break;
	case SKIP_CONDITIONAL_REG_EQUALITY:
		SkipInstructionOnCondition(m_GeneralPurposeRegisters[operandRegister1], m_GeneralPurposeRegisters[operandRegister2]
			, [](int x, int y) -> bool {return x == y; }
		);
		break;
	case SKIP_CONDITIONAL_REG_INEQUALITY:
		SkipInstructionOnCondition(m_GeneralPurposeRegisters[operandRegister1], m_GeneralPurposeRegisters[operandRegister2]
			, [](int x, int y) -> bool {return x != y; }
		);
		break;
	case SET_REG_WITH_IMMEDIATE_VALUE:
		m_GeneralPurposeRegisters[operandRegister1] = ByteManipulator::ExtractByte(instruction, 2);
		break;
	case ADD_VALUE_TO_REG:
		m_GeneralPurposeRegisters[operandRegister1] += ByteManipulator::ExtractByte(instruction, 2);
		break;
	case SET_INDEX_REG:
		m_IndexRegister = ByteManipulator::Extract12LSBits(instruction);
		break;
	case JUMP_WITH_OFFSET:
		JumpToAddress(m_GeneralPurposeRegisters[V0] + ByteManipulator::Extract12LSBits(instruction));
		break;
	case GENERATE_RANDOM:
		m_GeneralPurposeRegisters[operandRegister1] = m_GeneratorDistribution(m_Generator) & ByteManipulator::ExtractByte(instruction, 2);
	break;
	case DISPLAY:
		DisplaySprite(m_IndexRegister, m_GeneralPurposeRegisters[operandRegister1], m_GeneralPurposeRegisters[operandRegister2],
			ByteManipulator::ExtractNibble(instruction, 4));
		break;
	case SKIP_IF_KEY:
		if (ByteManipulator::ExtractNibble(instruction, 4) == KeyPressed)
		{
			if (InputHandler::GetKeyState(static_cast<Chip8Key>(m_GeneralPurposeRegisters[operandRegister1])) == PRESSED)
			{
				JumpToAddress(m_ProgramCounter + CHIP8_INSTRUCTION_WIDTH_IN_BYTES);
			}
		}
		else if (ByteManipulator::ExtractNibble(instruction, 4) == KeyNotPressed)
		{
			if (InputHandler::GetKeyState(static_cast<Chip8Key>(m_GeneralPurposeRegisters[operandRegister1])) == NOT_PRESSED)
			{
				JumpToAddress(m_ProgramCounter + CHIP8_INSTRUCTION_WIDTH_IN_BYTES);
			}
		}
		break;
	case LOGICAL_AND_ARETHMETIC_INSTRUCTIONS:
		switch (ByteManipulator::ExtractNibble(instruction, 4))
		{
		case SET_REG_WITH_REG_VALUE:
			m_GeneralPurposeRegisters[operandRegister1] = m_GeneralPurposeRegisters[operandRegister2];
			break;
		case BITWISE_OR:
			m_GeneralPurposeRegisters[operandRegister1] |= m_GeneralPurposeRegisters[operandRegister2];
			break;
		case BITWISE_AND:
			m_GeneralPurposeRegisters[operandRegister1] &= m_GeneralPurposeRegisters[operandRegister2];
			break;
		case BITWISE_XOR:
			m_GeneralPurposeRegisters[operandRegister1] ^= m_GeneralPurposeRegisters[operandRegister2];
			break;
		case ADD_REGISTERS:
			AddRegisters(operandRegister1, operandRegister2);
			break;
		case SUBTRACT_REGISTERS_NORMAL:
			SubtractRegisters(operandRegister1, operandRegister2, false);
			break;
		case SHIFT_REGISTER_RIGHT:
			ShiftRegister(operandRegister1, operandRegister2, true);
			break;
		case SUBTRACT_REGISTERS_REVERSED:
			SubtractRegisters(operandRegister1, operandRegister2, true);
			break;
		case SHIFT_REGISTER_LEFT:
			ShiftRegister(operandRegister1, operandRegister2, false);
			break;
		}
		break;
	case MISC_INSTRUCTIONS:
		switch (ByteManipulator::ExtractByte(instruction, 2))
		{
		case READ_DELAY_TIMER_INTO_VX:
			m_GeneralPurposeRegisters[operandRegister1] = m_DelayTimer.GetTimerValue();
			break;
		case SET_DELAY_TIMER_WITH_VX:
			m_DelayTimer.SetTimerValue(m_GeneralPurposeRegisters[operandRegister1]);
			break;
		case SET_SOUND_TIMER_WITH_VX:
			m_SoundDelayTimer.SetTimerValue(m_GeneralPurposeRegisters[operandRegister1]);
			break;
		case ADD_VX_TO_I_REG:
			m_IndexRegister += m_GeneralPurposeRegisters[operandRegister1];
			break;
		case SET_I_REG_TO_CHAR_FONT_ADDRESS:
			m_IndexRegister = m_GeneralPurposeRegisters[operandRegister1] * CHARACTER_FONT_LENGTH + FONT_START_ADDRESS;
			break;
		case GET_KEYBOARD_KEY_PRESSED_BLOCKING:
			GetKeyPressed(operandRegister1);
			break;
		case SAVE_REGISTER_RANGE_TO_MEMORY:
			StoreRegistersToMemory(operandRegister1);
			break;
		case LOAD_REGISTER_RANGE_FROM_MEMORY:
			LoadRegistersFromMemory(operandRegister1);
			break;
		case CONVERT_VX_TO_BCD:
			ConvertToBCD(m_GeneralPurposeRegisters[operandRegister1]);
			break;
		}
		break;
	}
}

void CPU::JumpToAddress(uint16_t address)
{
	m_ProgramCounter = address;
}

bool CPU::CallSubRoutine(const uint16_t subroutineAddress)
{
	if (m_Memory.PushOntoStack(m_ProgramCounter) == STACK_OPERATION_SUCCESS)
	{
		JumpToAddress(subroutineAddress);
		return true;
	}
	return false;
}

bool CPU::ReturnFromSubroutine()
{
	uint16_t result;
	if (m_Memory.PopFromStack(result) == STACK_OPERATION_SUCCESS)
	{
		JumpToAddress(result);
		return true;
	}
	return false;
}

void CPU::SkipInstructionOnCondition(uint8_t value1, uint8_t value2, std::function<bool(uint8_t, uint8_t)> comparator)
{
	if (comparator(value1, value2))
	{
		JumpToAddress(m_ProgramCounter + CHIP8_INSTRUCTION_WIDTH_IN_BYTES);
	}
}

void CPU::AddRegisters(GeneralPurposeRegisterNames operandReg1, GeneralPurposeRegisterNames operandReg2)
{
	m_GeneralPurposeRegisters[VF] = NOT_RAISED;
	uint16_t result = static_cast<uint16_t>(m_GeneralPurposeRegisters[operandReg1]) + m_GeneralPurposeRegisters[operandReg2];
	if (result > 255)
	{
		m_GeneralPurposeRegisters[VF] = RAISED;
	}
	m_GeneralPurposeRegisters[operandReg1] = static_cast<uint8_t>(result);
}

void CPU::SubtractRegisters(GeneralPurposeRegisterNames operandReg1, GeneralPurposeRegisterNames operandReg2, bool reversed)
{
	uint8_t reg1 = m_GeneralPurposeRegisters[operandReg1];
	uint8_t reg2 = m_GeneralPurposeRegisters[operandReg2];
	if (reversed)
	{
		reg1 = m_GeneralPurposeRegisters[operandReg2];
		reg2 = m_GeneralPurposeRegisters[operandReg1];

	}
	m_GeneralPurposeRegisters[VF] = NOT_RAISED;
	if (reg1 > reg2)
	{
		m_GeneralPurposeRegisters[VF] = RAISED;
	}
	m_GeneralPurposeRegisters[operandReg1] = reg1 - reg2;
}

void CPU::ShiftRegister(GeneralPurposeRegisterNames operandReg1, GeneralPurposeRegisterNames operandReg2, bool right)
{
	uint8_t shifftedResult = m_GeneralPurposeRegisters[operandReg2] << 1, shifftedBitNumber = 1;
	if (right)
	{
		shifftedBitNumber = 8;
		shifftedResult = m_GeneralPurposeRegisters[operandReg2] >> 1;
	}
	m_GeneralPurposeRegisters[VF] = ByteManipulator::GetBit(m_GeneralPurposeRegisters[operandReg1], shifftedBitNumber);
	m_GeneralPurposeRegisters[operandReg1] = shifftedResult;

}

void CPU::DisplaySprite(uint16_t spriteStartAddress, uint8_t initialXCoordinate, uint8_t initialYCoordinate, uint8_t spriteLength)
{
	initialXCoordinate %= CHIP_8_DISPLAY_WIDTH_IN_PIXELS;
	initialYCoordinate %= CHIP_8_DISPLAY_LENGTH_IN_PIXELS;
	uint8_t currentSpriteByte;
	m_GeneralPurposeRegisters[VF] = NOT_RAISED;
	for (uint8_t currentRow = 0; currentRow < spriteLength; currentRow++)
	{
		currentSpriteByte = m_Memory.ReadHalfWord(spriteStartAddress + currentRow);
		for (uint8_t currentCol = 0; currentCol < SPRITE_WIDTH; currentCol++)
		{
			if (ByteManipulator::GetBit(currentSpriteByte,  currentCol + 1))
			{
				if (m_Display.GetPixelValue(initialXCoordinate + currentCol, initialYCoordinate) == Display::Chip8PixelState::ON)
				{
					m_GeneralPurposeRegisters[VF] = RAISED;
				}
				m_Display.TogglePixel(initialXCoordinate + currentCol, initialYCoordinate);
			}
		}
		++initialYCoordinate;
	}
}

void CPU::StoreRegistersToMemory(GeneralPurposeRegisterNames endRegister)
{
	for (uint8_t registerNumber = 0; registerNumber <= endRegister; ++registerNumber)
	{
		m_Memory.WriteHalfWord(m_IndexRegister + registerNumber, m_GeneralPurposeRegisters[registerNumber]);
	}
}

void CPU::LoadRegistersFromMemory(GeneralPurposeRegisterNames endRegsiter)
{
	for (uint8_t registerNumber = 0; registerNumber <= endRegsiter; ++registerNumber)
	{
		m_GeneralPurposeRegisters[registerNumber] = m_Memory.ReadHalfWord(m_IndexRegister + registerNumber);
	}
}

void CPU::ConvertToBCD(uint8_t valueToConvert)
{
	for (int8_t digitNumber = 2; digitNumber >= 0; --digitNumber)
	{
		m_Memory.WriteHalfWord(m_IndexRegister + digitNumber, valueToConvert % 10);
		valueToConvert /= 10;
	}
}

void CPU::GetKeyPressed(GeneralPurposeRegisterNames operandRegister)
{
	JumpToAddress(m_ProgramCounter - CHIP8_INSTRUCTION_WIDTH_IN_BYTES);
	if (InputHandler::GetLastKeyPressed() != KEY_NONE)
	{
		m_GeneralPurposeRegisters[operandRegister] = InputHandler::GetLastKeyPressed();
		JumpToAddress(m_ProgramCounter + CHIP8_INSTRUCTION_WIDTH_IN_BYTES);
	}
}


CPU::~CPU()
{
	delete m_CPU_6502InstancePtr;
}

void CPU::Run(const std::string& binaryFileName)
{
	Memory::MemoryReturnCode res = m_Memory.LoadMemory(binaryFileName);
	if (res == Memory::MEMORY_OVERFLOW)
	{
		std::cout << "[Memory Overflow] ROM size very large" << std::endl;
		return;
	}
	else if (res == Memory::INVALID_ROM_FILE_NAME)
	{
		std::cout << "[ROM] file name not found " << std::endl;
		return;
	}
	uint16_t currentInstruction = Fetch();
	while (true)
	{
		InputHandler::ProcessKeyboardInputs();
		if (InputHandler::GetKeyState(KEY_EXIT) == PRESSED)
		{
			break;
		}
		Execute(currentInstruction);
		currentInstruction = Fetch();
		SDL_Delay(m_InstructionExecutionCyclePeriod);
	}
}
