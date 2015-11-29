#include <opencv2/highgui/highgui.hpp>
#include "../headers/features_extractor.h"
#include "../headers/debug.h"
#include "../headers/player_t.h"
#include "../headers/features_t.h"

namespace tmd {

    FeaturesExtractor::FeaturesExtractor(std::string model_file) : m_detector
                                                               (model_file) {
    }

    FeaturesExtractor::~FeaturesExtractor() {
        // TODO : Find why it segfualts.
        //delete m_detector;
    }

    void FeaturesExtractor::extractFeaturesFromPlayers(
            std::vector<player_t *> &players) {
        size_t size = players.size();
        size_t i = 0;
        for (i = 0; i < players.size(); i++) {
            extractFeatures(players[i]);
            if (players[i]->features.body_parts.size() == 0){
                tmd::debug("FeaturesExtractor", "extractFeaturesFromPlayers",
                           "Player " + std::to_string(i) + " has no body "
                                                                   "parts !");
                players.erase(players.begin()+i);
                i --;
            }
        }
    }

    void FeaturesExtractor::extractFeatures(player_t *player) {
        if (player == NULL) {
            throw std::invalid_argument("Error : Null pointer in "
                                        "FeaturesExtractor::extractFeatures()");
        }
        extractBodyParts(player);
        if (player->features.body_parts.size() > 0) {
            convertToHSV(player);
            updateMaskWithThreshold(player);
            createHistogram(player);
        }
    }

    void FeaturesExtractor::extractBodyParts(player_t *p) {
        m_detector.extractBodyParts(p);
        tmd::debug("FeaturesExtractor", "extractBodyParts", "Result : " +
                std::to_string(p->features.body_parts.size()) + "body parts.");
    }

    void FeaturesExtractor::convertToHSV(player_t *p) {
        cv::Mat hsv_tmp;
        cvtColor(p->features.torso, hsv_tmp, CV_BGR2HSV);
        p->features.torso = hsv_tmp;
    }

    bool FeaturesExtractor::withinThresholds(double h, double s, double v){
        float th_red_low = TMD_FEATURE_EXTRACTOR_TH_RED_LOW;
        float th_red_high = TMD_FEATURE_EXTRACTOR_TH_RED_HIGH;
        float th_green_low = TMD_FEATURE_EXTRACTOR_TH_GREEN_LOW;
        float th_green_high = TMD_FEATURE_EXTRACTOR_TH_GREEN_HIGH;
        float th_sat_low = TMD_FEATURE_EXTRACTOR_TH_SATURATION_LOW;
        float th_val_low = TMD_FEATURE_EXTRACTOR_TH_VALUE_LOW;

        return ((th_red_low < h && h <= 180) || (0 <= h && h <= th_red_high) ||
                (th_green_low <= h && h <= th_green_high)) &&
               (th_sat_low <= s) && (th_val_low <= v);

    }

    void FeaturesExtractor::updateMaskWithThreshold(player_t *p) {
        cv::Mat img = p->features.torso;
        int cols = img.cols;
        int rows = img.rows;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cv::Vec3b color = img.at<cv::Vec3b>(i, j);
                bool in_mask = p->features.torso_mask.at<uchar>(i,j) > 127;
                if (withinThresholds(color[0], color[1], color[2]) && in_mask){
                    p->features.torso_mask.at<uchar>(i,j) = 255;
                }
                else{
                    p->features.torso_mask.at<uchar>(i,j) = 0;
                }
            }
        }
    }

    void FeaturesExtractor::createHistogram(player_t *p) {
        int bins_count = TMD_FEATURE_EXTRACTOR_HISTOGRAM_SIZE;
        int dim = 1; // One dimension : The hue.
        float **range = new float *[1];
        range[0] = new float[2];
        range[0][0] = 0;
        range[0][1] = static_cast<float>(180);
        bool uniform = true;  // Make the histogram uniform.
        bool accumulate = false;
        std::vector<cv::Mat> imagechannels;
        cv::split(p->features.torso, imagechannels);
        cv::Mat images[] = {imagechannels[0]};
        std::vector<cv::Mat> maskchannels;
        cv::split(p->features.torso_mask, maskchannels);
        /*cv::imshow("Debug mask", maskchannels[0]);
        cv::waitKey(0);*/
        tmd::debug("FeaturesExtractor", "createHistogram",
                   "p->features.torso.channels() = " +
                           std::to_string(p->features.torso.channels()));
        tmd::debug("FeaturesExtractor", "createHistogram",
                   "p->features.torso_mask.channels() = " +
                           std::to_string(p->features.torso_mask.channels()));
        cv::calcHist(&images[0], 1, 0, maskchannels[0],
                     p->features.torso_color_histogram, dim, &bins_count,
                     (const float **) range,
                     uniform,
                     accumulate);
        cv::Mat histCpy = p->features.torso_color_histogram.clone();
        normalize(histCpy, p->features
                          .torso_color_histogram, 0, 1.0, cv::NORM_MINMAX, -1);
    }
}