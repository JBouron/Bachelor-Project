#ifndef TEAM_MEMBERSHIP_DETECTOR_PLAYER_T_H
#define TEAM_MEMBERSHIP_DETECTOR_PLAYER_T_H

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "team_t.h"
#include "features_t.h"

namespace tmd{
    /* Structure representing a player. */

    typedef struct{
        cv::Mat original_image;     // The original image of the player (when cropped from the frame).
        cv::Mat mask_image;         // The result of applying BGS on the original image. I gives us the important pixels.
        double frame_index;         // Frame indice from which this player has been extracted.
        cv::Rect pos_frame;         // Position of the player on the frame.
        tmd::team_t team;           // Team of the player (TBD by the TeamDecider).
        float likelihood;           // Likelihood of the team membership.
        features_t features;
    }player_t;

    inline void free_player(player_t* player){
        /*player->original_image.release();
        player->mask_image.release();*/
        //free_features(&player->features);
        if (player != NULL){
            delete player;
        }
    }
}

#endif //TEAM_MEMBERSHIP_DETECTOR_PLAYER_T_H
