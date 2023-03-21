#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

class Camera
{
private:
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;

    float cameraSpeed = 0.0f;
public:
    Camera();
    ~Camera();

    void ResetModel();
    void Rotate(glm::vec2 const& Rotate);
    void ModelTransform(glm::vec3 translationVector);

    void SetCameraSpeed(float deltaTime);
    void SetCameraFront(glm::vec3 camFront);

    void MoveForward();
    void MoveBackward();
    void StrafeLeft();
    void StrafeRight();
    void MoveUpward();
    void MoveDownward();

    void UpdateView();

	glm::mat4 ReturnProjection();
	glm::mat4 ReturnView();
	glm::mat4 ReturnModel();
	glm::mat4 ReturnPVM();

};

//glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
//{
//    //glm::pi<float>() * 0.25f
//    glm::mat4 Projection = glm::perspective(95.0f, 16.0f / 9.0f, 0.1f, 100.f);
//    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//    return Projection * View * Model;
//}