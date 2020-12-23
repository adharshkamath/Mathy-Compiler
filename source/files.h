/*
    This file contains the definition of the class Files 
    which was meant to be used to store the details of all 
    the input files I haven't used this class effectively yet, 
    since multiple file input isn't handled properly.
*/

#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>

namespace mathy {
    class Files {
    public:
        std::vector <std::string> names;
        int total_files_num = -1, current_file = 0;

        Files() {}
    };
} // namespace mathy

#endif