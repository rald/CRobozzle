#include "gl2d.h"

// mingw header excluding GL_BGRA in windows
#ifndef GL_BGRA
#define GL_BGRA  0x80E1
#endif

static GLuint gCurrentTexture = 0;

/******************************************************************************

    IMPLEMENTATION

******************************************************************************/


/******************************************************************************

    Private Functions

******************************************************************************/



/******************************************************************************

   Private!!!
   Sets up OpenGL for 2d rendering

******************************************************************************/

void Init2D( const int width, const int height )
{

    gCurrentTexture = 0;
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDisable( GL_DEPTH_TEST );

    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0 );


    glDisable( GL_STENCIL_TEST );
    glDisable( GL_TEXTURE_1D );
    glDisable( GL_LIGHTING );
    glDisable( GL_LOGIC_OP );
    glDisable( GL_DITHER );
    glDisable( GL_FOG );

    glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
    glHint( GL_LINE_SMOOTH_HINT , GL_NICEST );

    glPointSize( 1.0 );
    glLineWidth( 1.0 );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, width, height, 0, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glTranslatef( 0.375, 0.375, 0.0 );

}

/******************************************************************************

   Private!!!
   Abstracted(to be)  LoadTexture function

******************************************************************************/

int LoadTexture(const char *filename)
{

    if ( !glfwLoadTexture2D( filename, GLFW_BUILD_MIPMAPS_BIT | GLFW_ORIGIN_UL_BIT) )
    {
        glfwTerminate();
        printf( "Error loading %s", filename );
        return false;
    }


    return true;

}

/******************************************************************************


******************************************************************************/

void glScreenInit(const int width, const int height)
{

    glfwInit();
    
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);

    if( !glfwOpenWindow( width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) )
    {
        glfwTerminate();
    }

    glfwSetWindowTitle( "Easy GL2D" );


    double FOVy;
    double aspect;
    double znear;
    double zfar;

    // using screen info w and h as params
    glViewport( 0, 0, width, height );

    // Set current Mode to projection(ie: 3d)
    glMatrixMode( GL_PROJECTION );

    // Load identity matrix to projection matrix
    glLoadIdentity();

    // Set gluPerspective params
    FOVy = 90 / 2;                                     //45 deg fovy
    aspect = width / height;
    znear = 1;                                       //Near clip
    zfar = 500;                                      //far clip

    // use glu Perspective to set our 3d frustum dimension up
    gluPerspective( FOVy, aspect, znear, zfar );

    // Modelview mode
    // ie. Matrix that does things to anything we draw
    // as in lines, points, tris, etc.
    glMatrixMode( GL_MODELVIEW );

    // load identity(clean) matrix to modelview
    glLoadIdentity();

    glShadeModel( GL_SMOOTH );                 //'set shading to smooth(try GL_FLAT)
    glClearColor( 0.0, 0.0, 0.0, 1.0 );        //'set Clear color to BLACK
    glClearDepth( 1.0 );                       //'Set Depth buffer to 1(z-Buffer)
    glDisable( GL_DEPTH_TEST );                //'Disable Depth Testing so that our z-buffer works

    //'compare each incoming pixel z value with the z value present in the depth buffer
    //'LEQUAL means than pixel is drawn if the incoming z value is less than
    //'or equal to the stored z value
    glDepthFunc( GL_LEQUAL );

    //'have one or more material parameters track the current color
    //'Material is your 3d model
    glEnable( GL_COLOR_MATERIAL );


    //Enable Texturing
    glEnable( GL_TEXTURE_2D );


    //'Tell openGL that we want the best possible perspective transform
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    //'Disable Backface culling
    glDisable ( GL_CULL_FACE );

    glPolygonMode( GL_FRONT, GL_FILL );

    //'' enable blending for transparency
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDisable( GL_DEPTH_TEST );

    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0 );


    glDisable( GL_STENCIL_TEST );
    glDisable( GL_TEXTURE_1D );
    glDisable( GL_LIGHTING );
    glDisable( GL_LOGIC_OP );
    glDisable( GL_DITHER );
    glDisable( GL_FOG );

    glHint( GL_POINT_SMOOTH_HINT, GL_FASTEST );
    glHint( GL_LINE_SMOOTH_HINT , GL_FASTEST );

    glPointSize( 1.0 );
    glLineWidth( 1.0 );


    Init2D( width, height );


}


/******************************************************************************


******************************************************************************/

void glBlendMode( const GL2D_BLEND_MODE mode )
{
    switch ( mode )
    {
    	case GL2D_TRANS:
			glDisable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			break;
		case GL2D_SOLID:
			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
			break;
		case GL2D_BLENDED:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			break;
		case GL2D_GLOW:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_ONE, GL_ONE );
			break;
        case GL2D_ALPHA:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			break;
		case GL2D_BLACK:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_ZERO,GL_ONE_MINUS_SRC_ALPHA );
			break;
		default:
			glDisable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
	}

}


