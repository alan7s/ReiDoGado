#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "glm.h"
#include "glmint.h"
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h> //som
#include <SDL/SDL_mixer.h> //som
#define inicioVaca -0.05

//Desenvolvido por: Alan Ferreira e Lucas Loscheider

struct Mouse{ //camera fixa mas com movimento
    int x,y,z;
}mouse;

struct Camera{
    int x,y,z;
}camera;

float move_y=-0.22, move_z=-0.4, decremento_y=-0.012, decrementa_x=0.010,  decrementa_x_aux=0.010, decrementa_z=-0.010, girar_x=-0.0001, girar_y=-0.22, girar_z=-0.55; //VACA
float girar_no_x, girar_no_z, decremento_girar_y=-0.012; //porco
float voar_x=-1, voar_z=-1;
int CAM=1, mousex=0, mousey=0, virar[2]={0,0};
float phi=90,teta=0, teta1=0, incrementa_teta1=0.104;
float sol=5, dia=1, ligaDesliga=0;
int l_pressionado=2;
float cruzar_x=-0.4, incrementa_cruzar_x=0.001, cruzamento_fim=30, cruzamento_inicio=0, ativa_cruzar=1, nasceu=0, ponto_nascimento=-0.5; //coelhos cruzando

float move_x=inicioVaca;
GLMmodel *fazendaModel = NULL, *vacaModel = NULL, *porcoModel = NULL, *passaroModel = NULL, *galinhaModel = NULL;
GLMmodel *coelhoModel = NULL, *coelhinhoModel = NULL;

//Luz variaveis
GLfloat luzAmbiente[4]={1.00, 1.00, 1.00, 1.00};
GLfloat luzDifusa[4]={0.5, 0.5, 0.5, 1.00};
GLfloat luzEspecular[4]={1.00, 1.00, 1.00, 1.00};

//som
void tocar_musica(char const nome[40], int loop){
        Mix_Chunk *som = NULL;
        int canal;
   int canal_audio=2;
        int taxa_audio = 22050;
        Uint16 formato_audio = AUDIO_S16SYS;
        int audio_buffers = 4096;
        if(Mix_OpenAudio(taxa_audio, formato_audio, canal_audio, audio_buffers) != 0) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
       som = Mix_LoadWAV(nome);
        if(som == NULL) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
       Mix_HaltChannel(-1);
       canal = Mix_PlayChannel( -1, som, loop);
        if(canal == -1) {
                printf("Não pode inicializar audio: %s\n", Mix_GetError());
        }
}

void parar_musica(){
 Mix_HaltChannel(-1);
}

void posicaoCamera(int x, int y){
    double mudax, muday;
    mudax=x-mousex;
    muday=y-mousey;
    teta=(teta+mudax/150);
    phi=(phi-muday/150);
    if(phi>=180){
        phi=180;
    }
    mousex=x;
    mousey=y;
    glutPostRedisplay();
}

void fps(int tempo){
    glutPostRedisplay();
    glutTimerFunc(tempo,fps,10);
}

void redimensiona(int w, int h){
    glEnable(GL_DEPTH_TEST);
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 0.2, 400.0);
    glMatrixMode(GL_MODELVIEW);
}

