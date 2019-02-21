#pragma once

#include <map>
#include <iostream>
#include <fstream>

#include "Apollo/GL.h"
#include "Apollo/Core.h"
#include "Apollo/logger/Logger.h"

namespace Apollo
{
	class Shader
	{
	private:
		enum class ShaderObjectType
		{
			SHADER, PROGRAM
		};

		GLuint _programID;
		GLuint _vertexShader;
		GLuint _fragmentShader;

		std::map<const char*, int> _attributeMap;
		std::map<const char*, int> _uniformMap;

		bool _init;

		GLuint compileShader(const char* shaderSource, GLenum shaderType)
		{
			const char* shaderTypeString;
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				shaderTypeString = "GL_VERTEX_SHADER";
				break;
			case GL_FRAGMENT_SHADER:
				shaderTypeString = "GL_FRAGMENT_SHADER";
				break;
			case GL_GEOMETRY_SHADER:
				shaderTypeString = "GL_GEOMETRY_SHADER";
				break;
			default:
				shaderTypeString = "ERROR";
				AP_ENGINE_CRITICAL("Invalid shader type \"{}\" given when compiling shader.", shaderType);
				return -1;
			}

			GLuint shaderID = glCreateShader(shaderType);
			if (shaderID == 0)
			{
				AP_ENGINE_CRITICAL("Error creating shader. glCreateShader({}) returned 0.", shaderTypeString); // TODO: Handle this better?
				return -1;
			}

			glShaderSource(shaderID, 1, &shaderSource, nullptr);
			glCompileShader(shaderID);

			GLint shaderStatus;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderStatus);
			if (shaderStatus == GL_FALSE)
			{
				AP_ENGINE_CRITICAL("Shader copilation failed. Type: {} - Error: {}", shaderTypeString, getInfoLog(ShaderObjectType::SHADER, shaderID));
				return -1;
			}

			return shaderID;
		}

		void init(const char* vertexShaderSource, const char* fragmentShaderSource)
		{
			_programID = glCreateProgram();
			glUseProgram(_programID);

			_vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
			_fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

			glAttachShader(_programID, _vertexShader);
			glAttachShader(_programID, _fragmentShader);

			glLinkProgram(_programID);

			glDetachShader(_programID, _vertexShader);
			glDetachShader(_programID, _fragmentShader);

			GLint programLinkStatus = GL_FALSE;
			glGetProgramiv(_programID, GL_LINK_STATUS, &programLinkStatus);
			if (programLinkStatus == GL_FALSE)
			{
				AP_ENGINE_CRITICAL("Shader link failed. Error: {}", getInfoLog(ShaderObjectType::PROGRAM, _programID));
				return;
			}

			glValidateProgram(_programID);

			GLint programValidationStatus = GL_FALSE;
			glGetProgramiv(_programID, GL_VALIDATE_STATUS, &programValidationStatus);
			if (programValidationStatus == GL_FALSE)
			{
				AP_ENGINE_CRITICAL("Shader validation failed. Error: {}", getInfoLog(ShaderObjectType::PROGRAM, _programID));
				return;
			}

			_init = true;
		}

		std::string getInfoLog(ShaderObjectType type, GLuint id)
		{
			GLint infoLogLength;
			if (type == ShaderObjectType::SHADER)
			{
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			}
			else
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			}

			GLchar* infoLog = new GLchar[infoLogLength + 1];
			if (type == ShaderObjectType::SHADER)
			{
				glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog);
			}
			else
			{
				glGetProgramInfoLog(id, infoLogLength, nullptr, infoLog);
			}
			std::string infoLogString(infoLog);

			delete[] infoLog;

			return infoLogString;
		}

	public:
		Shader() : _init(false) {}

		~Shader() { glDeleteProgram(_programID); }

		inline void initFromString(const char* vertexShader, const char* fragmentShader)
		{
			init(vertexShader, fragmentShader);
		}

		inline void initFromFile(const char* vertexShaderLoc, const char* fragmentShaderLoc)
		{
			std::ifstream vertexFileStream(vertexShaderLoc);
			std::ifstream fragmentFileStream(fragmentShaderLoc);

			if (!vertexFileStream.is_open())
			{
				AP_ENGINE_CRITICAL("Could not open Vertex Shader File! Location: {}", vertexShaderLoc);
				return;
			}

			if (!fragmentFileStream.is_open())
			{
				AP_ENGINE_CRITICAL("Could not open Fragment Shader File! Location: {}", fragmentShaderLoc);
				return;
			}

			std::string vertexResult;
			std::string fragmentResult;
			std::string line;
			while (!vertexFileStream.eof())
			{
				std::getline(vertexFileStream, line);
				vertexResult.append(line + "\n");
			} vertexFileStream.close();
			while (!fragmentFileStream.eof())
			{
				std::getline(fragmentFileStream, line);
				fragmentResult.append(line + "\n");
			} fragmentFileStream.close();

			initFromString(vertexResult.c_str(), fragmentResult.c_str());
		}

		inline void use()
		{
			if (_init)
				glUseProgram(_programID);
			else
				AP_ENGINE_CRITICAL("Program must be initialized before being used.");
		}

		GLuint attribute(const char* attribName)
		{
			auto iter = _attributeMap.find(attribName);
			if (iter == _attributeMap.end())
			{
				GLint attribLookup = glGetAttribLocation(_programID, attribName);
				if (attribLookup == -1)
				{
					AP_ENGINE_CRITICAL("Could not find shader attribute: {}", attribName);
					return -1;
				}
				else
				{
					_attributeMap[attribName] = attribLookup;
					return attribLookup;
				}
			}
			return iter->second;
		}

		GLuint uniform(const char* uniformName)
		{
			auto iter = _uniformMap.find(uniformName);
			if (iter == _uniformMap.end())
			{
				GLint uniformLookup = glGetUniformLocation(_programID, uniformName);
				if (uniformLookup == -1)
				{
					AP_ENGINE_CRITICAL("Could not find shader uniform: {}", uniformName);
					return -1;
				}
				else
				{
					_uniformMap[uniformName] = uniformLookup;
					return uniformLookup;
				}
			}
			return iter->second;
		}
	};
}