/******************************************************************************


******************************************************************************/

void glClearScreen()
{

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}


/******************************************************************************


******************************************************************************/

void glPutPixel( int x1, int y1, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    glBegin( GL_POINTS );
		glVertex2i( x1, y1 );
		glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}

/******************************************************************************


******************************************************************************/
void glLine( int x1, int y1, int x2, int y2, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    glBegin( GL_LINES );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y2 );
		glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}

/******************************************************************************


******************************************************************************/
void glBox( int x1, int y1, int x2, int y2, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    glBegin( GL_LINE_STRIP );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y1 );
		glVertex2i( x2, y2 );
		glVertex2i( x1, y2 );
		glVertex2i( x1, y1 );
	  glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}

/******************************************************************************


******************************************************************************/

void glBoxFilled( int x1, int y1, int x2, int y2, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    x2++;
    y2++;

    glBegin( GL_QUADS );

        glVertex2i	( x1,y1 );
        glVertex2i	( x1,y2 );
        glVertex2i	( x2,y2 );
        glVertex2i	( x2,y1 );

    glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}


/******************************************************************************


******************************************************************************/
void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    glBegin( GL_LINE_STRIP );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y2 );
		glVertex2i( x3, y3 );
		glVertex2i( x1, y1 );
	glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}

/******************************************************************************


******************************************************************************/
void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color )
{
    glDisable( GL_TEXTURE_2D );
    glColor4ubv ( (GLubyte*)(&color)) ;

    glBegin( GL_TRIANGLES );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y2 );
		glVertex2i( x3, y3 );
	glEnd();

    glEnable( GL_TEXTURE_2D );

    glColor4ub( 255, 255, 255, 255 );

}


/******************************************************************************

    Contributed by Michael "h4tt3n" Nissen(I added a filled version ;*))
    syntax: ellipse(center x, center y, semimajor axis, semiminor axis, angle in degrees, color)

******************************************************************************/
void glEllipse(int x, int y, int a, int b, int degrees, const GLuint color)
{

	// these constants decide the quality of the ellipse
	const float  pi = M_PI;
	const float  twopi  = 2 * pi;   //        two pi (radians in a circle)
	const int face_length  = 8;     //        approx. face length in pixels
	const int max_faces = 256;      //        maximum number of faces in ellipse
	const int min_faces = 16;       //        minimum number of faces in ellipse

	// approx. ellipse circumference (hudson's method)
	float h = ( a-b*a-b ) / (float)( a+b*a+b );
	float circumference = 0.25 * pi * (a+b) * (3* (1+h*0.25)+1 / (float)(1-h*0.25));

	// number of faces in ellipse
	int num_faces = circumference/face_length;

	// clamp number of faces
	if( num_faces > max_faces ) num_faces = max_faces;
	if( num_faces < min_faces ) num_faces = min_faces;

	// keep number of faces divisible by 4
	num_faces -= (num_faces & 3);

	// precalc cosine theta
	float angle = degrees * M_PI /180.0;
	float s   = sin(twopi/(float)num_faces);
	float c   = cos(twopi/(float)num_faces);
	float xx  = 1;
	float yy  = 0;
	float xt  = 0;
	float ax  = cos(angle);
	float ay  = sin(angle);


	// draw ellipse
	glDisable( GL_TEXTURE_2D );
	glColor4ubv( (GLubyte*)(&color) );

    int i;
	glBegin( GL_LINE_LOOP );

		for( i = 0; i < num_faces; i++ )
		{
		    xt = xx;
			xx = c * xx - s * yy;
			yy = s * xt + c * yy;
			glVertex2f( x+a*xx*ax-b*yy*ay, y+a*xx*ay+b*yy*ax );
		}

		glVertex2f( x+a*ax, y+a*ay );

	glEnd();

	glEnable( GL_TEXTURE_2D );

	glColor4ub( 255, 255, 255,255 );

}

