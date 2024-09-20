//*****************************************************
//
// Código para visualização de um pêndulo!
//
// Eduardo Campos     (13838537)
// Henrique Casellato (13781858)
// 
//*****************************************************
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;

GLfloat angle, fAspect, largura, altura, ycamera;

// Variáveis para coordenadas esféricas
double phi,theta,r;

// Vetores para desenho de traço em pontos
double x[10000],y[10000],z[10000];

// Matriz para traço de cauda do pêndulo
double trace_x[10000][100],trace_y[10000][100],trace_z[10000][100];

// Tamanho máximo de pontos na matriz
int INT_DESCRITIVO = 100;

// Variável para função de traço da cauda do pêndulo
double t_trace;

// Vetor para mudança de cores da cauda do pêndulo
double color[3];

// Variáveis auxiliares para controle de tempo
int count = 0;
int aux = 0;

// Lado do Quadrado
int L = 200;

// Função para desenhar quadrado na cena
void fazer_quadrado(){
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);			// Face frontal
		glNormal3f(0.0, 0.0, 1.0); 	// Normal da face
		glVertex3f(L, L, L);
		glVertex3f(-L, L, L);
		glVertex3f(-L, -L, L);
		glVertex3f(L, -L, L);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);			// Face posterior
		glNormal3f(0.0, 0.0, -1.0);	// Normal da face
		glVertex3f(L, L, 0.0);
		glVertex3f(L, -L, 0.0);
		glVertex3f(-L, -L, 0.0);
		glVertex3f(-L, L, 0.0);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);			// Face inferior
		glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
		glVertex3f(-L, -L, 0.0);
		glVertex3f(L, -L, 0.0);
		glVertex3f(L, -L, L);
		glVertex3f(-L, -L, L);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);			// Face lateral esquerda
		glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
		glVertex3f(-L, L, L);
		glVertex3f(-L, L, 0.0);
		glVertex3f(-L, -L, 0.0);
		glVertex3f(-L, -L, L);
	glEnd();
	
}

// Função para desenhar cauda do pêndulo
void traço_bola(){
	// Posição inicial
	for(int j = 0;j<INT_DESCRITIVO;j++){
		trace_x[0][j] = r*cos(0)*sin(0);
		trace_y[0][j] = r*sin(0)*sin(0);
		trace_z[0][j] = r*(1-cos(0));
	}

	for(int j = 0;j<INT_DESCRITIVO;j++){
		// Pontos da cauda por função t/e^0.01*t
		trace_x[count][j] = r*cos(phi)*sin(theta) + cos(t_trace+j)*t_trace/exp(t_trace*0.01);
		trace_y[count][j] = r*sin(theta)*sin(phi) + sin(t_trace+j)*t_trace/exp(t_trace*0.01);
		trace_z[count][j] = r*(1-cos(theta));
		
		// Diminui a cauda progressivamente
		for(int k = 0;k<count;k++){
			trace_x[k][j] -= cos(t_trace+j)*t_trace/exp(t_trace*0.01)*(float(count - k)/float(count));
			trace_y[k][j] -= sin(t_trace+j)*t_trace/exp(t_trace*0.01)*(float(count - k)/float(count));
		}
		
	}

	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int j =0; j<INT_DESCRITIVO;j++){
		color[0] = float(j)/100;
		glColor3f(color[0]*5,color[0],0.f);
		
		for(int i=count; i > count+-20-int(aux/50); i--) 
		{
			glVertex3f(trace_x[i][j],trace_y[i][j],trace_z[i][j]);
		}
		
		// Print para debug
		//printf("%d \n",trace_x[count+-20-int(aux/50)+1][j]-x[count+-20-int(aux/50+1)]);
	}
	glEnd();
}

// Função para desenhar projeções nas faces do quadrado
void desenhar_projeções(void){
	glBegin(GL_POINTS);			
        for (int i = 0; i < count; i++) {
            glVertex3f(L, y[i],z[i]);
            glVertex3f(-L, y[i],z[i]);
			glVertex3f(x[i],-L,z[i]);
			//glVertex3f(x[i],L,z[i]);
			//glVertex3f(x[i],y[i],L);
		}
	glEnd();
}

// Desenha fio que prende centro do quadrado na bola
void desenhar_linha(void){
	glBegin(GL_LINE_LOOP);			
		glVertex3f(0, 0, 0);
		glVertex3f(r*cos(phi)*sin(theta),r*sin(theta)*sin(phi),r*(1-cos(theta)));
	glEnd();
}

// Função que desenha curva de pontos
void draw_points(){
	x[count] = r*cos(phi)*sin(theta);
	y[count] = r*sin(theta)*sin(phi);
	z[count] = r*(1-cos(theta));
	 glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 0.0);
        for (int i = 0; i < count; i++) {
            glVertex3f(x[i], y[i],z[i]);
        }
    glEnd();
}

// Desenho da cena
void Desenha(void)
{
	x[0] = 0;
	y[0] = 0;
	z[0] = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, largura, altura);

	glColor3f(0.0f, 0.0f, 1.0f);			
	desenhar_linha();
	fazer_quadrado();
	traço_bola();
	desenhar_projeções();
	glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);	
		glTranslated(r*cos(phi)*sin(theta),r*sin(theta)*sin(phi),r*(1-cos(theta)));
		glutWireSphere(10, 10, 10);
    glPopMatrix();
	draw_points();

	glutSwapBuffers();
}

// Controle de tempo
void ControleTempo(int value) {
	
	// Reinicia contador, para continuar contabilizando pontos
	if(count > 10000-1){count=0;}
	
	// Define randomização de movimento
	srand (time(NULL));
	if (rand()%100>70){
		phi -= 0.01/(2*3.14);
	}
	
	// Controle da variável t_trace
	if(count < 50)
		t_trace += 0.2;
	
	// Definição e atualização de variáveis
	r = 100;
	phi += 0.1/(2*3.14);
	theta += 0.1/(2*3.14);

	count++;
	aux++;

	// Redesenha a cena e executa o timer novamente para ter uma animacao continua
    glutPostRedisplay();
    glutTimerFunc(10, ControleTempo, 1); 
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
	gluLookAt(-100, ycamera, 500,  // posição da câmera
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
			ycamera += 10;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			ycamera -=10;
		}
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer  
    EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}
/*
void TeclasEspeciais(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		........;
	}
	if (key == GLUT_KEY_DOWN) {
		........;
	}
	if (key == GLUT_KEY_RIGHT) {
		........;
	}
	if (key == GLUT_KEY_LEFT) {
		........;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void GerenciaTeclado(unsigned char key, int x, int y) {
	switch (key) {
		case ' ': // restaura posição inicial da camera
			............
			break;
		// movimentacao do observador
		case 'd':
			..............
			break;
		case 'a':
			...............
			break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	//GLUT_DOUBLE trabalha com dois buffers: um para renderização e outro para exibição
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(700,100);
	
	largura = 1800;
	altura = 1500;
	glutInitWindowSize(largura,altura);

	fAspect = (GLfloat)largura / (GLfloat)altura;

	ycamera = 500;
	glutTimerFunc(10, ControleTempo, 1);

	glutCreateWindow("Trabalho 1 - Eduardo e Henrique"); 
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela); // Função para ajustar o tamanho da tela
	
	glutMouseFunc(GerenciaMouse);
	//glutKeyboardFunc(GerenciaTeclado); // Define qual funcao gerencia o comportamento do teclado
	//glutSpecialFunc(TeclasEspeciais); // Define qual funcao gerencia as teclas especiais
	
	Inicializa();
	glutMainLoop();
}
