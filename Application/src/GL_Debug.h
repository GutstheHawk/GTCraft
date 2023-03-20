#pragma once
#include <iostream>
#include <GL/glew.h>

void GLAPIENTRY
glDebugOutput(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	printf("Debug Callback Initialized.\n");
	printf("Debug message: id %d, %s\n", id, message);

	printf("Message Source: \n");
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				printf("API\n"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		printf("Window System\n"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	printf("Shader Compiler\n"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		printf("Third Party\n"); break;
	case GL_DEBUG_SOURCE_APPLICATION:		printf("Application\n"); break;
	case GL_DEBUG_SOURCE_OTHER:				printf("Other\n"); break;
	}

	printf("Error type: ");
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:					printf("Error\n"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		printf("Deprecated Behavior\n"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		printf("Undefined Behavior\n"); break;
	case GL_DEBUG_TYPE_PORTABILITY:				printf("Portability\n"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:				printf("Performance\n"); break;
	case GL_DEBUG_TYPE_MARKER:					printf("Marker\n"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:				printf("Push Group\n"); break;
	case GL_DEBUG_TYPE_POP_GROUP:				printf("Pop Group\n"); break;
	case GL_DEBUG_TYPE_OTHER:					printf("Other\n"); break;
	}

	printf("Severity: ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			printf("High\n\n"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:			printf("Medium\n\n"); break;
	case GL_DEBUG_SEVERITY_LOW:				printf("Low\n\n"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	printf("Notification\n\n"); break;
	}
}