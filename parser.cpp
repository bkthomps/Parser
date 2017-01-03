/**
 * Bailey Thompson
 * Parser (1.0.2)
 * 28 November 2016
 * Info: Parses  assembly  file.  If there are no assembly errors, various types of operations are counted. If there are
 * Info: errors, the user is told what type of error, and what lines the errors are present on.
 */
#include <iostream>
#include <fstream>

struct Line {
    char arg1[20];
    int arg2;
    int arg3;
    int arg4;
    int lineType; //0=nothing; 1=code; 2=data; 3=label; 4+=operations
    int codeNum;
    int errorCode;
};

int main(int argc, char* argv[]) {
    //array of struct
    Line lines[100];
    //initializing
    for (int i = 0; i < 100; i++) {
        lines[i].arg2 = 0;
        lines[i].arg3 = 0;
        lines[i].arg4 = 0;
        lines[i].lineType = 0;
        lines[i].codeNum = 0;
        lines[i].errorCode = 0;
        for (int j = 0; j < 20; j++) {
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
    char buffer[100];
    int lineNum = 0;
    //extracting file data
    while (fin.getline(buffer, 100)) {
        int i = 0, arg1val = 0;
        bool isReg = false;
        while ((buffer[i] == ' ' || buffer[i] == '\t') && buffer[i] != '#' && buffer[i] != '\0') {
            i++;
        }
        while (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '#' && buffer[i] != '\0') {
            lines[lineNum].arg1[arg1val] = buffer[i];
            arg1val++;
            i++;
        }
        while ((buffer[i] == ' ' || buffer[i] == '\t') && buffer[i] != '#' && buffer[i] != '\0') {
            i++;
        }
        while (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '#' && buffer[i] != '\0') {
            if (buffer[i] == 'R' && isReg == false) {
                if (buffer[i + 1] != '0' && buffer[i + 1] != '1' && buffer[i + 1] != '2' && buffer[i + 1] != '3'
                        && buffer[i + 1] != '4' && buffer[i + 1] != '5' && buffer[i + 1] != '6'
                        && buffer[i + 1] != '7' && buffer[i + 1] != '8' && buffer[i + 1] != '9') {
                    lines[lineNum].errorCode = 7;
                }
                isReg = true;
            } else if (buffer[i] == '0' || buffer[i] == '1' || buffer[i] == '2' || buffer[i] == '3'
                       || buffer[i] == '4' || buffer[i] == '5' || buffer[i] == '6'
                       || buffer[i] == '7' || buffer[i] == '8' || buffer[i] == '9') {
                lines[lineNum].arg2 = lines[lineNum].arg2 * 10 + int(buffer[i]) - 48;
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = 9;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg2 *= -1;
            isReg = false;
        }
        while ((buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == ',') && buffer[i] != '#' && buffer[i] != '\0') {
            i++;
        }
        while (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '#' && buffer[i] != '\0') {
            if (buffer[i] == 'R' && isReg == false) {
                if (buffer[i + 1] != '0' && buffer[i + 1] != '1' && buffer[i + 1] != '2' && buffer[i + 1] != '3'
                        && buffer[i + 1] != '4' && buffer[i + 1] != '5' && buffer[i + 1] != '6'
                        && buffer[i + 1] != '7' && buffer[i + 1] != '8' && buffer[i + 1] != '9') {
                    lines[lineNum].errorCode = 7;
                }
                isReg = true;
            } else if (buffer[i] == '0' || buffer[i] == '1' || buffer[i] == '2' || buffer[i] == '3'
                       || buffer[i] == '4' || buffer[i] == '5' || buffer[i] == '6'
                       || buffer[i] == '7' || buffer[i] == '8' || buffer[i] == '9') {
                lines[lineNum].arg3 = lines[lineNum].arg3 * 10 + int(buffer[i]) - 48;
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = 9;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg3 *= -1;
            isReg = false;
        }
        while ((buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == ',') && buffer[i] != '#' && buffer[i] != '\0') {
            i++;
        }
        while (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '#' && buffer[i] != '\0') {
            if (buffer[i] == 'R' && isReg == false) {
                if (buffer[i + 1] != '0' && buffer[i + 1] != '1' && buffer[i + 1] != '2' && buffer[i + 1] != '3'
                        && buffer[i + 1] != '4' && buffer[i + 1] != '5' && buffer[i + 1] != '6'
                        && buffer[i + 1] != '7' && buffer[i + 1] != '8' && buffer[i + 1] != '9') {
                    lines[lineNum].errorCode = 7;
                }
                isReg = true;
            } else if (buffer[i] == '0' || buffer[i] == '1' || buffer[i] == '2' || buffer[i] == '3'
                       || buffer[i] == '4' || buffer[i] == '5' || buffer[i] == '6'
                       || buffer[i] == '7' || buffer[i] == '8' || buffer[i] == '9') {
                lines[lineNum].arg4 = lines[lineNum].arg4 * 10 + int(buffer[i]) - 48;
                if (buffer[i + 1] == '.') {
                    lines[lineNum].errorCode = 9;
                }
            }
            i++;
        }
        if (isReg) {
            lines[lineNum].arg4 *= -1;
            isReg = false;
        }
        while ((buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == ',') && buffer[i] != '#' && buffer[i] != '\0') {
            i++;
        }
        if (buffer[i] != '#' && buffer[i] != '\0') {
            lines[lineNum].errorCode = 1;
        }
        lineNum++;
    }
    for (int i = 0; i < 100; i++) {
        bool isLabel = false;
        for (int j = 0; j < 19; j++) {
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
            lines[i].errorCode = 2;
        }
    }
    //counting what the code value of the line is
    int code = -1;
    for (int i = 0; i < 100; i++) {
        if (lines[i].lineType == 1) {
            code = lines[i].arg2;
            lines[i].codeNum = code;
        } else if (lines[i].lineType >= 4) {
            lines[i].codeNum = code;
            if (code == -1) {
                lines[0].errorCode = 4;
            }
            code++;
        } else if (lines[i].lineType == 2 || lines[i].lineType == 3) {
            lines[i].codeNum = code;
        }
    }
    //error code for too many parameters
    for (int i = 0; i < 100; i++) {
        if (lines[i].errorCode == 0 && lines[i].lineType == 16 && (lines[i].arg3 != 0 || lines[i].arg4 != 0)) {
            lines[i].errorCode = 1;
        } else if ((lines[i].lineType >= 17 || lines[i].lineType <= 7) && lines[i].arg4 != 0
                   && lines[i].errorCode == 0) {
            lines[i].errorCode = 1;
        }
    }
    //error code for wrong operation
    for (int i = 0; i < 100; i++) {
        if (lines[i].errorCode == 0) {
            if (lines[i].lineType == 16 && lines[i].arg2 < 0) {
                lines[i].errorCode = 3;
            } else if (lines[i].lineType > 16 && (lines[i].arg2 > 0 || lines[i].arg3 < 0)) {
                lines[i].errorCode = 3;
            } else if ((lines[i].lineType == 8 || lines[i].lineType == 10 || lines[i].lineType == 12
                        || lines[i].lineType == 14) && (lines[i].arg2 > 0 || lines[i].arg3 > 0 || lines[i].arg4 > 0)) {
                lines[i].errorCode = 3;
            } else if ((lines[i].lineType == 9 || lines[i].lineType == 11 || lines[i].lineType == 13
                        || lines[i].lineType == 15) && (lines[i].arg2 > 0 || lines[i].arg3 < 0 || lines[i].arg4 > 0)) {
                lines[i].errorCode = 3;
            } else if (lines[i].lineType == 4 && lines[i].arg3 > 0) {
                lines[i].errorCode = 3;
            } else if (lines[i].lineType == 5 && (lines[i].arg2 < 0 || lines[i].arg3 > 0)) {
                lines[i].errorCode = 3;
            } else if (lines[i].lineType == 6 && lines[i].arg2 > 0) {
                lines[i].errorCode = 3;
            } else if (lines[i].lineType == 7 && lines[i].arg2 < 0) {
                lines[i].errorCode = 3;
            }
        }
    }
    //checking for missing error
    for (int i = 0; i < 100; i++) {
        if (lines[i].errorCode == 0) {
            if (lines[i].lineType >= 4 && lines[i].lineType <= 7 && (lines[i].arg2 == 0 || lines[i].arg3 == 0)) {
                lines[i].errorCode = 5;
            } else if (lines[i].lineType >= 17 && (lines[i].arg2 == 0 || lines[i].arg3 == 0)) {
                lines[i].errorCode = 5;
            } else if (lines[i].lineType == 16 && lines[i].arg2 == 0) {
                lines[i].errorCode = 5;
            } else if (lines[i].lineType >= 8 && lines[i].lineType <= 15 && (lines[i].arg2 == 0
                       || lines[i].arg3 == 0 || lines[i].arg4 == 0)) {
                lines[i].errorCode = 5;
            }
        }
    }
    //checking for duplicate error
    for (int i = 0; i < 100; i++) {
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
                    lines[i].errorCode = 6;
                }
            }
        }
    }
    bool doneOnce = false;
    for (int i = 0; i < 100; i++) {
        if (lines[i].lineType == 1 && !doneOnce) {
            doneOnce = true;
        } else if (lines[i].lineType == 1 && doneOnce) {
            lines[i].errorCode = 6;
        }
    }
    //checking for extra characters
    for (int i = 0; i < 100; i++) {
        if (lines[i].lineType == 3) {
            for (int il = 0; il < 20; il++) {
                if (lines[i].arg1[il] == '.' || lines[i].arg1[il] == '_') {
                    lines[i].errorCode = 8;
                }
            }
        }
    }
    //outputing errors
    for (int i = 0; i < 100; i++) {
        if (lines[i].errorCode == 1) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Extra." << std::endl;
        } else if (lines[i].errorCode == 2) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Opcode" << std::endl;
            std::cerr << "Error on line " << i + 2 << ": Invalid because of Opcode. "
                      << "(This error trails onto this line)" << std::endl;
        } else if (lines[i].errorCode == 3) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Operand." << std::endl;
        } else if (lines[i].errorCode == 4) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Directive." << std::endl;
        } else if (lines[i].errorCode == 5) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Missing." << std::endl;
        } else if (lines[i].errorCode == 6) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Duplicate." << std::endl;
        } else if (lines[i].errorCode == 7) {
            std::cerr << "Error on line " << i + 1 << ": Invalid." << std::endl;
        } else if (lines[i].errorCode == 8) {
            std::cerr << "Error on line " << i << ": Invalid character in label. "
                      << "(The next error trails back to this line)" << std::endl;
            std::cerr << "Error on line " << i + 1 << ": Invalid character in label." << std::endl;
        } else if (lines[i].errorCode == 9) {
            std::cerr << "Error on line " << i + 1 << ": Invalid because of Extra decimal." << std::endl;
        }
    }
    //calculating
    int load = 0, alu = 0, jump = 0;
    for (int i = 0; i < 100; i++) {
        if (lines[i].lineType == 4 || lines[i].lineType == 5 || lines[i].lineType == 6 || lines[i].lineType == 7) {
            load++;
        } else if (lines[i].lineType >= 16) {
            jump++;
        } else if (lines[i].lineType >= 8 && lines[i].lineType <= 15) {
            alu++;
        }
    }
    //outputing
    std::cout << "Total number of assembly instructions: " << load + alu + jump << std::endl;
    std::cout << "Number of Load/Store: " << load << std::endl;
    std::cout << "Number of ALU: " << alu << std::endl;
    std::cout << "Number of Compare/Jump: " << jump << std::endl;
    for (int i = 0; i < 100; i++) {
        if (lines[i].lineType == 3) {
            int c = 0;
            while (lines[i].arg1[c] != ':') {
                std::cout << lines[i].arg1[c];
                c++;
            }
            std::cout << ": " << lines[i].codeNum << std::endl;
        }
    }
    return 0;
}
