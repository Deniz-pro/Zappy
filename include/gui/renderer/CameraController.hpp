/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** CameraController.hpp
*/

#ifndef CAMERACONTROLLER_HPP_
    #define CAMERACONTROLLER_HPP_
    #include <raylib.h>

class CameraController {
    public:
        struct Params {
            float centerX;
            float centerZ;
            float distance;
        };

        explicit CameraController(const Params &params);
        void update();
        const Camera3D &getCamera() const;
    private:
        void handleZoom(float dt);
        void handleRotation(float dt);
        void clampValues();
        void applySpherical();
        Camera3D _camera;
        float _yaw;
        float _pitch;
        float _distance;
        float _centerX;
        float _centerZ;
};

#endif
