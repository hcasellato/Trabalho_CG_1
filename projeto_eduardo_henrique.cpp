//*****************************************************
//
// Código para visualização de um pêndulo!
//
// Eduardo Campos     (13838537)
// Henrique Casellato (13781858)
// 
// g++ -o projeto_eduardo_henrique projeto_eduardo_henrique.cpp -lGL -lGLU -lglut -lm && ./projeto_eduardo_henrique
//
//*****************************************************
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;

GLfloat angle, fAspect, largura, altura, xcamera, ycamera, zcamera;

// Variáveis para coordenadas esféricas
double phi,theta,r;
float L = 200;

// Vetores para desenho de traço em pontos
double x[10000],y[10000],z[10000];
float aux;

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

// FUNÇÕES AUXILIARES
float D(float a, float b ,float c,float d,float e,float f){
	return sqrt(pow(a-b,2)+pow(c-d,2)+pow(e-f,2));
}

bool atualizar_cauda(int aux){
	int dist = 0;
	for(int n = 0;n<INT_DESCRITIVO;n++){
		dist  += D(trace_x[aux][0],trace_x[aux][n],trace_y[aux][0],trace_y[aux][n],trace_z[aux][0],trace_z[aux][n])/100;
	}
	if(dist>1){return 1;}
	else{return 0;}
}

// Função para desenhar quadrado na cena
void fazer_quadrado(){
	glPushMatrix();
	glTranslatef(0,0,L/2);

    glBegin(GL_LINES); // Desenha as linhas que conectam os vértices do cubo
	glColor3f(0.5f, 0.5f, 0.5f);

    // Vértices da face inferior

    glVertex3f(-L, -L, -L); glVertex3f(L, -L, -L);  // Linha 1
    glVertex3f(L, -L, -L); glVertex3f(L, L, -L);    // Linha 2
    glVertex3f(L, L, -L); glVertex3f(-L, L, -L);    // Linha 3
    glVertex3f(-L, L, -L); glVertex3f(-L, -L, -L);  // Linha 4

    // Vértices da face superior
    glVertex3f(-L, -L, L); glVertex3f(L, -L, L);    // Linha 5
    glVertex3f(L, -L, L); glVertex3f(L, L, L);      // Linha 6
    glVertex3f(L, L, L); glVertex3f(-L, L, L);      // Linha 7
    glVertex3f(-L, L, L); glVertex3f(-L, -L, L);    // Linha 8

    // Conectando as faces inferior e superior
    glVertex3f(-L, -L, -L); glVertex3f(-L, -L, L);  // Linha 9
    glVertex3f(L, -L, -L); glVertex3f(L, -L, L);    // Linha 10
    glVertex3f(L, L, -L); glVertex3f(L, L, L);      // Linha 11
    glVertex3f(-L, L, -L); glVertex3f(-L, L, L);    // Linha 12

    glEnd();
	glPopMatrix();
}

// Função para desenhar cauda do pêndulo
void traço_bola(){
    // Desenha pontos iniciais
	for(int j = 0;j<INT_DESCRITIVO;j++){
		trace_x[0][j] = r*cos(0)*sin(0);
		trace_y[0][j] = r*sin(0)*sin(0);
		trace_z[0][j] = r*(1-cos(0));
	
	}
	
	// Desenha nuvem de pontos até nro INT_DESCRITIVO
	for(int j = 0;j<INT_DESCRITIVO;j++){
		// Pontos da cauda por função t/e^0.01*t
		trace_x[count][j] = r*cos(phi)*sin(theta) + cos(t_trace+j)*t_trace/exp(t_trace*0.01);
		trace_y[count][j] = r*sin(theta)*sin(phi) + sin(t_trace+j)*t_trace/exp(t_trace*0.01);
		trace_z[count][j] = r*(1-cos(theta));
	
		// Diminui a cauda progressivamente
		if(count>300){
			aux = 300;
			for(int k = count;k>count-300;k--){
				trace_x[k][j] -= (cos(t_trace+j)*t_trace/exp(t_trace*0.01))*(float(-aux/300 +1));
				trace_y[k][j] -= (sin(t_trace+j)*t_trace/exp(t_trace*0.01))*(float(-aux/300 +1));
				aux--;
			}
		}
	    
	    // Desenho de fato
	    glBegin(GL_POINTS);
	    glColor3f(0.0f, 0.0f, 0.0f);
	    for (int j =0; j<INT_DESCRITIVO;j++){
		    color[0] = float(j)/100;
		    glColor3f(color[0]*5,color[0],0.f);

	        // Só desenha a cauda depois de 300 pontos, para ter um desenho bonito
		    if(count>300){
		        for(int i=count; i >count-26 ; i--) 
		        {
		            glVertex3f((trace_x[i][j]),(trace_y[i][j]),trace_z[i][j]);
		        }
        	}
	    }
	    glEnd();
    }
}

