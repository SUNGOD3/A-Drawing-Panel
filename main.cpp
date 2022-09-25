// This is a simple startup program example for using GLUT/FreeGLUT + OpenGL2, which is using legacy fixed pipeline.// !!! GLUT/FreeGLUT IS OBSOLETE PREHISTORIC SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT today, you are being abused. Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glut.h>
#include <imgui_impl_opengl2.h>
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#define    SIZEX   800
#define    SIZEY   800

#define    MAX_CHAR 128 

#define    MY_QUIT -1
#define    MY_CLEAR -2
#define    MY_SAVE  -3
#define    MY_BLEND -4
#define    MY_LOAD  -5

#define    ERASER  0
#define    WHITE   1
#define    RED     2
#define    GREEN   3
#define    BLUE    4
#define	   BLACK   5
#define    REV	   6
#define    MOSAIC  7
#define    MONO    8

#define    POINT   1
#define    LINE    2
#define    POLYGON 3
#define    CIRCLE  4
#define    CURVE   5
#define	   DASH	   6	

using namespace std;

typedef    int   menu_t;
menu_t     top_m, color_m, file_m, type_m, fill_m, size_m, fscreen_m;

int        height=512, width=512;
unsigned char  image[SIZEX*SIZEY][4];  /* Image data in main memory */
//儲存window狀態 
unsigned char  tmpimg[SIZEX*SIZEY][4];

int        pos_x=-1, pos_y=-1;
float      myColor[3],bColor[3]={1,1,1};
int        obj_type = 1, fill_type = 1;
int        first,first2;      /* flag of initial points for lines and curve,..*/
int        vertex[128][2]; /*coords of vertices */
int        side;         /*num of sides of polygon */
float      pnt_size=5.0;
int        isFirstCall = 1;
GLuint 	   lists;

/*------------------------------------------------------------*/ 

void display_func(void){
  	//顯示時橡皮擦顏色 glClearColor(myColor[0], myColor[1], myColor[2], 0);
  	glClear(GL_COLOR_BUFFER_BIT);	//全部塗白(橡皮擦) 
  	glColor3f(myColor[0], myColor[1], myColor[2]);	 //設定字型顏色
	glRasterPos2i(0, height-15);  //起始位置
  	glFlush();					//強制執行 
}

/*-------------------------------------------------------------
 * reshape callback function for window.
 */
void my_reshape(int new_w, int new_h){
  height = new_h;
  width = new_w;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double) width, 0.0, (double) height);
  glViewport(0,0,width,height);
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();   /*---Trigger Display event for redisplay window*/
}

/*---------------------------------------------------------
 * Procedure to draw a polygon
 */
void draw_polygon(){//填滿一個畫完的多邊形 
	if(fill_type==1){
	  	glBegin(GL_POLYGON);
	    for(int i=0;i<side;i++)
	    	glVertex2f(vertex[i][0], height-vertex[i][1]);
	  	glEnd();
  	}
  	else{
  		glBegin(GL_LINES);
	    glVertex2f(vertex[side-1][0], height-vertex[side-1][1]);
	    glVertex2f(vertex[0][0], height-vertex[0][1]);
	  	glEnd();
	}
  	glFinish();
  	side = 0;    /* set side=0 for next polygon */
}

/*------------------------------------------------------------
 * Procedure to draw a circle
 */
void draw_circle(){
  static GLUquadricObj *mycircle=NULL;

  if(mycircle==NULL){
    mycircle = gluNewQuadric();
    gluQuadricDrawStyle(mycircle,GLU_FILL);
  }
  glPushMatrix();
  glTranslatef(pos_x, height-pos_y, 0.0);
  gluDisk(mycircle,
       0.0,           /* inner radius=0.0 */
	  pnt_size*5,          /* outer radius=pnt_size */
	  16,            /* 16-side polygon */
	   3);
	if(fill_type==2){
		glColor3f(1.0, 1.0, 1.0);
		gluDisk(mycircle,
	       0.0,           /* inner radius=0.0 */
		  pnt_size*4,          /* outer radius=pnt_size */
		  16,            /* 16-side polygon */
		   3);
		glColor3f(myColor[0], myColor[1], myColor[2]);
	}
  glPopMatrix();
}


