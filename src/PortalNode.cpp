#include "PortalNode.h"

using namespace irr;

PortalNode::PortalNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int portalNumber)
		: scene::ISceneNode(parent, mgr, id)
{
    Material.Wireframe = false;
    Material.Lighting = false;

    PortalNumber = portalNumber;

    Vertices[0] = video::S3DVertex(30, -40, 20, 0, 0, 1,
            video::SColor(255,0,0,255), 0, 1);
    Vertices[1] = video::S3DVertex(-30, -40, 20,0, 0, 1,
            video::SColor(255,0,0,255), 1, 1);
    Vertices[2] = video::S3DVertex(-30, 40, 20, 0, 0, 1,
            video::SColor(255,0,0,255), 1, 0);
    Vertices[3] = video::S3DVertex(30, 40, 20,  0, 0, 1,
            video::SColor(255,0,0,255), 0, 0);
    Vertices[4] = video::S3DVertex(30, -40, 0,  0, 0, 1,
            video::SColor(255,0,0,255), 0, 1);
    Vertices[5] = video::S3DVertex(-30, -40, 0, 0, 0, 1,
            video::SColor(255,0,0,255), 1, 1);
    Vertices[6] = video::S3DVertex(-30, 40, 0,  0, 0, 1,
            video::SColor(255,0,0,255), 1, 0);
    Vertices[7] = video::S3DVertex(30, 40, 0,   0, 0, 1,
            video::SColor(255,0,0,255), 0, 0);

    /*
    The Irrlicht Engine needs to know the bounding box of a scene node.
    It will use it for automatic culling and other things. Hence, we
    need to create a bounding box from the 4 vertices we use.
    If you do not want the engine to use the box for automatic culling,
    and/or don't want to create the box, you could also call
    irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
    */
    Box.reset(Vertices[0].Pos);
    for (s32 i=1; i<8; ++i)
        Box.addInternalPoint(Vertices[i].Pos);
}

void PortalNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_TRANSPARENT);

    ISceneNode::OnRegisterSceneNode();
}

/*
*/
void PortalNode::render()
{
    if (Render)
    {
        u16 indices[] = {0,1,3, 1,2,3,  0,3,7, 0,7,4,  4,5,1, 4,1,0,  1,5,6, 1,6,2,  2,6,7,  2,7,3};
        video::IVideoDriver* driver = SceneManager->getVideoDriver();

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // we do not need to draw on color buffer, we only need to draw on stencil buffer
        glStencilFunc(GL_ALWAYS, PortalNumber, 0xFF); // we check everywhere on the screen
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE); // we write 1 on stencil buffer where the portal is visible. zpass => portal is the nearest thing*/

        driver->setMaterial(Material);
        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
        driver->drawVertexPrimitiveList(&Vertices[0], 8, &indices[0], 10, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);

        glStencilFunc(GL_ALWAYS, 0, 0xFF); // if there is something to render between camera and portal, stencil value return 0
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE); //

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // now we can draw again on color buffer
    }
}

const core::aabbox3d<f32>& PortalNode::getBoundingBox() const
{
    return Box;
}

u32 PortalNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& PortalNode::getMaterial(u32 i)
{
    return Material;
}
