#include "Bat.h"

Bat::Bat(float state[3][3],float r,float g,float b,float radius,float total_time,bool direction)
    : Object(state)
    , x(state[POSITION][X])
    , z(state[POSITION][Z])
    , r(r)
    , g(g)
    , b(b)
    , wing_angle_min(-50)
    , wing_angle_max(60)
    , flap_speed_min(0.3f)
    , flap_speed_max(0.5f)
    , radius(radius)
    , total_time(total_time)
    , direction(direction)
{
    // defines the vertices of the bat's wing
    float v[vertex_count][3] = {{0.90f,+0.02f,0.28f},  // 0
                                {0.74f,+0.05f,0.39f},  // 1
                                {0.92f,+0.03f,0.35f},  // 2
                                {0.74f,+0.02f,0.30f},  // 3
                                {0.40f,+0.00f,0.25f},  // 4
                                {0.61f,+0.03f,0.30f},  // 5
                                {0.16f,-0.02f,0.33f},  // 6
                                {0.22f,+0.03f,0.34f},  // 7
                                {0.29f,+0.05f,0.36f},  // 8
                                {0.36f,+0.06f,0.37f},  // 9
                                {0.46f,+0.06f,0.39f},  // 10
                                {0.56f,+0.06f,0.39f},  // 11
                                {0.07f,-0.03f,0.45f},  // 12
                                {0.16f,+0.03f,0.44f},  // 13
                                {0.23f,+0.03f,0.46f},  // 14
                                {0.35f,+0.04f,0.47f},  // 15
                                {0.52f,+0.05f,0.53f},  // 16
                                {0.61f,+0.05f,0.52f},  // 17
                                {0.69f,+0.04f,0.52f},  // 18
                                {0.80f,+0.02f,0.53f},  // 19
                                {0.87f,+0.03f,0.44f},  // 20
                                {0.92f,+0.02f,0.44f},  // 21
                                {0.85f,+0.00f,0.54f},  // 22
                                {0.91f,+0.01f,0.53f},  // 23
                                {0.95f,+0.01f,0.48f}}; // 24

    // vertices on the edge of the wing model are given a boolean flag to ensure that the top and bottom sections join
    bool edge_vertex[vertex_count] = {true,  // 0
                                      false, // 1
                                      false, // 2
                                      true,  // 3
                                      true,  // 4
                                      false, // 5
                                      true,  // 6
                                      false, // 7
                                      false, // 8
                                      false, // 9
                                      false, // 10
                                      false, // 11
                                      true,  // 12
                                      true,  // 13
                                      true,  // 14
                                      true,  // 15
                                      true,  // 16
                                      true,  // 17
                                      true,  // 18
                                      true,  // 19
                                      false, // 20
                                      false, // 21
                                      true,  // 22
                                      true,  // 23
                                      true}; // 24

    for (int i = 0 ; i < vertex_count ; i++)
    {
        for (int j = X ; j <= Z ; j++)
        {
            // the bottom wing is slightly lower than the top for vertices not on the edge of the model
            wing_vertices_top[i][j]    = v[i][j];
            wing_vertices_bottom[i][j] = v[i][j] - (j == Y && !edge_vertex[i] ? 0.001f : 0.f);
        }
    }

    // the top array has two extra vertices for the bat's thumb, and two extra vertices for the bat's feet
    wing_vertices_top[vertex_count+0][X] = 0.39f; wing_vertices_top[vertex_count+0][Y] = 0.f; wing_vertices_top[vertex_count+0][Z] = 0.22f;
    wing_vertices_top[vertex_count+1][X] = 0.34f; wing_vertices_top[vertex_count+1][Y] = 0.f; wing_vertices_top[vertex_count+1][Z] = 0.21f;
    wing_vertices_top[vertex_count+2][X] = 0.76f; wing_vertices_top[vertex_count+2][Y] = 0.f; wing_vertices_top[vertex_count+2][Z] = 0.56f;
    wing_vertices_top[vertex_count+3][X] = 0.80f; wing_vertices_top[vertex_count+3][Y] = 0.f; wing_vertices_top[vertex_count+3][Z] = 0.57f;

    // randomise flap speed, starting wing angle, and starting point in the animation cycle
    wing_angle = RandFloatR(wing_angle_min,wing_angle_max);
    flap_speed = RandFloatR(flap_speed_min,flap_speed_max);
    current_time = total_time * RandFloat();
}

