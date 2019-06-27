
#include "IWorld.h"
#include <deque>

class IEngine;
class Physics;
class PhysicsTask;
class LogicTask;
class RenderTask;
class IGraphic;
class ISensor;

class World : public IWorld
{
	IEngine *engine;
	Physics *physics;
	
	Set<IWantInput *> inputSubscribers;
	
	IInputProfile *inputProfile;
	ICameraController *cameraController;
	ICameraController *lastCameraController;
	ICamera *camera;
	
	// Transformables
	Set<ITransformable *> transformables;
	// All those in these sub lists are also present in the main transformables list.
	Set<IGraphic *> graphics;
	Set<ICharacter *> characters;
	Set<ISensor *> sensors;
	Set<ISoundSource *> soundSources;
	
	// Waiting for removal
	std::deque<ITransformable *> removalQueue;
	
	// Some world effects
	scene::ISceneNode *skyBoxNode;
	IShader *skyBoxShader;
	
protected:
	void OnPause() override;
	void OnResume() override;
	
public:
	World();
	~World();
	
	// Used by render task.
	const Set<IGraphic *> &GetAllGraphics() { return graphics; }
	
	IPhysics *GetPhysics() override;
	ICamera *GetCamera() override;
	
	void SetInputProfile(IInputProfile *profile) override;
	IInputProfile *GetInputProfile() override;
	
	void SubscribeToInput(IWantInput *wantInput) override;
	void UnsubscribeFromInput(IWantInput *wantInput) override;
	
	void SetCameraController(ICameraController *cameraController) override;
	ICameraController *GetCameraController() override;
	
	void AddTransformable(ITransformable *transformable) override;
	void RemoveTransformable(ITransformable *transformable) override;
	void QueueForRemoval(ITransformable *transformable) override;
	void RemoveAllTransformables() override;
	
	IMesh *AddMesh(const c8 *meshName) override;
	
	INodeHandler *AddIrrNodeHandler(scene::ISceneNode *irrNode, bool removeOnDestruct) override;
	
	ITransformable *AddTransform() override;
	
	ICharacter *AddCharacter(IMesh *mesh, s32 idAnimIdle, s32 idAnimWalk, f32 height) override;
	
	IProxyTransformable *AddProxyTransformable(ITransformable *other) override;
	
	ISoundSource *AddSoundSource() override;
	
	IMotionSensor *AddMotionSensor() override;
	
	IMotionSensor *AddSoundMotionSensor(const c8 *startSound, const c8 *stopSound, const c8 *motionLoopSound) override;
	
	IInputProfile *CreateInputProfile(int buttonCount, int axesCount) override;
	
	ICharacterController *CreateUserCharacterController(
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo) override;
	
	IThirdPersonCameraController *CreateThirdPersonCameraController(f32 distance, f32 heightOffset,
			f32 minPitch, f32 maxPitch, f32 turnSpeed) override;
	
	IMotionAnimator *CreateRotationAnimator(core::vector3df speed) override;
	IBobAnimator *CreateBobAnimator(f32 amplitude, f32 speed) override;
	
	void SetSkyBox(const core::stringc &top, const core::stringc &bottom, const core::stringc &left,
			const core::stringc &right, const core::stringc &front, const core::stringc &back) override;
	
	void SetSkyBoxShader(IShader *shader) override;
	video::SMaterial &GetSkyBoxMaterial(u32 i) override;
	
	void ClearSkyBox() override;
	
	void Update(f32 dt) override;
};

