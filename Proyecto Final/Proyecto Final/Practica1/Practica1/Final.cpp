/*---------------------------------------------------------*/
/* -----  Proyecto final CGeIHC - QUINTA MARETTA -----------*/
/*-----------------    2021-2   ---------------------------*/
/*------------- Alumnos: ---------------------------------*/
/*---------------Alvarez González Ian Arturo   ---------------*/
/*---------------Martínez Martínez Vanessa   ---------------*/
/*---------------Ramírez Martínez Humberto   ---------------*/
/*---------------Suxo Pérez Luis Axel   ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

//#include <iostream>

#include <mmsystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 2500;
unsigned int SCR_HEIGHT = 2500;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(7000.0f, 100.0f, 2300.0f));
float MovementSpeed = 1000.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting 
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f); //posición de la luz
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 6500.0f,
		movAuto_z = -3800.0f,
		movAuto_y = -1.0f,
		orienta = 0.0f,
		giroLlantas = 0.0f,
	//Jeep
		movJeep_x = 4000.0f,
		movJeep_y = 0.0f,
		movJeep_z = 5000.0f,
		giroJeep = 270.0f,
	incX = 0.0f,// variables de incremento
	incZ = 0.0f,
	incGiro =0.0f,
	//helicóptero
		giroHelice = 0.0f,
		movHelicopX = -3100.0f,
		movHelicopZ = 7325.0f,
		movHelicopY = 1310.0f,
		orientaHelicop = 0.0f,
	//pelota
		movPelotaX = 4000.0f,
		movPelotaY = 220.0f,
		movPelotaZ = 9300.0f,
		capPlanY = 0.0f,
		capPlanY2 = 0.0f,
	//vaca
		orientaVaca = 0.0f,
		orientaPatasVacaIzq = 0.0f,
		orientaPatasVacaDer = 0.0f,
		vacaDerX = 2500.0f,
		vacaDerY = 77.0f,
		vacaDerZ = 3400.0f,
		vacaIzqX = 2500.0f,
		vacaIzqY = 88.7f,
		vacaIzqZ = 3400.0f,
	//naoto
		giroNaoto = 0.0f,
		giroNaotoDer = 0.0f,
		giroNaotoIzq = 0.0f,
		naotoBIzq = 0.0f,
		naotoBDer = 0.0f,
		naotoDX = 980.0f,
		naotoDZ = 1000.0f,
		naotoIX = 1030.0f,
		naotoIZ = 1000.0f,
	//mujer caminando
		mujerX = 68.5f,
		mujerZ = -3800.0f,
		mujerPXd = 55.5f,
		mujerPXi = 80.5f,
		mujerPizqGiro = 0.0f,
		mujerPderGiro = 0.0f,
		mujerPizqZ = -3800.0f,
		mujerPderZ = -3800.0f;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;
bool	avanza = false,
		avanza_y = false,
		avanza_x = false,
		//helicóptero
		avanzaHelicop = false,
		avanzaHelicopX = false,
		avanzaHelicopY = false,
		avanzaHelicopZ = false,
		avanzaHelicopAux1 = false,
		avanzaHelicopAux2 = false,
		avanzaHelicopAux3 = false,
		//pelota
		pelotaInicio = false,
		pelotaInicio2 = false,
		pelotaInicio3 = false,
		pelotaInicio4 = false,
		//vaca
		vacaInicio = false,
		vacaInicio2 = false,
		vacaInicio3 = false,
		vacaInicio4 = false,
		vacaAux = false,
		vacaAux2 = false,
		vacaAux3 = false,
		vacaAux4 = false,
		//naoto
		naotoInicio = false,
		naotoInicio2 = false,
		naotoInicio3 = false,
		naotoInicio4 = false,

		//mujer caminando 
		mujerInicio = false,
		mujerInicio2 = false,
		mujerInicio3 = false,
		mujerInicio4 = false;

//Variables para guaardar KeyFrames
#define MAX_FRAMES 13
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame {
	float movJeep_x;
	float movJeep_z;
	float giroJeep;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 10;			//introducir datos
bool play = false;
int playIndex = 0;
void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].movJeep_x = movJeep_x;
	KeyFrame[FrameIndex].movJeep_z = movJeep_z;

	KeyFrame[FrameIndex].giroJeep = giroJeep;
	FrameIndex++;
}

void resetElements(void)
{
	movJeep_x = KeyFrame[0].movJeep_x;
	movJeep_z = KeyFrame[0].movJeep_z;
	giroJeep = KeyFrame[0].giroJeep;
}

void interpolation(void) //Calcula el incremento de la propiedad
{ //realiza operación de incremento 
	incX = (KeyFrame[playIndex + 1].movJeep_x - KeyFrame[playIndex].movJeep_x) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].movJeep_z - KeyFrame[playIndex].movJeep_z) / i_max_steps;

	incGiro = (KeyFrame[playIndex + 1].giroJeep - KeyFrame[playIndex].giroJeep) / i_max_steps;
}//PlayIndex valor inicial

void animate(void)
{
	//Animación por Keyframes Jeep
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			movJeep_x += incX;
			movJeep_z += incZ;

			giroJeep += incGiro;

			i_curr_steps++;
		}
	}
	
	//Vehículo animación
	if (avanza)
	{

		movAuto_z += 60.0f;
		if (movAuto_z >= -1500.0f)
		{
			orienta += 2.0f;
		}
		if (movAuto_z >= 900.0f)
		{
			avanza = false;
			movAuto_z = 900.0f;
			
			orienta = 90.0f;

			if (movAuto_z == 900.0f)
			{				
				movAuto_x += 60.0f;
				avanza = true;


				if (movAuto_x >= 7200.0f)
				{
					orienta += 2.0f;
					avanza = false;
					orienta = 180.0f;
					//movAuto_x = 100.0f;				
					avanza_y = true;
																
				}				
					
			}
				
		}

	}

	if (avanza_y)
	{
		//orienta += 2.0f;
		movAuto_z += -60.0f;
		if (movAuto_z <= -1400.0f)
		{
			orienta += 2.0f;
		}
	
	
		if (movAuto_z <= -3800.0f)
		{
			avanza_y = false;
			orienta = 270.0f;
			movAuto_z = -3800.0f;

			if (movAuto_z == -3800.0f)
			{
				orienta += 2.0f;
				movAuto_x -= 60.0f;
				avanza_y = true;

				if (movAuto_x <= 6500.0f)
				{
					avanza_y = false;
					orienta = 0.0f;
					
				}
			}
		}
	}

	giroLlantas += 3.0f;
	
	//Animación helicóptero
	if (avanzaHelicop)
	{

		movHelicopY += 10.0f;
		orientaHelicop += 10.0f;

		if (movHelicopY >= 1510.0f && orientaHelicop >= 200.0f)
		{
			giroHelice += 100.0;

			avanzaHelicop = false;
			movHelicopY = 1510.0f;
			orientaHelicop = 200.0f;

			if (movHelicopY == 1510.0f)
			{

				movHelicopZ -= 8.5f;
				movHelicopX += 20.0f;
				avanzaHelicop = true;

				if (movHelicopX >= 4200.0f)
				{
					movHelicopX = 4200.0f;
					movHelicopZ = 4217.7f;
					avanzaHelicop = false;
					avanzaHelicopAux1 = true;

				}
			}

		}

	}
	if (avanzaHelicopAux1)
	{
		movHelicopY -= 10;
		if (movHelicopY <= 1310.0f)
		{
			avanzaHelicopAux1 = false;
			avanzaHelicopX = true;
		}
	}


	if (avanzaHelicopX)
	{

		movHelicopY += 10.0f;
		orientaHelicop += 10.0f;

		if (movHelicopY >= 1510.0f && orientaHelicop >= 60.0f)
		{
			giroHelice += 100.0;

			avanzaHelicopX = false;
			movHelicopY = 1510.0f;
			orientaHelicop = 60.0f;

			if (movHelicopY == 1510.0f)
			{
				movHelicopX -= 11.0f;
				movHelicopZ += 20.0f;
				avanzaHelicopX = true;
	
				if (movHelicopX <= 2100.0f)
				{
					movHelicopX = 2100.0f;
					movHelicopZ = 8250.0f;
					avanzaHelicopX = false;
					avanzaHelicopAux2 = true;

				}
			}

		}

	}
	if (avanzaHelicopAux2)
	{
		movHelicopY -= 10;
		if (movHelicopY <= 1310.0f)
		{
			avanzaHelicopAux2 = false;
			avanzaHelicopY = true;
		}
	}


	if (avanzaHelicopY)
	{

		movHelicopY += 10.0f;
		orientaHelicop += 10.0f;

		if (movHelicopY >= 1510.0f && orientaHelicop >= 0.0f)
		{
			giroHelice += 100.0;

			avanzaHelicop = false;
			movHelicopY = 1510.0f;
			orientaHelicop = 0.0f;

			if (movHelicopY == 1510.0f)
			{

				movHelicopZ -= 3.2f;
				movHelicopX -= 20.0f;
				avanzaHelicopY = true;

				if (movHelicopX <= -3100.0f)
				{
					movHelicopX = -3100.0f;
					movHelicopZ = 7325.0f;
					avanzaHelicopY = false;
					avanzaHelicopAux3 = true;

				}
			}

		}

	}
	if (avanzaHelicopAux3)
	{
		movHelicopY -= 10;
		if (movHelicopY <= 1310.0f)
		{
			avanzaHelicopAux3 = false;
			
		}
	}
	giroHelice += 5.0f;

	//Animación parábola pelota
	if (pelotaInicio)
	{
		capPlanY = 0.0f;
		float v = 10.0f,
			u = 45.0f,
			t = 1.0f;
		double	n = 3.1416;

		for (float i = 0; i <= t; i += 0.25)
		{

			movPelotaY = movPelotaY + ((v * sin(u)) * i) + ((9.81 * i * i) / 2);
			movPelotaX = movPelotaX + (v * cos(u)) * i;

			if (movPelotaY >= 775.0f)
			{
				pelotaInicio = false;
				pelotaInicio2 = true;
			}
		}
	}

	if (pelotaInicio2)
	{
		float v = 10.0f,
			u = 45.0f,
			t = 1.0f;
		double	n = 3.1416;

		for (float i = 0; i <= t; i += 0.25)
		{

			movPelotaY = movPelotaY - ((v * sin(u)) * i) + ((9.81 * i * i) / 2);
			movPelotaX = movPelotaX + (v * cos(u)) * i;

			if (movPelotaY <= 195.0f)
			{
				capPlanY2 = 10.0f;
			}

			if (movPelotaY <= 200.0f)
			{				
				pelotaInicio2 = false;
				pelotaInicio3 = true;
			}
		}
	}

	if (pelotaInicio3)
	{
		capPlanY2 = 0.0f;
		float v = 10.0f,
			u = 45.0f,
			t = 1.0f;
		double	n = 3.1416;

		for (float i = 0; i <= t; i += 0.25)
		{

			movPelotaY = movPelotaY + ((v * sin(u)) * i) + ((9.81 * i * i) / 2);
			movPelotaX = movPelotaX - (v * cos(u)) * i;

			if (movPelotaY >= 775.0f)
			{
				pelotaInicio3 = false;
				pelotaInicio4 = true;
			}
		}
	}

	if (pelotaInicio4)
	{
		float v = 10.0f,
			u = 45.0f,
			t = 1.0f;
		double	n = 3.1416;

		for (float i = 0; i <= t; i += 0.25)
		{

			movPelotaY = movPelotaY - ((v * sin(u)) * i) + ((9.81 * i * i) / 2);
			movPelotaX = movPelotaX - (v * cos(u)) * i;

			if (movPelotaY <= 195.0f)
			{
				capPlanY = 10.0f;
			}

			if (movPelotaY <= 200.0f)
			{
				pelotaInicio4 = false;
				pelotaInicio = true;
			}
		}
	}

	//vaca bailando
	if (vacaInicio)
	{
		orientaPatasVacaDer -= 5.0f;
		orientaVaca += 0.25f;
		vacaDerX -= 2.0f;
		vacaDerY += 1.7f;
		if (orientaPatasVacaDer <= -90.0f)
		{
			vacaInicio = false;
			vacaAux = true;
			
		}
	}

	if (vacaAux)
	{
		orientaPatasVacaDer += 5.0f;
		if (orientaPatasVacaDer >= -40.0f)
		{
			vacaAux = false;
			vacaAux2 = true;
		}
	}

	if (vacaAux2)
	{
		orientaPatasVacaDer -= 5.0f;
		if (orientaPatasVacaDer <= -90.0f)
		{
			vacaAux2 = false;
			vacaInicio2 = true;
		}

	}

	if (vacaInicio2)
	{
		orientaPatasVacaDer += 10.0f;
		orientaVaca -= 0.5f;
		vacaDerX += 5.0f;
		vacaDerY -= 3.5f;
		if (orientaPatasVacaDer >= 0.0f)
		{
			vacaInicio2 = false;
			vacaInicio3 = true;
		}
	}



	if (vacaInicio3)
	{
		orientaPatasVacaIzq += 5.0f;
		orientaVaca -= 0.25f;
		vacaIzqX += 2.0f;
		vacaIzqY += 1.7f;
		if (orientaPatasVacaIzq >= 90.0f)
		{
			vacaInicio3 = false;
			vacaAux3 = true;

		}
	}

	if (vacaAux3)
	{
		orientaPatasVacaIzq -= 5.0f;
		if (orientaPatasVacaIzq <= 40.0f)
		{
			vacaAux3 = false;
			vacaAux4 = true;
		}
	}

	if (vacaAux4)
	{
		orientaPatasVacaIzq += 5.0f;
		if (orientaPatasVacaIzq >= 90.0f)
		{
			vacaAux4 = false;
			vacaInicio4 = true;
		}

	}

	if (vacaInicio4)
	{
		orientaPatasVacaIzq -= 10.0f;
		orientaVaca += 0.5f;
		vacaIzqX -= 5.0f;
		vacaIzqY -= 3.5f;
		if (orientaPatasVacaIzq <= 0.0f)
		{
			vacaInicio4 = false;

			vacaDerX = 2500.0f;
			vacaDerY = 77.0f;
			vacaDerZ = 3400.0f;
			vacaIzqX = 2500.0f;
			vacaIzqY = 88.7f;
			vacaIzqZ = 3400.0f;

			vacaInicio = true;
		}
	}


	//Naoto Vibin

	if (naotoInicio)
	{
		giroNaoto += 5.0f;
		giroNaotoDer += 5.0f;
		giroNaotoIzq += 5.0f;
		naotoBIzq -= 3.0f;
		naotoBDer += 3.0f;
		naotoDX += 1.0f;
		naotoDZ += 1.0f;
		naotoIZ -= 1.0f;
		naotoIX -= 1.0f;

		if (giroNaoto >= 90.0f)
		{
			naotoInicio = false;
			naotoInicio2 = true;
		}
	}

	if (naotoInicio2)
	{
		giroNaoto -= 5.0f;
		giroNaotoDer -= 5.0f;
		giroNaotoIzq -= 5.0f;
		naotoBIzq += 3.0f;
		naotoBDer -= 3.0f;
		naotoDX -= 1.0f;
		naotoDZ -= 1.0f;
		naotoIZ += 1.0f;
		naotoIX += 1.0f;
		if (giroNaoto <= 0.0f)
		{
			naotoInicio2 = false;
			naotoInicio = true;
		}
	}

	//mujer caminando animación

	if (mujerInicio)
	{
		mujerZ += 5.0f;
		mujerPizqZ += 5.0f;
		mujerPderZ += 5.0f;

		mujerPderGiro += 2.0f;
		mujerPizqGiro -= 2.0f;
		if (mujerPderGiro >= 25.0f)
		{
			mujerInicio = false;
			mujerInicio2 = true;
		}
	}
	if (mujerInicio2)
	{
		mujerZ += 5.0f;
		mujerPizqZ += 5.0f;
		mujerPderZ += 5.0f;

		mujerPderGiro -= 2.0f;
		mujerPizqGiro += 2.0f;
		if (mujerPderGiro <= 0.0f)
		{
			mujerInicio2 = false;
			mujerInicio = true;
		}

		if (mujerZ >= 500.0f)
		{
			mujerInicio2 = false;
			mujerInicio = false;
		}
	}


}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.png",
		"resources/skybox/left.png",
		"resources/skybox/top.png",
		"resources/skybox/bottom.png",
		"resources/skybox/front.png",
		"resources/skybox/back.png"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	//AUDIO
	PlaySound(TEXT("resources/audio/fondomusical.wav"),NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model venecia("resources/objects/VeneciaMod1/Venecia1.obj");
	Model arbol("resources/objects/arbol/arbol4.obj");
	Model palmera("resources/objects/palmera/palmera.obj");
	Model gato("resources/objects/gato/gato.obj");
	Model silla("resources/objects/Silla Amenidades/Silla.obj");
	Model pelota("resources/objects/pelota/pelota.obj");
	Model departamentos("resources/objects/Departamentos/Departamentos.obj");
	Model departamentos2("resources/objects/Departamentos2/Departamentos2.obj");
	Model edificioC("resources/objects/EdificioC/EdificioC.obj");
	Model genova("resources/objects/Genova/Genova.obj");
	Model helicoptero("resources/objects/helicoptero/helicop5.obj");
	Model helice("resources/objects/helicoptero/helice2.obj");
	Model capitan("resources/objects/capPlaneta/capPlan.obj");
	Model vaca("resources/objects/vacaBailando/vaca.obj");
	Model patasVacaDer("resources/objects/vacaBailando/patasVacaDer.obj");
	Model patasVacaIzq("resources/objects/vacaBailando/patasVacaIzq.obj");
	Model naotoCuerpo("resources/objects/Naoto/naotoCuerpo.obj");
	Model naotoIzq("resources/objects/Naoto/naotoBrazoIzq.obj");
	Model naotoDer("resources/objects/Naoto/naotoBrazoDer.obj");
	Model mujer("resources/objects/mujer/mujer.obj");
	Model piernaderecha("resources/objects/mujer/piernaderecha.obj");
	Model piernaizquierda("resources/objects/mujer/piernaizquierda.obj");
	Model kiosko("resources/objects/kios/kiosko.obj");
	Model caseta("resources/objects/caset/caseta.obj");
	Model piscina("resources/objects/Waterpool/Piscina.obj");
	Model perro("resources/objects/Dog_obj/Dog.obj");
	Model nyan("resources/objects/Nyan Cat/nyan_cat.obj");
	Model genovaCuarto("resources/objects/GenovaCuarto/Genova.obj");
	Model jeep("resources/objects/Carro/Jeep_Renegade_2016.obj");

	//Animacion por Keyframes Jeep
	KeyFrame[0].movJeep_x = 4000;
	KeyFrame[0].movJeep_z = 5000;
	KeyFrame[0].giroJeep = 268;

	KeyFrame[1].movJeep_x = 2800;
	KeyFrame[1].movJeep_z = 5000;
	KeyFrame[1].giroJeep = 248;

	KeyFrame[2].movJeep_x = 2700;
	KeyFrame[2].movJeep_z = 4900;
	KeyFrame[2].giroJeep = 238;

	KeyFrame[3].movJeep_x = 2600;
	KeyFrame[3].movJeep_z = 4800;
	KeyFrame[3].giroJeep = 218;

	KeyFrame[4].movJeep_x = 2600;
	KeyFrame[4].movJeep_z = 4400;
	KeyFrame[4].giroJeep = 148;

	KeyFrame[5].movJeep_x = 2600;
	KeyFrame[5].movJeep_z = 4100;
	KeyFrame[5].giroJeep = 98;

	KeyFrame[6].movJeep_x = 2600;
	KeyFrame[6].movJeep_z = 3800;
	KeyFrame[6].giroJeep = 78;

	KeyFrame[7].giroJeep = 58;
	KeyFrame[7].movJeep_x = 3500;
	KeyFrame[7].movJeep_z = 3800;

	KeyFrame[7].giroJeep = 68;
	KeyFrame[7].movJeep_z = 3900;
	KeyFrame[7].movJeep_x = 3500;

	KeyFrame[7].giroJeep = 58;
	KeyFrame[7].movJeep_z = 3900;
	KeyFrame[7].movJeep_x = 3600;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		//Fuente de luz Direccional - Sol
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //valores RGB
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f)); //(color tendencia de las sombras) si los pones en 0, no habrá luz solar, casi todo estará oscuro
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f)); //(luz blanca) componente difusa: color principal del objeto
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); //brillo de la superficie
		//Fuentes de luz posicionales
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//PISO//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

//***************************************************CASA VENECIA/GENOVA (VAN INTERCALADOS)**************************************************************
//*****************************************************************************************************************************
		//======FILA 1=========
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2450.0f, -40.0f, -4600.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1600.0f, 0.0f, -2700.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genovaCuarto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1900.0f, -40.0f, -200.0f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1350.0f, 0.0f, 2000.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-950.0f, -40.0f, 4000.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);
		//=================================
		//===========FILA 2 ===============

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 0.0f, -4600.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, -40.0f, -2400.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-125.0f, 0.0f, -200.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -40.0f, 2000.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);


		//=================================
		//===========FILA 3 ===============

		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, -40.0f, -4700.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(85.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1700.0f, 0.0f, -2500.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(90.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1900.0f,-40.0f, -300.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 40.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		//=================================
		//===========FILA 4 ===============

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2700.0f, 0.0f, -4300.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(90.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3800.0f, -40.0f, -2000.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(90.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		//=================================
		//===========FILA 5 ===============

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3650.0f, 0.0f, -9000.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4125.0f, 0.0f, -7000.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4375.0f, 0.0f, -4800.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		venecia.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5475.0f, 0.0f, -2900.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 50.0f, 40.0f));
		staticShader.setMat4("model", model);
		genova.Draw(staticShader);
		//=================================
//*****************************************************************************************************************************

//***************    EDIFICIO C    ************************************************************************************************
//*****************************************************************************************************************************
		model = glm::translate(glm::mat4(1.0f), glm::vec3(3000.0f, 0.0f, 5000.0f));
		model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(45.0f, 50.0f, 25.0f));
		staticShader.setMat4("model", model);
		edificioC.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, 11000.0f));
		model = glm::rotate(model, glm::radians(250.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(45.0f, 50.0f, 25.0f));
		staticShader.setMat4("model", model);
		edificioC.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3700.0f, 0.0f, 6000.0f));
		model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(45.0f, 50.0f, 25.0f));
		staticShader.setMat4("model", model);
		edificioC.Draw(staticShader);
		//=================================
//*****************************************************************************************************************************

//***************    DEPARTAMENTOS   ************************************************************************************************
//*****************************************************************************************************************************
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3100.0f, 0.0f, -8200.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 30.0f, 20.0f));
		staticShader.setMat4("model", model);
		departamentos.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1300.0f, 0.0f, -8200.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 30.0f, 20.0f));
		staticShader.setMat4("model", model);
		departamentos2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 0.0f, -8200.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 30.0f, 20.0f));
		staticShader.setMat4("model", model);
		departamentos.Draw(staticShader);

		//=================================
//*****************************************************************************************************************************

//***************    PALMERA    ************************************************************************************************
//*****************************************************************************************************************************
		model = glm::translate(glm::mat4(1.0f), glm::vec3(3000.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2600.0f, 0.0f, -2000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, 0.0f, 7500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(750.0f, 0.0f, 6200.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1185.0f, 0.0f, 4900.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1610.0f, 0.0f, 3600.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2300.0f, 0.0f, 2300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2300.0f, 0.0f, 1000.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3500.0f, 0.0f, 1000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4800.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5806.0f, 0.0f, -8250.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5502.0f, 0.0f, -3200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5502.0f, 0.0f, -8506.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5652.0f, 0.0f, 1050.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5822.0f, 0.0f, 1050.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5822.0f, 0.0f, 6302.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5652.0f, 0.0f, 5603.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);
//*****************************************************************************************************************************

//***************    ARBOL    ************************************************************************************************
//*****************************************************************************************************************************
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, -9000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5500.0f, 0.0f, -8500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5250.0f, 0.0f, -8000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5800.0f, 0.0f, -7225.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5620.0f, 0.0f, -6900.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5300.0f, 0.0f, -7600.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5753.0f, 0.0f, -7015.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5600.0f, 0.0f, -6800.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5210.0f, 0.0f, -6200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5900.0f, 0.0f, -5810.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5625.0f, 0.0f, -4500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5200.0f, 0.0f, -4400.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5200.0f, 0.0f, -4000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, -3000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-4500.0f, 0.0f, -2500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3850.0f, 0.0f, -1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, -1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5500.0f, 0.0f, -1275.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5250.0f, 0.0f, -1200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5800.0f, 0.0f, -1000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5620.0f, 0.0f, -950.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5300.0f, 0.0f, -900.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5753.0f, 0.0f, -105.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5600.0f, 0.0f, -795.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5210.0f, 0.0f, -760.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5900.0f, 0.0f, -0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5625.0f, 0.0f, -635.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5200.0f, 0.0f, -582.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5200.0f, 0.0f, 500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, 1000.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-4500.0f, 0.0f, 1250.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3850.0f, 0.0f, 1915.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, 1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5500.0f, 0.0f, 2005.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5250.0f, 0.0f, 2615.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5800.0f, 0.0f, 2900.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5620.0f, 0.0f, 3600.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5300.0f, 0.0f, 3800.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5753.0f, 0.0f, 4300.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5600.0f, 0.0f, 4856.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5210.0f, 0.0f, 4520.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5900.0f, 0.0f, 5200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-6000.0f, 0.0f, 5825.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5500.0f, 0.0f, 6352.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5250.0f, 0.0f, 6825.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5800.0f, 0.0f, 6950.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5620.0f, 0.0f, 7222.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5300.0f, 0.0f, 7890.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5753.0f, 0.0f, 8360.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5600.0f, 0.0f, 8821.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5210.0f, 0.0f, 6521.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5900.0f, 0.0f, 9100.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);
	//--
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5300.0f, 0.0f, 7890.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5753.0f, 0.0f, 8360.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4900.0f, 0.0f, 8821.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5210.0f, 0.0f, 6521.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5000.0f, 0.0f, 9100.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4725.0f, 0.0f, 8121.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4300.0f, 0.0f, 5801.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4252.0f, 0.0f, 9501.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5533.0f, 0.0f, -6521.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5000.0f, 0.0f, -9100.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5735.0f, 0.0f, -8121.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5281.0f, 0.0f, -5801.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5360.0f, 0.0f, -9501.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);
//---------------------------------------------------------------------------------------------------------------------------------
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movAuto_x, movAuto_y, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(68.0f, 20.0f, 103.2f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-68.0f, 20.0f, 103.2f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-68.0f, 20.0f, -116.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(68.0f, 20.0f, -116.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase

		//GATO

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2900.0f, 0.0f, -1500.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		gato.Draw(staticShader);

		//PERRO

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2900.0f, 0.0f, 5300.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		perro.Draw(staticShader);

		//NYAN CAT

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1200.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		staticShader.setMat4("model", model);
		nyan.Draw(staticShader);

		//KIOSKO

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2400.0f, 0.0f, 5700.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(80.0f, 80.0f, 80.0f));
		staticShader.setMat4("model", model);
		kiosko.Draw(staticShader);

		//SILLA

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4900.0f, 0.0f, 5600.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4550.0f, 0.0f, 5650.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(5200.0f, 0.0f, 5700.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		//caseta

		model = glm::translate(glm::mat4(1.0f), glm::vec3(6100.0f, 200.0f, 2300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 70.0f, 100.0f));
		staticShader.setMat4("model", model);
		caseta.Draw(staticShader);

		//Piscina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5500.0f, -100.0f, 6100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 70.0f, 100.0f));
		staticShader.setMat4("model", model);
		piscina.Draw(staticShader);

		//pelota
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movPelotaX, movPelotaY, movPelotaZ));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		pelota.Draw(staticShader);

		//capPlaneta
		model = glm::translate(glm::mat4(1.0f), glm::vec3(4000.0f, capPlanY, 9300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		capitan.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(4900.0f, capPlanY2, 9300.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		capitan.Draw(staticShader);

		//helicoptero
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movHelicopX, movHelicopY, movHelicopZ));
		model = glm::rotate(model, glm::radians(orientaHelicop), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		helicoptero.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(movHelicopX, movHelicopY + 128.3f, movHelicopZ));
		model = glm::rotate(model, glm::radians(giroHelice), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		helice.Draw(staticShader);

		//Jeep
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movJeep_x, movJeep_y, movJeep_z));
		model = glm::rotate(model, glm::radians(giroJeep), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);


		//vaca bailando

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2500.0f, 0.0f, 3400.0f));
		model = glm::rotate(model, glm::radians(orientaVaca), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaVaca), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		vaca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(vacaDerX, vacaDerY, vacaDerZ));
		model = glm::rotate(model, glm::radians(orientaPatasVacaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		patasVacaDer.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(vacaIzqX, vacaIzqY, vacaIzqZ));
		model = glm::rotate(model, glm::radians(orientaPatasVacaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		patasVacaIzq.Draw(staticShader);

		//Naoto

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0, 1000.0f));
		model = glm::rotate(model, glm::radians(giroNaoto), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		naotoCuerpo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(naotoIX, 65.0, naotoIZ));
		model = glm::rotate(model, glm::radians(naotoBIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroNaotoIzq), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		naotoIzq.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(naotoDX, 65.0, naotoDZ));
		model = glm::rotate(model, glm::radians(naotoBDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroNaotoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(70.0f, 70.0f, 70.0f));
		staticShader.setMat4("model", model);
		naotoDer.Draw(staticShader);

		// Mujer

		model = glm::translate(glm::mat4(1.0f), glm::vec3(mujerX, 0.0f, mujerZ));
		staticShader.setMat4("model", model);
		mujer.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(mujerPXd, 105.0f, mujerPderZ));
		model = glm::rotate(model, glm::radians(mujerPderGiro), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaderecha.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(mujerPXi, 105.0f, mujerPizqZ));
		model = glm::rotate(model, glm::radians(mujerPizqGiro), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaizquierda.Draw(staticShader);

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	//navegación

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//helicóptero animación
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		avanzaHelicop = true;

	//pelota animación
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		pelotaInicio = true;

	//vaca animación
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		vacaInicio = true;

	//naoto animación
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		naotoInicio = true;

	//mujer caminando animación
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		mujerInicio = true;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		avanza ^= true;
	//Jeep
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		movJeep_x +=100.0f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		movJeep_x -= 100.0f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		movJeep_z += 100.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		movJeep_z -= 100.0f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		giroJeep += 10.0f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		giroJeep -= 10.0f;

	//reseteo
	if (key == GLFW_KEY_E && action == GLFW_PRESS) { //reseteo
		
		//reset auto
		avanza = false;
		avanza_y = false;
		movAuto_x = 6500.0f;
		movAuto_z = -3800.0f;
		movAuto_y = -1.0f;
		movJeep_x = 4000.0f;
		movJeep_y = 0.0f;
		movJeep_z = 5000.0f;
		orienta = 0.0f;

		//reset helicópter
		avanzaHelicop = false;
		avanzaHelicopX = false;
		avanzaHelicopY = false;
		avanzaHelicopZ = false;
		giroHelice = 0.0f;
		movHelicopX = -3100.0f;
		movHelicopZ = 7325.0f;
		movHelicopY = 1310.0f;
		orientaHelicop = 0.0f;

		//reset pelota
		pelotaInicio = false;
		movPelotaX = 4000.0f;
		movPelotaY = 220.0f;
		movPelotaZ = 9300.0f;

		//reset vaca
		vacaInicio = false;
		orientaVaca = 0.0f;
		orientaPatasVacaIzq = 0.0f;
		orientaPatasVacaDer = 0.0f;
		vacaDerX = 2500.0f;
		vacaDerY = 77.0f;
		vacaDerZ = 3400.0f;
		vacaIzqX = 2500.0f;
		vacaIzqY = 88.7f;
		vacaIzqZ = 3400.0f;

		//reset naoto
		naotoInicio = false;
		giroNaoto = 0.0f;
		giroNaotoDer = 0.0f;
		giroNaotoIzq = 0.0f;
		naotoBIzq = 0.0f;
		naotoBDer = 0.0f;
		naotoDX = 980.0f;
		naotoDZ = 1000.0f;
		naotoIX = 1030.0f;
		naotoIZ = 1000.0f;

		//reset mujer caminando
		mujerInicio = false;
		mujerX = 68.5f;
		mujerZ = -3800.0f;
		mujerPXd = 55.5f;
		mujerPXi = 80.5f;
		mujerPizqGiro = 0.0f;
		mujerPderGiro = 0.0f;
		mujerPizqZ = -3800.0f;
		mujerPderZ = -3800.0f;

	}
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}