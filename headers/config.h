#ifndef BACHELOR_PROJECT_CONFIG_H
#define BACHELOR_PROJECT_CONFIG_H

#include <libconfig.h++>
#include "../headers/debug.h"

#define TMD_CONFIG_FILE "./config.cfg"

namespace tmd{
    /**
     * Class representing the config of the program.
     */
    class Config{
    public:
        /**
         * Load the config from the config file.
         */
        static void load_config();

        /**
         * Here are all configs / parameters / values ...
         * They are public for ease of use.
         */

        /**********************************************************************/
        /* Misc                                                               */
        /**********************************************************************/
        static std::string mask_folder;
        static std::string model_file_path;

        /**********************************************************************/
        /* BGS                                                                */
        /**********************************************************************/
        static bool bgs_detect_shadows;
        static float bgs_threshold;
        static int bgs_history;
        static float bgs_learning_rate;

        /**********************************************************************/
        /* Calibration tool                                                   */
        /**********************************************************************/
        static int calibration_tool_escape_char;

        /**********************************************************************/
        /* DPM Detector                                                       */
        /**********************************************************************/
        static int dpm_detector_numthread;

        /**********************************************************************/
        /* DPM Player Extractor                                               */
        /**********************************************************************/
        static float dpm_extractor_score_threshold;
        static float dpm_extractor_overlapping_threshold;
        static float dpm_extractor_duplicate_area_threshold;

        /**********************************************************************/
        /* Features Comparator                                                */
        /**********************************************************************/
        static float features_comparator_correlation_threshold;

        /**********************************************************************/
        /* Features Extractor                                                 */
        /**********************************************************************/
        static float feature_extractor_threshold_red_low;
        static float feature_extractor_threshold_red_high;
        static float feature_extractor_threshold_green_low;
        static float feature_extractor_threshold_green_high;
        static float feature_extractor_threshold_saturation;
        static float feature_extractor_threshold_value;
        static int feature_extractor_histogram_size;

        /**********************************************************************/
        /* SDL Binds                                                          */
        /**********************************************************************/
        static int sdl_binds_default_width;
        static int sdl_binds_default_height;

        /**********************************************************************/
        /* Blob Player Extractor                                              */
        /**********************************************************************/
        static int blob_player_extractor_buffer_size;
        static int blob_player_extractor_min_blob_size;
    };
}

#endif //BACHELOR_PROJECT_CONFIG_H
