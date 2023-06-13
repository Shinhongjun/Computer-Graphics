//
//  Primi.cpp
//

#include "Primi.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define FPUSH_VTX3(p, vx, vy, vz)\
do{\
p.push_back(vx);\
p.push_back(vy);\
p.push_back(vz);\
}while(0)

#define FPUSH_VTX3_AT(p, i, vx, vy, vz)\
do{\
size_t i3 = 3*(i);\
p[i3+0] = (float)(vx);\
p[i3+1] = (float)(vy);\
p[i3+2] = (float)(vz);\
}while(0)

#define FSET_VTX3(vx, vy, vz, valx, valy, valz)\
do{\
vx=(float)(valx);\
vy=(float)(valy);\
vz=(float)(valz);\
}while(0)

void get_box_3d(std::vector<GLfloat> & p, GLfloat lx, GLfloat ly, GLfloat lz)
{
    static const GLfloat box_vertices[] = {
        // Every side has two triangles.
        // side at z = -0.5 first triangle
        0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        // side at x = -0.5
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
        // side at y = -0.5
        0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        // side at z = 0.5
        -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
        // side at x = 0.5
        0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
        // side at y = 0.5
        0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f
    };

    p.resize(sizeof(box_vertices) / sizeof(GLfloat));
    memcpy(p.data(), box_vertices, sizeof(box_vertices));
    size_t n = p.size() / 3;
    for (int i = 0; i < n; ++i) {
        p[3 * i + 0] *= lx;
        p[3 * i + 1] *= ly;
        p[3 * i + 2] *= lz;
    }
}

void get_cone_3d(
                 std::vector<GLfloat>& p,
                 std::vector<size_t>& side_idx,
                 std::vector<size_t>& bottom_idx,
                 GLfloat radius, GLfloat height, GLint n)
{
    GLfloat half_height = height / 2;
    GLfloat theta, x, z;

    FPUSH_VTX3(p, 0, half_height, 0); // top vertex
    side_idx.push_back(0);
    for (int i = 0; i <= n; ++i) {
        theta = (GLfloat)(2.0*M_PI*i/n);
        x = radius * sin(theta);
        z = radius * cos(theta);
        FPUSH_VTX3(p, x, -half_height, z);
        side_idx.push_back(i+1);
        bottom_idx.push_back(n+2-i);
    }
    FPUSH_VTX3(p, 0, -half_height, 0); // bottom-center vertex
    bottom_idx.push_back(1);
}

void get_cylinder_3d(
                     std::vector<GLfloat>& p,
                     std::vector<size_t>& side_idx,
                     std::vector<size_t>& top_idx,
                     std::vector<size_t>& bottom_idx,
                     GLfloat radius,
                     GLfloat height,
                     GLint n)
    {
    GLfloat half_height = height / 2;
    GLfloat theta, x, z;
    p.resize(3*(2*n+4));

    FPUSH_VTX3_AT(p, 0, 0, half_height, 0);
    top_idx.push_back(0);
    bottom_idx.push_back(2 * n + 3);
    for (int i = 0; i <= n; ++i) {
        theta = (GLfloat)(2.0*M_PI*i/n);
        x = radius * sin(theta);
        z = radius * cos(theta);
        FPUSH_VTX3_AT(p, 2*i+1, x, half_height, z);
        FPUSH_VTX3_AT(p, 2*i+2, x, -half_height, z);
        side_idx.push_back(2*i+1);
        side_idx.push_back(2*i+2);
        top_idx.push_back(2*i+1);
        bottom_idx.push_back(2*n+2 - 2*i);
    }
    FPUSH_VTX3_AT(p, 2*n+3, 0, -half_height, 0);
}

void get_torus_3d(
                  std::vector<GLfloat>& p,
                  std::vector<std::vector<size_t>>& side_idx,
                  GLfloat r0,
                  GLfloat r1,
                  GLint longs,
                  GLint lats)
{
    side_idx.resize(lats);
    for (int j = lats, k = 0; j >= 0; --j, ++k){
        GLfloat phi = (GLfloat)(2.0*M_PI*j/lats);
        GLfloat y = r1 * sin(phi);
        GLfloat l = r1 * cos(phi);
        int a = k*(longs + 1);
        int b = (k + 1)*(longs + 1);
        for (int i = 0; i <= longs; ++i){
            GLfloat theta = (GLfloat)(2.0 * M_PI * i / longs);
            GLfloat st = sin(theta);
            GLfloat ct = cos(theta);
            GLfloat x0 = (r0 + r1) * st;
            GLfloat z0 = (r0 + r1) * ct;
            GLfloat dx = l * st;
            GLfloat dz = l * ct;
            
            FPUSH_VTX3(p, x0 + dx, y, z0 + dz);
            
            if(k < lats){
                side_idx[k].push_back(a+i);
                side_idx[k].push_back(b+i);
            }
        }
    }
}

