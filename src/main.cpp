#include <Arduino.h>
#include "config.h"
#include "camera_handler.h"
#include "face_detector.h"
#include "pan_tilt_servos.h"
#include "pid_controller.h"

// main face tracking loop
// 1. get camera frame
// 2. detect face
// 3. figure out how far off center it is
// 4. pid calculates how much to move
// 5. move servos
// 6. repeat

FaceDetector faceDetector;
PanTiltServos servos(PAN_PWM_PIN, TILT_PWM_PIN);

// separate pid for left/right and up/down
PIDController panPID(PID_KP, PID_KI, PID_KD, -PID_MAX_OUTPUT, PID_MAX_OUTPUT);
PIDController tiltPID(PID_KP, PID_KI, PID_KD, -PID_MAX_OUTPUT, PID_MAX_OUTPUT);

unsigned long lastTime = 0;

// center pixel of the frame
int centerX = CAM_FRAME_WIDTH / 2;
int centerY = CAM_FRAME_HEIGHT / 2;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    Serial.println("booting face tracker...");

    // init camera - stop everything if it fails
    if (!camera_init()) {
        Serial.println("camera failed!! stopping.");
        while (1) { delay(1000); }
    }

    // move servos to center
    servos.begin();
    servos.center();

    Serial.println("ready! starting tracking loop");
    lastTime = micros();
}

void loop() {
    // calculate time since last loop (needed for pid)
    unsigned long now = micros();
    float dt = (now - lastTime) / 1000000.0;
    lastTime = now;

    // get a frame from the camera
    camera_fb_t *fb = camera_capture();
    if (!fb) {
        delay(LOOP_DELAY_MS);
        return;
    }

    // run face detection
    FaceBox face = faceDetector.detect(fb);

    if (face.detected) {
        // how many pixels is the face away from center
        float errorX = face.center_x - centerX;
        float errorY = face.center_y - centerY;

        // pid calculates correction
        float panMove  = panPID.update(errorX, dt);
        float tiltMove = tiltPID.update(errorY, dt);

        // move the servos
        // if camera moves wrong direction, flip the signs here
        servos.adjustPan(panMove);
        servos.adjustTilt(tiltMove);

#if DEBUG_PRINT_FACE
        Serial.print("face at: (");
        Serial.print(face.center_x);
        Serial.print(", ");
        Serial.print(face.center_y);
        Serial.print(") error: (");
        Serial.print(errorX);
        Serial.print(", ");
        Serial.print(errorY);
        Serial.println(")");
#endif

#if DEBUG_PRINT_PID
        Serial.print("servo pan=");
        Serial.print(servos.getPan());
        Serial.print(" tilt=");
        Serial.println(servos.getTilt());
#endif

    } else {
        // no face - reset pid so it doesnt freak out when face comes back
        panPID.reset();
        tiltPID.reset();

#if DEBUG_PRINT_FACE
        Serial.println("no face detected");
#endif
    }

    // give frame buffer back to camera driver
    esp_camera_fb_return(fb);

    delay(LOOP_DELAY_MS);
}