// Função para desenhar projeções nas faces do quadrado
void desenhar_projeções(void){
	glBegin(GL_POINTS);			
        for (int i = 0; i < count; i++) {
            glVertex3f(L, y[i],z[i]);
            glVertex3f(-L, y[i],z[i]);
			glVertex3f(x[i],-L,z[i]);
		}
	glEnd();
}

// Desenha fio da bola
void desenhar_linha(void){
	glBegin(GL_LINE_LOOP);			
		glVertex3f(0,200,100);
		glVertex3f(r*cos(phi)*sin(theta),r*sin(theta)*sin(phi),r*(1-cos(theta)));
	glEnd();
}

// Função que desenha traço de pontos
void draw_points(){
	x[count] = r*cos(phi)*sin(theta);
	y[count] = r*sin(theta)*sin(phi);
	z[count] = r*(1-cos(theta));
	 glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer  
	glViewport(0, 0, largura, altura);

	glColor3f(1.0f, 1.0f, 1.0f);			
	desenhar_linha();
	fazer_quadrado();
	traço_bola();
	desenhar_projeções();
	glPushMatrix();
		glColor3f(0.6f, 0.0f, 0.3f);	
		glTranslated(r*cos(phi)*sin(theta),r*sin(theta)*sin(phi),r*(1-cos(theta)));
		glutSolidSphere(10, 10, 10);
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
	if (rand()%100>50){
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
	GLfloat luzAmbiente[4]={0.2, 0.2, 0.2, 1.0};   // {R, G, B, alfa}
	GLfloat luzDifusa[4]={0.3, 0.3, 0.3, 0.8};	   // o 4o componente, alfa, controla a opacidade/transparência da luz
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};
	GLfloat posicaoLuz[4]={0,200,100, 1.0};  // aqui o 4o componente indica o tipo de fonte:
                                                    // 0 para luz direcional (no infinito) e 1 para luz pontual (em x, y, z)
	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0, 1.0, 1.0, 1.0}; 
	GLint especMaterial = 100;     // 0 a 128

 	// Especifica a cor de fundo da janela
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Habilita e define o modelo de colorização 
	glShadeModel(GL_SMOOTH);  // modelo de GOURAUD: a cor de cada ponto da primitiva é interpolada a partir dos vértices

	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
	
	//glDepthFunc(GL_LESS); // O fragmento mais próximo será desenhado
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
	gluLookAt(xcamera, ycamera, zcamera,  // posição da câmera
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
			zcamera += 20;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			zcamera -=20;
		}
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer  
    EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}
void TeclasEspeciais(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ycamera += 20;  
	}
	if (key == GLUT_KEY_DOWN) {
		ycamera -= 20;  
	}
	if (key == GLUT_KEY_RIGHT) {
		xcamera += 20;  
	}
	if (key == GLUT_KEY_LEFT) {
		xcamera -= 20;  
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //aplica o zBuffer  
    EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	//GLUT_DOUBLE trabalha com dois buffers: um para renderização e outro para exibição
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(700,100);
	
	largura = 600;
	altura = 500;

    xcamera = -220;
	ycamera = 260;
    zcamera = 700;
	glutInitWindowSize(largura,altura);

	fAspect = (GLfloat)largura / (GLfloat)altura;

	glutTimerFunc(10, ControleTempo, 1);

	glutCreateWindow("Trabalho 1 - Eduardo e Henrique"); 
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela); // Função para ajustar o tamanho da tela
	
	glutMouseFunc(GerenciaMouse);

	glutSpecialFunc(TeclasEspeciais); // Define qual funcao gerencia as teclas especiais
	
	Inicializa();
	glutMainLoop();
}
