
#include "IMesh.h"

#include "IShader.h"
#include <map>

// Utility struct
struct Pass
{
	// Create using the materials from an Irrlicht scene node.
	Pass(scene::ISceneNode *node)
	{
		for (u32 i = 0; i < node->getMaterialCount(); i ++)
			materials.push_back(node->getMaterial(i));
		
		shaders.assign(node->getMaterialCount(), nullptr);
	}
	
	// Create using the materials from a scene::IMesh
	Pass(scene::IMesh *mesh)
	{
		for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
		{
			if (scene::IMeshBuffer *mb = mesh->getMeshBuffer(i))
				materials.push_back(mb->getMaterial());
			else
			{
				materials.push_back(video::SMaterial()); // Default material. This should probably never occur.
			}
		}
		
		shaders.assign(mesh->getMeshBufferCount(), nullptr);
	}
	
	~Pass()
	{
		for (auto & elem : shaders)
		{
			if (elem)
				elem->drop();
		}
	}
	
	std::vector<video::SMaterial> materials;
	std::vector<IShader *> shaders;
};

struct Animation
{
	s32 start,end;
	f32 fps;
	bool looped;
	bool interruptible;
	f32 transitionTime;
};

class Mesh : public IMesh
{
	scene::ISceneManager *smgr;
	
	scene::IAnimatedMeshSceneNode *meshNode;
	
	std::vector<Pass> passes;
	
	std::map<s32, Animation> animations;
	Animation *currentAnimation;
	
	// a permanent rotation to apply transparently to mesh scene node
	core::vector3df rotation;
	// a permanent translation
	core::vector3df translation;
	
	// fixes an Irrlicht bug
	//bool onFirstFrame;
	
public:
	Mesh(scene::ISceneManager *smgr, const c8 *name);
	~Mesh();
	
	core::aabbox3df GetBoundingBox() override;
	
	u32 GetMaterialCount() override;
	
	video::SMaterial &GetMaterial(u32 material, u16 pass) override;
	
	void SetShader(u32 material, IShader *shader, u16 pass) override;
	
	void SetAllShaders(IShader *shader, u16 pass) override;
	
	void DisableMaterial(u32 material, u16 pass) override;
	
	void DisableAllMaterials(u16 pass) override;
	
	void SetVisible(bool visible) override;
	
	void ReceiveRenderPosition(core::vector3df pos) override;
	void ReceiveRenderRotation(core::vector3df rot) override;
	
	u16 GetPassCount() override;
	
	void SetCurrentPass(u16 pass) override;
	
	void AddPass(const c8 *meshName) override;
	
	void Render(u16 pass) override;
	
	// an animation is: {start,end,FPS,looped?,interruptible?}
	void DefineAnimation(s32 id, s32 start, s32 end, f32 fps, bool looped, bool interruptible, f32 transitionTime) override;
	
	void SetAnimation(s32 id) override;
	
	bool IsPlayingAnimation(s32 id) override;
	
	void HaltAnimation();
	
	scene::IAnimatedMeshSceneNode *GetIrrlichtNode() override;
	
	// Mesh can be scaled. Should be taken into account by physics mesh geometry
	void Scale(const core::vector3df &scale) override;
	
	void Rotate(const core::vector3df &rotation) override;
	
	void Translate(const core::vector3df &translation) override;
};

