#ifndef ALI_H
#define ALI_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

const int MEMORY_SIZE = 256;
const int MIN_VALUE = -32768;
const int MAX_VALUE = 32767;

class ALI {
public:
    vector<int> memory;
    int accumulator;
    int dataRegister;
    int programCounter;
    int zeroBit;
    int overflowBit;
    map<string, int> symbolTable;
    int dataAddress;

    ALI();

    void updateZeroBit(int result);
    void setOverflow(int result);
    void printState(const string &instruction);
};

class Command {
public:
    virtual void execute(ALI &state) = 0;
    virtual ~Command() = default;
};

class DecInstruction : public Command {
private:
    string symbol;
public:
    explicit DecInstruction(const string &symbol);
    void execute(ALI &state) override;
};

class AddInstruction : public Command {
public:
    void execute(ALI &state) override;
};

class SubInstruction : public Command {
public:
    void execute(ALI &state) override;
};

class LdaInstruction : public Command {
private:
    int address;
public:
    explicit LdaInstruction(int address);
    void execute(ALI &state) override;
};

class LdiInstruction : public Command {
private:
    int value;
public:
    explicit LdiInstruction(int value);
    void execute(ALI &state) override;
};

class StrInstruction : public Command {
private:
    int address;
public:
    explicit StrInstruction(int address);
    void execute(ALI &state) override;
};

class XchInstruction : public Command {
public:
    void execute(ALI &state) override;
};

class JmpInstruction : public Command {
private:
    int address;
public:
    explicit JmpInstruction(int address);
    void execute(ALI &state) override;
};

class JzsInstruction : public Command {
private:
    int address;
public:
    explicit JzsInstruction(int address);
    void execute(ALI &state) override;
};

class HltInstruction : public Command {
public:
    void execute(ALI &state) override;

};

void loadProgram(const string &filename, vector<string> &instructions);
void executeInstruction(const string &instruction, ALI &state);
void commandLoop(ALI &state, const vector<string> &instructions);
void printAll(const ALI &state);

#endif
