#pragma once

#include <GL/glew.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/intersect.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Shader.h"

static std::pair<glm::vec3, glm::vec3> TwoVec3s(glm::vec3 vecn1, glm::vec3 vecn2)
{
    return std::make_pair(vecn1, vecn2);
}


static std::string to_string(const glm::vec3& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

static std::string to_string(const glm::ivec3& v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

class Camera
{
private:
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, .0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float yaw;
    float pitch;
    float cameraSpeed = 0.0f;

    Shader* rayShader;
    Shader* worldShader;
public:
    Camera();
    ~Camera();

    std::pair<glm::vec3, glm::vec3> CastRay(float length);
    void RenderRay(glm::vec3 startPos, glm::vec3 endPos);
    void SetWorldShader(Shader* inWorldShader);
    void SetRayShader(Shader* inRayShader);

    void Teleport(glm::vec3 position);

    void ResetModel();
    void Rotate(glm::vec2 const& Rotate);
    void ModelTransform(glm::vec3 translationVector);

    void SetCameraSpeed(float deltaTime);
    void SetCameraFront(glm::vec3 camFront);
    void SetCameraRight();
    void SetCameraUp();
    void UpdateView();
    glm::mat4 ReturnUpdatedView();

    void MoveForward();
    void MoveBackward();
    void StrafeLeft();
    void StrafeRight();
    void MoveUpward();
    void MoveDownward();

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