/*------------------------------------------------------------
 * Callback function handling mouse-press events
 */
void mouse_func(int button, int state, int x, int y){
	first=0;
  if(button!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)
    return;

  switch(obj_type){
  case POINT:
    glPointSize(pnt_size);     /*  Define point size */
    glBegin(GL_POINTS);     /*  Draw a point */
    glVertex2f(x, height-y);
    glEnd();
    break;
  case LINE:
    if(first2==0){
      first2 = 1;
      pos_x = x; pos_y = y;
	  glPointSize(pnt_size);
      glBegin(GL_POINTS);   /*  Draw the 1st point */
	    glVertex3f(x, height-y, 0);
      glEnd();
    }else{
      first2=0;
      glLineWidth(pnt_size);     /* Define line width */
      glBegin(GL_LINES);    /* Draw the line */
        glVertex2f(pos_x, height - pos_y);
	    glVertex2f(x, height - y);
      glEnd();
    }
    break;
  case POLYGON:  /* Define vertices of poly */
    if(side==0){
      vertex[side][0] = x; vertex[side][1] = y;
      side ++;
    }else{
      if(fabs(vertex[side-1][0]-x) + fabs(vertex[side-1][1]-y)<2)//點兩次相同點 
	    draw_polygon();
      else{
      	glLineWidth(pnt_size);
	    glBegin(GL_LINES);
          glVertex2f(vertex[side-1][0], height-vertex[side-1][1]);
	      glVertex2f(x, height-y);
	    glEnd();
	    vertex[side][0] = x; 
		vertex[side][1] = y;
	    side ++;
      }
    }
    break;
  case CIRCLE:
    pos_x = x; pos_y = y;
    draw_circle();
    break;
  default:
      break;
  }
  glFinish();
}

/*-------------------------------------------------------------*/ 

void motion_func(int  x, int y){
  	if(obj_type<CURVE) return;	//若狀態為曲線才做動作 
  	if(obj_type==DASH){
	  glBegin(GL_POINTS);
	  glPointSize(pnt_size);
	  glVertex2f(x, height-y);
	  glEnd();
	}
	else if(obj_type==CURVE){
		if(first==0){
			first = 1;
			pos_x = x; pos_y = y;
		}
		else{
			glLineWidth(pnt_size);
			glBegin(GL_LINES);
		  	glVertex3f(pos_x, height-pos_y, 0.0);
		  	glVertex3f(x, height - y, 0.0);
			glEnd();
			pos_x = x; pos_y = y;
		}
	}
  	glFinish();
}

/*--------------------------------------------------------
 * procedure to clear window
 */
