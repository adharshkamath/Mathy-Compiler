#include <vector>
#include <string>

#ifndef FILES_H
#define FILES_H
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

    Files files;
} // namespace mathy

#endif