#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../include/u-gine.h"
#define FULLSCREEN false
#define CAMSPEED 50
#define ROTSPEED 0.1

#define PI 3.1415926535
#define HALFPI 1.57079632675

//void moveCam()
//{
//}

int main(int, char**) {
	// Creamos contexto de OpenGL (ventana o pantalla completa)
	if ( FULLSCREEN )	Screen::Instance()->Open(Screen::Instance()->GetDesktopWidth(), Screen::Instance()->GetDesktopHeight(), true);
	else				Screen::Instance()->Open(800, 600, false);

	// Inicializamos estados para renderizado 3D
	Renderer::Instance()->Setup3D();

	// Creamos arrays de vertices e indices
	//std::vector<Vertex> vertices = {
	//	Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.f, 0.f)),
	//	Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.f, 0.f)),
	//	Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.f, 1.f)),
	//	Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.f, 1.f))
	//};
	//std::vector<uint16> indices = { 0, 1, 2, 0, 2, 3 };	
	//std::shared_ptr<Mesh> mesh = ResourceManager::Instance()->LoadMesh("data/simple-dm5.msh.xml");
	//std::shared_ptr<Model> model = Model::Create(mesh);
	//model->GetPosition() = glm::vec3(0, 0, 100.0f);
	//Scene::Instance()->AddEntity(Model::Create(mesh));

	// Creamos buffers en memoria de video donde copiar los arrays
	// NOTA: Estos metodos no hacen nada en la implementacion actual del Renderer.
	// Es necesario implementar el soporte de VBOs.
	//std::shared_ptr<Texture> texture = Texture::Create("data/eq2_weap_spawnlite.jpg");
	//std::shared_ptr<Submesh> spawnlite = Submesh::Create(texture);
	//std::shared_ptr<Mesh> mesh = Mesh::Create("data/simple-dm5.msh.xml");
	//uint32 vbo = Renderer::Instance()->CreateBuffer();
	//uint32 ibo = Renderer::Instance()->CreateBuffer();
	//Renderer::Instance()->SetVertexBufferData(vbo, &vertices[0], sizeof(Vertex) * vertices.size());
	//Renderer::Instance()->SetIndexBufferData(ibo, &indices[0], sizeof(uint16) * indices.size());
	//Renderer::Instance()->SetTexture(texture.get()->GetHandle());
	//for (int i = 0; i < vertices.size(); ++i)
	//{
	//	spawnlite.get()->AddVertex(vertices[i]);
	//}
	//for (int i = 0; i < indices.size(); i += 3) 
	//{
	//	spawnlite.get()->AddTriangle(indices[i], indices[i + 1], indices[i + 2]);
	//}

	//spawnlite.get()->Rebuild();
	// Establecemos las matrices de proyeccion y vista
	//std::shared_ptr<Mesh> bunnyMesh = Mesh::Create("data/dwarf.msh.xml");
	//bunnyMesh->GetSubmesh(0)->SetColor(glm::vec3(1.0f, 1.f, 0.4f));
	//bunnyMesh->GetSubmesh(0)->SetShininess(255);

	//Scene::Instance()->SetAmbient(glm::vec3(0.0f));
	std::shared_ptr<Mesh>suzMesh = ResourceManager::Instance()->LoadMesh("data/suzanne_refract.msh.xml");
	suzMesh->GetSubmesh(0)->SetShininess(124);
	std::shared_ptr<Model> suzanne = Model::Create(suzMesh);
	suzanne->GetPosition() = glm::vec3(-3.0f, 0.f, 0.f);
	std::shared_ptr<Model> teapot = Model::Create(ResourceManager::Instance()->LoadMesh("data/teapot_reflect.msh.xml"));
	teapot->GetPosition() = glm::vec3(3.0f, 0.f, 0.f);
	std::shared_ptr<Model> skybox = Model::Create(ResourceManager::Instance()->LoadMesh("data/skybox.msh.xml"));
	skybox->GetScale() = glm::vec3(100.0f, 100.0f, 100.0f);
	std::shared_ptr<Model> cube = Model::Create(ResourceManager::Instance()->LoadMesh("data/cube.msh.xml"));
	cube->GetPosition() = glm::vec3(0.0f, 0.0f, 0.0f);
	
	
	Scene::Instance()->AddEntity(suzanne);
	Scene::Instance()->AddEntity(teapot);
	Scene::Instance()->AddEntity(skybox);
	Scene::Instance()->AddEntity(cube);
	
	
	//std::shared_ptr<Model> bunny = Model::Create(bunnyMesh);
	//bunny->Animate(true);
	//bunny->SetFPS(60);
	glm::vec3 axisX(1, 0, 0);
	glm::vec3 axisY(0, 1, 0);
	glm::vec3 axisZ(0, 0, 1);
	//bunny->GetRotation() = glm::rotate(glm::quat(), float(HALFPI), axisX);
	//bunny->GetRotation() = glm::rotate<float>(glm::quat(1, 0, 0, 0), 90.0f, axisX);
	//Scene::Instance()->AddEntity(bunny);

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), float(Screen::Instance()->GetWidth()) / Screen::Instance()->GetHeight(), 1.f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 100.0f, -2.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//POINT WHITE LIGHT
	std::shared_ptr<Light> DLight = Light::Create();
	DLight->SetType(Light::Type::POINT);
	DLight->SetColor(glm::vec3(1.0f));
	DLight->SetAttenuation(0.25f);
	DLight->GetPosition() = glm::vec3(1.0f);
	Scene::Instance()->AddEntity(DLight);


	//MAIN CAMERA
	std::shared_ptr<Camera> cam = Camera::Create();
	cam.get()->SetProjection(proj);
	cam.get()->SetUsesTarget(true);
	cam->GetTarget() = glm::vec3(0.f, 0.f, 0.0f);
	cam.get()->SetColor(glm::vec3(0.5f));
	cam->GetPosition() = glm::vec3(0.0f, 0.0f, -6.f); 
	skybox->GetPosition() = cam->GetPosition();
	DLight->GetPosition() = cam->GetPosition();

	cam.get()->SetViewport(0, 0, Screen::Instance()->GetWidth(), Screen::Instance()->GetHeight());
	float dist = 0.5f *Screen::Instance()->GetWidth() * glm::tan(glm::radians(45.f)); //(800/2) (Screenwidth/2)
	Scene::Instance()->AddEntity(cam);

	//RED POINT LIGHT
	//std::shared_ptr<Light> RLight1 = Light::Create();
	//RLight1->SetType(Light::Type::POINT);
	//RLight1->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
	//RLight1->SetAttenuation(0.1f);
	//RLight1->GetPosition() = glm::vec3(5.0f, 0.1f, 0.f);
	//Scene::Instance()->AddEntity(RLight1);


	
	
	float angle = PI * 0.25;
	glm::mat4 Rot;
	int speedMX = 0, speedMY = 0;
	float yaw = 0, pitch = 0;
	glm::quat qyaw, qpitch;
	// Bucle principal. Se ejecuta mientras la ventana esté abierta y no pulsemos ESC
	while ( !Screen::Instance()->ShouldClose() && !Screen::Instance()->IsKeyPressed(GLFW_KEY_ESCAPE) ) {
		// Establecemos la matriz modelo
		Screen::Instance()->MoveMouse(Screen::Instance()->GetWidth() / 2, Screen::Instance()->GetHeight() / 2);
		angle += 0.25 * PI * Screen::Instance()->GetElapsed();		
		//Rot = glm::rotate<float>(Rot, angle, axis);
		//angle = 0;
		cam->GetPosition() = glm::vec3(0.0f, 0.f, -6.f) * glm::rotate(glm::quat(), angle, axisY);
		//speedMX = (Screen::Instance()->GetMouseX() - Screen::Instance()->GetWidth() / 2);
		//speedMY = (Screen::Instance()->GetMouseY() - Screen::Instance()->GetHeight() / 2);
		//yaw += -speedMX * Screen::Instance()->GetElapsed() * ROTSPEED;
		//pitch += -speedMY * Screen::Instance()->GetElapsed() * ROTSPEED;

		if (pitch > HALFPI) pitch = HALFPI;
		if (pitch < -HALFPI) pitch = -HALFPI;

		//Screen::Instance()->SetTitle(std::string("yaw: " + std::to_string(yaw) + ", pitch: " + std::to_string(pitch)));
    //qyaw = glm::rotate(glm::quat(), yaw, axisY);
    //qpitch = glm::rotate(glm::quat(), pitch, axisX);
		//qyaw = glm::rotate<float>(glm::quat(1, 0, 0, 0), -yaw, axisY);
		//qpitch = glm::rotate<float>(glm::quat(1, 0, 0, 0), pitch, qyaw * axisX);
		//yaw = ROTSPEED * atan2(speedMY, dist);
		//pitch = ROTSPEED * atan2(-speedMX, dist);
		//Rot = glm::rotate<float>(glm::mat4(), pitch, axisX);
		//Rot = glm::rotate<float>(Rot, yaw, axisY);

		cam->GetRotation() = glm::quat(glm::vec3(pitch, yaw, 0.f));
		
		//cam.get()->GetRotation() = glm::quat(Rot); /*qyaw*qpitch* glm::conjugate(qyaw*qpitch)*/;

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_W)) 
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(0, 0, -1));
		}

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_A))
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(-1, 0, 0));
		}

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_S))
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(0, 0, 1));
		}

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_D))
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(1, 0, 0));
		}

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_Q))
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(0, 1, 0));
		}

		if (Screen::Instance()->IsKeyPressed(GLFW_KEY_E))
		{
			cam.get()->Move(CAMSPEED * Screen::Instance()->GetElapsed() * glm::vec3(0, -1, 0));
		}

		skybox->GetPosition() = cam->GetPosition();
		DLight->GetPosition() = cam->GetPosition();
    //ROTACION EULER

    //glm::vec3 camEuler;
    //camEuler += glm::vec3(
    //  -speedMY * Screen::Instance()->GetElapsed() * CAMSPEED,
    //  -speedMX * Screen::Instance()->GetElapsed() * CAMSPEED,
    //  0);
    //cam->getRotation() = glm::quat(camEuler);

		//glm:mat4 Rot
		//Rot = glm::rotate<float>(Rot, static_cast<float>(speedMY) * Screen::Instance()->GetElapsed(), glm::vec3(1, 0, 0));
		//Rot = glm::rotate<float>(Rot, static_cast<float>(speedMX) * Screen::Instance()->GetElapsed(), glm::vec3((Rot * glm::vec4(glm::vec3(0, 1, 0),1))));
		//cam.get()->GetRotation() = glm::quat(Rot);
		//Renderer::Instance()->SetMatrices(Rot, view, proj);
		//if (Screen::Instance()->IsKeyPressed(5));

		//cam.get()->Move();
		// Limpiamos color y depth buffer
		//Renderer::Instance()->ClearColorBuffer(glm::vec3(0.2f, 0.6f, 1.0f));
		//Renderer::Instance()->ClearDepthBuffer();

		// Dibujamos los buffers
		// NOTA: La linea comentada es la que habra que utilizar cuando implementemos el soporte de VBOs.
		// La linea utilizada es una version temporal que podemos eliminar una vez implementemos los VBOs.
		//Renderer::Instance()->DrawBuffers(vbo, ibo, indices.size());
		//Renderer::Instance()->DrawBuffers(reinterpret_cast<uint32>(&vertices[0]), reinterpret_cast<uint32>(&indices[0]), indices.size());
		Scene::Instance()->Update(Screen::Instance()->GetElapsed());
		Scene::Instance()->Render();
		
		//mesh.get()->Render();
		// Refrescamos la pantalla
		Screen::Instance()->Refresh();

		//We return the mouse to the centre of the screen

		

		// Escribimos en el titulo de la ventana cualquier posible error que se pueda haber producido
		// al compilar los shaders
		Screen::Instance()->SetTitle(Renderer::Instance()->GetProgramError());
		//Screen::Instance()->SetTitle(std::to_string(cam->GetPosition().x) + " " + std::to_string(cam->GetPosition().y) + " " + std::to_string(cam->GetPosition().z));
		//std::cout << Renderer::Instance()->GetProgramError() << std::endl;
	}

	return 0;
}
