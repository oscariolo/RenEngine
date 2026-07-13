#pragma once
#include <Engine.h>


class Brick : public PhysicsObject {
    public:
        Brick(float brickWidth, float brickHeight, float brickDepth);

    public:
        bool isBeingDestroyed = false;
        float destructionTimer = 0.0f;
        glm::vec3 originalScale = glm::vec3(1.0f, 1.0f, 1.0f);
        void killBrick();
        void reset();
        void update()override;
        void setScale(const glm::vec3& newScale) {
            originalScale = newScale;
            scale = newScale;
        }
    
    private:
        static Texture& getSharedTexture() {
            static Texture texture = [](){
                Texture t;
                t.loadTexture("assets/textures/wall.jpg");
                return t;
            }();
            return texture;
        }


};