void inicializa(){
    glClearColor(1,1,1,1);
    glEnable(GL_BLEND);
    mouse.x=0;
    mouse.y=0;
    mouse.z=0;
    //som de fundo
    tocar_musica("som/fazenda.wav",-1);
    //iluminacao inicializar ativo
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

/*carregar obj's*/
void desenhaPorco(){
    if(!porcoModel){
        porcoModel = glmReadOBJ("model/fazenda/animais/porco.obj");
        if(!porcoModel){
            exit(0);
        }
        glmUnitize(porcoModel);
        glmFacetNormals(porcoModel);
        glmVertexNormals(porcoModel, 90.0, 1);
    }
    glmDraw(porcoModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaCoelho(){
    if(!coelhoModel){
        coelhoModel = glmReadOBJ("model/fazenda/animais/coelho.obj");
        if(!coelhoModel){
            exit(0);
        }
        glmUnitize(coelhoModel);
        glmFacetNormals(coelhoModel);
        glmVertexNormals(coelhoModel, 90.0, 1);
    }
    glmDraw(coelhoModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaCoelhinho(){
    if(!coelhinhoModel){
        coelhinhoModel = glmReadOBJ("model/fazenda/animais/coelhinho.obj"); //testando loader
        if(!coelhinhoModel){
            exit(0);
        }
        glmUnitize(coelhinhoModel);
        glmFacetNormals(coelhinhoModel);
        glmVertexNormals(coelhinhoModel, 90.0, 1);
    }
    glmDraw(coelhinhoModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaGalinha(){
    if(!galinhaModel){
        galinhaModel = glmReadOBJ("model/fazenda/animais/galinha.obj");
        if(!galinhaModel){
            exit(0);
        }
        glmUnitize(galinhaModel);
        glmFacetNormals(galinhaModel);
        glmVertexNormals(galinhaModel, 90.0, 1);
    }
    glmDraw(galinhaModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaVaca(){
    if(!vacaModel){
        vacaModel = glmReadOBJ("model/fazenda/animais/bigavaca.obj"); //testando loader
        if(!vacaModel){
            exit(0);
        }
        glmUnitize(vacaModel);
        glmFacetNormals(vacaModel);
        glmVertexNormals(vacaModel, 90.0, 1);
    }
    glmDraw(vacaModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaPassaro(){
    if(!passaroModel){
        passaroModel = glmReadOBJ("model/fazenda/animais/passarinho.obj"); //testando loader
        if(!passaroModel){
            exit(0);
        }
        glmUnitize(passaroModel);
        glmFacetNormals(passaroModel);
        glmVertexNormals(passaroModel, 90.0, 1);
    }
    glmDraw(passaroModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}

void desenhaFazenda(){
    if(!fazendaModel){
        fazendaModel = glmReadOBJ("model/fazenda/fazendinha2.obj"); //testando loader
        if(!fazendaModel){
            exit(0);
        }
        glmUnitize(fazendaModel);
        glmFacetNormals(fazendaModel);
        glmVertexNormals(fazendaModel, 90.0, 1);
    }
    glmDraw(fazendaModel, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
}
/*alguns movimentos*/
void girar(int valor){
    if(teta1<=2*M_PI){
        teta1=teta1+incrementa_teta1;
        girar_no_x=girar_x+0.1*cos(teta1);
        girar_no_z=girar_z+0.1*sin(teta1);
    }
    if(teta1>2*M_PI){
        teta1=0;
    }
    if(girar_no_x>0){
        virar[1]=4;
    }else{
        virar[1]=2;
    }
    //andar pulando
    if(girar_y<=0.22){
        decremento_girar_y=-1*decremento_girar_y;
    }if(girar_y>=0.233){
        decremento_girar_y=-1*decremento_girar_y;
    }
    girar_y+=decremento_girar_y;
    glutPostRedisplay();
    glutTimerFunc(valor,girar,valor);
}

void move(int valor){
    if(move_y<=0.22){
        decremento_y=-1*decremento_y;
    }if(move_y>=0.233){
        decremento_y=-1*decremento_y;
    }
    move_y+=decremento_y;
    //decrementa_x=-0.010;
    if(move_x==inicioVaca)
        move_x+=decrementa_x;
    if(move_x>inicioVaca){
       if(move_x>0.18)
       {
            virar[0]=1;
            decrementa_x=0;

            if(move_z<=-0.4&&move_z>=-0.64)
            {
                move_z+=decrementa_z;
                if(move_z<=-0.64)
                {
                    virar[0]=2;
                    move_z-=decrementa_z;
                    decrementa_x=(-1)*decrementa_x_aux;
                }
            }
       }
    }
       if(move_x<=inicioVaca)
       {
       decrementa_x=0;
            virar[0]=3;
            if(move_z<=-0.4&&move_z>=-0.64)
            {
                move_z-=decrementa_z;
                if(move_z>=-0.4)
                {
                    virar[0]=4;
                    move_z+=decrementa_z;
                    decrementa_x=decrementa_x_aux;

                }
            }

       }
       move_x+=decrementa_x;
    //move_x=0.0, move_y=-0.22, move_z=-0.4, decremento_y=-0.012, decrementa_x=0.010, decrementa_z=-0.010;
    glutPostRedisplay();
    glutTimerFunc(valor,move,valor);
}
/*altenar dia e noite*/
void diaNoite(int valor){
    if(dia==1){
        sol=sol+0.01;
    }else{
        sol=sol-0.01;
    }
    if(sol>5){
            dia=0;
    }
    if(sol<-5){
        sol=5;
    }
    glutPostRedisplay();
    glutTimerFunc(valor,diaNoite,valor);
}

void desenhaCena(){
    camera.x=2*sin(phi)*cos(teta);
    camera.z=2*sin(phi)*sin(teta);
    camera.y=2*cos(phi);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if(CAM==1){ //camera fixa
        gluLookAt(-1, 0.5, -1.5, 0, 0, 0, 0, 1, 0);
    }
    if(CAM==2){ //muito bugado, enjoa de assistir.. desistimos de implementar resto
        if(virar[0]==3){
            gluLookAt(move_x,-0.20,move_z,(2*sin(90)*cos(90)),0.362813,(2*sin(90)*sin(90)),0,1,0);
        }
        if(virar[0]==1){
            gluLookAt(move_x-0.05,-0.20,move_z,(2*sin(90)*cos(-90)),0.362813,(2*sin(90)*sin(-90)),0,1,0);
        }
        if(virar[0]==4){
            gluLookAt(move_x,-0.20,move_z,(2*sin(90)*cos(270)),0.362813,(2*sin(90)*sin(270)),0,1,0);
        }
        if(virar[0]==2){
            gluLookAt(move_x,-0.20,move_z,(2*sin(90)*cos(-360)),0.362813,(2*sin(90)*sin(-360)),0,1,0);
        }
    }
    if(CAM==3){ //'movimentar mundo com mouse'
        gluLookAt(mouse.x+camera.x,camera.y,mouse.z+camera.z,mouse.x+0,0,mouse.z+0,0,1,0);
    }
    /*definindo materiais*/
        GLfloat luzPosicao[4]={sol, 0.00, 0.00, 1.00 };
        glEnable(GL_LIGHT_MODEL_AMBIENT);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
        glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    /*desenhando fazenda 'chão'*/
    glPushMatrix();
      desenhaFazenda();
    glPopMatrix();
    /*colocando vaca no mundo*/
    glPushMatrix();
      glTranslatef(move_x,move_y,move_z); //(frente/tras, cima/baixo ,esquerda/direita) //variam de 1 a -1
      if(virar[0]==1){
        glRotatef(90,0,1,0);
      }
      if(virar[0]==2){
        glRotatef(180,0,1,0);
      }
      if(virar[0]==3){
        glRotatef(270,0,1,0);
      }
      if(virar[0]==4){
        glRotatef(360,0,1,0);
      }
      desenhaVaca();
    glPopMatrix();
    /*colocando porco no mundo*/
    glPushMatrix();
      glTranslated(girar_no_x,girar_y,girar_no_z);
      if(virar[1]==1){
        glRotatef(90,0,1,0);
      }
      if(virar[1]==2){
        glRotatef(180,0,1,0);
      }
      if(virar[1]==3){
        glRotatef(270,0,1,0);
      }
      if(virar[1]==4){
        glRotatef(360,0,1,0);
      }
      desenhaPorco();
    glPopMatrix();
    /*colocando pássaros no mundo*/
    glPushMatrix();
        glTranslatef(voar_x,0,0);
        desenhaPassaro();
        voar_x=voar_x+0.01;
        if(voar_x>8){
            voar_x=-2;
        }
    glPopMatrix();

    glPushMatrix(); //passaro 2 voando
        glTranslatef(0.2,0.2,voar_z);
        glRotatef(-90,0,1,0);
        desenhaPassaro();
        voar_z=voar_z+0.01;
        if(voar_z>8){
            voar_z=-2;
        }
    glPopMatrix();

    glPushMatrix(); //galinha
        glTranslatef(move_x+0.8,move_y,move_z+0.1);
          if(virar[0]==1){
            glRotatef(90,0,1,0);
          }
          if(virar[0]==2){
            glRotatef(180,0,1,0);
          }
          if(virar[0]==3){
            glRotatef(270,0,1,0);
          }
          if(virar[0]==4){
            glRotatef(360,0,1,0);
          }
        desenhaGalinha();
    glPopMatrix();

    glPushMatrix(); //coelho femea
        glTranslatef(-0.5,move_y,0);
        desenhaCoelho();
    glPopMatrix();

    glPushMatrix(); //coelho macho
        glTranslatef(cruzar_x,move_y,0);
        desenhaCoelho();
        if(ativa_cruzar==1){
            cruzar_x=cruzar_x-incrementa_cruzar_x;
            if(cruzar_x<-0.49){
                incrementa_cruzar_x=0;
                cruzamento_inicio+=0.1;
            }
            if(cruzamento_inicio>=cruzamento_fim){
                incrementa_cruzar_x=0.001;
                cruzamento_inicio=0;
                ativa_cruzar=0;
            }
        }
        if(ativa_cruzar==0){
            cruzar_x+=0.001;
            nasceu=1;
        }
        if(cruzar_x>=-0.39){
            cruzar_x=-0.4;
            ativa_cruzar=1;
        }
    glPopMatrix();

    if(nasceu==1){ //filhotes de coelhos que se suicidam na água após nascer
        glPushMatrix();
            glTranslatef(ponto_nascimento,move_y,0); //-0.5
            glRotatef(-90,0,1,0);
            ponto_nascimento=ponto_nascimento+0.001;
            if(ponto_nascimento>=-0.1){
                ponto_nascimento=-0.5;
                nasceu=0;
            }
            desenhaCoelhinho();
        glPopMatrix();
    }
    glutSwapBuffers();
}
void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
    case 27: //sai do programa
        exit(0);
        break;
    case '1': //escolhe camera fixa em cima
        CAM=1;
        break;
    case '2': //camera terceira pessoa (era a intenção.. mas parece com uma de primeira pessoa)
        CAM=2;
        break;
    case '3': //camera controlada pelo mouse
        CAM=3;
        break;
    case 'l': //ativa e desativa modelo de iluminação
        if(l_pressionado%2==0){
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            ligaDesliga=1;
        }else{
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            ligaDesliga=0;
        }l_pressionado++;
    default:
        break;
    }
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitContextVersion(1,1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Rei do Gado");
    //glutEnterGameMode(); //fullscreen
    //glutSetCursor(GLUT_CURSOR_NONE); //ocultar mouse
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclaPressionada);
    glutTimerFunc(10, fps, 10);
    glutPassiveMotionFunc(posicaoCamera);
    inicializa();
    glutTimerFunc(80,move,80);
    glutTimerFunc(80,girar,80);
    glutTimerFunc(80,diaNoite,80);
    glutMainLoop();
}
