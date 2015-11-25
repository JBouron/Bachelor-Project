#include "../../headers/test_cases/bgsubstractor_tests.h"
#include "../../headers/debug.h"
#include "../../headers/frame_t.h"

namespace tmd {
    void BGSubstractorTest::setUp() {
        tmd::debug("BGSubstractorTest", "setUp", "creating local varaibles");
        m_video = new cv::VideoCapture;
        m_video->open("./res/tests/video_test.mkv");

        const double frameH = m_video->get(CV_CAP_PROP_FRAME_HEIGHT);
        const double frameW = m_video->get(CV_CAP_PROP_FRAME_WIDTH);
        const double frameCount = m_video->get(CV_CAP_PROP_FRAME_COUNT);
        tmd::debug("BGSubstractorTest", "setUp", "frameW = " + std::to_string
                (frameW) + " frameH = " + std::to_string(frameH));
        tmd::debug("BGSubstractorTest", "setUp", "frameCount = " +
                                                 std::to_string(frameCount));
        tmd::debug("BGSubstractorTest", "setUp", "exitting setUp method.");
    }

    void BGSubstractorTest::tearDown() {

    }

    void BGSubstractorTest::testInvalidVideoInput() {
        tmd::debug("BGSubstractorTest", "testInvalidVideoInput", "Begin test.");
        tmd::BGSubstractor s(NULL, 0);
    }

    void BGSubstractorTest::testInvalidCameraIndex1() {
        tmd::debug("BGSubstractorTest", "testInvalidCameraIndex1",
                   "Begin test.");
        tmd::BGSubstractor s(m_video, -1);
    }

    void BGSubstractorTest::testInvalidCameraIndex2() {
        tmd::debug("BGSubstractorTest", "testInvalidCameraIndex2", "Begin "
                "test.");
        tmd::BGSubstractor s(m_video, 8);
    }

    void BGSubstractorTest::testNextFrameReturnsNullWhenNoFramesLeft() {
        tmd::debug("BGSubstractorTest",
                   "testNextFrameThrowsExceptionWhenNoFramesLeft",
                   "Begin test.");
        tmd::BGSubstractor bgs(m_video, 0);
        bgs.jump_to_frame(147);
        CPPUNIT_ASSERT(bgs.next_frame() == NULL);
    }

    void BGSubstractorTest::testNextFrameConstructCorrectFramesCameraIndex() {
        tmd::debug("BGSubstractorTest",
                   "testNextFrameConstructCorrectFramesCameraIndex",
                   "Begin test.");
        tmd::BGSubstractor bgs(m_video, 0);
        frame_t *f = bgs.next_frame();
        while (f != NULL) {
            CPPUNIT_ASSERT(f->camera_index == 0);
            delete f;
            f = bgs.next_frame();
        }
    }

    void BGSubstractorTest::testNextFrameConstructCorrectFramesFrameIndex() {
        tmd::debug("BGSubstractorTest",
                   "testNextFrameConstructCorrectFramesFrameIndex",
                   "Begin test.");
        tmd::BGSubstractor bgs(m_video, 0);
        int findex = 0;
        frame_t *f = bgs.next_frame();
        int c  = 0;
        while (f != NULL) {
            c ++;
            CPPUNIT_ASSERT(f->frame_index == findex);
            findex++;
            delete f;
            f = bgs.next_frame();
        }
        tmd::debug("BGSubstractorTest",
                   "testNextFrameConstructCorrectFramesFrameIndex", "Total "
                           "frame count = " + std::to_string(c));
    }
}