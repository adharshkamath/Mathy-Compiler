/*
    This file is the main file that instantiates the Compiler class.
    Once the output is generated, I'm using the 'indent' tool
    to format the code. It's only for neatness :)
*/

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <cstring>
#include "scanner.h"
#include "parser.hpp"
#include "compiler.h"
#include "files.h"

int main(int argc, char *argv[]) {
    mathy::Compiler compiler;
    int opt;
    extern bool success;
    while ((opt = getopt(argc, argv, "t:o:")) != -1) {
        if (opt == 't') {
            if (strcmp(optarg, "float") == 0);
            else if (strcmp(optarg, "double") == 0) {
                mathy::data_type = optarg;
            } else if (strcmp(optarg, "long_double") == 0) {
                mathy::data_type = "long double";
            } else {
                std::cerr << "Invalid type " << optarg << " selected\n" << std::endl;
                exit(1);
            }
        } else if (opt == 'o') {
            mathy::output_name = optarg;
        } else {
            std::cerr << "Usage: " << argv[0] << " [-t type] file(s)\n" << std::endl;
            exit(2);
        }
    }

    if (optind == argc) {
        std::cerr << "Enter the name of the file to be compiled!\n" << std::endl;
        std::cerr << "Usage: " << argv[0] << " [-t type] file(s)\n" << std::endl;
        exit(3);
    }

    while (optind < argc) {
        compiler.m_scanner.files.names.push_back(argv[optind++]);
        compiler.m_scanner.files.total_files_num++;
    }

    std::filebuf fbuff;
    std::string filename = compiler.m_scanner.files.names[0];
    compiler.m_scanner.files.current_file++;
    if (fbuff.open(filename, std::ios::in)) {
        std::istream is(&fbuff);
        compiler.changeInput(&is);
    } else {
        std::cerr << "File " << compiler.m_scanner.files.names[compiler.m_scanner.files.current_file - 1]
                  << " does not exist!" << std::endl;
        return -2;
    }
    int res = compiler.parse();
    fbuff.close();
    if (success) {
        std::string cmd = std::string("indent -linux -l120 -i4 -nut ") + mathy::output_name;
        const char *command = cmd.c_str();
        try {
            system(command);
            remove(std::string(mathy::output_name + "~").c_str());
        }
        catch (int error) {
            cout << "Code not pretty printed!" << endl << "indent not found" << endl << "Please install indent using : "
                 << endl << "sudo apt-get install indent" << endl;
        }
    }
    return res;
}