Bat::~Bat() {}

void Bat::Draw()
{
    // draw the body
    Body();

    // draw the left wing
    Wing();

    // draw the right wing, mirroring the left by scaling negatively in the X axis
    // this effectively turns the wing inside-out, so switch to frontface culling before Wing() call, and then back afterwards
    glPushMatrix();
    glScalef(-1.f,1.f,1.f);
    glCullFace(GL_FRONT);
    Wing();
    glCullFace(GL_BACK);
    glPopMatrix();

}

void Bat::Update(const float& dT)
{

    // reverse flap speed when wing angle reaches max or min
    // also takes measures to prevent wings from becoming stuck
    if (wing_angle > wing_angle_max && flap_speed > 0 || wing_angle < wing_angle_min && flap_speed < 0)
    {
        flap_speed = -flap_speed;
    }
    wing_angle += dT * flap_speed;

    // update bat's position in a circle
    // direction flag is set to true for a clockwise flight path, otherwise anticlockwise
    current_time = fmod(dT+current_time,total_time);
    float t = current_time/total_time;
    float angle = 2*(float)M_PI * (direction ? +t : -t);
    state[POSITION][X] = x+radius*cos(angle);
    state[POSITION][Z] = z+radius*sin(angle);
    state[ROTATION][Y] = 360 * (direction ? 1-t : t) + (direction ? 180 : 0);
}

// draws a triangle between the three vertices
// calculates the normal for the face with vector maths
void Bat::Triangle(const float* a,const float* b,const float* c)
{
    float normal[3],v[3];
    sub(b,a,v);
    sub(c,a,normal);
    cross(v,normal,normal);
	norm(normal);

    glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(a);
        glVertex3fv(b);
    glVertex3fv(c);
    glEnd();
}

