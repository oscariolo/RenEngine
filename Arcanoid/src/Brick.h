#pragma once
#include <Engine.h>


class Brick : public PhysicsObject {
    public:
        Brick(float brickWidth, float brickHeight, float brickDepth, int hitPoints = 1);

    public:
        bool isBeingDestroyed = false;
        float destructionTimer = 0.0f;
        glm::vec3 originalScale = glm::vec3(1.0f, 1.0f, 1.0f);
        
        int maxHitPoints = 1; // Maximum hit points for the brick
        int currentHitPoints = 1; // Current hit points for the brick
        float hitCooldown = 0.0f; // Cooldown time in seconds
        
        bool hit(); //True if the brick was hit and is still alive, false if it was destroyed

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

    static glm::vec3 GetColorForHitPoints(int hitPoints);

    void UpdateColor();


};