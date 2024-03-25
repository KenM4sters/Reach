#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(glm::vec3& position, uint32_t& window_width, uint32_t& window_height)
    : m_position(position)
{
    m_props.window_width = window_width;
    m_props.window_height = window_height;
    m_projectionMatrix = glm::perspective(m_props.Zoom, (float)window_width / (float)window_height, 0.1f, 1000.0f);
}

void PerspectiveCamera::SetPosition(glm::vec3& position) 
{
    m_position = position;
    UpdateViewMatrix();
}

void PerspectiveCamera::SetRotation(glm::vec3& quaternion) 
{
    m_quarternion = quaternion;
    UpdateViewMatrix();
}

void PerspectiveCamera::SetZoom(float& zoom) 
{
    m_props.Zoom = zoom;
}

void PerspectiveCamera::UpdateViewMatrix() 
{
    m_viewMatrix = glm::lookAt(m_position, m_position + m_props.Front, m_props.Up);
}

void PerspectiveCamera::UpdateProjectionMatrix() 
{   
    m_projectionMatrix = glm::perspective(m_props.Zoom, (float)m_props.window_width / (float)m_props.window_height, 0.1f, 1000.0f);
}

void PerspectiveCamera::HandleUserInput(TransformDirection direction, bool speed_up) 
{
    float speed = 1.0f * ReachUtils::Time::GetDeltaTime();
    if(speed_up)
        speed *= 10;
    
    switch(direction) 
    {
        case TransformDirection::LEFT:
            m_position -= m_props.Right;
            break;
        case TransformDirection::RIGHT:
            m_position += m_props.Right;
            break;
        case TransformDirection::UP:
            m_position += m_props.Up;
            break;
        case TransformDirection::DOWN:
            m_position -= m_props.Up;
            break;
        case TransformDirection::FORWARD:
            m_position += m_props.Front;
            break;
        case TransformDirection::BACKWARD:
            m_position -= m_props.Front;
            break;
    }
    UpdateViewMatrix();
}