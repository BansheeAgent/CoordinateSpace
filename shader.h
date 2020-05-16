//
// Created by Michael Walker on 5/8/2020.
//

#ifndef SHADERCLASS_SHADER_H
#define SHADERCLASS_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

///////////////////////////////////////////////////////////////////////////
/*
 * classes can make our life a bit easier when considering shaders. This
 * class will read shaders from disk, compiles and links them, checks for
 * errors and is easy to use. The shader class is created entirely in the
 * header file for learning purposes and portability.
 *
 * The shader class holds the ID of the shader program. Its constructor
 * requires the file paths of the source code of the vertex and fragment
 * shader respectively that we can store on disk as simple text files.
 */
///////////////////////////////////////////////////////////////////////////

class Shader
{
public:
  //the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);
  // use/activate the shader
  void use();
  //utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value)const;
  void checkCompileErrors(GLuint shader, std::string type);
  unsigned int getProgram();
};
#endif //SHADERCLASS_SHADER_H
