#pragma once

#include "esp_camera.h"
#include "config.h"

// sets up the ov2640 camera with all the right pins
// returns true if it worked, false if something broke

bool camera_init() {
    camera_config_t config;

    // had to look up what ledc channel/timer meant
    // basically just tells it which pwm channel to use for xclk
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;

    // all the data pins
    config.pin_d0 = CAM_PIN_D0;
    config.pin_d1 = CAM_PIN_D1;
    config.pin_d2 = CAM_PIN_D2;
    config.pin_d3 = CAM_PIN_D3;
    config.pin_d4 = CAM_PIN_D4;
    config.pin_d5 = CAM_PIN_D5;
    config.pin_d6 = CAM_PIN_D6;
    config.pin_d7 = CAM_PIN_D7;

    // sync and clock pins
    config.pin_xclk  = CAM_PIN_XCLK;
    config.pin_pclk  = CAM_PIN_PCLK;
    config.pin_vsync = CAM_PIN_VSYNC;
    config.pin_href  = CAM_PIN_HREF;

    // i2c for camera settings
    config.pin_sccb_sda = CAM_PIN_SDA;
    config.pin_sccb_scl = CAM_PIN_SCL;

    // not using these on my pcb
    config.pin_pwdn  = CAM_PIN_PWDN;
    config.pin_reset = CAM_PIN_RESET;

    // 20mhz clock for the camera
    config.xclk_freq_hz = 20000000;

    // rgb565 format needed for face detection
    config.pixel_format = PIXFORMAT_RGB565;

    // qqvga (160x120) - small enough to fit in ram without psram
    config.frame_size = FRAMESIZE_QQVGA;

    config.jpeg_quality = 12;
    config.fb_count = 1;           // only 1 buffer, no psram
    config.fb_location = CAMERA_FB_IN_DRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.print("camera init failed, error: ");
        Serial.println(err, HEX);
        return false;
    }

    // tweak some camera settings
    sensor_t *s = esp_camera_sensor_get();
    if (s != nullptr) {
        s->set_brightness(s, 0);
        s->set_contrast(s, 0);
        s->set_saturation(s, 0);
        s->set_whitebal(s, 1);
        s->set_awb_gain(s, 1);
        s->set_exposure_ctrl(s, 1);
        s->set_hmirror(s, 0);
        s->set_vflip(s, 0);
    }

    Serial.println("camera initialized ok");
    return true;
}

// grab a frame from the camera
// remember to call esp_camera_fb_return(fb) when done
camera_fb_t* camera_capture() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("couldnt get frame from camera");
    }
    return fb;
}
