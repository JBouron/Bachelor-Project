#ifndef TEAM_MEMBERSHIP_DETECTOR_FRAME_T_H
#define TEAM_MEMBERSHIP_DETECTOR_FRAME_T_H

#include <opencv2/core/core.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "player_t.h"
#include "features_t.h"

namespace tmd {

    /* Structure frame_t.
     * The role of the frame_t structure is to hold relevant information of a frame
     * taken from the input video.
    */

    typedef struct {
        cv::Mat original_frame;         // Original frame taken from the video.
        double frame_index;             // Index of the frame in the video.
        cv::Mat mask_frame;             // Frame after applying background substraction.
        int camera_index;     // Index of the camera which took the frame.
        std::vector<tmd::player_t*> players; // Players on the frame.
    } frame_t;

    inline void free_frame(frame_t* frame){
        frame->original_frame.release();
        frame->mask_frame.release();
        for (size_t i = 0 ; i < frame->players.size() ; i ++){
            free_player(frame->players[i]);
        }
        delete frame;
    }

    /**
     * Create a 'colored mask' ie all pixel belonging to the foreground
     * are in color whereas pixels from the background are black.
     */
    inline cv::Mat get_colored_mask_for_frame(tmd::frame_t *frame) {
        cv::Mat resulting_image;
        frame->original_frame.copyTo(resulting_image);
        cv::Vec3b black;
        black.val[0] = 0;
        black.val[1] = 0;
        black.val[2] = 0;
        for (int c = 0; c < frame->mask_frame.cols; c++) {
            for (int r = 0; r < frame->mask_frame.rows; r++) {
                if (frame->mask_frame.at<uchar>(r, c) < 127) {
                    resulting_image.at<cv::Vec3b>(r, c) = black;
                }
            }
        }
        return resulting_image;
    }

    /**
     * Draw the players of the frame on another image and returns it.
     */
    inline cv::Mat draw_player_on_frame(tmd::frame_t *frame, bool draw_torso =
    false,
                                 bool draw_parts = false){
        cv::Mat result = frame->original_frame;

        std::vector<tmd::player_t*> &players = frame->players;

        CvScalar torso_color;
        torso_color.val[0] = 255;
        torso_color.val[1] = 255;
        torso_color.val[2] = 0;
        torso_color.val[3] = 255;
        CvScalar body_part_color;
        body_part_color.val[0] = 255;
        body_part_color.val[1] = 0;
        body_part_color.val[2] = 255;
        body_part_color.val[3] = 255;

        const int thickness = 1; // Thickness of the box.
        const int line_type = 8; // 8 connected line.
        const int shift = 0;

        for (player_t* p : players){
            // Due to overlapping we draw first the body parts, then the
            // torso and finally th box around the player.
            if (draw_parts){
                for (cv::Rect part : p->features.body_parts){
                    cv::Rect pos = part;
                    pos.x += p->pos_frame.x;
                    pos.y += p->pos_frame.y;
                    cv::rectangle(result, pos, body_part_color, thickness,
                                  line_type, shift);
                }
            }

            if (draw_torso){
                cv::Rect pos = p->features.torso_pos;
                pos.x += p->pos_frame.x;
                pos.y += p->pos_frame.y;
                cv::rectangle(result, pos, torso_color,
                              thickness, line_type, shift);
            }

            cv::rectangle(result, p->pos_frame, tmd::get_team_color(p->team),
                          thickness, line_type, shift);
        }
        return result;
    }
}

#endif //TEAM_MEMBERSHIP_DETECTOR_FRAME_T_H
