#ifndef PORTALNODE_H
#define PORTALNODE_H


#include <irrlicht.h>
#include <GL/gl.h>

using namespace irr;

class PortalNode : public scene::ISceneNode
{

	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[8];
	video::SMaterial Material;
	int PortalNumber;

	core::vector3df PortalPosition;
	core::vector3df PortalRotation;

public:
	bool Render = 1;

public:

	PortalNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int portalNumber);

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

};

#endif
