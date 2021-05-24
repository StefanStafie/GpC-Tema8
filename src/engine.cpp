#include "engine.h"
#include <math.h>
using namespace std;
using namespace SdimpleRocket;

std::vector< Vector3 > trailCache;
Vector3 peCache;
Vector3 apCache;



Engine::Engine(unsigned int screenWidth, unsigned int screenHeight) {
  m_screen1.w = screenWidth;
  m_screen1.h = screenHeight;
  m_screen1.updateRatio();
  if(!init()) {
    cerr<<"Error. Could not initialise engine."<<endl;
    exit(1);
  }
  m_startTime = 0;
  m_currentTime = 0;
  m_frames = 0;
  m_pFrames = 0; //phys sim frames

  //hacks below
  m_rotation = 0;
  m_rotationIncrease = 0.0001;
  m_resetRotation = 0;
  m_thrust = false;

  scene1();
}

void Engine::scene1() {
    //accelerate the physics simulation
  m_scene.setTimeMul(1024);

  m_controlledObject = 1;
  DataObject o;
  o = m_cache.addObject("data/moon.obj", "data/moon.bmp");
  m_scene.addObject(o, Vector3(0, 0, 0), 100, 10000000000.0, Vector3(), 0, 0.001);
  //eh, manual set rotation
  m_scene.begin()->setGlRotationAngle(90);
  m_scene.begin()->setGlRotationAxis(Vector3(1, 0, 0));

  Light l;
  l.lightId = GL_LIGHT1;
  l.amb[0] = 1; l.amb[1] = 1; l.amb[2] = 1; l.amb[3] = 1;
  l.dif[0] = 1; l.dif[1] = 0.5; l.dif[2] = 0.2; l.dif[3] = 1;
  l.spe[0] = 1; l.spe[1] = 0.5; l.spe[2] = 0.5; l.spe[3] = 1;
  l.offset = Vector3(0, -0.5, 0); //on the bottom
  o = m_cache.addObject("data/ares.obj", "data/ares.bmp");
  m_scene.addObject(o, Vector3(0, 115, 0), 30, 30000, Vector3(-0.07, 0, 0), M_PI / 2.0, 0, Vector3(0, 10, 0));
  (m_scene.begin() + 1)->m_light = l;//worst hack
  (m_scene.begin() + 1)->m_hasLight = true;

  //  o = m_cache.addObject("data/ares.obj", "data/purple.bmp");
  //m_scene.addObject(o, Vector3(0, -130, 0), 30, 90000, Vector3(-0.08, 0.5, 0), - M_PI / 2.0); // try this to see the dynamic object scale
  //m_scene.addObject(o, Vector3(0, -110, 0), 30, 90000, Vector3(-0.05, 0.01, 0), - M_PI / 2.0);

  //o = m_cache.addObject("data/moon.obj", "data/moon.bmp");
  //m_scene.addObject(o, Vector3(-100, 0, 0), 80, 10000000000, Vector3(0, -0.2, 0), 0, 0.001);

}

void Engine::scene2() {
  m_scene.setTimeMul(1);

  m_controlledObject = 1;
  DataObject o;
  //earth
  o = m_cache.addObject("data/moon.obj", "data/blue.bmp");
  m_scene.addObject(o, Vector3(0, 0, 0), 6371000, 5.972 * pow(10, 24), Vector3(), 0, 0.001); //earth radius and mass, but not its angular velocity
  //eh, manual set rotation
  m_scene.begin()->setGlRotationAngle(90);
  m_scene.begin()->setGlRotationAxis(Vector3(1, 0, 0));

 
  Light l;
  l.lightId = GL_LIGHT1;
  l.amb[0] = 0; l.amb[1] = 0; l.amb[2] = 0; l.amb[3] = 0;
  l.dif[0] = 1; l.dif[1] = 0.5; l.dif[2] = 0.2; l.dif[3] = 1;
  l.spe[0] = 1; l.spe[1] = 0.5; l.spe[2] = 0.5; l.spe[3] = 1;
  l.offset = Vector3(0, -0.5, 0); //on the bottom
  o = m_cache.addObject("data/ares.obj", "data/ares.bmp");
  m_scene.addObject(o, Vector3(0, 6371000, 0), 30, 30000, Vector3(-7800, 0, 0), M_PI / 2.0, 0, Vector3(0, 12500000, 0));
  (m_scene.begin() + 1)->m_light = l;//worst hack
  (m_scene.begin() + 1)->m_hasLight = true;


  //moon
  o = m_cache.addObject("data/moon.obj", "data/moon.bmp");
  
  m_scene.addObject(o, Vector3(0, 380000000.0, 0), 1737000, 7.342 * pow(10, 22), Vector3(-1022, 0, 0), 0, 0.001); //moon radius and mass, but not its angular velocity. makes for bad gameplay at this stage
  
  //eh, manual set rotation
  m_scene.begin()->setGlRotationAngle(90);
  m_scene.begin()->setGlRotationAxis(Vector3(1, 0, 0));
}