/******************************************************************************

    Contributed by Michael "h4tt3n" Nissen(I added a filled version ;*))
    syntax: ellipse(center x, center y, semimajor axis, semiminor axis, angle in degrees, color)

******************************************************************************/
void glEllipseFilled(int x, int y, int a, int b, int degrees, const GLuint color)
{

	// these constants decide the quality of the ellipse
	const float  pi = M_PI;
	const float  twopi  = 2 * pi;   //        two pi (radians in a circle)
	const int face_length  = 8;     //        approx. face length in pixels
	const int max_faces = 256;      //        maximum number of faces in ellipse
	const int min_faces = 16;       //        minimum number of faces in ellipse

	// approx. ellipse circumference (hudson's method)
	float h = ( a-b*a-b ) / (float)( a+b*a+b );
	float circumference = 0.25 * pi * (a+b) * (3* (1+h*0.25)+1 / (float)(1-h*0.25));

	// number of faces in ellipse
	int num_faces = circumference/face_length;

	// clamp number of faces
	if( num_faces > max_faces ) num_faces = max_faces;
	if( num_faces < min_faces ) num_faces = min_faces;

	// keep number of faces divisible by 4
	num_faces -= (num_faces & 3);

	// precalc cosine theta
	float angle = degrees * M_PI /180.0;
	float s   = sin(twopi/(float)num_faces);
	float c   = cos(twopi/(float)num_faces);
	float xx  = 1;
	float yy  = 0;
	float xt  = 0;
	float ax  = cos(angle);
	float ay  = sin(angle);


	// draw ellipse
	glDisable( GL_TEXTURE_2D );
	glColor4ubv( (GLubyte*)(&color) );

    int i;
	glBegin( GL_TRIANGLE_FAN );

		for( i = 0; i < num_faces; i++ )
		{
		    xt = xx;
			xx = c * xx - s * yy;
			yy = s * xt + c * yy;
			glVertex2f( x+a*xx*ax-b*yy*ay, y+a*xx*ay+b*yy*ax );
		}

		glVertex2f( x+a*ax, y+a*ay );

	glEnd();

	glEnable( GL_TEXTURE_2D );

	glColor4ub( 255, 255, 255,255 );

}

