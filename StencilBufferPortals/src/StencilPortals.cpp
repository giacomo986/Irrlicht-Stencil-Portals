#include "StencilPortals.h"

StencilPortals::StencilPortals()
{
    //ctor
}

StencilPortals::~StencilPortals()
{
    //ctor
}

void StencilPortals::InitStencilPortals(IrrlichtDevice* Device,
                                        ICameraSceneNode* MainCamera,
                                        ICameraSceneNode* PortalCamera1,
                                        ICameraSceneNode* PortalCamera2)
{
    device = Device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();

    mainCamera = MainCamera;
    portalCamera1 = PortalCamera1;
    portalCamera2 = PortalCamera2;

    portal1 = new PortalNode(smgr->getRootSceneNode(), smgr, 16, 1);
    portal1->setPosition(portal1Position);

	portal2 = new PortalNode(smgr->getRootSceneNode(), smgr, 17, 2);
    portal2->setPosition(portal2Position);
    portal2->setRotation(portal2Rotation);


    PortalBox *PortalBox1 = new PortalBox(smgr->getRootSceneNode(), smgr, 14);
	PortalBox *PortalBox2 = new PortalBox(smgr->getRootSceneNode(), smgr, 15);

	PortalBox1->setScale(core::vector3df(1.01f, 1.01f, 1.01f));
	PortalBox1->setPosition(portal1Position);

	PortalBox2->setScale(core::vector3df(1.01f, 1.01f, 1.01f));
    PortalBox2->setPosition(portal2Position);
    PortalBox2->setRotation(portal2Rotation);

    glEnable(GL_STENCIL_TEST);
}

void StencilPortals::refreshStencilBuffer()
{
    glStencilMask(0xFF); // allow writing to stencil buffer, by default (0xFF) no mask.
    glClearStencil(0x0); // clear stencil value, by default = 0
    glClear(GL_STENCIL_BUFFER_BIT);

     // draw everywhere
    glStencilFunc(GL_ALWAYS, 0x0, 0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    driver->enableClipPlane(irr::u32(0), false);
}

void StencilPortals::setupPortal1()
{
    core::vector3df CameraPosition =  mainCamera->getPosition() - portal1Position;
    CameraPosition.rotateXZBy(-90);
    CameraPosition += portal2Position;

    portalCamera1->setPosition(CameraPosition);
    portalCamera1->setRotation(mainCamera->getRotation() + core::vector3df(0, 90, 0));

    driver->setClipPlane(irr::u32(0), plane3df(portal2Position.X, portal2Position.Y, portal2Position.Z,1,0,0), true); // clipping everything between the camera and the portal

    glStencilFunc(GL_EQUAL, 0x1, 0xFF); // Draw the scene where the stencil value is 1
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP); // do not change the stencil value

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // now we need again to draw the scene
    glClear( GL_DEPTH_BUFFER_BIT ); //clear the depth buffer for the drawing inside the portal

    smgr->setActiveCamera(portalCamera1);
}

void StencilPortals::setupPortal2()
{
    core::vector3df CameraPosition =  mainCamera->getPosition() - portal2Position;
    CameraPosition.rotateXZBy(90);
    CameraPosition += portal1Position;

    portalCamera2->setPosition(CameraPosition);
    portalCamera2->setRotation(mainCamera->getRotation() + core::vector3df(0, -90, 0));

    driver->setClipPlane(irr::u32(0), plane3df(portal1Position.X, portal1Position.Y, portal1Position.Z,0,0,-1), true); // clipping everything between the camera and the portal

    glStencilFunc(GL_EQUAL, 0x2, 0xFF); // Draw the scene where the stencil value is 2
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP); // do not change the stencil value

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // now we need again to draw the scene
    glClear( GL_DEPTH_BUFFER_BIT ); //clear the depth buffer for the drawing inside the portal

    smgr->setActiveCamera(portalCamera2);
}

void StencilPortals::checkCollisionWithPortals()
{
    core::vector3df Position = mainCamera->getPosition();

    if((Position.X < -165 && Position.X > -180) && (Position.Y < 20 && Position.Y > -60) && (Position.Z < 120 && Position.Z > 60))
    {
        mainCamera->setPosition(portalCamera2->getPosition());
        mainCamera->setRotation(portalCamera2->getRotation());
    }

    if((Position.X < -60 && Position.X > -120) && (Position.Y < 20 && Position.Y > -60) && (Position.Z < 220 && Position.Z > 205))
    {
        mainCamera->setPosition(portalCamera1->getPosition());
        mainCamera->setRotation(portalCamera1->getRotation());
    }
}

void StencilPortals::showPortals()
{
    portal1->Render = true;
    portal2->Render = true;
}

void StencilPortals::hidePortals()
{
    portal1->Render = false;
    portal2->Render = false;
}

void StencilPortals::drop()
{
    portal1->drop();
	portal1 = 0;

	portal2->drop();
	portal2 = 0;
}
