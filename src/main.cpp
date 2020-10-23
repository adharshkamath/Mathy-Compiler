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
    std::string data_type = "float";
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        if (opt == 't') {
            if (strcmp(optarg, "float") == 0);
            else if (strcmp(optarg, "double") == 0) {
                data_type = optarg;
            } else if (strcmp(optarg, "long_double") == 0) {
                data_type = optarg;
            } else {
                std::cerr << "Invalid type " << optarg << " selected\n" << std::endl;
                exit(1);
            }
        } else {
            std::cerr << "Usage: " << argv[0] << " [-t type] file\n" << std::endl;
            exit(2);
        }
    }

    if (optind == argc) {
        std::cerr << "Enter the name of the file to be compiled!\n" << std::endl;
        std::cerr << "Usage: " << argv[0] << " [-t type] file\n" << std::endl;
        exit(3);
    }

    while (optind < argc) {
        compiler.m_scanner.files.names.push_back(argv[optind++]);
    }

    std::filebuf fbuff;
    std::string filename;
    compiler.m_scanner.files.next(&filename);
    if (fbuff.open(filename, std::ios::in)) {
        std::istream is(&fbuff);
        compiler.changeInput(&is);
    }
    int res = compiler.parse();
    std::cout << "Parse complete. Result = " << res << std::endl;
    fbuff.close();
    return res;
}
