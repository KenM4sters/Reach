#pragma once
#include "../Core.h"


/**
 * For this application, I've decided to integrate quite a robust PerspectiveCamera class
 * that doesn't really require much information passed the constructor. 
 * 
 * Once constructed, all you need to do it call the appropraite setter functions and the 
 * important variables will update themselves. Since only references are returned,
 * there should be no need to use any getter after its respective setter. 
 * 
 * A much lighter approach may leave setting and recalculating important variables to the user,
 * as opposed to storing slightly abstract variables like the screen dimensions in order to handle
 * certain functions itself. 
 * 
*/

// An enum of directions used to organise camera transformations.
enum TransformDirection {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACKWARD
};

// Since this is a fairly robust class, we need quite a few more abstract properties in order
// to handle events, such as user input, properly. 
struct CameraProps {
    float MovementSpeed = 1.0f;
    float RotationSpeed = 1.0f;
    float Zoom = 45.0f;
    uint32_t window_width, window_height;
    glm::vec3 Right     = {1.0f, 0.0f, 0.0f};
    glm::vec3 Up        = {0.0f, 1.0f, 0.0f};
    glm::vec3 Front     = {0.0f, 0.0f, 1.0f};
};

class PerspectiveCamera {
    public:
        PerspectiveCamera(glm::vec3& position, uint32_t& window_width, uint32_t& window_height);
        // Getters
        const glm::mat4& GetProjectionMatrix() const {return m_projectionMatrix;}
        const glm::mat4& GetViewMatrix() const {return m_viewMatrix;}
        const float& GetZoom() const {return m_props.Zoom;}
        inline glm::vec3& GetPosition() {return m_position;}
        inline glm::quat& GetRotation() {return m_quaternion;}
        // Setters
        void SetPosition(glm::vec3& position);
        // @param rotation_axis Specifies the desired axis around which the camera should rotate.
        // @param angle specifies the angle of rotation in DEGREES. 
        void SetRotation(glm::vec3& rotation_axis, float angle);
        void SetZoom(float& zoom);

        // The class will also handle input from the user itself.
        // @param direction glm::vec3 that holds the direction of the movement/rotation.
        // @param speed_up A boolean which, if set to TRUE, will speed up camera movement.
        void HandleUserInput(TransformDirection direction, bool speed_up);

    public:
    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::vec3 m_position;
        glm::quat m_quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
        CameraProps m_props;
};