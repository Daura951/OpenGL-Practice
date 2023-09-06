#ifndef MATERIALREADER_H
#define MATERIALREADER_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<fstream>
#include<string>
#include<sstream>

using namespace glm;

	void ApplyMaterials(const char* materialFilePath, Shader shader)
	{
		string materialFileContents;
		ifstream file;

		file.exceptions(ifstream::failbit || ifstream::badbit);

		try
		{
			file.open(materialFilePath);
			stringstream data;
			data << file.rdbuf();
			file.close();
			materialFileContents = data.str();
			float materialData[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
			string token = "";
			int dataIndex = 0;
			for (int i = 0; i < materialFileContents.length(); i++)
			{
				if ((materialFileContents[i] >= '0' && materialFileContents[i] <= '9') || materialFileContents[i] == '.')
				{
					token += materialFileContents[i];
				}
				else
				{
					float floatToken = stof(token);
					token = "";
					materialData[dataIndex] = floatToken;
					dataIndex++;
				}
			}
			/*cout << "Ambient: " << materialData[0] <<", "<< materialData[1] <<", "<< materialData[2] << endl;
			cout << "Diffuse: " << materialData[3] <<", "<< materialData[4] << ", "<<materialData[5] << endl;
			cout << "Specular: " << materialData[6] <<", "<< materialData[7] << ", "<<materialData[8] << endl;
			cout << "Shininess: " << materialData[9] <<endl;
			cout << "-------------------------------------" <<endl;*/

			unsigned int vecLocation = glGetUniformLocation(shader.ID, "material.ambient");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(materialData[0], materialData[1], materialData[2])));


			vecLocation = glGetUniformLocation(shader.ID, "material.diffuse");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(materialData[3], materialData[4], materialData[5])));

			vecLocation = glGetUniformLocation(shader.ID, "material.specular");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(materialData[6], materialData[7], materialData[8])));


			vecLocation = glGetUniformLocation(shader.ID, "material.shininess");
			glUniform1f(vecLocation, materialData[9]);


			vecLocation = glGetUniformLocation(shader.ID, "light.ambient");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(0.2f, 0.2f, 0.2f)));

			vecLocation = glGetUniformLocation(shader.ID, "light.diffuse");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(0.5f, 0.5f, 0.5f)));

			vecLocation = glGetUniformLocation(shader.ID, "light.specular");
			glUniform3fv(vecLocation, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));


		}
#endif
