#ifndef GL_HELPERS_H
#define GL_HELPERS_H

#include <GL/gl3w.h>

#include "../math/vec2.h"

inline void DrawLine(Vec2f start, Vec2f stop)
{
        glBegin(GL_LINES);
        glVertex3f(start.x, start.y, 0.0f);
        glVertex3f(stop.x, stop.y, 0.0f);
        glEnd();
}

inline void DrawCircle(Vec2f center, float radius)
{
        int num_segments = 16;

        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++)
        {
                float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 

                float x = radius * cosf(theta);//calculate the x component 
                float y = radius * sinf(theta);//calculate the y component 

                glVertex3f(x + center.x, y + center.y, 0.0f);//output vertex 
        }
        glEnd();
}

#endif

