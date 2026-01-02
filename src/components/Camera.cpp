#include <raylib.h>
#include <memory>
#include <raymath.h>
#include "../entity/common/EntityPosition.h"
#include "../config.h"

class CameraComponent
{
private:
    Camera2D camera;

public:
    CameraComponent(Camera2D camera) : camera(camera) {};
    ~CameraComponent() = default;

    static std::unique_ptr<CameraComponent> create(Vector2 position, Rectangle rectangle)
    {
        Camera2D camera2d;
        camera2d.zoom = 1.0f;
        camera2d.rotation = 0.0f;
        camera2d.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f};
        Vector2 cameraTarget = Vector2{
            rectangle.x + rectangle.width / 2.0f,
            rectangle.y + rectangle.height / 2.0f};

        return std::make_unique<CameraComponent>(camera2d);
    }

    Camera2D getCamera()
    {
        return camera;
    }

    void update(Vector2 position, Rectangle rectangle)
    {
        Vector2 cameraTarget = Vector2{
            rectangle.x + rectangle.width / 2,
            rectangle.y + rectangle.height - SPRITEH_FRAME_HEIGHT / 4};
        camera.target = Vector2Lerp(camera.target, cameraTarget, 5.0f * GetFrameTime());

        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

        if (camera.zoom > 3.0f)
            camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f)
            camera.zoom = 0.1f;
    }
};
