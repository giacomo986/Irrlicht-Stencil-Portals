/** Example 002 Quake3Map

This Tutorial shows how to load a Quake 3 map into the engine, create a
SceneNode for optimizing the speed of rendering, and how to create a user
controlled camera.

Please note that you should know the basics of the engine before starting this
tutorial. Just take a short look at the first tutorial, if you haven't done
this yet: http://irrlicht.sourceforge.net/tut001.html

Lets start like the HelloWorld example: We include the irrlicht header files
and an additional file to be able to ask the user for a driver type using the
console.
*/
#include <irrlicht.h>
#include <iostream>

#include "StencilPortals.h"
#include "PortalBox.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{

    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1920, 1080), 32, true, true, false);

	if (device == 0)
		return 1; // could not create selected driver.


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files which are nothing else than .zip files.
	So we add the .pk3 file to our irr::io::IFileSystem. After it was added,
	we are able to read from the files in that archive as if they are
	directly stored on the disk.
	*/
	device->getFileSystem()->addFileArchive("../../media/map-20kdm2.pk3");

	/*
	Now we can load the mesh by calling
	irr::scene::ISceneManager::getMesh(). We get a pointer returned to an
	irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not
	really animated, they are only a huge chunk of static geometry with
	some materials attached. Hence the IAnimatedMesh consists of only one
	frame, so we get the "first frame" of the "animation", which is our
	quake level and create an Octree scene node with it, using
	irr::scene::ISceneManager::addOctreeSceneNode().
	The Octree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the Octree would be a
	irr::scene::IMeshSceneNode, which would always draw the complete
	geometry of the mesh, without optimization. Try it: Use
	irr::scene::ISceneManager::addMeshSceneNode() instead of
	addOctreeSceneNode() and compare the primitives drawn by the video
	driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn()
	method in the irr::video::IVideoDriver class). Note that this
	optimization with the Octree is only useful when drawing huge meshes
	consisting of lots of geometry.
	*/
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
//		node = smgr->addMeshSceneNode(mesh->getMesh(0));

	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));


	ICameraSceneNode* mainCamera = smgr->addCameraSceneNodeFPS(0,100.0f,0.1f);

    ICameraSceneNode* portalCamera1 = smgr->addCameraSceneNode();
    portalCamera1->bindTargetAndRotation(true);

    ICameraSceneNode* portalCamera2 = smgr->addCameraSceneNode();
    portalCamera2->bindTargetAndRotation(true);


    StencilPortals stencilPortals;

    stencilPortals.InitStencilPortals(device, mainCamera, portalCamera1, portalCamera2/*, Portal1, Portal2*/);

	/*
	The mouse cursor needs not be visible, so we hide it via the
	irr::IrrlichtDevice::ICursorControl.
	*/
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

    smgr->setActiveCamera(mainCamera);

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255, 200, 200, 200));

            stencilPortals.refreshStencilBuffer(); // restore openGL Stencil buffer
            stencilPortals.showPortals(); // showing the portals to write on stencil buffer

            smgr->drawAll(); // render the main scene

            stencilPortals.hidePortals(); // hiding the portal because it is not recursive yet
            stencilPortals.setupPortal1(); // Set up stencil parameters for rendering the scene from another camera

            smgr->drawAll();  // render the scene inside the first portal

            stencilPortals.setupPortal2(); // Set up stencil parameters for rendering the scene from another camera

            smgr->drawAll(); // render the scene inside the second portal

		    smgr->setActiveCamera(mainCamera); //

			driver->endScene();

            stencilPortals.checkCollisionWithPortals(); // if main camera enter a portal, it is positioned to the other portal

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		else
			device->yield();
	}

	stencilPortals.drop();

	device->drop();
	return 0;
}
