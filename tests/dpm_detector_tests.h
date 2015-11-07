#ifndef BACHELOR_PROJECT_DPM_DETECTOR_TESTS_H
#define BACHELOR_PROJECT_DPM_DETECTOR_TESTS_H

#include <opencv2/highgui/highgui.hpp>
#include "../headers/dpm_detector.h"
#include "../headers/debug.h"

namespace tmd{
    std::vector<tmd::player_t *> get_player_vector() {
        std::vector<tmd::player_t *> v;
        for (int i = 0; i < 5; i++) {
            v.push_back(new tmd::player_t);
            v[i]->original_image = cv::imread(
                    "/home/jbouron/EPFL/BA5/PlayfulVision/Bachelor-Project/misc/images/player" +
                    std::to_string(i) + ".jpg");
        }
        return v;

    }

    void show_body_parts(cv::Mat image, std::vector<cv::Rect> parts) {
        CvScalar color;
        color.val[0] = 255;
        color.val[1] = 0;
        color.val[2] = 255;
        color.val[3] = 255;
        const int thickness = 1;
        const int line_type = 8; // 8 connected line.
        const int shift = 0;
        for (int i = 0; i < parts.size(); i++) {
            CvRect r;
            r.x = parts[i].x;
            r.y = parts[i].y;
            r.width = parts[i].width;
            r.height = parts[i].height;
            cv::rectangle(image, r, color, thickness, line_type, shift);
        }
        cv::imshow("Body parts", image);
        cv::waitKey(0);
    }

    void test_dpm_class() {
        tmd::DPMDetector d(
                "/home/jbouron/EPFL/BA5/PlayfulVision/Bachelor-Project/misc/xmls/person.xml");
        std::vector<tmd::player_t *> v = get_player_vector();

        int64 start = cvGetTickCount();
        for (int i = 0; i < v.size(); i++) {
            tmd::debug("player " + std::to_string(i));
            d.extractBodyParts(v[i]);
            show_body_parts(v[i]->original_image, v[i]->features.body_parts);
        }
        int64 finish = cvGetTickCount();
        printf("total time = %.3f\n", (float) (finish - start) /
                                      (float) (cvGetTickFrequency() * 1000000.0));

    }
}
#endif //BACHELOR_PROJECT_DPM_DETECTOR_TESTS_H