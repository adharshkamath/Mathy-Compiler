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