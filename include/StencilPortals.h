#ifndef STENCILPORTALS_H
#define STENCILPORTALS_H


#include <windows.h>
#include <irrlicht.h>
#include <iostream>
#include <GL/gl.h>
#include "PortalNode.h"
#include "PortalBox.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

#pragma comment(lib, "Irrlicht.lib")


class StencilPortals
{
    private:

        IrrlichtDevice* device; //pointer to the irr device

        ISceneManager* smgr;
        IVideoDriver* driver;

        ICameraSceneNode* mainCamera;
        ICameraSceneNode* portalCamera1;
        ICameraSceneNode* portalCamera2;

        PortalNode *portal1;
        PortalNode *portal2;

        // Box around the portal
        PortalBox *PortalBox1;
        PortalBox *PortalBox2;


        core::vector3df portal1Position = core::vector3df(-90, -20, 200);
        core::vector3df portal1Rotation = core::vector3df(0, 0, 0); // rotation in degrees

        core::vector3df portal2Position = core::vector3df(-160, -20, 90);
        core::vector3df portal2Rotation = core::vector3df(0, -90, 0); // rotation in degrees

    public:
        StencilPortals();
        virtual ~StencilPortals();

        void InitStencilPortals(IrrlichtDevice* Device,
                                ICameraSceneNode* MainCamera,
                                ICameraSceneNode* PortalCamera1,
                                ICameraSceneNode* PortalCamera2);

        void refreshStencilBuffer();

        void setupPortal1();

        void setupPortal2();

        void checkCollisionWithPortals();

        void showPortals();

        void hidePortals();

        void drop();
    protected:
    private:
};
#endif // STENCILPORTALS_H
