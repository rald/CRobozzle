/*

	Easy GL2D

	Relminator 2010
	Richard Eric M. Lope BSN RN

	http://rel.betterwebber.com


*/


#include <stdio.h>
#include <math.h>
#include <GL/glfw.h>

#include "gl2d.h"

/******************************************************************************

    MAIN

******************************************************************************/


#include "uvcoord_texturepack.h"
#include "uvcoord_tiles.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480



int main(int argc, char *argv[])
{

    glImage sprites[TEXTUREPACK_NUM_IMAGES];
    glImage tiles[TEXTUREPACK_NUM_IMAGES];
    glImage artery;

    int     frame = 0;
    int    finished = false;

    glScreenInit( SCREEN_WIDTH, SCREEN_HEIGHT );
    glfwSwapInterval( 1 );

    GLuint texture = glLoadSpriteset( "texturepack.tga",
                                      sprites,
                                      TEXTUREPACK_BITMAP_WIDTH,
                                      TEXTUREPACK_BITMAP_HEIGHT,
                                      TEXTUREPACK_NUM_IMAGES,
                                      texturepack_texcoords,
                                      GL_NEAREST
                                    );

    GLuint TileTexture = glLoadSpriteset( "tiles.tga",
                                          tiles,
                                          TILES_BITMAP_WIDTH,
                                          TILES_BITMAP_HEIGHT,
                                          TILES_NUM_IMAGES,
                                          tiles_texcoords,
                                          GL_NEAREST
                                        );

    GLuint ArteryTexture = glLoadSprite( "artery.tga",
                                          &artery,
                                          256,
                                          256,
                                          GL_NEAREST
                                       );

    int i, x, y, SpriteFrame;
    int FramesPerSec = 0;

    SpriteFrame = 0;

    while(!finished)
    {

        frame++;
        FramesPerSec++;
        glClearScreen();

        glColor4ub( 255, 255, 255, 255 );
        glBlendMode( GL2D_SOLID );


        for ( y = 0; y < 480/16; y++ )
        {
            for (  x = 0; x < 640/16; x++ )
            {
                glSprite( x * 16, y * 16, GL2D_FLIP_NONE, &tiles[(x+y) % TILES_NUM_IMAGES] );
            }
        }

        if ( (frame & 7) == 0 )
        {
            SpriteFrame++;
            if( SpriteFrame > 2 ) SpriteFrame = 0;
        }

        x = ( ((cos(frame/140.0) + sin(frame/110.0)) * (SCREEN_WIDTH/5)));
        y = ( ((sin(-frame/120.0) + sin(frame/60.0)) * (SCREEN_HEIGHT/5)));


        glBlendMode( GL2D_SOLID );

        glSpriteRotateScale( 128, 128, frame/2, fabs(sin(frame/300.0)*2), GL2D_FLIP_NONE, &artery );

        glBlendMode( GL2D_GLOW );
        // glow lines on steroids
        for( i = frame; i < (360+frame); i += 10 )
        {
            int px =  sin(frame/30.0)  * 220 * cos( (i * M_PI) / 180.0 );
            int py =  sin(frame/80.0)  * 120 * sin( (i * M_PI) / 180.0 );
            int px2 = sin(frame/130.0)  * 170 * cos( ( (i+20) * M_PI ) / 180.0 );
            int py2 =  sin(frame/40.0)  * 160 * sin( ( (i+20) * M_PI ) / 180.0 );
            float adder = fabs( 7*sin( frame/ 20.0) );
            int j;
            for( j = 0; j < adder; j++ )
            {
                glLineGlow( 128+px, 128+py, 128+px2, 128+py2, 25-(adder*2), GL2D_RGBA(255+frame,255-frame,128+frame,255) );
            }
        }

        glBlendMode( GL2D_SOLID );
        glBoxFilled( 400, 40, 639, 479, GL2D_RGBA( 0,0,0,0 ) );

        // ellipse test
        glBlendMode( GL2D_BLENDED );
        glEllipseFilled( 520, 240, 10 + fabs(sin(frame/15.0)*250), 10+250 - fabs(sin(frame/25.0)*250), frame, GL2D_RGBA(255, 128, 64, 255) );


        glBlendMode( GL2D_TRANS );

        float scale = fabs( sin(frame / 20.0) );
        glSpriteScale ( 310,30,scale*3, GL2D_FLIP_NONE, &sprites[81] );

        // return to solid for kicks
        glBlendMode( GL2D_SOLID );
        glSprite( 280, 30, GL2D_FLIP_NONE, &sprites[81] );


        // transparent again
        glBlendMode( GL2D_TRANS );
        glSprite( 310, 30, GL2D_FLIP_NONE, &sprites[81] );



        glSprite( 230, 300, GL2D_FLIP_NONE, &sprites[88] );


        // flipped and shadow test
        int f = 82;
        glSprite( 50, 400, GL2D_FLIP_NONE, &sprites[87 + SpriteFrame] );
        glSprite( 50 + sprites[f].width, 400, GL2D_FLIP_H, &sprites[87 + SpriteFrame] );
        glColor4ub( 255, 255, 255, 128); // blend with /14 opacity for shadow fx
        glBlendMode( GL2D_ALPHA );
        glSprite( 50, 400 + sprites[f].height, GL2D_FLIP_V, &sprites[87 + SpriteFrame] );
        glBlendMode( GL2D_BLENDED );
        glSprite( 50 + sprites[f].width, 400+sprites[f].height, GL2D_FLIP_V | GL2D_FLIP_H, &sprites[87 + SpriteFrame] );

        // glow
        glBlendMode( GL2D_GLOW );
        glSprite( 230,330, GL2D_FLIP_NONE, &sprites[88] );
        glSprite( 230,330, GL2D_FLIP_NONE, &sprites[88] );
        glSprite( 230,330, GL2D_FLIP_NONE, &sprites[88] );


        glColor4ub( 255, 255, 255, 200 );  // 200/256 translucency
        // alpha blended
        glBlendMode( GL2D_BLENDED );
        glSprite( 200, 200, GL2D_FLIP_NONE, &sprites[32] );

        glSprite( 200, 400, GL2D_FLIP_NONE, &sprites[81] );


        // transparent
        glBlendMode( GL2D_TRANS );
        glSpriteRotate( SCREEN_WIDTH/2,SCREEN_HEIGHT/2, frame*3, GL2D_FLIP_NONE, &sprites[30] );

        // Alpha blended
        glColor4ub( 255, 255, 255, 100 );  // 100/256 translucency
        glBlendMode( GL2D_ALPHA );
        glSpriteRotateScale( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, frame*3, 5-scale*8, GL2D_FLIP_NONE, &sprites[30] );
        glSpriteRotate( SCREEN_WIDTH/2 + 30, SCREEN_HEIGHT/2, -frame*3, GL2D_FLIP_NONE, &sprites[30] );

        // black mode
        glColor4ub( 255, 255, 255, 255 );
        glBlendMode( GL2D_BLACK );
        glSprite( 30, 130, GL2D_FLIP_NONE, &sprites[81] );


        // color mode
        glColor4ub( 255, 0, 0, 255 );
        glBlendMode( GL2D_TRANS );
        glSprite( 130, 130, GL2D_FLIP_NONE, &sprites[81] );

        glBlendMode( GL2D_BLENDED );
        glSprite( 230, 130, GL2D_FLIP_NONE, &sprites[81] );

        glColor4ub( 255, 0, 255, 150 );
        glBlendMode( GL2D_ALPHA );
        glSprite( 30, 330, GL2D_FLIP_NONE, &sprites[32] );

        // back to standard coloring
        // and draw our animated sprite
        glColor4ub( 255, 255, 255, 128 );		// factor of 128 blending
        // trans
        glBlendMode( GL2D_TRANS );
        glSprite( (SCREEN_WIDTH/2) + x,(SCREEN_HEIGHT/2) + y, GL2D_FLIP_NONE, &sprites[87 +SpriteFrame] );
        // blended
        glBlendMode( GL2D_ALPHA );
        glSprite( (SCREEN_WIDTH/2) - x,(SCREEN_HEIGHT/2) + y, GL2D_FLIP_V, &sprites[87 +SpriteFrame] );
        // green colored blended by 128/256
        glColor4ub( 0, 255, 0, 128);
        glSprite( (SCREEN_WIDTH/2) - x,(SCREEN_HEIGHT/2) - y, GL2D_FLIP_H, &sprites[87 +SpriteFrame] );
        // back to trans but colored
        glBlendMode( GL2D_TRANS );
        glSprite( (SCREEN_WIDTH/2) + x,(SCREEN_HEIGHT/2) - y, GL2D_FLIP_V | GL2D_FLIP_H, &sprites[87 +SpriteFrame] );

        // test gradient box
        glBoxFilledGradient( 420,350,560,460,
			   			  	 GL2D_RGBA(0,255,255,255),
							 GL2D_RGBA(255,255,0,255),
							 GL2D_RGBA(255,255,255,255),
							 GL2D_RGBA(255,0,255,255)
                           );


        glBlendMode( GL2D_GLOW );
        //draw 3 blended lines with decreasing widths to
        //make the effect  better.
        glLineGlow( 320, 290, 620, 290, 20, GL2D_RGBA( 128, 32, 225, 128) );
        glLineGlow( 320, 290, 620, 290, 15, GL2D_RGBA( 128, 32, 225, 200) );
        glLineGlow( 320, 290, 620, 290, 10, GL2D_RGBA( 128, 32, 225, 255) );

        //  draw just a single glowline
        glLineGlow( 320, 290, 620, 425, 40, GL2D_RGBA( 255, 32, 2, 255) );

        // draw a solid line
        glLine( 620, 290, 620, 425, GL2D_RGBA( 255, 255, 0, 255 ) );

        glBlendMode( GL2D_SOLID );

        // box
        glBox( 400, 40, 639, 479, GL2D_RGBA( 220, 255, 55, 0 ) );

        // triangle test
        glTriangle( 480, 100, 630, 50, 560, 200, GL2D_RGBA(25,2,255,255) );
        glTriangleFilled( 490, 105, 610, 60, 590, 135, GL2D_RGBA(255,255,255,255) );

        glColor4ub( 255,255,255,64 );		// factor of 64/256 blending
        glBlendMode( GL2D_BLENDED );
        glTriangleFilledGradient( 290,150,510,60,490,135,
                                  GL2D_RGBA(0,255,255,255),
                                  GL2D_RGBA(255,255,0,255),
                                  GL2D_RGBA(255,0,255,255) );

        glBlendMode( GL2D_SOLID );  // back to solid
        glColor4ub( 255,255,255,255 );		// Opaque again
        // pset test
        for( i  = 0;  i < 360; i += 10 )
        {
            int px = 20 * cos( (i * M_PI) / 180.0 );
            int py = 20 * sin( (i * M_PI) / 180.0 );
            glPutPixel( 430+px, 70+py, GL2D_RGBA(255,2,255,255) );

        }



//        glSprite( 400, 10, GL2D_FLIP_NONE, &artery );
//
//        glSpriteRotateScale( 320, 240, frame/2, fabs(sin(frame/300.0)*2), GL2D_FLIP_NONE, &artery );
//
//        //glBoxFilled(1, 1, 638, 478, GL2D_RGBA(255,0,255,255));
//
//        glBlendMode( GL2D_BLACK );
//        glSprite( 100, 10, GL2D_FLIP_NONE, &sprites[81] );
//
//        glBlendMode( GL2D_GLOW );
//        glColor4ub( 255, 255, 255, 200 );
//        glSprite( 100, 100, GL2D_FLIP_V, &sprites[81] );
//
//        glBlendMode( GL2D_SOLID );
//        glColor4ub( 255, 255, 255, 255 );
//        glSprite( 200, 10, GL2D_FLIP_H, &sprites[81] );
//
//        glBlendMode( GL2D_ALPHA );
//        glColor4ub( 255,255,0,200 );
//        glSpriteScale( 200, 100, fabs(sin(frame/200.0)*1.5), GL2D_FLIP_H | GL2D_FLIP_V, &sprites[81] );
//
//        glBlendMode( GL2D_TRANS );
//        glColor4ub( 255, 255, 255, 255 );
//        glSpriteRotate( 200, 240, -frame, GL2D_FLIP_NONE, &sprites[30] );
//
//        glBlendMode( GL2D_BLENDED );
//        glColor4ub( 0,255,255,200 );
//        glSpriteRotateScale( 400, 240, frame, fabs(sin(frame/100.0)*2), GL2D_FLIP_NONE, &sprites[30] );
//
//        glBlendMode( GL2D_GLOW );
//        glColor4ub( 255,0,255,255 );
//
//
//        glLineGlow( 10, 10, 400, 300, 15, RGBA( 255,255,255,255 ) );
//
        float TimeStart = glfwGetTime();
        while( (glfwGetTime() - TimeStart) < (1.0/60.0) ){};


        glfwSwapBuffers();


        // exit if ESC was pressed or window was closed
        finished = glfwGetKey( GLFW_KEY_ESC )| !glfwGetWindowParam( GLFW_OPENED );
    }

    glDeleteTextures ( 1, &texture );
    glDeleteTextures ( 1, &TileTexture );
    glDeleteTextures ( 1, &ArteryTexture );


    glfwTerminate();

    return 0;
}
