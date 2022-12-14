/* Hello Triangle - c?digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * C?digo de Rossana Baptista Queiroz, adaptado por Gabriela Mezetti Vieira
 * para a disciplina de Processamento Gr?fico - Unisinos
 * Vers?o inicial : 07 / 04 / 2017
 * ?ltima atualiza??o em 19 / 09 / 2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//Shader
#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float Pi = 3.1419;

// Prot?tipo da fun??o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot?tipos das fun??es
int setupGeometry();
int generateCircle(float radius, int nPoints);

// Dimens?es da janela (pode ser alterado em tempo de execu??o)
const GLuint WIDTH = 800, HEIGHT = 600;

enum Movment { Left, Right, Top, Down, Stop };
static Movment mov = Stop;

//using namespace glm;

// Fun??o MAIN
int main()
{
	// Inicializa??o da GLFW
	glfwInit();


	// Cria??o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Gabi!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun??o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa??es de vers?o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/ortho.vs", "../shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um tri?ngulo
	int nPoints = 200;
	GLuint VAO = generateCircle(0.5, nPoints);//setupGeometry();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a vari?veis do tipo uniform em GLSL para armazenar esse tipo de info
	// que n?o est? nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	//Criando a matriz de proje??o usando a GLM
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	glm::mat4 model = glm::mat4(1); //matriz identidade

	model = glm::translate(model, glm::vec3(400, 300, 0.0));
	model = glm::scale(model, glm::vec3(200, 100, 1.0));

	GLint modelLoc = glGetUniformLocation(shader.ID, "model");

	float x = 400.0f, y = 300.0f;

	// Loop da aplica??o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens?es da viewport com as mesmas dimens?es da janela da aplica??o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(8);
		glPointSize(20);

		model = glm::mat4(1); //matriz identidade

		switch (mov)
		{
		case Left:
			if (x >= 50) {
				x -= 0.2f;	break;
			}
			else break;

		case Right:
			if (x <= 750) {
				x += 0.2f;	break;
			}
			else break;

		case Top:
			if (y <= 550) {
				y += 0.2f;	break;
			}
			else break;

		case Down:
			if (y >= 50) {
				y -= 0.2f;	break;
			}
			else break;
		default:	break;
		}

		model = glm::translate(model, glm::vec3(x, y, 0.0f));
		model = glm::scale(model, glm::vec3(100, 100, 1.0));

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glUniform4f(colorLoc, 0.8f, 0.9f, 0.0f, 1.0f); //enviando cor para vari?vel uniform inputColor
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints + 2);

		glUniform4f(colorLoc, 0.7f, 0.1f, 1.0f, 1.0f); //enviando cor para vari?vel uniform inputColor
		glDrawArrays(GL_LINE_LOOP, 0, nPoints + 2);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu??o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun??o de callback de teclado - s? pode ter uma inst?ncia (deve ser est?tica se

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { mov = Right; }
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { mov = Left; }
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { mov = Top; }
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { mov = Down; }
	else { mov = Stop; }
}

int setupGeometry()
{

	GLfloat vertices[] = {
		-0.5, -0.5, 0.0,
		 0.5, -0.5, 0.0,
		 0.0, 0.5, 0.0,
		 //outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int generateCircle(float radius, int nPoints)
{
	int totalSize = (nPoints + 2) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	//Primeiro, colocamos o ponto central do c?rculo (x, y, z)
	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angle = 0.0;
	float slice = 2 * Pi / (GLfloat)nPoints;
	for (int i = 3; i < totalSize; i += 3)
	{
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = 0.0;

		vertices[i] = x;
		vertices[i + 1] = y;
		vertices[i + 2] = z;

		angle += slice;
	}

	GLuint VBO, VAO;

	//Gera??o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex?o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Gera??o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v?rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso ? permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v?rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran?a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (? uma boa pr?tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
