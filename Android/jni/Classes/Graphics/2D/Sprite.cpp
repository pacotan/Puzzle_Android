//! @file 		Sprite.cpp
//! @author 	Keita Tanji
//! @breif		
#include "Sprite.h"
#include "../../Asset/Asset.h"

static GLshort sprVtx[] = { 0, 0, 1, 0, 0, 1, 1, 1 };

Sprite::Sprite(const char* imgName, int fw, int fh)
: _tag(0)
, _frameSize()
, _texSize()
, _pitch(0)
{
	memset(_sprCol, 255, sizeof(GLubyte) * 16);

	_frameSize.width = fw;
	_frameSize.height = fh;

	loadImage(imgName);
}

Sprite::~Sprite()
{}

void Sprite::loadImage(const char* imgName)
{
	//texture生成
	unsigned int width = 0;
	unsigned int height = 0;
	GLint type = 0;
	GLubyte *textureImage;

	//画像をロード
	Asset::loadPngImage(imgName, &width, &height, &type, &textureImage);
	_texSize.width = width;
	_texSize.height = height;
	if(_frameSize.width < 0) _frameSize.width = _texSize.width;
	if(_frameSize.height < 0) _frameSize.height = _texSize.height;
	_pitch = width / _frameSize.width;

    // テクスチャを作成し、データを転送
	glEnable( GL_TEXTURE_2D );
    glGenTextures(1, &_tag);
    glBindTexture(GL_TEXTURE_2D, _tag);
    //※ typeであってる？
    glTexImage2D(GL_TEXTURE_2D, 0, type/*GL_RGBA*/, width, height, 0, type/*GL_RGBA*/, GL_UNSIGNED_BYTE, textureImage);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::drawWithFrame(int f, int x, int y, int w, int h)
{
	CGRect frameRect;
	if(_tag) {
		frameRect.origin.x = _frameSize.width * (f % _pitch) / _texSize.width;
		frameRect.origin.y = _frameSize.height * (f / _pitch) / _texSize.height;
		frameRect.size.width = _frameSize.width / _texSize.width;
		frameRect.size.height = _frameSize.height / _texSize.height;
	}
	//Make TexCoord
	GLfloat texCoord[8];
	texCoord[0] = texCoord[4] = frameRect.origin.x + 0.005f;
	texCoord[1] = texCoord[3] = frameRect.origin.y + 0.005f;
	texCoord[2] = texCoord[6] = frameRect.origin.x + frameRect.size.width + 0.005f;
	texCoord[5] = texCoord[7] = frameRect.origin.y + frameRect.size.height + 0.005f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	if(w<0) w = _frameSize.width;
	if(h<0) h = _frameSize.height;
	glScalef(w, h, 1);

	if(_tag > 0)
		glBindTexture(GL_TEXTURE_2D, _tag);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_SHORT, 0, sprVtx);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _sprCol);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}