void Engine::scene3() {
  m_scene.setTimeMul(1);

  m_controlledObject = 1;
  DataObject o;
  //earth
  o = m_cache.addObject("data/moon.obj", "data/blue.bmp");
  m_scene.addObject(o, Vector3(0, 0, 0), 6371000, 5.972 * pow(10, 24), Vector3(), 0, 0.001); //earth radius and mass, but not its angular velocity
  //eh, manual set rotation
  m_scene.begin()->setGlRotationAngle(90);
  m_scene.begin()->setGlRotationAxis(Vector3(1, 0, 0));
 
  Light l;
  l.lightId = GL_LIGHT1;
  l.amb[0] = 0; l.amb[1] = 0; l.amb[2] = 0; l.amb[3] = 0;
  l.dif[0] = 1; l.dif[1] = 0.5; l.dif[2] = 0.2; l.dif[3] = 1;
  l.spe[0] = 1; l.spe[1] = 0.5; l.spe[2] = 0.5; l.spe[3] = 1;
  l.offset = Vector3(0, -0.5, 0); //on the bottom
  o = m_cache.addObject("data/ares.obj", "data/ares.bmp");
  m_scene.addObject(o, Vector3(0, 6371000, 0), 30, 30000, Vector3(-7800, 0, 0), M_PI / 2.0, 0, Vector3(0, 12500000, 0));
  (m_scene.begin() + 1)->m_light = l;//worst hack
  (m_scene.begin() + 1)->m_hasLight = true;

  //moon
  o = m_cache.addObject("data/moon.obj", "data/moon.bmp");
  
  m_scene.addObject(o, Vector3(0, 30000000, 0), 1737000, 7.342 * pow(10, 22), Vector3(-3500, 0, 0), 0, 0.001); //moon radius and mass, but not its angular velocity. makes for bad gameplay at this stage
  
  //eh, manual set rotation
  m_scene.begin()->setGlRotationAngle(90);
  m_scene.begin()->setGlRotationAxis(Vector3(1, 0, 0));

}


Engine::~Engine() {
  close();
}

void Engine::loadScene(std::string sceneFile) {
}

