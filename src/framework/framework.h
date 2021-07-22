/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	Here we define all the mathematical classes like Vector3, Matrix44 and some extra useful geometrical functions
*/

#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#include <vector>
#include <cmath>
#include <random>

#ifndef PI
#define PI 3.14159265359
#endif
#define DEG2RAD 0.0174532925

//clamp a value 'x' between 'a' and 'b'
inline float clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
inline unsigned int clamp(unsigned int x, unsigned int a, unsigned int b) { return x < a ? a : (x > b ? b : x); }
inline unsigned char clamp(unsigned char x, unsigned char a, unsigned char b) { return x < a ? a : (x > b ? b : x); }

class Vector3;

//Color class to store colors in unsigned byte
class Color
{
public:
	union
	{
		struct { unsigned char r;
				 unsigned char g;
				 unsigned char b; };
		unsigned char v[3];
	};
	Color() { r = g = b = 0; }
	Color(float r, float g, float b) { this->r = (unsigned char)r; this->g = (unsigned char)g; this->b = (unsigned char)b; }
	void operator = (const Vector3& v);

	void set(float r, float g, float b) { this->r = (unsigned char)clamp(r,0.0,255.0); this->g = (unsigned char)clamp(g,0.0,255.0); this->b = (unsigned char)clamp(b,0.0,255.0); }
	void random() { r = rand() % 255; g = rand() % 255; b = rand() % 255; }

	Color operator * (float v) { return Color((unsigned char)(r*v), (unsigned char)(g*v), (unsigned char)(b*v)); }
	void operator *= (float v) { r = (unsigned char)(r * v); g = (unsigned char)(g * v); b = (unsigned char)(b * v); }
	Color operator / (float v) { return Color((unsigned char)(r/v), (unsigned char)(g/v), (unsigned char)(b/v)); }
	void operator /= (float v) { r = (unsigned char)(r / v); g = (unsigned char)(g / v); b = (unsigned char)(b / v); }
	Color operator + (const Color& v) { return Color((float)(r+v.r), (float)(g+v.g), (float)(b+v.b) ); }
	void operator += (const Color& v) { r += v.r; g += v.g; b += v.b; }
	Color operator - (const Color& v) { return Color((float)(r-v.r), (float)(g-v.g), (float)(b-v.b)); }
	void operator -= (const Color& v) { r -= v.r; g -= v.g; b -= v.b; }

	//some colors to help
	static const Color WHITE;
	static const Color BLACK;
	static const Color GRAY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color PURPLE;
};

inline Color operator * (const Color& c, float v) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
inline Color operator * (float v, const Color& c) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
//*********************************

class Vector3
{
public:
	union
	{
		struct { float x,y,z; };
		float v[3];
	};

	Vector3() { x = y = z = 0.0f; }
	Vector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z;	}

	double length();
	double length() const;

	void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	Vector3& normalize();
	void random(float range);
	void random(Vector3 range);

	float distance(const Vector3& v) const;

	Vector3 cross( const Vector3& v ) const;
	float dot( const Vector3& v ) const;
};

//****************************
//Matrix44 class
class Matrix44
{
	public:

		//This matrix works in 
		union { //allows to access the same var using different ways
			struct
			{
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;
			};
			float M[4][4]; //[row][column]
			float m[16];
		};

		Matrix44();
		Matrix44(const float* v);

		void set(); //multiply with opengl matrix
		void load(); //load in opengl matrix
		void clear();
		void setIdentity();
		void transpose();

		//get base vectors
		Vector3 rightVector() { return Vector3(m[0],m[1],m[2]); }
		Vector3 topVector() { return Vector3(m[4],m[5],m[6]); }
		Vector3 frontVector() { return Vector3(m[8],m[9],m[10]); }

		bool inverse();
		void setUpAndOrthonormalize(Vector3 up);
		void setFrontAndOrthonormalize(Vector3 front);

		Matrix44 getRotationOnly(); //used when having scale

		//rotate only
		Vector3 rotateVector( const Vector3& v);

		//transform using world coordinates
		void traslate(float x, float y, float z);
		void rotate( float angle_in_rad, const Vector3& axis  );

		//transform using local coordinates
		void traslateLocal(float x, float y, float z);
		void rotateLocal( float angle_in_rad, const Vector3& axis  );

		//create a transformation matrix from scratch
		void setTranslation(float x, float y, float z);
		void setRotation( float angle_in_rad, const Vector3& axis );

		bool getXYZ(float* euler) const;

		Matrix44 operator * (const Matrix44& matrix) const;
};

//Operators, they are our friends
//Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, float v);

class Vector2
{
public:
	union
	{
		struct { float x,y; };
		float value[2];
	};

	Vector2() { x = y = 0.0f; }
	Vector2(float x, float y) { this->x = x; this->y = y; }

	double length() { return sqrt(x*x + y*y); }
	double length() const { return sqrt(x*x + y*y); }

	float dot( const Vector2& v );
	float perpdot( const Vector2& v );

	void set(float x, float y) { this->x = x; this->y = y; }

	Vector2& normalize() { *this *= 1.0f / (float)length(); return *this; }

	float distance(const Vector2& v);
	void random(float range);

	void operator *= (float v) { x*=v; y*=v; }
};

Vector2 operator * (const Vector2& a, float v);
Vector2 operator + (const Vector2& a, const Vector2& b);
Vector2 operator - (const Vector2& a, const Vector2& b);

inline float distance(const Vector2& a, const Vector2& b) { return (float)(a-b).length(); }
inline float distance(float x, float y, float x2, float y2) { return sqrtf( (x-x2)*(x-x2) + (y-y2)*(y-y2)); }


class Vector3u
{
public:
	union
	{
		struct { unsigned int x;
				 unsigned int y;
				 unsigned int z; };
		struct { unsigned int r;
				 unsigned int g;
				 unsigned int b; };
		unsigned int v[3];
	};
	Vector3u() { x = y = z = 0; }
	Vector3u(unsigned int x, unsigned int y, unsigned int z) { this->x = x; this->y = y; this->z = z; }

	void operator *= (float v) { x = (unsigned int)(x * v); y = (unsigned int)(y * v); z = (unsigned int)(z * v); }
	Vector3u operator / (float v) { return Vector3u((unsigned int)(r/v), (unsigned int)(g/v), (unsigned int)(b/v)); }
	void operator /= (float v) { x = (unsigned int)(x / v); y = (unsigned int)(y / v); z = (unsigned int)(z / v); }
	Vector3u operator + (const Vector3u& v) { return Vector3u(x+v.x, y+v.y, z+v.z); }
	void operator += (const Vector3u& v) { x += v.x; y += v.y; z += v.z; }
	Vector3u operator - (const Vector3u& v) { return Vector3u(x-v.x, y-v.y, z-v.z); }
	void operator -= (const Vector3u& v) { x -= v.x; y -= v.y; z -= v.z; }
};

inline Vector3u operator * (float v, const Vector3u& c) { return Vector3u((unsigned int)(c.x*v), (unsigned int)(c.y*v), (unsigned int)(c.z*v)); }

float ComputeSignedAngle( Vector2 a, Vector2 b);
Vector3 RayPlaneCollision( const Vector3& plane_pos, const Vector3& plane_normal, const Vector3& ray_origin, const Vector3& ray_dir );


#endif