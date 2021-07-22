#include "image.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int toWidth, unsigned int toHeight)
{
	Color* new_pixels = new Color[toWidth * toHeight];
	unsigned int min_width = this->width > toWidth ? toWidth : this->width;
	unsigned int min_height = this->height > toHeight ? toHeight : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * toWidth + x ] = getPixel(x, y);

	delete pixels;
	this->width = toWidth;
	this->height = toHeight;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int toWidth, unsigned int toHeight)
{
	Color* new_pixels = new Color[toWidth * toHeight];

	for(unsigned int x = 0; x < toWidth; ++x)
		for(unsigned int y = 0; y < toHeight; ++y)
			new_pixels[y * toWidth + x ] = getPixel((unsigned int)(this->width * (x / (float)toWidth)), (unsigned int)(this->height * (y / (float)toHeight)) );

	delete pixels;
	this->width = toWidth;
	this->height = toHeight;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int areaWidth, unsigned int areaHeight)
{
	Image result(areaWidth, areaHeight);
	for(unsigned int x = 0; x < areaWidth; ++x)
		for(unsigned int y = 0; y < areaHeight; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return false;
		else
		{
			fclose(file);
			return false;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return false;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

void Image::drawRectangle(int x, int y, int w, int h, Color c, bool fill) {
    if (!fill){
        int startX = x;
        int startY = y;
        for (x=startX; x <= startX + w; x++){ // Drawing the vertical sides of the rectangle.
            setPixelSafe(x, startY, c);
            setPixelSafe(x, startY+h, c);
        }
        for (y=startY; y <= startY + h; y++){ // Drawing the horizontal lines of the rectangle.
            setPixelSafe(startX, y, c);
            setPixelSafe(startX + w, y, c);
        }
    }
    else{
        for (int i=0; i<h; i++){
            drawRectangle(x, y, w-i, h-i, c, false);
        }
    }
}

void Image::drawHorizontalLine(int x1, int y1, int x2, int y2, Color C){ // Realmente no tiene ningún sentido que haya y1 y y2.
    if (x1 > x2) std::swap(x1, x2); // Si es necesario, intercambiamos los puntos para siempre iterar hacia delante (Es decir, sumar x+1).
    for (int x=x1; x<x2; x++) setPixelSafe(x, y1, C);
}

void Image::setPixelsForCirlce(int x, int y, int x0, int y0, Color C, bool fill) {
    setPixelSafe(x+x0, y+y0, C); setPixelSafe(-x+x0, y+y0, C); setPixelSafe(x+x0, -y+y0, C); setPixelSafe(-x+x0, -y+y0, C); // (x,y) (-x, y) (x, -y) (-x -y)
    setPixelSafe(y+x0, x+y0, C); setPixelSafe(-y+x0, x+y0, C); setPixelSafe(y+x0, -x+y0, C); setPixelSafe(-y+x0, -x+y0, C); // (y, x) (-y, x), (y, -x) (-y, -x)
    if (fill){ // Dibujamos una línea horizontal, sin usar nada de Bresenham, simplemente un bucle for.
        drawHorizontalLine(-x+x0, y+y0, x+x0, y+y0, C); // (-x, y) ---> (x, y)
        drawHorizontalLine(-y+x0, x+y0, y+x0, x+y0, C); // (-y, x) ---> (y, x)
        drawHorizontalLine(-x+x0, -y+y0, x+x0, -y+y0, C); // (-x ,-y) ---> (x, -y)
        drawHorizontalLine(-y+x0, -x+y0, y+x0, -x+y0, C); // (-y, -x) ---> (y, -x)
    }
}
void Image::drawCircle(int x0, int y0, int R, Color C, bool fillCircle) {
    int x = 0; int y = R;
    int v = 3 - 2*R;
    setPixelsForCirlce(x, y, x0, y0, C, fillCircle);
    while(y > x){ // La idea es la misma que en el Bresenham del DrawLine.
        if (v < 0){
            v = v + 4*x + 6;
            x++;
        }
        else{
            v = v + 4*(x - y) + 10;
            x++; y--;
        }
        setPixelsForCirlce(x, y, x0, y0, C, fillCircle);
    }
}