/******************************************************************************

    Draws a sprite
	Parameters:
       x 		-> x position of the sprite
	   y 		-> y position of the sprite
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSprite( const int x, const int y, const int flipmode, const glImage *spr )
{
    int x1 = x;
    int y1 = y;
    int x2 = x + spr->width;
    int y2 = y + spr->height;

    float u1 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off + spr->u_width  : spr->u_off;
    float u2 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off 			       : spr->u_off + spr->u_width;
    float v1 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off + spr->v_height : spr->v_off;
    float v2 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off 			       : spr->v_off + spr->v_height;


    if ( spr->textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, spr->textureID );
        gCurrentTexture = spr->textureID;
    }

    glBegin( GL_QUADS );

        glTexCoord2f( u1, v1 ); glVertex2i( x1, y1 );
        glTexCoord2f( u1, v2 ); glVertex2i( x1, y2 );
        glTexCoord2f( u2, v2 ); glVertex2i( x2, y2 );
        glTexCoord2f( u2, v1 ); glVertex2i( x2, y1 );

    glEnd();


}

/******************************************************************************

    Draws a scaled sprite
	Parameters:
       x 		-> x position of the sprite
	   y 		-> y position of the sprite
	   scale    -> float value to scale the sprite (1.0 for no scaling)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSpriteScale( const int x, const int y, const float scale, const int flipmode, const glImage *spr )
{
    int x1 = 0;
    int y1 = 0;
    int x2 = spr->width;
    int y2 = spr->height;

    float u1 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off + spr->u_width  : spr->u_off;
    float u2 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off 			       : spr->u_off + spr->u_width;
    float v1 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off + spr->v_height : spr->v_off;
    float v2 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off 			       : spr->v_off + spr->v_height;


    if ( spr->textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, spr->textureID );
        gCurrentTexture = spr->textureID;
    }

    glPushMatrix();

    glTranslatef( x, y, 0.0 );
    glScalef( scale, scale, 1.0 );

    glBegin( GL_QUADS );

        glTexCoord2f( u1, v1 ); glVertex2i( x1, y1 );
        glTexCoord2f( u1, v2 ); glVertex2i( x1, y2 );
        glTexCoord2f( u2, v2 ); glVertex2i( x2, y2 );
        glTexCoord2f( u2, v1 ); glVertex2i( x2, y1 );

    glEnd();

    glPopMatrix();

}

/******************************************************************************

    Draws a center rotated sprite
	Parameters:
       x 		-> x position of the sprite center
	   y 		-> y position of the sprite center
	   angle    -> is the angle to rotated (in degrees)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSpriteRotate( const int x, const int y, const int angle, const int flipmode, const glImage *spr )
{

    int s_half_x = spr->width/2;
    int	s_half_y = spr->height/2;

    int x1 =  -s_half_x;
    int y1 =  -s_half_y;

    int x2 =  s_half_x;
    int y2 =  s_half_y;


    float u1 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off + spr->u_width  : spr->u_off;
    float u2 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off 			       : spr->u_off + spr->u_width;
    float v1 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off + spr->v_height : spr->v_off;
    float v2 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off 			       : spr->v_off + spr->v_height;


    if ( spr->textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, spr->textureID );
        gCurrentTexture = spr->textureID;
    }

    glPushMatrix();

    glTranslatef( x, y, 0.0 );
    glRotatef( angle, 0.0, 0.0, 1.0 );


    glBegin( GL_QUADS );

        glTexCoord2f( u1, v1 ); glVertex2i( x1, y1 );
        glTexCoord2f( u1, v2 ); glVertex2i( x1, y2 );
        glTexCoord2f( u2, v2 ); glVertex2i( x2, y2 );
        glTexCoord2f( u2, v1 ); glVertex2i( x2, y1 );

    glEnd();

    glPopMatrix();


}



/******************************************************************************

    Draws a center rotated and scaled sprite
	Parameters:
       x 		-> x position of the sprite center
	   y 		-> y position of the sprite center
	   angle    -> is the angle to rotated (-32768 to 32767)
	   scale    -> is a 20.12 FP scale value (you can use floattof32(1.45) to scale
	               and GCC would optimize it to a constant)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSpriteRotateScale(const int x, const int y, const int angle, const float scale, const int flipmode, const glImage *spr)
{

    int s_half_x = spr->width / 2;
    int	s_half_y = spr->height / 2;

    int x1 =  -s_half_x;
    int y1 =  -s_half_y;

    int x2 =  s_half_x;
    int y2 =  s_half_y;


    float u1 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off + spr->u_width  : spr->u_off;
    float u2 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off 			       : spr->u_off + spr->u_width;
    float v1 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off + spr->v_height : spr->v_off;
    float v2 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off 			       : spr->v_off + spr->v_height;


    if ( spr->textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, spr->textureID );
        gCurrentTexture = spr->textureID;
    }

    glPushMatrix();

    glTranslatef( x, y, 0.0 );
    glScalef( scale, scale, 1.0 );
    glRotatef(angle, 0.0, 0.0, 1.0);

    glBegin( GL_QUADS );

        glTexCoord2f( u1, v1 ); glVertex2i( x1, y1 );
        glTexCoord2f( u1, v2 ); glVertex2i( x1, y2 );
        glTexCoord2f( u2, v2 ); glVertex2i( x2, y2 );
        glTexCoord2f( u2, v1 ); glVertex2i( x2, y1 );

    glEnd();

    glPopMatrix();


}

/******************************************************************************

    Initializes our spriteset with Texture Packer generated UV coordinates
	Very safe and easy to use.
	Parameters:
       filename     -> name of the file to load as texture
       *sprite 		-> pointer to an array of glImage
       width/height -> image dimensions
	   numframes 	-> number of frames in a spriteset (auto-generated by Texture Packer)
	   *texcoords 	-> Texture Packer auto-generated array of UV coords
	   textureID 	-> Texture handle returned by glGenTextures()
       filtemode    -> GL_NEAREST or GL_LINEAR

******************************************************************************/
GLuint glLoadSpriteset( const char *filename,
                        glImage *sprite,
                        const int width,
                        const int height,
                        const int numframes,
                        const unsigned int *texcoords,
                        const  GLuint filtermode
                      )
{

    int i;
    GLuint textureID;

    glGenTextures ( 1, &textureID );
    glBindTexture ( GL_TEXTURE_2D, textureID );

    LoadTexture( filename );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtermode );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtermode );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glBindTexture( GL_TEXTURE_2D, 0 );

    // init sprites texture coords and texture ID
    for ( i = 0; i < numframes; i++)
    {
        int j = i * 4; // texcoords array is u_off, wid, hei
        sprite[i].textureID = textureID;
        sprite[i].width = texcoords[j+2];
        sprite[i].height = texcoords[j+3];
        sprite[i].u_off = texcoords[j] / (float)width;				// set x-coord
        sprite[i].v_off = texcoords[j+1] / (float)height;			// y-coord
        sprite[i].u_width = texcoords[j+2] / (float)width;			// set x-coord
        sprite[i].v_height = texcoords[j+3] / (float)height;		// y-coord

    }

    return textureID;

}


GLuint glLoadSprite( const char *filename,
                     glImage *sprite,
                     const int width,
                     const int height,
                     const  GLuint filtermode
                   )
{

    GLuint textureID;

    glGenTextures ( 1, &textureID );
    glBindTexture ( GL_TEXTURE_2D, textureID );

    LoadTexture( filename );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtermode );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtermode );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glBindTexture( GL_TEXTURE_2D, 0 );

    sprite->textureID = textureID;
    sprite->width = width;
    sprite->height = height;
    sprite->u_off = 0.0;		// set x-coord
    sprite->v_off = 0.0;		// y-coord
    sprite->u_width = 1.0;		// set x-coord
    sprite->v_height = 1.0;		// y-coord


    return textureID;

}
