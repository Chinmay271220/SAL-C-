#include "ali.h"

using namespace std;

int main() {
    ALI state;
    vector<string> instructions;
    string filename;

    cout << "Enter the SAL program filename: ";
    cin >> filename;

    loadProgram(filename, instructions);
    commandLoop(state, instructions);

    //printAll(state);
    return 0;
}