bool Engine::init() {
  m_screen1.active = true;
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr<<"Error. SDL_Init(SDL_INIT_VIDEO) failed. SDL Error: "<<SDL_GetError()<<endl;
    return false;
  }
  //Use OpenGL 2.1. Among other things, VBOs are part of the core OpenGL 2.1 specs.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //Set Depth Buffer datatype bit size. Greater size means better precision when choosing which object is closer.
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // errors might occur here. Change to 16 or 32, or remove line.
  //32-bit RGBA display
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  //disabled for maximum compatibility
  //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // change to 2 for faster display
  //hw accelerated graphics
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 

  //After setting up the parameters of the above, we now create the window.
  m_screen1.window = SDL_CreateWindow("Sdimple rocket", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screen1.w, m_screen1.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
  if(m_screen1.window == NULL) {
    cerr<<"Error. SDL_CreateWindow(...) failed. SDL Error: "<<SDL_GetError()<<endl;
    return false;
  }
  //the window is the window manager object, the context is the image we draw on.
  m_screen1.context = SDL_GL_CreateContext(m_screen1.window);
  if(m_screen1.context == NULL) {
    cerr<<"Error. OpenGL context creation failed. SDL Error: "<<SDL_GetError()<<endl;
    return false;
  }
  //Enable Vertical Sync, to provide framerate control and lessen image tearing.
  if(SDL_GL_SetSwapInterval(1) < 0) {
    cerr<<"Warning. Activating VSync failed. SDL Error: "<<SDL_GetError()<<endl;
  }
  if(!initGl()) {
    cerr<<"Error during OpenGL init."<<endl;
    return false;
  }

  //we check if we truely have the OpenGl version we requested, at least 2.1

  //version, sub-version of our OpenGL version
  int v = 0, sv = 0, i;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &v);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sv);
  
  //we get SDL to report the OpenGL version. Easy, right?
  cout<<"SDL reports GL version "<<v<<"."<<sv<<endl;

  //No. Unfortunately, we can get an OpenGL version different than what we requested, or what SDL thinks it's giving us. We need to ask OpenGL
  const GLubyte *openGlVer;
  char c;
  openGlVer = glGetString(GL_VERSION);
  i = 0;
  //searching for the first number in the string, which should be the start of the version number. We want this number to not be 1. So, we search for the string '1.'. If this is found, we know our opengl version is 1 (and not 10 or something else).
  while(openGlVer[i] != 0) {
    c = openGlVer[i];
    //we break at the first number we encounter, we don't look for 1's in the string beyond
    if(c <= '9' && c >= '1') {
      if(c == '1' && openGlVer[i + 1] == '.') {//we can [i + 1], since strings are null-terminated, and we know [i] != 0
	//oops. We do have an OpenGl version 1 context.
	cerr<<"Error: However, the current context is an OpenGl version 1 context("<<glGetString(GL_VERSION)<<").";
	if(!SDL_GL_ExtensionSupported("GL_ARB_vertex_buffer_object")) {
	  //our video context (likely, driver or hardware) cannot use Vertex Buffer Objects.
	  cerr<<" Exiting."<<endl;
	  return false;
	}
	else {
	  //while we don't have a high-enough OpenGL version to get VBOs in core OpenGL, we do seem to have the extension available.
	  cerr<<" Since GL_ARB_vertex_buffer_object extension is present, an attempt will be made to fall back to it."<<endl;
	  //simply bind the empty function pointers, defined in glew, with those present in the ARB extensions.
	  //GL_ARRAY_BUFFER and the like are just constants, defined already by glew. No need to assign them.
	  if(glBindBuffer == NULL) //we check if the function pointers are initialised, step-by step. In the strange, crazy case that some of these functions _are_ pointing someplace valid, we don't overwrite GLEW arcana.
	    glBindBuffer = glBindBufferARB;
	  if(!glGenBuffers)
	    glGenBuffers = glGenBuffersARB;
	  if(!glBufferData)
	    glBufferData = glBufferDataARB;
	  if(!glBufferSubData)
	    glBufferSubData = glBufferSubDataARB;
	  if(!glGetBufferParameteriv)
	    glGetBufferParameteriv = glGetBufferParameterivARB;
	  if(!glGetBufferSubData)
	    glGetBufferSubData = glGetBufferSubDataARB;
	  if(!glDeleteBuffers)
	    glDeleteBuffers = glDeleteBuffersARB;
	  if(!glDrawRangeElements) //widely used draw function. If using similar functions, add them here.
	    glDrawRangeElements = glDrawRangeElementsEXT;
	  cerr<<"Finished binding ARB functions."<<endl;
	}
      }
      break;
    }
  }
  //if this is disabled, the scene - and the screen - will be dark
  initLights();
  //see explanation in  struct screen  definition; the memory region this points to will be updated thoughout the program (through a SDL function), as keys are pressed and released
  m_screen1.kbState = SDL_GetKeyboardState(NULL);
  return true;
}

