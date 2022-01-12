#include "Scene.h"

#include <vector>
#include <algorithm>
/*! \brief Brief description.
*  Scene class is a container for loading all the game objects in your simulation or your game.
*
*/


Scene::Scene()
{
	// Set up your scene here......
	// Set a camera
	_camera = new Camera();
	// Don't start simulation yet
	_simulation_start = false;

	// Position of the light, in world-space
	_lightPosition = glm::vec3(10, 10, 0);

	// Create a game object
	//rope_obj = new Rope();
	
	for (int i = 0; i < 5; i++)
	{
		rope_objects.push_back(new Rope);
	}

	rope_obj = new Rope();

	// Create a game level object
	_level = new GameObject();

	//create iterator

	// Create the material for the game object- level
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour(glm::vec3(1,1,1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("assets/textures/diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_level->SetMaterial(modelMaterial);



	// The mesh is the geometry for the object
	Mesh *groundMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	groundMesh->LoadOBJ("assets/models/woodfloor.obj");
	// Tell the game object to use this mesh
	_level->SetMesh(groundMesh);
	_level->SetPosition(0.0f, 0.0f, 0.0f);
	_level->SetRotation(3.141590f, 0.0f, 0.0f);


	// Create the material for the game object- level
	Material *objectMaterial = new Material();
	Material *swordMat = new Material();
	// Shaders are now in files
	objectMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	objectMaterial->SetDiffuseColour(glm::vec3(1, 1, 1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	objectMaterial->SetTexture("assets/textures/pearl.bmp");
	swordMat->SetTexture("assets/textures/default.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	objectMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	// Set the geometry for the object
	Mesh *modelMesh = new Mesh();
	Mesh *swordMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	swordMesh->LoadOBJ("assets/models/swordSmall.obj");
	// Tell the game object to use this mesh

	for (float i = 0; i < rope_objects.size(); i++)
	{
		rope_objects[i]->SetMaterial(objectMaterial);
		rope_objects[i]->SetMesh(modelMesh);
		rope_objects[i]->SetPosition(glm::vec3(0.0f, (20.0f - i)-(i/5.0f), 0.0f)); 
		rope_objects[i]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		rope_objects[i]->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	}
	rope_obj->SetMaterial(objectMaterial);
	rope_obj->SetMesh(swordMesh);
	rope_obj->SetPosition(glm::vec3(-5.0f, 17.0f,0.0f));
	rope_obj->SetScale(glm::vec3(10.0f,10.0f,10.0f));
	rope_obj->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
}

Scene::~Scene()
{
	// You should neatly clean everything up here
	delete _level;
	delete _camera;

}

void Scene::Update(float deltaTs, Input* input)
{
	if (input->cmd_c)
	{
		std::cout << "C Pressed - move top ball" << std::endl;
		c_pressed = true;
	}
	if (input->cmd_z)
	{
		std::cout << "X Pressed - move sword... showing collision" << std::endl;
		z_pressed = true;
	}
	// Update the game object (this is currently hard-coded motion)
	if (input->cmd_x)
	{
		_simulation_start = true;
		for (int i = 0; i < rope_objects.size(); i++)
		{
			rope_objects[i]->StartSimulation(_simulation_start);
		}
		rope_obj->StartSimulation(_simulation_start);
	}
	for (int i = 0; i < rope_objects.size(); i++)
	{
		rope_objects[i]->Update(deltaTs, rope_objects, c_pressed, rope_obj, z_pressed);
	}
	rope_obj->Update(deltaTs, rope_objects, c_pressed,rope_obj, z_pressed);
	_level->Update(deltaTs);
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	for (int i = 0; i < rope_objects.size(); i++)
	{
		rope_objects[i]->Draw(_viewMatrix, _projMatrix);
	}
	rope_obj->Draw(_viewMatrix, _projMatrix);
	_level->Draw(_viewMatrix, _projMatrix);
}


