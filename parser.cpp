/**
 * Bailey Thompson
 * Parser (1.0.3)
 * 4 February 2017
 * Info: Parses  assembly  file.  If there are no assembly errors, various types of operations are counted. If there are
 * Info: errors, the user is told what type of error, and what lines the errors are present on.
 */
#include <iostream>
#include <fstream>

const int CHARACTERS_PER_LINE = 100;
const int OPERATIONS_PER_LINE = 20;

enum ErrorCode {
    NO_ERROR, EXTRA, OPCODE, OPERAND, DIRECTIVE, MISSING, DUPLICATE, INVALID, LABEL, EXTRA_DECIMAL
};

struct Line {
    char arg1[OPERATIONS_PER_LINE];
    int arg2;
    int arg3;
    int arg4;
    int lineType; //0=nothing; 1=code; 2=data; 3=label; 4+=operations
    int codeNum;
    ErrorCode errorCode;
};

bool isNumber(const char input) {
    return input >= '0' && input <= '9';
}

bool isUnimportantCharacter(const char input) {
    return input == ' ' || input == '\t' || input == '#' || input == '\0';
}

bool isOnlyWhitespace(const char input) {
    return (input == ' ' || input == '\t') && input != '#' && input != '\0';
}

bool isOnlyWhitespaceOrComma(const char input) {
    return (input == ' ' || input == '\t' || input == ',') && input != '#' && input != '\0';
}

bool isTooManyParameters(const Line input) {
    return (input.errorCode == NO_ERROR && input.lineType == 16 && (input.arg3 != 0 || input.arg4 != 0))
           || ((input.lineType >= 17 || input.lineType <= 7) && input.arg4 != 0 && input.errorCode == NO_ERROR);
}

bool isWrongOperation(const Line input) {
    return (input.lineType == 16 && input.arg2 < 0) || (input.lineType > 16 && (input.arg2 > 0 || input.arg3 < 0))
           || ((input.lineType == 8 || input.lineType == 10 || input.lineType == 12 || input.lineType == 14)
               && (input.arg2 > 0 || input.arg3 > 0 || input.arg4 > 0))
           || ((input.lineType == 9 || input.lineType == 11 || input.lineType == 13 || input.lineType == 15)
               && (input.arg2 > 0 || input.arg3 < 0 || input.arg4 > 0)) || (input.lineType == 4 && input.arg3 > 0)
           || (input.lineType == 5 && (input.arg2 < 0 || input.arg3 > 0)) || (input.lineType == 6 && input.arg2 > 0)
           || (input.lineType == 7 && input.arg2 < 0);
}

bool isErrorMissing(const Line input) {
    return (input.lineType >= 4 && input.lineType <= 7 && (input.arg2 == 0 || input.arg3 == 0))
           || (input.lineType >= 8 && input.lineType <= 15 && (input.arg2 == 0 || input.arg3 == 0 || input.arg4 == 0))
           || (input.lineType >= 17 && (input.arg2 == 0 || input.arg3 == 0))
           || (input.lineType == 16 && input.arg2 == 0);
}

void errorOutput(const Line* input) {
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        switch (input[i].errorCode) {
            case EXTRA:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Extra." << std::endl;
                break;
            case OPCODE:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Opcode" << std::endl;
                std::cerr << "Error on line " << i + 2 << ": Invalid because of Opcode. "
                          << "(This error trails onto this line)" << std::endl;
                break;
            case OPERAND:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Operand." << std::endl;
                break;
            case DIRECTIVE:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Directive." << std::endl;
                break;
            case MISSING:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Missing." << std::endl;
                break;
            case DUPLICATE:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Duplicate." << std::endl;
                break;
            case INVALID:
                std::cerr << "Error on line " << i + 1 << ": Invalid." << std::endl;
                break;
            case LABEL:
                std::cerr << "Error on line " << i << ": Invalid character in label. "
                          << "(The next error trails back to this line)" << std::endl;
                std::cerr << "Error on line " << i + 1 << ": Invalid character in label." << std::endl;
                break;
            case EXTRA_DECIMAL:
                std::cerr << "Error on line " << i + 1 << ": Invalid because of Extra decimal." << std::endl;
                break;
        }
    }
    return;
}

void outputCommandStatistics(const Line* input) {
    int load = 0, alu = 0, jump = 0;
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (input[i].lineType == 4 || input[i].lineType == 5 || input[i].lineType == 6 || input[i].lineType == 7) {
            load++;
        } else if (input[i].lineType >= 16) {
            jump++;
        } else if (input[i].lineType >= 8 && input[i].lineType <= 15) {
            alu++;
        }
    }

    std::cout << "Total number of assembly instructions: " << load + alu + jump << std::endl;
    std::cout << "Number of Load/Store: " << load << std::endl;
    std::cout << "Number of ALU: " << alu << std::endl;
    std::cout << "Number of Compare/Jump: " << jump << std::endl;
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (input[i].lineType == 3) {
            int c = 0;
            while (input[i].arg1[c] != ':') {
                std::cout << input[i].arg1[c];
                c++;
            }
            std::cout << ": " << input[i].codeNum << std::endl;
        }
    }
    return;
}

