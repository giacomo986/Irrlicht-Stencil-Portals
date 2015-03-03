#include "PortalBox.h"

using namespace irr;

PortalBox::PortalBox(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
		: scene::ISceneNode(parent, mgr, id)
{
    Material.Wireframe = false;
    Material.Lighting = false;
    Material.BackfaceCulling = false;

    Vertices[0] = video::S3DVertex(30, -40, 20, 0, 0, 1,
            video::SColor(255,0,0,0), 0, 1);
    Vertices[1] = video::S3DVertex(-30, -40, 20,0, 0, 1,
            video::SColor(255,0,0,0), 1, 1);
    Vertices[2] = video::S3DVertex(-30, 40, 20, 0, 0, 1,
            video::SColor(255,0,0,0), 1, 0);
    Vertices[3] = video::S3DVertex(30, 40, 20,  0, 0, 1,
            video::SColor(255,0,0,0), 0, 0);
    Vertices[4] = video::S3DVertex(30, -40, 0,  0, 0, 1,
            video::SColor(255,0,0,0), 0, 1);
    Vertices[5] = video::S3DVertex(-30, -40, 0, 0, 0, 1,
            video::SColor(255,0,0,0), 1, 1);
    Vertices[6] = video::S3DVertex(-30, 40, 0,  0, 0, 1,
            video::SColor(255,0,0,0), 1, 0);
    Vertices[7] = video::S3DVertex(30, 40, 0,   0, 0, 1,
            video::SColor(255,0,0,0), 0, 0);


    Box.reset(Vertices[0].Pos);
    for (s32 i=1; i<8; ++i)
        Box.addInternalPoint(Vertices[i].Pos);
}

void PortalBox::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);

    ISceneNode::OnRegisterSceneNode();
}


void PortalBox::render()
{
    u16 indices[] = {1,0,3, 2,1,3,  3,0,7, 7,0,4,  5,4,1, 1,4,0,  5,1,6, 6,1,2,  6,2,7,  7,2,3};
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setMaterial(Material);
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&Vertices[0], 8, &indices[0], 10, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);

}


const core::aabbox3d<f32>& PortalBox::getBoundingBox() const
{
    return Box;
}

u32 PortalBox::getMaterialCount() const
{
    return 1;
}

video::SMaterial& PortalBox::getMaterial(u32 i)
{
    return Material;
}
