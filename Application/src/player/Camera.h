#pragma once

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp> 
#include <glm/gtx/intersect.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Shader.h"

static std::pair<glm::vec3, glm::vec3> TwoVec3s(glm::vec3 vecn1, glm::vec3 vecn2)
{
    return std::make_pair(vecn1, vecn2);
}

class Camera
{
public:
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 worldUp;
    
    float yaw;
    float pitch;
    float cameraSpeed = 0.0f;

public:
    Camera();
    ~Camera();

    std::pair<glm::vec3, glm::vec3> CastRay(float length);

    void Teleport(glm::vec3 position);

    void ResetModel();
    void Rotate(glm::vec2 const& Rotate);
    void ModelTransform(glm::vec3 translationVector);

    void SetCameraPosition(glm::vec3 camPos);
    void SetCameraFront(glm::vec3 camFront);
    void SetCameraUp(glm::vec3 camUp);
    void SetCameraSpeed(float deltaTime);
    void UpdateCameraRight();

    void UpdateCameraUp();
    void UpdateView();

    glm::mat4 ReturnUpdatedView();

    void MoveForward();
    void MoveBackward();
    void StrafeLeft();
    void StrafeRight();
    void MoveUpward();
    void MoveDownward();

	glm::mat4 GetProjection();
	glm::mat4 getView();
	glm::mat4 GetModel();
	glm::mat4 ReturnPVM();

    glm::vec3 GetCameraPosition();
    glm::vec3 GetCameraFront();
    glm::vec3 GetCameraUp();

};