int main(int argc, char* argv[]) {
    //array of struct
    Line lines[CHARACTERS_PER_LINE];
    //initializing
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        lines[i].arg2 = 0;
        lines[i].arg3 = 0;
        lines[i].arg4 = 0;
        lines[i].lineType = 0;
        lines[i].codeNum = 0;
        lines[i].errorCode = NO_ERROR;
        for (int j = 0; j < OPERATIONS_PER_LINE; j++) {
            lines[i].arg1[j] = ' ';
        }
    }
    //checking for invalid amount of arguments
    if (argc != 2) {
        std::cerr << "Error: invalid arguments" << std::endl;
        return -1;
    }
    //attempting to open file
    std::ifstream fin;
    fin.open(argv[1]);
    //if file cannot open
    if (fin.fail()) {
        std::cerr << "Error: invalid file" << std::endl;
        return -1;
    }
    //character buffer
    char buffer[CHARACTERS_PER_LINE];
    int lineNum = 0;
    //extracting file data
    while (fin.getline(buffer, CHARACTERS_PER_LINE)) {
        int i = 0, arg1val = 0;
        bool isReg = false;
        while (isOnlyWhitespace(buffer[i])) {
            i++;
        }
        while (!isUnimportantCharacter(buffer[i])) {
            lines[lineNum].arg1[arg1val] = buffer[i];
            arg1val++;
            i++;
        }
        while (isOnlyWhitespace(buffer[i])) {
            i++;
        }
        while (!isUnimportantCharacter(buffer[i])) {
            if (buffer[i] == 'R' && isReg == false) {
                if (!isNumber(buffer[i + 1])) {
                    lines[lineNum].errorCode = INVALID;
                }
                isReg = true;
            } else if (isNumber(buffer[i])) {
                lines[lineNum].arg2 = lines[lineNum].arg2 * 10 + int(buffer[i]) - '0';
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = EXTRA_DECIMAL;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg2 *= -1;
            isReg = false;
        }
        while (isOnlyWhitespaceOrComma(buffer[i])) {
            i++;
        }
        while (!isUnimportantCharacter(buffer[i])) {
            if (buffer[i] == 'R' && isReg == false) {
                if (!isNumber(buffer[i + 1])) {
                    lines[lineNum].errorCode = INVALID;
                }
                isReg = true;
            } else if (isNumber(buffer[i])) {
                lines[lineNum].arg3 = lines[lineNum].arg3 * 10 + int(buffer[i]) - '0';
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = EXTRA_DECIMAL;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg3 *= -1;
            isReg = false;
        }
        while (isOnlyWhitespaceOrComma(buffer[i])) {
            i++;
        }
        while (!isUnimportantCharacter(buffer[i])) {
            if (buffer[i] == 'R' && isReg == false) {
                if (!isNumber(buffer[i + 1])) {
                    lines[lineNum].errorCode = INVALID;
                }
                isReg = true;
            } else if (isNumber(buffer[i])) {
                lines[lineNum].arg4 = lines[lineNum].arg4 * 10 + int(buffer[i]) - '0';
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = EXTRA_DECIMAL;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg4 *= -1;
            isReg = false;
        }
        while (isOnlyWhitespaceOrComma(buffer[i])) {
            i++;
        }
        if (buffer[i] != '#' && buffer[i] != '\0') {
            lines[lineNum].errorCode = EXTRA;
        }
        lineNum++;
    }

    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        bool isLabel = false;
        for (int j = 0; j < OPERATIONS_PER_LINE - 1; j++) {
            if (lines[i].arg1[j] == ':' && lines[i].arg1[j + 1] == ' ') {
                isLabel = true;
            }
        }
        if (lines[i].arg1[0] == 'C' && lines[i].arg1[1] == 'o' && lines[i].arg1[2] == 'd' && lines[i].arg1[3] == 'e'
                && lines[i].arg1[4] == ':') {
            lines[i].lineType = 1;
        } else if (lines[i].arg1[0] == 'D' && lines[i].arg1[1] == 'a' && lines[i].arg1[2] == 't'
                   && lines[i].arg1[3] == 'a' && lines[i].arg1[4] == ':') {
            lines[i].lineType = 2;
        } else if (isLabel) {
            lines[i].lineType = 3;
        } else if (lines[i].arg1[0] == 'L' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == ' ') {
            lines[i].lineType = 4;
        } else if (lines[i].arg1[0] == 'L' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == 'i'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 5;
        } else if (lines[i].arg1[0] == 'S' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == ' ') {
            lines[i].lineType = 6;
        } else if (lines[i].arg1[0] == 'S' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == 'i'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 7;
        } else if (lines[i].arg1[0] == 'A' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == 'D'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 8;
        } else if (lines[i].arg1[0] == 'A' && lines[i].arg1[1] == 'D' && lines[i].arg1[2] == 'D'
                   && lines[i].arg1[3] == 'i' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 9;
        } else if (lines[i].arg1[0] == 'S' && lines[i].arg1[1] == 'U' && lines[i].arg1[2] == 'B'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 10;
        } else if (lines[i].arg1[0] == 'S' && lines[i].arg1[1] == 'U' && lines[i].arg1[2] == 'B'
                   && lines[i].arg1[3] == 'i' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 11;
        } else if (lines[i].arg1[0] == 'M' && lines[i].arg1[1] == 'U' && lines[i].arg1[2] == 'L'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 12;
        } else if (lines[i].arg1[0] == 'M' && lines[i].arg1[1] == 'U' && lines[i].arg1[2] == 'L'
                   && lines[i].arg1[3] == 'i' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 13;
        } else if (lines[i].arg1[0] == 'D' && lines[i].arg1[1] == 'I' && lines[i].arg1[2] == 'V'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 14;
        } else if (lines[i].arg1[0] == 'D' && lines[i].arg1[1] == 'I' && lines[i].arg1[2] == 'V'
                   && lines[i].arg1[3] == 'i' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 15;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'M' && lines[i].arg1[2] == 'P'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 16;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'Z' && lines[i].arg1[2] == ' ') {
            lines[i].lineType = 17;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'N' && lines[i].arg1[2] == 'Z'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 18;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'G' && lines[i].arg1[2] == 'Z'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 19;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'G' && lines[i].arg1[2] == 'E'
                   && lines[i].arg1[3] == 'Z' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 20;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'L' && lines[i].arg1[2] == 'Z'
                   && lines[i].arg1[3] == ' ') {
            lines[i].lineType = 21;
        } else if (lines[i].arg1[0] == 'J' && lines[i].arg1[1] == 'L' && lines[i].arg1[2] == 'E'
                   && lines[i].arg1[3] == 'Z' && lines[i].arg1[4] == ' ') {
            lines[i].lineType = 22;
        } else if (lines[i].arg1[0] != ' ' || lines[i].arg2 != 0 || lines[i].arg3 != 0 || lines[i].arg4 != 0) {
            lines[i].errorCode = OPCODE;
        }
    }

    //counting what the code value of the line is
    int code = -1;
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].lineType == 1) {
            code = lines[i].arg2;
            lines[i].codeNum = code;
        } else if (lines[i].lineType >= 4) {
            lines[i].codeNum = code;
            if (code == -1) {
                lines[0].errorCode = DIRECTIVE;
            }
            code++;
        } else if (lines[i].lineType == 2 || lines[i].lineType == 3) {
            lines[i].codeNum = code;
        }
    }

    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (isTooManyParameters(lines[i])) {
            lines[i].errorCode = EXTRA;
        }
    }

    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].errorCode == NO_ERROR && isWrongOperation(lines[i])) {
            lines[i].errorCode = OPERAND;
        }
    }

    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].errorCode == NO_ERROR && isErrorMissing(lines[i])) {
            lines[i].errorCode = MISSING;
        }
    }

    //checking for duplicate error
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].lineType == 3) {
            for (int p = 0; p < i; p++) {
                bool same = true;
                int h = 0;
                while (lines[i].arg1[h] != ':' && lines[p].arg1[h] != ':') {
                    if (lines[i].arg1[h] != lines[p].arg1[h]) {
                        same = false;
                    }
                    h++;
                }
                if (same) {
                    lines[i].errorCode = DUPLICATE;
                }
            }
        }
    }
    bool doneOnce = false;
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].lineType == 1 && !doneOnce) {
            doneOnce = true;
        } else if (lines[i].lineType == 1 && doneOnce) {
            lines[i].errorCode = DUPLICATE;
        }
    }

    //checking for extra characters
    for (int i = 0; i < CHARACTERS_PER_LINE; i++) {
        if (lines[i].lineType == 3) {
            for (int j = 0; j < OPERATIONS_PER_LINE; j++) {
                if (lines[i].arg1[j] == '.' || lines[i].arg1[j] == '_') {
                    lines[i].errorCode = LABEL;
                }
            }
        }
    }

    errorOutput(lines);
    outputCommandStatistics(lines);
    return 0;
}
