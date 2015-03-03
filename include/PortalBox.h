#ifndef PORTALBOX_H
#define PORTALBOX_H


#include <irrlicht.h>
#include <GL/gl.h>

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


class PortalBox : public scene::ISceneNode
{

	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[8];
	video::SMaterial Material;

public:

	PortalBox(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

};

#endif
