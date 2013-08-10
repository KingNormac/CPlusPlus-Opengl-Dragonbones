#include <GL\glew.h>
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include "DragonBones\DragonBones.h"

using namespace DragonBones;
void initGLFW();
GLFWwindow* window;
Armature* armature;
bool _isJumping = false;
int _moveDir = 0;
float _speedX = 0;
float _speedY = 0;
DBFactory* factory;
bool left = false;
bool right = false;

void updateMovement()
{
	if (_isJumping) 
	{
		return;
	}

	if (_moveDir == 0) 
	{
		_speedX = 0;
		armature->getAnimation()->gotoAndPlay("stand");
	}
	else 
	{
		_speedX = _moveDir * 4;
		armature->getAnimation()->gotoAndPlay("run");
		armature->getDisplay()->setScaleX( _moveDir);
	}
}

void move(int dir) 
{	
	if (left && right) 
	{
	}
	else if (left)
	{
		dir = -1;
	}
	else if (right)
	{
		dir = 1;
	}
	else 
	{
		dir = 0;
	}

	if (_moveDir == dir) 
	{
		return;
	}
	_moveDir = dir;
	updateMovement();
}

void updateSpeed() 
{
	if (_isJumping) 
	{
		_speedY += 0.6;
	}

	if (_speedX != 0) 
	{
		armature->getDisplay()->setX(armature->getDisplay()->getX() + _speedX);
		if (armature->getDisplay()->getX() < 0) 
		{
			armature->getDisplay()->setX(0);
		}
		else if (armature->getDisplay()->getX() > 800) 
		{
			armature->getDisplay()->setX(800);
		}
}


	if (_speedY != 0) 
	{
		armature->getDisplay()->setRotation(_speedY * 0.02 * armature->getDisplay()->getScaleX());
		armature->getDisplay()->setY(armature->getDisplay()->getY() + _speedY);
		if (armature->getDisplay()->getY() > 400) 
		{
			armature->getDisplay()->setY(400);
			_isJumping = false;
			_speedY = 0;
			updateMovement();
			armature->getDisplay()->setRotation(0);
		}
	}
}

void jump()
{
	if (_isJumping) 
	{
		return;
	}
	_speedY = -15;
	_isJumping = true;
	armature->getAnimation()->gotoAndPlay("jump");
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		left = true;
		move(-1);
	}
	if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		//_speedX = 0;
		left = false;
		move(0);
	}

	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		right = true;
		move(1);
	}
	
	if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		right = false;
		move(0);
	}

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		jump();
	}

	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		Bone* _arm = armature->getBone("armOutside");
		_arm->getChildArmature()->getAnimation()->gotoAndPlay("attack_sword_1");
	}
}

void FrameEventHandeler(Event* e)
{
	FrameEvent* frameEvent = (FrameEvent*) e;
	if(frameEvent->frameLabel == "ready")
		armature->getBone("armOutside")->getChildArmature()->getAnimation()->gotoAndPlay("ready_sword");
}

int main(void)
{
	initGLFW();

	DisplayObjectContainer* scene = new DisplayObjectContainer();
	scene->setTransformationMatrix(glm::ortho( 0.0f, 800.f*1, 600.f*1, 0.0f, -1.f, 1.f));

	Image* bigLogo = new Image( new Texture("../data/images/big-logo.png") );
	Image* small_logo = new Image( new Texture("../data/images/logo.png") );
	Image* poweredby = new Image( new Texture("../data/images/poweredbydb.png") );

	poweredby->setX(800 - poweredby->texture->width);
	small_logo->setY(600 - small_logo->texture->height);
	small_logo->setX(800/2 - small_logo->texture->width/2);

	scene->addChild(bigLogo);
	scene->addChild(small_logo);
	scene->addChild(poweredby);

	TextureAtlas* textureAtlas = new TextureAtlas("../data/DragonBones/KnightJSON/texture.png",
		JSONParser::parseTextureAtlas("../data/DragonBones/KnightJSON/texture.json"));

	SkeletonData* skeleton = JSONParser::parseSkeletonData("../data/DragonBones/KnightJSON/skeleton.json");
	factory = new DBFactory();
	factory->addSkeletonData(skeleton);
	factory->addTextureAtlas(textureAtlas);
	armature = factory->buildArmature("knight");

	armature->getDisplay()->setX(400);
	armature->getDisplay()->setY(400);
	armature->getAnimation()->play();

	WorldClock->add(armature);

	armature->getBone("armOutside")->getChildArmature()->addEventListener(FrameEvent::ANIMATION_FRAME_EVENT, FrameEventHandeler);


	scene->addChild(armature->getDisplay());


	float gray = 103.0f/255.0f;

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(gray, gray, gray, 1);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		WorldClock->advanceTime(-1);
		updateSpeed();
		scene->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES,4);
	
    window = glfwCreateWindow(800, 600, "DragonBones OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);

	GLenum glew_status = glewInit();

	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	}

    glfwSetKeyCallback(window, key_callback);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}