bool Engine::initGl() {
  //set the error variable. We'll use it to ask OpenGL if (and which) errors occured.
  GLenum error = GL_NO_ERROR;
  //we check if VBOs are supported in hardware/driver. Still, we hope we won't be using the functions ending in ARB in our code.
  if(!SDL_GL_ExtensionSupported("GL_ARB_vertex_buffer_object")) {
    cerr<<"Error: GL ARB VBO extension not supported."<<endl;
    return false;
  }
  //enable multisampling
  glEnable(GL_MULTISAMPLE);

  //glDisable(GL_BLEND); //weird wiremap/texture interaction
  //glBlendFunc(GL_ONE, GL_ONE);
  
  // Enable Texture Mapping.
  glEnable(GL_TEXTURE_2D);
  
  // Enable smooth shading 
  glShadeModel(GL_SMOOTH);
  
  // Depth buffer setup
  glClearDepth(1.0f);
  // Enables Depth Testing
  glEnable(GL_DEPTH_TEST);
  // strictly closest geometry will be shown
  glDepthFunc(GL_LESS);
  
  // Nicest Perspective Calculations
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
  //primitive smoothing
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  //enable lights and shadows. if there are no lights set up, the display will be entirely dark
  glEnable(GL_LIGHTING);
  
  //this enables the culling (i.e. not drawing) of the back faces of primitives; try setting culling to GL_FRONT for x-ray vision
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  //Initialize Projection Matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //set up a perspective projection. The horizontal view angle is 90degrees; it also needs the screen ratio (so we don't get a distorted image), near clipping plane, far clipping plane.
  gluPerspective(90.0f, m_screen1.ratio, 2.0f, 100.0f);
  //Check for errors until now
  error = glGetError();
  if(error != GL_NO_ERROR) {
    cerr << "Error initialising OpenGL! " << gluErrorString(error);
    return false;
  }

  glClearColor(0, 0, 0, 1);
  //set the initial modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //initialising glew, for VBOs and other extensions, as needed.
  error = glewInit();
  if(GLEW_OK != error) {
    cerr<<"Error initialising GLEW! "<<glewGetErrorString(error)<<endl;
    return false;
  }
  else {
    //output OpenGL-related information
    cout<<"Initialised OpenGl version: "<<glGetString(GL_VERSION)<<"\nVendor "<<glGetString(GL_VENDOR)<<"\nRenderer: "<<glGetString(GL_RENDERER)<<"\nExtensions:"<<endl<<glGetString(GL_EXTENSIONS)<<endl;
  }
  return true;
}

void Engine::initLights() {
  //all coordinates are homogenous coordinates (xh, yh, zh, w). To get cartesian coordinates, simply x = xh/w.
  //colors are RGBA
  
  //this is starlight. It is somewhat blue-ish. Also, it has a low alpha value: we seek to simulate eye adaptation: when it is the only ligh source, it should be somewhat visibile, but not affect the perception of brigter lightsources. TODO: does it work like this?
  //GLfloat globalAmbient[] = {0.05+1, 0.1+1, 0.2+1, 0.001+1};
  GLfloat globalAmbient[] = {0.05+1, 0.1+1, 0.2+1, 0.001+1};

  //this is the default light, so the scene isn't dark.
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

  //now, the 'Sun'
  //set up light colors (ambient, diffuse, specular)
  //ambient light means the omnidirectional, general background light, without a lightsource
  GLfloat ambCol[] = {1, 1, 1, 1};
  //diffuse light means the most common concept of light - a source lights a surface, according to direction and distance. Most displays have a 6400k white, the same approximate color as Sol. However, I chose a sliightly more orange color, of approx (255, 255, 251).
  GLfloat difCol[] = {1.f, 1.f, .9843f, 1.0f}; 
  //the reflections of a shiny surface; metallic sheen
  GLfloat speCol[] = {1, 1, 1, 1};

  //set up our chosen light parameters to the first OpenGL light. Be aware these are limited, as low as 4 or 8 in some cases.
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambCol);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, difCol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, speCol);
  
  // position the light
  float pos[4] = {-2, 0, 1, 0}; // positional light; left, up, and towards the viewer, and positioned at infinity
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  //after defining the color and position of the light, we can enable it
  glEnable(GL_LIGHT0);   
}

void Engine::close() {
  SDL_DestroyWindow(m_screen1.window);
  m_screen1.window = NULL;
  SDL_Quit();
}

void Engine::resize(SDL_Event *e) {
  m_screen1.w = e->window.data1;
  m_screen1.h = e->window.data2;
  m_screen1.updateRatio();
  //update the viewport to match the window
  glViewport(0, 0, m_screen1.w, m_screen1.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0f, m_screen1.ratio, 2, 100);
  //we don't change the mode to modelview. It should be placed before any operations that need it.
  //glMatrixMode(GL_MODELVIEW);
}

void Engine::toggleFullscreen(bool fake) {
  //real fullscreen
  //    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
  //fake (and superior) fullscreen
  // Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
  Uint32 fullscreenFlag;
  if (fake) 
    fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
  else
    fullscreenFlag = SDL_WINDOW_FULLSCREEN;
  bool isFullscreen = SDL_GetWindowFlags(m_screen1.window) & fullscreenFlag;
  SDL_SetWindowFullscreen(m_screen1.window, isFullscreen ? 0 : fullscreenFlag);
  //toggle mouse off in fullscreen:
  //SDL_ShowCursor(IsFullscreen);
}

