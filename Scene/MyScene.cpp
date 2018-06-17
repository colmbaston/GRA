
/*
Use this as the starting point to your work. Include the header file for each object in your scene, then add it to the scene using AddObjectToScene().
*/

#include "MyScene.h"

// Constructor creates your CourseworkScene and initialises the base class Scene
MyScene::MyScene( int argc, char **argv, const char *title, const int windowWidth, const int windowHeight ) : Scene(argc, argv, title, windowWidth, windowHeight) {}

// Initialise your scene by adding all the objects you want to be in the scene here
void MyScene::Init()
{
    // set background colour
    glClearColor(0.f,0.f,0.f,1.f);

    // add the skybox
    int* textures = new int[6];
    textures[0] = GetTexture("./Resources/Skybox/front.bmp");
    textures[1] = GetTexture("./Resources/Skybox/left.bmp");
    textures[2] = GetTexture("./Resources/Skybox/right.bmp");
    textures[3] = GetTexture("./Resources/Skybox/back.bmp");
    textures[4] = GetTexture("./Resources/Skybox/top.bmp");
    AddObjectToScene(new Skybox((float)skybox_size,textures));

    // load textures
    int rock_texture = GetTexture("./Resources/rock.bmp");
    int lava_texture = GetTexture("./Resources/lava.bmp");

    // add the lava lake
    AddObjectToScene(new LavaLake(300,(float)skybox_size,1000,500000,lava_texture));

    // add the volcano
    float volcano_state[3][3] = {{0,300,0},{400,400,400},{0,0,0}};
    AddObjectToScene(new Volcano(volcano_state,rock_texture,lava_texture));

    // add some bats
    const int bat_count = 20;
    for (int i = 0 ; i < bat_count ; i++)
    {
        AddObjectToScene(RandomBat());
    }

    // set directional light coming upwards from the lava
    glEnable(GL_LIGHTING);
    GLfloat ambient0[]  = { 0.f, 0.f, 0.f, 1.0f };
    GLfloat diffuse0[]  = { (float)255/255, (float)89/255, (float)0/255, 1.0f }; // dark orange
    GLfloat specular0[] = { (float)255/255, (float)89/255, (float)0/255, 1.0f }; // dark orange
    GLfloat position0[] = { 0.f, -1.0f, 0.f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);

    // positional light at the top of the volcano
    float ambient1[]   = { .5f, .5f, 0.f, 1.f };
    float diffuse1[]   = { 1.f, 1.f, 0.f, 1.f }; // yellow
    float specular1[]  = { 1.f, 1.f, 0.f, 1.f }; // yellow
    float position1[]  = { 0.f, 800, 0.f, 1.f };
    float attenuation = 0.00001f;
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,&attenuation);
    glEnable(GL_LIGHT1);
}

// returns a pointer to a randomised bat
// returned bat is guaranteed to not fly beyond the skybox during its animation
// the total to complete its animation is proportional to the circumference of its flight path, and inversely proportional to its scale
Bat* MyScene::RandomBat()
{
    const float min_radius = 500.f;
    const float max_radius = skybox_size-500.f;
    float radius = RandFloatR(min_radius,max_radius);
    float circumference = 2*(float)M_PI*radius;

    const float min_x = -skybox_size+100+radius;
    const float max_x = -min_x;
    const float min_y = 800.f;
    const float max_y = 2*skybox_size-100;
    const float min_z = min_x;
    const float max_z = max_x;
    float x = RandFloatR(min_x,max_x);
    float y = RandFloatR(min_y,max_y);
    float z = RandFloatR(min_z,max_z);

    const float scale_min = 100.f;
    const float scale_max = 400.f;
    float scale = RandFloatR(scale_min,scale_max);

    float state[3][3] = {{x,y,z},{scale,scale,scale},{0,0,0}};

    const float speed_min = 0.003f;
    const float speed_max = 0.006f;
    float speed = RandFloatR(speed_min,speed_max);

    bool direction = rand()%2 ? true : false;

    return new Bat(state,RandFloat(),RandFloat(),RandFloat(),radius,circumference/(scale*speed),direction);
}

int main(int argc, char * argv[])
{
    srand((unsigned int)time(NULL));
    MyScene *scene = new MyScene(argc, argv, "G53GRA", (const int)800, (const int)600);
    scene->Run();
    delete scene;
    return 0;
}
