/*
Bananaman
An open source game project

This project uses the free GFX engine developed by Romain Marucchi-Foino
http://gfx.sio2interactive.com/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of

1. The origin of this software must not be misrepresented; you must not claim that
you wrote the original software. If you use this software in a product, an acknowledgment
in the product would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented
as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BANANAMAN_H
#define BANANAMAN_H

#include "gfx/gfx.h"

typedef struct
{
void ( *Init			)( int width, int height );
void ( *Draw			)( void );
void ( *TouchBegan	    )( float x, float y, unsigned int tap_count );
void ( *TouchMoved	    )( float x, float y, unsigned int tap_count );
void ( *TouchEnded	    )( float x, float y, unsigned int tap_count );
void ( *Accelerometer   )( float x, float y, float z );

} BANANAMAN;

extern BANANAMAN bananaman;

void bananamanInit( int width, int height );

void bananamanDraw( void );

void bananamanTouchBegan( float x, float y, unsigned int tap_count );

void bananamanTouchMoved( float x, float y, unsigned int tap_count );

void bananamanTouchEnded( float x, float y, unsigned int tap_count );

void bananamanTouchCancelled( float x, float y, unsigned int tap_count );

void bananamanAccelerometer( float x, float y, float z );

void bananamanExit( void );

extern "C"
{
    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_Init( JNIEnv *env, jobject obj, jint width, jint height, jstring apkFilePath );

    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_Draw( JNIEnv *env, jobject obj );

    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchBegan( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );

    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchMoved( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );

    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchEnded( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count );

    JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_templateApp_Accelerometer( JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z );
};

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_Init( JNIEnv *env, jobject obj, jint width, jint height, jstring apkFilePath )
{
    setenv( "FILESYSTEM", env->GetStringUTFChars( apkFilePath, NULL ), 1 );

    if( bananaman.Init ) bananaman.Init( width, height );
}

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_Draw( JNIEnv *env, jobject obj )
{ if( bananaman.Draw ) bananaman.Draw(); }

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchBegan( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
{ if( bananaman.TouchBegan ) bananaman.TouchBegan( x, y, tap_count ); }

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchMoved( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
{ if( bananaman.TouchMoved ) bananaman.TouchMoved( x, y, tap_count ); }

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_GL2View_TouchEnded( JNIEnv *env, jobject obj, jfloat x, jfloat y, jint tap_count )
{ if( bananaman.TouchEnded ) bananaman.TouchEnded( x, y, tap_count ); }

JNIEXPORT void JNICALL Java_in_liquidmetal_bananaman_templateApp_Accelerometer( JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z )
{ if( bananaman.Accelerometer ) bananaman.Accelerometer( x, y, z ); }

#endif