void Engine::handleKeys(SDL_Event *e, int x, int y) {
  //use this to debug / get key values:
  //  cout << "Physical key: " << SDL_GetScancodeName(e->key.keysym.scancode) <<  " acting as key: " << SDL_GetKeyName(e->key.keysym.sym) << endl;
  
  //Esc: quit
  if (k(SDL_SCANCODE_ESCAPE))
    m_screen1.active = false;
  //Alt-Enter: fake fullscreen
  if ((k(SDL_SCANCODE_LALT) || k(SDL_SCANCODE_RALT)) && !(k(SDL_SCANCODE_LCTRL) || k(SDL_SCANCODE_RCTRL)) && k(SDL_SCANCODE_RETURN)) {
    toggleFullscreen();
  }
  //Ctrl-Alt-Enter: true fullscreen
  if ((k(SDL_SCANCODE_LALT) || k(SDL_SCANCODE_RALT)) && (k(SDL_SCANCODE_LCTRL) || k(SDL_SCANCODE_RCTRL)) && k(SDL_SCANCODE_RETURN)) {
    toggleFullscreen(false);
  }
  //rocket controls
  //w controls thrust, a rotates left, d rotates right
  m_thrust = 0;
  m_rotation = 0;
  if(k(SDL_SCANCODE_W))
    m_thrust = 1;
  if(k(SDL_SCANCODE_A))
    m_rotation = 1;
  if(k(SDL_SCANCODE_D))
    m_rotation = -1;
  //both a and d null the rotation
  if(k(SDL_SCANCODE_A) && k(SDL_SCANCODE_D))
    m_rotation = 0;
  //t nulls rotation
  if(k(SDL_SCANCODE_T))
    m_resetRotation = 1;
  else
    m_resetRotation = 0;

  //z slows down time, x accelerates time
  if(k(SDL_SCANCODE_Z)) {
    Float timeFactor = 0.5 * m_scene.getTimeMul();
    if(timeFactor < 1) timeFactor = 1;
    m_scene.setTimeMul(timeFactor);
    cout<<"Time factor: " << m_scene.getTimeMul() << endl;
  }
  if(k(SDL_SCANCODE_X)) {
    m_scene.setTimeMul(2.0 * m_scene.getTimeMul());
    cout<<"Time factor: " << m_scene.getTimeMul() << endl;
  }
  if(k(SDL_SCANCODE_C)) {
    m_scene.setTimeMul(1);
    cout<<"Time factor: " << m_scene.getTimeMul() << endl;
  }
}

void inline Engine::update() {
  //No per frame update needed
}

void inline Engine::renderObject(DataObject d) {
  m_cache.renderObject(d);
}

void Engine::renderLight(Light l, bool active) {
  //if an object has a light, this function can render it.
  GLuint id = l.lightId;
  Vector3 r = l.offset;
  if(active) {
    glLightfv(id, GL_AMBIENT, l.amb);
    glLightfv(id, GL_DIFFUSE, l.dif);
    glLightfv(id, GL_SPECULAR, l.spe);
    //homogenous coordinates, x, y, z, w. w = 1 makes them coincide with cartesian coordinates.
    GLfloat lightPos[4] = {GLfloat(r.getX()), GLfloat(r.getY()), GLfloat(r.getZ() + 1), 1};//slightly above the horizontal plane, so lights are even more visible. Also, we don't set any fall-offs, for maximum light visibility.
    glLightfv(id, GL_POSITION, lightPos);
    glEnable(id);
  }
  else {
    glDisable(id);
  }
}

// describes the vertex points inside a line
void drawLine(float x0, float y0, float x1, float y1) {
  int segments = 20;
  float x_deviation, y_deviation;
      
  x_deviation = (x0 - x1)/segments;    
  y_deviation = (y0 - y1)/segments;
  for(int i = 0; i<segments; i++){
      glVertex2f(x1 + x_deviation * i, y1 + y_deviation * i);
  }
}


