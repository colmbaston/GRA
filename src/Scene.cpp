/*
Scene object handles drawing, updating and sending key events to all the objects in the scene.
Do not change this file. You may only add objects to the Scene using AddObjectsToScene in the Init function of MyScene.
*/

#include "Scene.h"
#include "Object.h"

Texture Scene::texture = Texture();
Camera Scene::camera = Camera();

Scene::Scene( int argc, char **argv, const char *title, const int windowWidth, const int windowHeight ) : StaticEngine(argc, argv, title, windowWidth, windowHeight), pKey(0), last_pKey(0), paused(false)
{
}

Scene::~Scene()
{
	for(Object* obj : sceneObjs)
	{
	    if(obj != NULL)
	       delete obj;
	    obj = NULL;
	}
}

int Scene::GetWindowWidth()
{
    return StaticEngine::windowWidth;
}

int Scene::GetWindowHeight()
{
    return StaticEngine::windowHeight;
}

int Scene::GetTexture(std::string fileName)
{
    return texture.GetTexture(fileName);
}

void Scene::Init()
{
}

void Scene::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    	camera.SetUpCamera();
		for (Object *obj : sceneObjs)
        {
            obj->Display();
        }

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	CheckGLError();
}

void Scene::HandleResize(int w, int h)
{
	camera.SetViewport(w, h);
}

void Scene::HandleKey( int key, int state, int x, int y )
{
    if (key == 'P' || key == 'p')
    {
        pKey = state;
    }

    camera.HandleKey(key, state, x, y);
	for( Object *obj : sceneObjs )
         obj->HandleKey(key, state, x, y );
}

void Scene::Mouse(int button, int state, int x, int y)
{
    camera.HandleMouseClick( button, state, x, y );
	for( Object *obj : sceneObjs )
         obj->HandleMouseClick( button, state, x, y );
}

void Scene::MouseMotion(int x, int y)
{
    camera.HandleMouseMotion( x, y );
    for( Object *obj : sceneObjs )
         obj->HandleMouseMotion( x, y );
}

void Scene::PassiveMouseMotion( int x, int y)
{
    camera.HandlePassiveMouseMotion( x, y );
    for( Object *obj : sceneObjs )
         obj->HandlePassiveMouseMotion( x, y );
}

// functionality added to pause scene's animations by pressing 'P'
void Scene::Update( const float& dT )
{
    camera.Update(dT);

    if (pKey)
    {
        if (!last_pKey)
        {
                paused = !paused;
        }
    }
    last_pKey = pKey;

    if (paused)
    {
        return;
    }

    for (Object *obj : sceneObjs)
    {
         obj->Update(dT);
    }
}

void Scene::AddObjectToScene( Object *object )
{
    sceneObjs.push_back(object);
}
