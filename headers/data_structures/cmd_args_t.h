#ifndef BACHELOR_PROJECT_CMD_ARGS_T_H
#define BACHELOR_PROJECT_CMD_ARGS_T_H

#include <string>

namespace tmd{

    /**
     * Structure holding the command line arguments given to the program.
     */
    typedef struct{
        std::string video_folder = "./";
        int camera_index = 0;
        bool show_results = false;
        bool save_results = false;
        std::string save_folder = "./";
        bool show_torsos = false;
        int s = 0;
        int e = std::numeric_limits<int>::max();
        int j = 1;
        int t = 1;
        int b = 1;
    }cmd_args_t;
}

#endif //BACHELOR_PROJECT_CMD_ARGS_T_H