// calculates and 
void Engine::orbitalTrajectory(SceneObject ship, SceneObject planet, int max_steps, bool recalculate) {
  if (recalculate) {  
    trailCache.clear();
    Trail trail(ship.getPosition(), ship.getMass(), ship.getVelocity());

    int step = 0;
    Float r2, d_ap = 0, d_pe = 999999999;
    Vector3 r, accel, ap, pe;
    Float k_G = 0.000000000066740831;
    Float pantaInit = trail.getPanta();
    Float xInit = trail.getPosition().getX();
    Float yInit = trail.getPosition().getY();
    bool outOfQuarter = false;
    bool clockwise = false;  
    Float vx = trail.getVelocity().getX();
    Float vy = trail.getVelocity().getY();  
    if (xInit > 0 && yInit > 0) {// cadran NE
      if (vx > 0)
        clockwise = true; 
    } else if (xInit < 0 && yInit > 0) {// cadran NW
      if (vy > 0)
        clockwise = true; 
    } else if (xInit > 0 && yInit < 0) {// cadran SE
      if (vx < 0)
        clockwise = true; 
    } else if (xInit < 0 && yInit < 0) {// cadran SW
      if (vy > 0)
        clockwise = true; 
    } 

    // forward projection
    do{    
      trail.setAcceleration();
      r = planet.getPosition()-trail.getPosition();
      r2 = r.getNorm2();
      accel = r;
      accel.normalise();
      accel *= k_G * planet.getMass() / r2;
      trail.addAcceleration(accel);
      trail.integrateAccel(0.00015/(accel.distance()));
      
      if (trail.getPosition().distance() > d_ap) {
        d_ap = trail.getPosition().distance();
        ap = trail.getPosition();
      }
      if (trail.getPosition().distance() < d_pe) {
        d_pe = trail.getPosition().distance();
        pe = trail.getPosition();
      } 


      if (outOfQuarter) {
        if (trail.getPanta() * pantaInit > 0 && trail.getPosition().getX() * xInit > 0 && trail.getPosition().getY() * yInit > 0)
          if (clockwise && trail.getPanta() - pantaInit < 0 || !clockwise && trail.getPanta() - pantaInit > 0)        
            {cout<<trail.getPanta() << "  " <<pantaInit<<endl; break;}  
      } else {
        if (trail.getPosition().getX() * xInit < 0 && trail.getPosition().getY() * yInit < 0)
          outOfQuarter = true;
      }
      if (step % 50 == 0)
        trailCache.push_back(trail.getPosition());
    } while (++step < max_steps);
    /*
    // backward projection:
    step = max_steps;
    trail = Trail(ship.getPosition(), ship.getMass(), ship.getVelocity());
    trail.setVelocity(-trail.getVelocity());
    do {
      trail.setAcceleration();
      r = planet.getPosition()-trail.getPosition();
      r2 = r.getNorm2();
      accel = r;
      accel.normalise();
      accel *= k_G * planet.getMass() / r2;
      trail.addAcceleration(accel);
      trail.integrateAccel(0.0005/(accel.distance()));
      
      if(step % 5 == 0)
        glVertex2d(trail.getPosition().getX(), trail.getPosition().getY());    
    } while (++step < max_steps); 
    */
    apCache = ap;
    peCache = pe;
  } 

  glLineWidth(1);
  glColor3f(0, 250, 0);
  glBegin(GL_LINE_STRIP);



  for(std::vector<Vector3>::iterator it = trailCache.begin() ; it != trailCache.end(); ++it) {
    glVertex2d(it->getX(), it->getY());
  }
  glEnd();

  glBegin(GL_LINES);
  glVertex2d(apCache.getX(), apCache.getY());
  glVertex2d(0, 0);
  glEnd();
  
  glLineWidth(8);
  glBegin(GL_LINES);
  glVertex2d(peCache.getX(), peCache.getY());
  glVertex2d(0, 0);
  glEnd();

}