// draws the left wing of the bat
void Bat::Wing()
{
    // set material properties
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    float mat_ambient[] = { (float)r/300, (float)g/300, (float)b/300, 1.f };
    float mat_diffuse[] = { (float)r/255, (float)g/255, (float)b/255, 1.f };
    float mat_spec[]    = { (float)r/600, (float)g/600, (float)b/600, 1.f };
    float shininess = 50;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    glColor3f(r,g,b);

    glPushMatrix();
    // rotate about Z axis to flap
    glRotatef(wing_angle,0.f,0.f,1.f);
    // translate so that the back of the left wing meets the back of the right
    glTranslatef(-0.95f,0.f,0.f);

    // draw the wing membrane's top
    // face 1
    Triangle(wing_vertices_top[0],
             wing_vertices_top[1],
             wing_vertices_top[2]);
    // face 2
    Triangle(wing_vertices_top[0],
             wing_vertices_top[3],
             wing_vertices_top[1]);
    // face 3 -- this face is a quad split into two triangles
    Triangle(wing_vertices_top[3],
             wing_vertices_top[5],
             wing_vertices_top[1]);
    Triangle(wing_vertices_top[3],
             wing_vertices_top[4],
             wing_vertices_top[5]);
    // face 4
    Triangle(wing_vertices_top[4],
             wing_vertices_top[6],
             wing_vertices_top[7]);
    // face 5
    Triangle(wing_vertices_top[4],
             wing_vertices_top[7],
             wing_vertices_top[8]);
    // face 6
    Triangle(wing_vertices_top[4],
             wing_vertices_top[8],
             wing_vertices_top[9]);
    // face 7
    Triangle(wing_vertices_top[4],
             wing_vertices_top[9],
             wing_vertices_top[10]);
    // face 8
    Triangle(wing_vertices_top[4],
             wing_vertices_top[10],
             wing_vertices_top[11]);
    // face 9
    Triangle(wing_vertices_top[4],
             wing_vertices_top[11],
             wing_vertices_top[5]);
    Triangle(wing_vertices_top[5],
             wing_vertices_top[11],
             wing_vertices_top[1]);
    // face 10
    Triangle(wing_vertices_top[6],
             wing_vertices_top[12],
             wing_vertices_top[7]);
    Triangle(wing_vertices_top[7],
             wing_vertices_top[12],
             wing_vertices_top[13]);
    // face 11
    Triangle(wing_vertices_top[7],
             wing_vertices_top[13],
             wing_vertices_top[8]);
    Triangle(wing_vertices_top[8],
             wing_vertices_top[13],
             wing_vertices_top[14]);
    // face 12
    Triangle(wing_vertices_top[8],
             wing_vertices_top[14],
             wing_vertices_top[9]);
    Triangle(wing_vertices_top[9],
             wing_vertices_top[14],
             wing_vertices_top[15]);
    // face 13
    Triangle(wing_vertices_top[9],
             wing_vertices_top[15],
             wing_vertices_top[10]);
    Triangle(wing_vertices_top[10],
             wing_vertices_top[15],
             wing_vertices_top[16]);
    // face 14
    Triangle(wing_vertices_top[10],
             wing_vertices_top[16],
             wing_vertices_top[11]);
    Triangle(wing_vertices_top[11],
             wing_vertices_top[16],
             wing_vertices_top[17]);
    // face 15
    Triangle(wing_vertices_top[11],
             wing_vertices_top[17],
             wing_vertices_top[1]);
    Triangle(wing_vertices_top[1],
             wing_vertices_top[17],
             wing_vertices_top[18]);
    // face 16
    Triangle(wing_vertices_top[1],
             wing_vertices_top[18],
             wing_vertices_top[20]);
    Triangle(wing_vertices_top[20],
             wing_vertices_top[18],
             wing_vertices_top[19]);
    // face 17
    Triangle(wing_vertices_top[1],
             wing_vertices_top[20],
             wing_vertices_top[2]);
    // face 18
    Triangle(wing_vertices_top[2],
             wing_vertices_top[20],
             wing_vertices_top[21]);
    // face 19
    Triangle(wing_vertices_top[20],
             wing_vertices_top[19],
             wing_vertices_top[22]);
    // face 20
    Triangle(wing_vertices_top[20],
             wing_vertices_top[22],
             wing_vertices_top[23]);
    // face 21
    Triangle(wing_vertices_top[20],
             wing_vertices_top[23],
             wing_vertices_top[21]);
    Triangle(wing_vertices_top[21],
             wing_vertices_top[23],
             wing_vertices_top[24]);

    // draw the wing membrane's bottom (winding differerently from the top)
    // face 1
    Triangle(wing_vertices_bottom[0],
             wing_vertices_bottom[2],
             wing_vertices_bottom[1]);
    // face 2
    Triangle(wing_vertices_bottom[0],
             wing_vertices_bottom[1],
             wing_vertices_bottom[3]);
    // face 3
    Triangle(wing_vertices_bottom[3],
             wing_vertices_bottom[1],
             wing_vertices_bottom[5]);
    Triangle(wing_vertices_bottom[3],
             wing_vertices_bottom[5],
             wing_vertices_bottom[4]);
    // face 4
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[7],
             wing_vertices_bottom[6]);
    // face 5
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[8],
             wing_vertices_bottom[7]);
    // face 6
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[9],
             wing_vertices_bottom[8]);
    // face 7
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[10],
             wing_vertices_bottom[9]);
    // face 8
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[11],
             wing_vertices_bottom[10]);
    // face 9
    Triangle(wing_vertices_bottom[4],
             wing_vertices_bottom[5],
             wing_vertices_bottom[11]);
    Triangle(wing_vertices_bottom[5],
             wing_vertices_bottom[1],
             wing_vertices_bottom[11]);
    // face 10
    Triangle(wing_vertices_bottom[6],
             wing_vertices_bottom[7],
             wing_vertices_bottom[12]);
    Triangle(wing_vertices_bottom[7],
             wing_vertices_bottom[13],
             wing_vertices_bottom[12]);
    // face 11
    Triangle(wing_vertices_bottom[7],
             wing_vertices_bottom[8],
             wing_vertices_bottom[13]);
    Triangle(wing_vertices_bottom[8],
             wing_vertices_bottom[14],
             wing_vertices_bottom[13]);
    // face 12
    Triangle(wing_vertices_bottom[8],
             wing_vertices_bottom[9],
             wing_vertices_bottom[14]);
    Triangle(wing_vertices_bottom[9],
             wing_vertices_bottom[15],
             wing_vertices_bottom[14]);
    // face 13
    Triangle(wing_vertices_bottom[9],
             wing_vertices_bottom[10],
             wing_vertices_bottom[15]);
    Triangle(wing_vertices_bottom[10],
             wing_vertices_bottom[16],
             wing_vertices_bottom[15]);
    // face 14
    Triangle(wing_vertices_bottom[10],
             wing_vertices_bottom[11],
             wing_vertices_bottom[16]);
    Triangle(wing_vertices_bottom[11],
             wing_vertices_bottom[17],
             wing_vertices_bottom[16]);
    // face 15
    Triangle(wing_vertices_bottom[11],
             wing_vertices_bottom[1],
             wing_vertices_bottom[17]);
    Triangle(wing_vertices_bottom[1],
             wing_vertices_bottom[18],
             wing_vertices_bottom[17]);
    // face 16
    Triangle(wing_vertices_bottom[1],
             wing_vertices_bottom[20],
             wing_vertices_bottom[18]);
    Triangle(wing_vertices_bottom[20],
             wing_vertices_bottom[19],
             wing_vertices_bottom[18]);
    // face 17
    Triangle(wing_vertices_bottom[1],
             wing_vertices_bottom[2],
             wing_vertices_bottom[20]);
    // face 18
    Triangle(wing_vertices_bottom[2],
             wing_vertices_bottom[21],
             wing_vertices_bottom[20]);
    // face 19
    Triangle(wing_vertices_bottom[20],
             wing_vertices_bottom[22],
             wing_vertices_bottom[19]);
    // face 20
    Triangle(wing_vertices_bottom[20],
             wing_vertices_bottom[23],
             wing_vertices_bottom[22]);
    // face 21
    Triangle(wing_vertices_bottom[20],
             wing_vertices_bottom[21],
             wing_vertices_bottom[23]);
    Triangle(wing_vertices_bottom[21],
             wing_vertices_bottom[24],
             wing_vertices_bottom[23]);

    glPopAttrib();

    // draw the bat's limbs on top of the wings
    glColor3ub(0,0,0);
    Limbs();

    glPopMatrix();
}

