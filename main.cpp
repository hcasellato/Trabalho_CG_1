//*****************************************************
//
// pratica4.cpp
// Visualização 3D
// 
//*****************************************************
#include <GL/glut.h>
#include <iostream>
GLfloat angle, fAspect, largura, altura, ycamera;

void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

    // Especifica a viewport
	glViewport(0, 0, largura, altura);

	glutWireSphere(50, 20, 20);

    glColor3f(1.0f, 0.2f, 0.2f);
    glPushMatrix();
	    glutWireSphere(50, 20, 20);
    glPopMatrix();

	glutSwapBuffers();
}

void Inicializa(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);   //ativa o zBuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer

	angle = 45;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.1, 1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0, ycamera, 300,  // posição da câmera
              0, 0, 0,          // posição do alvo
              0, 1, 0);         // vetor UP da câmera
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLint largura, GLint altura)
{
	// Para previnir uma divisão por zero
	if (altura == 0) altura = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, largura, altura);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)largura / (GLfloat)altura;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			//if (angle >= 10) angle -= 5;
            ycamera += 10;  
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			//if (angle <= 130) angle += 5;
            ycamera -= 10; 
		}
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer  
    EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ycamera += 10;
	}
	if (key == GLUT_KEY_DOWN) {
		ycamera -= 10;
	}
	if (key == GLUT_KEY_RIGHT) {
		//........;
	}
	if (key == GLUT_KEY_LEFT) {
		//........;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void GerenciaTeclado(unsigned char key, int x, int y) {
	switch (key) {
		case ' ': // restaura posição inicial da camera
			//............
			break;
		// movimentacao do observador
		case 'd':
			//..............
			break;
		case 'a':
			//...............
			break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  //GLUT_DOUBLE trabalha com dois buffers: um para renderização e outro para exibição
	
    glutInitWindowPosition(700,100);
    largura = 600;
    altura = 500;
	glutInitWindowSize(largura,altura);
	fAspect = (GLfloat)largura / (GLfloat)altura;
    ycamera = 0;
    glutCreateWindow("Aula Pratica 4"); 

	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela); // Função para ajustar o tamanho da tela
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado); // Define qual funcao gerencia o comportamento do teclado
    glutSpecialFunc(TeclasEspeciais); // Define qual funcao gerencia as teclas especiais
	Inicializa();
	glutMainLoop();

}	


// COMPILAÇÃO
// g++ -o main main.cpp -lGL -lGLU -lglut -lm && ./main