void Engine::renderMyHomeworkUi(unsigned int object, int time_elapsed) {

  std::vector< SceneObject > m_objects = m_scene.getObjects();
  vector<SceneObject>::iterator begin, end, ii, centerMass, self;
  begin = m_objects.begin(); end = m_objects.end();
  Float maxForce = 0, force = 0;
  float oldX = 0, oldY = 0;

  //draw orbital trajectory
  if(time_elapsed >= 2000) { //every 0.5 secs
    orbitalTrajectory(m_objects[1], m_objects[0], 30000, true);
  } else {
    orbitalTrajectory(m_objects[1], m_objects[0], 30000, false);
  }
  
  //calculate distance between moon and rocket-------------------------------------------------------------------
  //set up an ortho matrix, but save the previous matrix state
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  //  glOrtho(-1, 1, -1, 1, -1, 1);//we can set up a different perspective in the '2d' UI. It is merely an overlay, and we can still draw 3d objects, if so desired.

  //save modelview matrix state, so we can change it here, if needed
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  //disable various 3d-related operations
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glClear(GL_DEPTH_BUFFER_BIT);

  float distance = (m_objects[0].getPosition() - m_objects[1].getPosition()).distance();
  float velocity = (m_objects[0].getVelocity() - m_objects[1].getVelocity()).distance();
    
  distance = log(distance * distance);
  velocity= log(velocity * velocity * 1000);  


  Float s = -1, inc, offset=-1;
  inc = 0.01;
  glColor3f(250, 0, 0);
  glLineWidth(1);
  glBegin(GL_LINES);
  for(float i = 5; i< distance; i+=0.1) {
    glVertex2d(-1, offset);
    glVertex2d(-0.9, offset);
    offset += inc;
  }
  glEnd();

  glColor3f(0, 250, 0);
  offset = -1;
  glBegin(GL_LINES);
  for(float i = 0; i< velocity; i+=0.05) {
    glVertex2d(-0.9, offset);
    glVertex2d(-0.8, offset);
    offset +=inc;
  }
  glEnd();

  //re-enable and restore previous state
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

//render a '2d' Ui on top of a 3d scene
void Engine::renderUi() {
  //set up an ortho matrix, but save the previous matrix state
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  //  glOrtho(-1, 1, -1, 1, -1, 1);//we can set up a different perspective in the '2d' UI. It is merely an overlay, and we can still draw 3d objects, if so desired.

  //save modelview matrix state, so we can change it here, if needed
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  //disable various 3d-related operations
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glClear(GL_DEPTH_BUFFER_BIT);


  //actual UI drawing
  int p;
  p = log(m_scene.getTimeMul()) / log(2);
  if(p >=1) {
    Float start = -1, end = 1, inc, offset;
    inc = (end - start) / Float(2 * p - 1);
    glLineWidth(3);
    glColor3f(10.5, 10, 10.5);
    glBegin(GL_LINES);
    for(offset = start; offset <= end; ) {
      glVertex2d(offset, -1);
      offset += inc;
      glVertex2d(offset, -1);
      offset +=inc;
    }
    glEnd();
  }
  //re-enable and restore previous state
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void Engine::render() {
  //we're drawing a new frame
  ++m_frames;
  //time since SDL initialised
  GLint time = SDL_GetTicks();
  //the time since last we called SDL_GetTicks()
  m_frameTime = time - m_currentTime;
  /*if(m_frameTime > 100) { //it more than 100 milliseconds passed since the last frame, slow down time accel
    m_scene.setTimeMul(m_scene.getTimeMul() / 2);
    cout<<"Engine is too slow. Decreasing time multiplier to "<<m_scene.getTimeMul()<<endl;
  }*/
  //updating current time
  m_currentTime = time;
  //the time since last we displayed a FPS status
  GLint deltaTime = m_currentTime - m_startTime;
  if(deltaTime >= 2000) { //every two seconds
    cout << "FPS: " << m_frames * 1000.0 / deltaTime << endl;
    cout << "PFPS: " << m_pFrames * 1000.0 / deltaTime << endl;
    m_startTime = m_currentTime;
    m_frames = 0;
    m_pFrames = 0;
    std::vector<SceneObject> m_objects = m_scene.getObjects();

    float distance = (m_objects[0].getPosition() - m_objects[1].getPosition()).distance();
    float velocity = (m_objects[0].getVelocity() - m_objects[1].getVelocity()).distance(); 
    cout<<"Distance between rocket and moon: " << distance << "\n Relative velocity of moon-rocket: " << velocity<<"\n";
    cout<<"Ap: "; 
    apCache.print();
    cout<< "Pe: ";
    peCache.print();
    cout<<endl;
    cout<<"Extra info position of rocket: "<< m_objects[1].getPosition().getX() << " " << m_objects[1].getPosition().getY()<< "\n ";
    cout<<"Extra info velocity of rocket: "<< m_objects[1].getVelocity().getX() << " " << m_objects[1].getVelocity().getY()<< "\n ";
  }

  Vector3 tv; //temp vector
  Float tf; //temp float
  std::vector< SceneObject >::iterator it, end;

  //eh, hack, but the whole rotation is a hack. Quaternions are the way to go.
  // we modify the current rotation speed for the second object
  if((m_resetRotation || m_rotation) && m_scene.getObjectCount() > m_controlledObject) {
    it = m_scene.begin() + m_controlledObject;
    if(m_rotation) {
      tf = m_rotation;
      tf *= m_rotationIncrease;
      it->setAngularVelocity(it->getAngularVelocity() + tf);
    } //reset rotation only if we don't change the rotation, at the same time
    else if(m_resetRotation) {
      it->setAngularVelocity(0);
    }
  }
    
  //Clear The Screen And The Depth Buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //backing off the camera just a bit, so we can see deeper 3d objects
  /*since we gave a perspective horizontal view angle of 90 degrees, the horizontal visible size of z = 0 is 10.
    45d/|\45d
      / | \
     /  |5 \
    /   |   \
   /  5 |  5 \
   -----------45d
  */
  glTranslatef(0.0f, 0.0f, -5.0f);

  //advance simulation, with a certain time. Also, if our input tells us to apply engine thrust, we pass that to the simulation.
  m_pFrames += m_scene.nextFrame(Float(m_frameTime) / 1000.0, m_thrust); //we convert from milliseconds to seconds
  
  //apply the general scene scale. This ensures all objects are small enough to be in frame
  tf = m_scene.getScale();
  glScalef(tf, tf, tf);
  
    
  //we apply the general scene translation. This centers on the geometric center of all objects (thus, of the scene)
  tv = m_scene.getOffset();
  glTranslatef(tv.getX(), tv.getY(), tv.getZ());
  
  //now, we draw each object in the scene, in their proper positions and orientations.
  it = m_scene.begin();
  end = m_scene.end();
  while(it != end) {
    //since these are local to each object, we save and restore the modelview matrix after we're done
    glPushMatrix();
    //TODO: at the moment, we don't deal with many of the gl adjustment properties of the object
    //set object position
    tv = it->getPosition();
    glTranslatef(tv.getX(), tv.getY(), tv.getZ());

    //set object in-plane 'physics' rotation
    tf = it->getRotation() * 180.0 / M_PI; //OpenGL has degree-based angles; we use pi-radian angles in our math, but degree-based in gl-related tasks. O_o
    glRotatef(tf, 0, 0, 1); //rotation in the plane of the screen

    //apply glRotation, the correction added to the model
    tf = it->getGlRotationAngle();
    tv = it->getGlRotationAxis();
    glRotatef(tf, tv.getX(), tv.getY(), tv.getZ());

    //set object visual scale. It involves object size and the model scale adjustment
    tf = it->getSize(); //we do apply object visual scale adjustment here
    //glScalef(tf * it->getGlScale().getX(), tf * it->getGlScale().getY(), tf * it->getGlScale().getZ());
    tf *= it->getGlScale().getX();//uniformous scale
    glScalef(tf, tf, tf);

    //the light should be rendered if the object is thrusting, since it's engine light
    if(it->m_hasLight) {
      renderLight(it->m_light, it->isThrustActive() & m_thrust);
    }
    //this wrapper set up the modelview matrix. Now, we call the actual 'drawing' function
    renderObject(it->getDataObject());
    ++it;
    glPopMatrix(); //the transformations in this block are local to the current object
  }
  
  renderMyHomeworkUi(0, deltaTime);
  //after rendering the 3d scene, draw the 2d UI 'on top'. it is currently not accelerated, so it might slow down the engine
  renderUi();

}

void Engine::run(unsigned int steps) {
  //this is an event loop. It scans for and processes events using SDL.
  SDL_Event e;
  int x, y, step = 1;
  x = y = 0;
  if (steps == 0) {
    steps = 1;
    step = 0;
  }
  SDL_StartTextInput();
  while(m_screen1.active && steps > 0) {
    //we can run a finite number of steps, or until window close.
    steps -= step;
    while(SDL_PollEvent(&e) != 0) {
      switch(e.type) {
      case SDL_QUIT:
	m_screen1.active = false;
	break;
      case SDL_WINDOWEVENT:
	if(e.window.event == SDL_WINDOWEVENT_RESIZED) {
	  resize(&e);
	}
	break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
	SDL_GetMouseState(&x, &y);
	handleKeys(&e, x, y);
	break;
      }
    }
    render();
    //since we're using double-buffering
    SDL_GL_SwapWindow(m_screen1.window);
    //add at least this delay. Good to cede control of the CPU, so our program doesn't lock it up during normal execution.
    SDL_Delay(0);
  }
  //stop capturing keyboard as we close the event loop
  SDL_StopTextInput();
}