// uses vector maths to draw the bat's limbs between vertex pairs on its wings
void Bat::Limbs()
{
    // define all vertex pairs
    const int pair_count = 17;
    const float* pairs[pair_count][2] = {{wing_vertices_top[0] ,wing_vertices_top[1]},
                                         {wing_vertices_top[1] ,wing_vertices_top[5]},
                                         {wing_vertices_top[5] ,wing_vertices_top[4]},
                                         {wing_vertices_top[21],wing_vertices_top[20]},
                                         {wing_vertices_top[20],wing_vertices_top[19]},
                                         {wing_vertices_top[4] ,wing_vertices_top[6]},
                                         {wing_vertices_top[6] ,wing_vertices_top[12]},
                                         {wing_vertices_top[4] ,wing_vertices_top[7]},
                                         {wing_vertices_top[7] ,wing_vertices_top[13]},
                                         {wing_vertices_top[4] ,wing_vertices_top[9]},
                                         {wing_vertices_top[9] ,wing_vertices_top[15]},
                                         {wing_vertices_top[4] ,wing_vertices_top[10]},
                                         {wing_vertices_top[10],wing_vertices_top[16]},
                                         {wing_vertices_top[4] ,wing_vertices_top[25]},
                                         {wing_vertices_top[25],wing_vertices_top[26]},
                                         {wing_vertices_top[19],wing_vertices_top[27]},
                                         {wing_vertices_top[19],wing_vertices_top[28]}};

    // define all joint vertices
    const int joint_count = 19;
    const float* joints[joint_count] = {wing_vertices_top[0],
                                        wing_vertices_top[1],
                                        wing_vertices_top[4],
                                        wing_vertices_top[5],
                                        wing_vertices_top[6],
                                        wing_vertices_top[7],
                                        wing_vertices_top[9],
                                        wing_vertices_top[10],
                                        wing_vertices_top[12],
                                        wing_vertices_top[13],
                                        wing_vertices_top[15],
                                        wing_vertices_top[16],
                                        wing_vertices_top[19],
                                        wing_vertices_top[20],
                                        wing_vertices_top[21],
                                        wing_vertices_top[25],
                                        wing_vertices_top[26],
                                        wing_vertices_top[27],
                                        wing_vertices_top[28]};

    // draw a sphere at each joint vertex
    for (int i = 0 ; i < joint_count ; i++)
    {
        glPushMatrix();
          glTranslatef(joints[i][X],joints[i][Y],joints[i][Z]);
          glutSolidSphere(0.006f,20,20);
        glPopMatrix();
    }

    // loop for each pair
    for (int i = 0 ; i < pair_count ; i++)
    {
        glPushMatrix();

        // translate to the vertex of the first in the pair
        glTranslatef(pairs[i][0][X],pairs[i][0][Y],pairs[i][0][Z]);

        // we want to align the Y axis with target vector
        float axis_vector[3] = {0.f,1.f,0.f};
        float target_vector[3];

        // calculate target vector with vector subtraction, and normalise it
        // before normalising, calculate how long the limb should be with the Pythagorean theorem
        sub(pairs[i][1],pairs[i][0],target_vector);
        float length = sqrt(pow(target_vector[X],2)+pow(target_vector[Y],2)+pow(target_vector[Z],2));
        norm(target_vector);

        // calculate the rotation angle
        float angle;
        dot(axis_vector,target_vector,angle);
        angle = 180/(float)M_PI * acos(angle);

        // calculate the rotation vector
        float rotation_vector[3];
        cross(axis_vector,target_vector,rotation_vector);

        // rotate about the rotation vector by the rotation angle
        glRotatef(angle,rotation_vector[X],rotation_vector[Y],rotation_vector[Z]);

        // draw the limb
        Cylinder(length,0.006f,10);

        glPopMatrix();
    }
}

