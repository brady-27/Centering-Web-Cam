#pragma once

#include "esp_camera.h"
#include "config.h"

// face detection using esp-who library
// the actual neural network stuff is all inside esp-who
// i just call infer() and it tells me where the face is
// reference: https://github.com/espressif/esp-who

#include "human_face_detect_msr01.hpp"

// stores info about where the face is in the frame
struct FaceBox {
    bool detected;
    int x, y;
    int width, height;
    int center_x, center_y;
};

class FaceDetector {
public:
    // these threshold values are from the esp-who example
    // 0.3 = how confident it needs to be before calling it a face
    FaceDetector() : _detector(0.3F, 0.3F, 10, 0.3F) {}

    FaceBox detect(camera_fb_t *fb) {
        FaceBox result;
        result.detected = false;

        if (!fb) {
            Serial.println("no frame to detect on");
            return result;
        }

        // run the detection - this is where the magic happens
        // infer() runs the neural network on the frame
        std::list<dl::detect::result_t> &candidates =
            _detector.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3});

        // nothing found
        if (candidates.empty()) {
            return result;
        }

        // just use the first face detected
        // could handle multiple faces later but keeping it simple for now
        auto &face = candidates.front();

        result.detected = true;
        result.x = face.box[0];
        result.y = face.box[1];
        result.width  = face.box[2] - face.box[0];
        result.height = face.box[3] - face.box[1];

        // find the center of the face box
        result.center_x = result.x + result.width / 2;
        result.center_y = result.y + result.height / 2;

        return result;
    }

private:
    HumanFaceDetectMSR01 _detector;
};
