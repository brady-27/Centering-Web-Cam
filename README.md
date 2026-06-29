# Centering Webcam

A face tracking webcam I built on a custom PCB that uses two servo motors to automatically keep your face centered on camera. It runs on an ESP32-S3 and uses Espressif's ESP-WHO library for face detection. When it detects a face, a PID controller figures out how far off center it is and moves the pan/tilt servos to correct it. Designed in KiCad, firmware written in C++ with PlatformIO.