void init_window(void){
	/*Do nothing else but clear window to black*/
	isFirstCall = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double) width, 0.0, (double) height);
	glViewport(0,0,width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


/*------------------------------------------------------
 * Procedure to initialize data alighment and other stuff
 */
void init_func(){   
	isFirstCall = 1;
	glReadBuffer(GL_FRONT);
    glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glFlush();
}

/*-----------------------------------------------------------------
 * Callback function for color menu
 */
void color_func(int value){
	switch(value){
		
		case ERASER:
			for(int i=0;i<3;++i){
				myColor[i] = bColor[i];
			}
		break;
		
		case WHITE:
			myColor[0] = myColor[1] = myColor[2] = 1.0;
		break;
		
		case RED:
			myColor[0] = 1.0;
			myColor[1] = myColor[2] = 0.0;
		break;
		
		case GREEN:
			myColor[0] = myColor[2] = 0.0;
			myColor[1] = 1.0;
		break;
		
		case BLUE:
			myColor[0] = myColor[1] = 0.0;
			myColor[2] = 1.0;
		break;
		
		case BLACK:
			myColor[0] = myColor[1] = myColor[2] = 0.0;
		break;
		
		default:
		break;
	}
	glColor3f(myColor[0], myColor[1], myColor[2]);
}

/*------------------------------------------------------------
 *
 */

void fill_func(int value){
	fill_type = value;
}

/*------------------------------------------------------------
 * Callback function for top menu.
 */
void file_func(int value){ 
  	if(value==MY_QUIT) exit(0);
  	else if(value==MY_CLEAR) init_window();
  	else if(value==MY_SAVE){ /* Save current window */
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	for(int i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  	for(int j=0;j<height;j++)
			if(image[i*width+j][0]==0 && image[i*width+j][1]==0 && image[i*width+j][2]==0) image[i*width+j][3] = 0;
			else image[i*width+j][3] = 127; /* Other pixels have A=127*/
  	}
	else if(value==MY_LOAD){ /* Restore the saved image */
	 	glRasterPos2i(0, 0);
     	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
  	}
	else if(value==MY_BLEND){ /* Blending current image with the saved image */	
	  	glEnable(GL_BLEND); 
	  	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      	glRasterPos2i(0, 0);
      	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	  	glDisable(GL_BLEND);
  	}

  	glFlush();
}
//形成互補色 
void rev_func(){
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
	for(int i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  	for(int j=0;j<height;j++){
	  		for(int k=0;k<4;++k){
	  			tmpimg[i*width+j][k]=255-tmpimg[i*width+j][k];
			}
	  	}
			
	glRasterPos2i(0, 0);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
}
//打碼 
void mosaic_func(){
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
	cout << tmpimg[5][5]; 
	for(int i=1;i<width;i+=3)   /* Assign 0 opacity to black pixels */
	  	for(int j=1;j<height;j+=3){
	  		for(int k=0;k<4;++k){ 
			  	int tmp=0;
				for(int ii=-1;ii<=1;++ii){
					for(int jj=-1;jj<=1;++jj){
						tmp+=tmpimg[(i+ii)*width+(j+jj)][k];
					}
				}
				tmp/=9;
				for(int ii=-1;ii<=1;++ii){
					for(int jj=-1;jj<=1;++jj){
						tmpimg[(i+ii)*width+(j+jj)][k]=tmp;
					}
				}
			}
	  	}
	glRasterPos2i(0, 0);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
}
//黑白化 
void monochrome_func(){
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
	for(int i=0;i<width;i++)   /* Assign 0 opacity to black pixels */
	  	for(int j=0;j<height;j++){
	  		int tmp=0;
	  		for(int k=0;k<4;++k){ 
	  			tmp+=tmpimg[i*width+j][k];	
			}
			tmp/=4;
			for(int k=0;k<4;++k){ 
	  			tmpimg[i*width+j][k]=tmp;	
			}
	  	}	
	glRasterPos2i(0, 0);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpimg);
}

void fscreen_func(int value){
	bool d=0;
	switch(value){
		case WHITE:
			bColor[0] = bColor[1] = bColor[2] = 1.0;
		break;
		
		case RED:
			bColor[0] = 1.0;
			bColor[1] = bColor[2] = 0.0;
		break;
		
		case GREEN:
			bColor[0] = bColor[2] = 0.0;
			bColor[1] = 1.0;
		break;
		
		case BLUE:
			bColor[0] = bColor[1] = 0.0;
			bColor[2] = 1.0;
		break;
		
		case BLACK:
			bColor[0] = bColor[1] = bColor[2] = 0.0;
		break;
		
		case REV:
			rev_func();
			d=1;
		break;
		
		case MONO:
			monochrome_func();
			d=1;
		break;
		
		case MOSAIC:
			mosaic_func();
			d=1;
		break;
		
		default:
		break;
	}
	if(d==0){
		glClearColor(bColor[0], bColor[1], bColor[2], 0);
		display_func();
	}
	glFlush();
}

void size_func(int value){
	if(value==1){
	  if(pnt_size<50.0) pnt_size += 2 ;
	}else{
	  if(pnt_size>1.0) pnt_size -= 2;
	}
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value){
	
}


/*-------------------------------------------------------------
 * Callback Func for type_m, define drawing object
 */
void draw_type(int value){
  obj_type = value;
  if(value==LINE||value==CURVE)
    first = 0;
  else if(value==POLYGON) side = 0;
}


void draw_grid(){
	//畫格子
	glBegin(GL_LINES);
	glColor3f(myColor[0], myColor[1], myColor[2]);
	for (float i = -800; i < 800; i+=10) {
		glVertex2f(-800.0, i);
		glVertex2f(800.0, i);
		glVertex2f(i, 800.0);
		glVertex2f(i, -800.0);
	}
	glEnd();
}
/*--------------------------------------------------------------
 * Callback function for keyboard event.
 * key = the key pressed,
 * (x,y)= position in the window, where the key is pressed.
 */

void keyboard(unsigned char key, int x, int y){
	glRasterPos2i(x, height-y);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int) key);
  switch(key){
  	case 'Q': case 'q': exit(0);
    case 'C': case 'c': init_window();
    case 'G': case 'g': draw_grid(); 
    case 'S': case 's': file_func(MY_SAVE);
    case 'L': case 'l': file_func(MY_LOAD);
	break;
  }
}
/*---------------------------------------------------------------
 * Main procedure sets up the window environment.
 */