void get_sphere_3d(
                   GLvec& p,
                   GLfloat r,
                   GLint subh,
                   GLint suba)
{
    for (int i = 1; i <= subh; ++i) {
        double theta0 = M_PI * (i - 1) / subh;
        double theta1 = M_PI * i / subh;

        double y0 = r * cos(theta0);
        double rst0 = r * sin(theta0);
        double y1 = r * cos(theta1);
        double rst1 = r * sin(theta1);

        for (int j = 1; j <= suba; ++j) {
            double phi0 = 2 * M_PI * (j - 1) / suba;
            double phi1 = 2 * M_PI * j / suba;

            double cp0 = cos(phi0);
            double sp0 = sin(phi0);
            double cp1 = cos(phi1);
            double sp1 = sin(phi1);

            float vx0, vy0, vz0, vx1, vy1, vz1;
            float vx2, vy2, vz2, vx3, vy3, vz3;

            FSET_VTX3(vx0, vy0, vz0, sp0 * rst0, y0, cp0 * rst0);
            FSET_VTX3(vx1, vy1, vz1, sp0 * rst1, y1, cp0 * rst1);
            FSET_VTX3(vx2, vy2, vz2, sp1 * rst0, y0, cp1 * rst0);
            FSET_VTX3(vx3, vy3, vz3, sp1 * rst1, y1, cp1 * rst1);


            if (i < subh) {
                // first triangle (v0 - v1 - v3)
                FPUSH_VTX3(p, vx0, vy0, vz0);
                FPUSH_VTX3(p, vx1, vy1, vz1);
                FPUSH_VTX3(p, vx3, vy3, vz3);


            }

            if (1 < i) {
                // second triangle (v3 - v2 - v0)
                FPUSH_VTX3(p, vx3, vy3, vz3);
                FPUSH_VTX3(p, vx2, vy2, vz2);
                FPUSH_VTX3(p, vx0, vy0, vz0);

            }
        }
    }
}


void get_color_3d_by_pos(
    GLvec& c,
    GLvec& p,
    GLfloat offset)
{
    GLfloat max_val[3] = { -INFINITY, -INFINITY, -INFINITY };
    GLfloat min_val[3] = { INFINITY, INFINITY, INFINITY };

    int n = (int)(p.size() / 3);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            GLfloat val = p[i * 3 + j];
            if (max_val[j] < val) max_val[j] = val;
            else if (min_val[j] > val) min_val[j] = val;
        }
    }

    GLfloat width[3] = {
        max_val[0] - min_val[0],
        max_val[1] - min_val[1],
        max_val[2] - min_val[2]
    };

    c.resize(p.size());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            int k = i * 3 + j;
            c[k] = std::fminf((p[k] - min_val[j]) / width[j] + offset, 1.0f);
        }
    }
}

void get_box_random_color(std::vector<GLfloat>& color) {

    color.resize(108);

    GLfloat* p = color.data();
    for (size_t i = 0; i < 6; ++i) {
        GLfloat side_colors[3] = {
            0.0f,
            0.0f,
            1.0f
        };
        for (size_t j = 0; j < 6; ++j) {
            *p++ = side_colors[0];
            *p++ = side_colors[1];
            *p++ = side_colors[2];
        }
    }
}

void get_spe_random_color(std::vector<GLfloat>& color) {

     // Calculate the total number of coordinates in the sphere
    int coordinateCount = 15*15*3*3*2-10;

    // Resize the color vector to match the size of the sphere coordinates
    color.resize(coordinateCount);
    GLfloat* p = color.data();
    // 여기에 구의 각 좌표에 대한 색상 설정을 추가하세요
    // 예시로 구의 모든 좌표에 빨간색(1.0f, 0.0f, 0.0f)을 할당하겠습니다.
    for (size_t i = 0; i < color.size(); i += 3) {
        *p++ = 1.0f;   // Red
        *p++ = 1.0f;   // Green
        *p++ = 0.0f;   // Blue
    }
}

void get_cone_random_color(std::vector<GLfloat>& color) {
    color.resize(10*10*3*3 );

    GLfloat* p = color.data();
    // 여기에 원뿔의 각 좌표에 대한 색상 설정을 추가하세요
    // 예시로 원뿔의 모든 좌표에 녹색(0.0f, 1.0f, 0.0f)을 할당하겠습니다.
    for (size_t i = 0; i < color.size(); i += 3) {
        *p++ = 1.0f;   // Red
        *p++ = 0.0f;   // Green
        *p++ = 0.0f;   // Blue
    }
}
void get_torus_random_color(std::vector<GLfloat>& color) {
    // Calculate the total number of coordinates in the torus
    int coordinateCount = 30*10*3*3;

    // Resize the color vector to match the size of the torus coordinates
    color.resize(coordinateCount);
    GLfloat* p = color.data();

    // 여기에 토러스의 각 좌표에 대한 색상 설정을 추가하세요
    // 예시로 토러스의 모든 좌표에 녹색(0.0f, 1.0f, 0.0f)을 할당하겠습니다.
    for (size_t i = 0; i < color.size(); i += 3) {
        *p++ = 1.0f;   // Red
        *p++ = 0.0f;   // Green
        *p++ = 0.0f;   // Blue
    }
}

void get_cylinder_random_color(std::vector<GLfloat>& color) {
    // Calculate the total number of coordinates in the torus
    int coordinateCount = 15*4*3*3;

    // Resize the color vector to match the size of the torus coordinates
    color.resize(coordinateCount);
    GLfloat* p = color.data();

    // 여기에 토러스의 각 좌표에 대한 색상 설정을 추가하세요
    // 예시로 토러스의 모든 좌표에 녹색(0.0f, 1.0f, 0.0f)을 할당하겠습니다.
    for (size_t i = 0; i < color.size(); i += 3) {
        *p++ = 0.0f;   // Red
        *p++ = 1.0f;   // Green
        *p++ = 0.0f;   // Blue
    }
}



void bind_buffer(GLint buffer, std::vector<GLfloat>& vec, int program, const GLchar* attri_name, GLint attri_size)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vec.size(), vec.data(),
                 GL_STATIC_DRAW);
    GLuint location = glGetAttribLocation(program, attri_name);
    glVertexAttribPointer(location, attri_size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
}

void get_vertex_color(GLvec& color, GLuint n, GLfloat r, GLfloat g, GLfloat b)
{
    color.resize(n * 3);
    for (GLuint i = 0; i < n; ++i)
    {
        color[i * 3 + 0] = r;
        color[i * 3 + 1] = g;
        color[i * 3 + 2] = b;
    }
}
