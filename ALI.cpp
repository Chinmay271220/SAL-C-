#include "ali.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

ALI::ALI() 
    : memory(MEMORY_SIZE, 0), 
      accumulator(0), 
      dataRegister(0), 
      programCounter(0), 
      zeroBit(0), 
      overflowBit(0), 
      dataAddress(128) {}

void ALI::updateZeroBit(int result) {
    zeroBit = (result == 0) ? 1 : 0;
}

void ALI::setOverflow(int result) {
    overflowBit = (result < MIN_VALUE || result > MAX_VALUE) ? 1 : 0;
}

void ALI::printState(const string &instruction) {
    cout << "Instruction: " << instruction << "\n"
              << "PC: " << programCounter 
              << ", A: " << accumulator 
              << ", B: " << dataRegister 
              << ", Zero: " << zeroBit 
              << ", Overflow: " << overflowBit 
              << "\n";
}

DecInstruction::DecInstruction(const string &symbol) : symbol(symbol) {}

void DecInstruction::execute(ALI &state) {
    state.symbolTable[symbol] = state.dataAddress++;
}

void AddInstruction::execute(ALI &state) {
    int result = state.accumulator + state.dataRegister;
    state.setOverflow(result);
    state.accumulator = result;
    state.updateZeroBit(state.accumulator);
}

void SubInstruction::execute(ALI &state) {
    int result = state.accumulator - state.dataRegister;
    state.setOverflow(result);
    state.accumulator = result;
    state.updateZeroBit(state.accumulator);
}

LdaInstruction::LdaInstruction(int address) : address(address) {}

void LdaInstruction::execute(ALI &state) {
    state.accumulator = state.memory[address];
}

LdiInstruction::LdiInstruction(int value) : value(value) {}

void LdiInstruction::execute(ALI &state) {
    state.accumulator = value;
}

StrInstruction::StrInstruction(int address) : address(address) {}

void StrInstruction::execute(ALI &state) {
    if (state.accumulator < MIN_VALUE || state.accumulator > MAX_VALUE) {
        cerr << "Error: Out-of-bounds value " << state.accumulator 
                  << " at address " << address << ".\n";
    } else {
        state.memory[address] = state.accumulator;
    }
}

void XchInstruction::execute(ALI &state) {
    swap(state.accumulator, state.dataRegister);
}

JmpInstruction::JmpInstruction(int address) : address(address) {}

void JmpInstruction::execute(ALI &state) {
    if (address < 0 || address >= 128) {
        cerr << "Error: JMP address out of bounds.\n";
        return;
    }
    state.programCounter = address - 1;
}

JzsInstruction::JzsInstruction(int address) : address(address) {}

void JzsInstruction::execute(ALI &state) {
    if (state.zeroBit == 1) {
        state.programCounter = address - 1;
    }
}

void HltInstruction::execute(ALI &state) {
    cout << "Program halted.\n";
    printAll(state);
    exit(0);
}

void loadProgram(const string &filename, vector<string> &instructions) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        instructions.push_back(line);
    }

    if (instructions.size() > 128) {
        cerr << "Error: Program exceeds 128 instructions. Truncating.\n";
        instructions.resize(128);
    }
}

void executeInstruction(const string &instruction, ALI &state) {
    istringstream iss(instruction);
    string opcode, operand;
    iss >> opcode >> operand;

    if (opcode == "DEC") {
        DecInstruction(operand).execute(state);
    } else if (opcode == "LDA") {
        LdaInstruction(state.symbolTable[operand]).execute(state);
    } else if (opcode == "LDI") {
        LdiInstruction(stoi(operand)).execute(state);
    } else if (opcode == "STR") {
        StrInstruction(state.symbolTable[operand]).execute(state);
    } else if (opcode == "ADD") {
        AddInstruction().execute(state);
    } else if (opcode == "SUB") {
        SubInstruction().execute(state);
    } else if (opcode == "XCH") {
        XchInstruction().execute(state);
    } else if (opcode == "JMP") {
        JmpInstruction(stoi(operand)).execute(state);
    } else if (opcode == "JZS") {
        JzsInstruction(stoi(operand)).execute(state);
    } else if (opcode == "HLT") {
        HltInstruction().execute(state);
    } else {
        cerr << "Unknown opcode: " << opcode << "\n";
    }
    state.printState(instruction);
}

void commandLoop(ALI &state, const vector<string> &instructions) {
    int instructionCount = 0;        // Count of executed instructions
    const int instructionLimit = 1000; // Maximum instruction count

    while (true) {
        // Prompt user for a command
        cout << "Enter command (s - step, a - execute all, q - quit): ";
        char command;
        cin >> command;

        if (command == 's') { // Step through a single instruction
            if (state.programCounter >= instructions.size()) {
                cout << "Error: Program counter exceeded program size. Halting execution.\n";
                break;
            }
            cout << "Executing instruction at PC: " << state.programCounter << "\n";
            executeInstruction(instructions[state.programCounter], state);
            state.programCounter++; // Move to the next instruction
            instructionCount++;

        } else if (command == 'a') { // Execute all instructions
            while (state.programCounter < instructions.size()) {
                if (state.programCounter >= 128) {
                    cout << "Error: Program counter exceeded legal range (0-127). Halting execution.\n";
                    break;
                }

                cout << "Executing instruction at PC: " << state.programCounter << "\n";
                executeInstruction(instructions[state.programCounter], state);
                state.programCounter++;
                instructionCount++;

                if (instructionCount >= instructionLimit) { // Check for instruction limit
                    printAll(state);
                    cout << "1,000 instructions executed. Continue? (y/n): ";
                    char choice;
                    cin >> choice;
                    if (choice != 'y') {
                        return; // Exit the loop if the user chooses not to continue
                    }
                    instructionCount = 0; // Reset instruction count after confirmation
                }
            }
            //printAll(state); // Print the machine state after each step

        } else if (command == 'q') { // Quit the program
            printAll(state); // Print the final state
            break;

        } else { // Handle invalid input
            cout << "Invalid command. Please enter 's', 'a', or 'q'.\n";
        }
    }
}

void printAll(const ALI &state) {
    cout << "--- Machine State ---\n";
    cout << "Accumulator: " << state.accumulator << "\n";
    cout << "Data Register: " << state.dataRegister << "\n";
    cout << "Program Counter: " << state.programCounter << "\n";
    cout << "Zero Bit: " << state.zeroBit << "\n";

    cout << "\n--- Data Memory ---\n";
    for (const auto &entry : state.symbolTable) {
        cout << "  " << entry.first << " (Address: " << entry.second 
                  << ") = " << state.memory[entry.second] << "\n";
    }
    cout << "--- End of State ---\n";
}
