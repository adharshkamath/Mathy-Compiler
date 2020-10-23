#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>

namespace mathy {
    class Files {
    public:
        std::vector <std::string> names;
        int total_files_num = 1, current_file = 0;

        Files() {}

        explicit Files(const std::vector <std::string> &files) : names(files), total_files_num(files.size()),
                                                                 current_file(0) {}

        int next(std::string *name) {
            if (current_file == total_files_num) {
                return -1;
            } else {
                *name = names[current_file];
                current_file++;
                return 0;
            }
        }
    };
} // namespace mathy

#endif