// draws a cylinder with height h and radius r with circle resolution (2*PI)/segments
void Bat::Cylinder(float h, float r,int segments)
{
    const float res = (2*(float)M_PI)/segments;
    float a = 0.f;
    float x = r, z = 0.f;

    do
    {
    glBegin(GL_QUADS);
        glNormal3f(x, 0.f, z);
        glVertex3f(x, 0.f, z);
        glVertex3f(x, h,   z);

        a += res;
        x = r*cos(a);
        z = r*sin(a);

        glNormal3f(x, 0.f, z);
        glVertex3f(x, h,   z);
        glVertex3f(x, 0.f, z);
    glEnd();
    }
    while (a <= 2*M_PI);
}

// draws the bat's body using simple shapes: cylinders, spheres, and cones
void Bat::Body()
{
    glPushMatrix();

    // initial transformations
    glTranslatef(0.f,0.f,0.275f);
    glScalef(0.072f,0.05f,0.072f);
    glPushMatrix();
    glRotatef(90,1.f,0.f,0.f);
    glColor3ub(0,0,0);

    // a cylinder forming the main part of the body
    Cylinder(2.f,1.f,10);
    glPopMatrix();
    glTranslatef(0.f,0.f,2.f);

    // a sphere forming a rounded back end of the body
    glutSolidSphere(1.f,10,10);
    glTranslatef(0.f,0.f,-2.2f);

    // a sphere forming a rounded front end of the body (its head)
    glutSolidSphere(1.1f,10,10);

    // right ear (cone) and eye (sphere)
    glColor3ub(0,0,0);
    glPushMatrix();
    glTranslatef(0.2f,0.2f,0.f);
    glPushMatrix();
    glRotatef(-120.f,1.f,0.f,0.3f);
    glColor3ub(0,0,0);
    glutSolidCone(0.5f,1.5f,10,10);
    glPopMatrix();
    glTranslatef(0.2f,0.f,-0.8f);
    glColor3ub(255,0,0);
    glutSolidSphere(0.25f,10,10);
    glPopMatrix();

    // left ear and eye
    glTranslatef(-0.2f,0.2f,0.f);
    glPushMatrix();
    glRotatef(-120.f,1.f,0.f,-0.3f);
    glColor3ub(0,0,0);
    glutSolidCone(0.5f,1.5f,10,10);
    glPopMatrix();
    glTranslatef(-0.2f,0.f,-0.8f);
    glColor3ub(255,0,0);
    glutSolidSphere(0.25f,10,10);

    glPopMatrix();
}
