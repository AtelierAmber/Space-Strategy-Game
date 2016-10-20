#pragma once

#include <GL/glew.h>

namespace Sakura {

    struct Position {
		Position(float X, float Y) : x(X), y(Y){ }
        float x;
        float y;
    };

    struct ColorRGBA8 {
		//Constructors
		ColorRGBA8() : r(0), g(0), b(0), a(255){ }
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A){ }

        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };

    struct UV {
		UV(float U, float V) : u(U), v(V){ }
        float u;
        float v;
    };

    //The vertex definition
    struct Vertex {
		//Base constructors
		Vertex() : position(0.0f, 0.0f), color(), uv(0, 0){ }
		Vertex(float x, float y, ColorRGBA8 Color, UV Uv) : position(x, y), color(Color), uv(Uv){ }

        //This is the position struct. When you store a struct or class
        //inside of another struct or class, it is called composition. This is
        //layed out exactly the same in memory as if we had a float position[2],
        //but doing it this way makes more sense.
        Position position;

        //4 bytes for r g b a color.
		ColorRGBA8 color;

        //UV texture coordinates.
        UV uv;

        void setPosition(float x, float y) {
            position.x = x;
            position.y = y;
        }

        void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }

        void setUV(float u, float v) {
            uv.u = u;
            uv.v = v;
        }
    };

}