int main(int argc, char **argv){

	glutInit(&argc, argv);   
	glutInitWindowPosition(0,0);					//window開始位子 
	glutInitWindowSize(width, height);			//window長寬 
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA); 	//window mode
	init_func();
	
	glutCreateWindow("Menu"); 					//window生成 
	
	glutDisplayFunc(display_func); 				//window顯示時的func 
	glutReshapeFunc(my_reshape);  				//window大小改變時的func 
	glutKeyboardFunc(keyboard); 					//keyboard event的func 
	glutMouseFunc(mouse_func);  					//mouse event的func 
	glutMotionFunc(motion_func);					//motion (鼠標移動) event的func 
	
	color_m = glutCreateMenu(color_func); /* Create color-menu */
	glutAddMenuEntry("white", WHITE);
	glutAddMenuEntry("black", BLACK);
	glutAddMenuEntry("red", RED);
	glutAddMenuEntry("green", GREEN);
	glutAddMenuEntry("blue", BLUE);
	glutAddMenuEntry("eraser", ERASER);
	
	file_m = glutCreateMenu(file_func);   /* Create another menu, file-menu */
	glutAddMenuEntry("save", MY_SAVE);
	glutAddMenuEntry("load", MY_LOAD);
	glutAddMenuEntry("blend", MY_BLEND);
	glutAddMenuEntry("clear", MY_CLEAR);
	glutAddMenuEntry("quit", MY_QUIT);
	
	type_m = glutCreateMenu(draw_type);   /* Create draw-type menu */
	glutAddMenuEntry("Point", POINT);
	glutAddMenuEntry("Line", LINE);
	glutAddMenuEntry("Poly", POLYGON);
	glutAddMenuEntry("Curve", CURVE);
	glutAddMenuEntry("Circle", CIRCLE);
	glutAddMenuEntry("Dash", DASH);
	
	size_m = glutCreateMenu(size_func);
	glutAddMenuEntry("Bigger", 1);
	glutAddMenuEntry("Smaller",2);
	
	fill_m = glutCreateMenu(fill_func);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Outline",2);
	
	fscreen_m = glutCreateMenu(fscreen_func);
	glutAddMenuEntry("white", WHITE);
	glutAddMenuEntry("black", BLACK);
	glutAddMenuEntry("red", RED);
	glutAddMenuEntry("green", GREEN);
	glutAddMenuEntry("blue", BLUE);
	glutAddMenuEntry("reverse", REV);
	glutAddMenuEntry("mosaic", MOSAIC);
	glutAddMenuEntry("monochrome", MONO);
	
	top_m = glutCreateMenu(top_menu_func);/* Create top menu */
	glutAddSubMenu("colors", color_m);    /* add color-menu as a sub-menu */
	glutAddSubMenu("type", type_m);
	glutAddSubMenu("Size", size_m);
	glutAddSubMenu("fill", fill_m);      
	glutAddSubMenu("file", file_m); 
	glutAddSubMenu("fill_screen", fscreen_m);  
	glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	/*---Test whether overlay support is available --*/
	if(glutLayerGet(GLUT_OVERLAY_POSSIBLE)){
		fprintf(stderr,"Overlay is available\n");
	}
	else{
		fprintf(stderr,"Overlay is NOT available, May encounter problems for menu\n");
	}
	/*---Enter the event loop ----*/
	glutMainLoop();       
}
