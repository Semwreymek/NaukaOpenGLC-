#include"MainLoop.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800

int main(void) {

	//source dla Ramek obiektu, musi byc char *, string wywala b��d
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\n";


	//Source dla kolork�w obiektu, musi byc char *, string wywala b��d 
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
		"}\n";


	//Tabela z danymi o wierzcho�kach, nie tylko wsp�rz�dne mog� by� w tej tabeli
	GLfloat	vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lewy wierzcho�ek
		 0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Prawy wierzcho�ek
		 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, //G�rny wierzcho�ek
	};

	//MainLoop::run();

	//Inicjalizujemy glfw
	glfwInit();


	//U�ywamy openl 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	//Paczka funkcji, Core (modern funkcje) oraz Compability (stare funkcje)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Tytul okna", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	//U�ywamy okna: window
	glfwMakeContextCurrent(window);


	//�adujemy bibliotek� Glad
	gladLoadGL();


	//Obszar okna ktory chcemy renderowac
	glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);


	//Tworzymy vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);


	//Podajemy �r�d�o shadera czyli ten string, w kt�rym jest shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);


	//Kompilujemy �eby GPU mog�o zrozumie� shader
	glCompileShader(vertexShader);


	//Robimy to samo ale z Fragment shaderem
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//Tworzymy ala obiekt/shader zlozony z ramek i koloru (vertex i fragment)shader
	GLuint shaderProgram = glCreateProgram();


	//Przyczepiamy do niego wybrane vertex i fragment shadery
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);


	//Linkowanie shadera, wszystkie te vertex, fragment, program s� ��czone (linkowane) w ca�o��-
	//- to ostatni etap, ten link b�dzie wysy�any na GPU
	glLinkProgram(shaderProgram);


	//Usuwamy shadery, bo one ju� s� Zlinkowane, wi�c nie s� potrzebne
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//VAO Przechowuje adresy r�nych VBO, czyli adres�w GLfloat vertices[]{....};-
	//- powsta� po to �eby mo�na by�o si� na Ez prze��cza� mi�dzy wieloma VBO, OpenGL wymaga VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//Vertex (tablica) of references do tablicy wierzcho�k�w GLfloat vertices[]{....};
	GLuint VBO;


	//Bufery czyli VBO, VAO, EBO etc, to duze pakiety danych wysy�ane w jednym czasie na GPU
	//Przesy�anie z CPU na GPU jest wolne, wi�c robimy to du�ymi chonkami (buferami)
	glGenBuffers(1, &VBO); // 1 tablica wierzcho�k�w (1 obiekt)  GLfloat vertices[]{....};


	//Mo�emy mie� kilka r�nych bufer�w, BindingBuffer m�wi jakiego typu jest ten kt�rego chcemy -
	//- u�y�, oraz jego nazwe (nazwy to te� referencje na obiekty smh)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Array_buffer no bo do VBO Przypiszemy vertices[]{....};


	//Przypisujemy dane z  GLfloat vertices[]{....}; do VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Komunikacja z vertexShader z "zewn�trz"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Unbindujemy VAO i VBO, zeby ich nie u�y� przez przypadek
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//G��wna p�tla
	while (!glfwWindowShouldClose(window)) {

		//Ustawiamy kolor na GL_COLOR_BUFFER_BIT
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);


		//U�ywamy koloru z GL_COLOR_BUFFER_BIT zrobionego z glClearColor(....);
		glClear(GL_COLOR_BUFFER_BIT);


		//Aktywujemy shader program
		glUseProgram(shaderProgram);


		//U�ywamy tego konretnie VAO, teraz kinda useless bo jest 1 obiekt
		glBindVertexArray(VAO);


		//Rysujemy obiekt z GLfloat vertices[]{....};, jest on ju� zapisany w Arrays
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// Zmieniamy star� klatke na now�
		glfwSwapBuffers(window);


		//bez tego nie b�dzie mo�na rusza� okna ani z nim obcowa�
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);


	//Niszczymy okno
	glfwDestroyWindow(window);


	//zamykamy glfw, zwalniamy zasoby uzywane przez GLFW
	glfwTerminate();
	return 0;
}