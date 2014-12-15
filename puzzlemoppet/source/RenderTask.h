
#include "Task.h"
#include "IRenderSystem.h"

class IEngine;
class World;
class IPostProcessingChain;
class ShaderManager;

class RenderTask : public Task, public IRenderSystem
{
	IEngine *engine;
	World *world;
	
	video::IVideoDriver *driver;
	scene::ISceneManager *smgr;
	gui::IGUIEnvironment *guienv;
	
	ShaderManager *shaderManager;
	
	// currently set post processing chain
	IPostProcessingChain *ppChain;
	bool postProcessingEnabled;
	
	// screen fading state and params
	bool fadeGUI;
	f32 oldScreenFade;
	f32 newScreenFade;
	f32 fadeStartTime;
	f32 fadeFinishTime;
	bool isFadeFinished;
	
	// Updated on each call to RenderFade
	f32 currentScreenFade;
	
	// background colour
	video::SColor backgroundCol;
	
	bool renderInvisible;
	
	void Render(u16 passCount);
	void RenderFade();
	
public:
	RenderTask(World *world);
	~RenderTask();
	
	// Render system methods
	
	IPostProcessingChain *CreatePostProcessingChain(bool renderScreen) override;
	void SetActivePostProcessingChain(IPostProcessingChain *chain) override;
	
	void SetShaderLevel(E_SHADER_LEVEL level) override;
	
	IShader *CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial) override;
	
	bool ShadersAreAvailable() override;
	bool PostProcessingEnabled() override;
	void ForceNoShaders(bool noShaders) override;
	void ForceNoPostProcessing(bool noPostProcessing) override;
	
	void ScreenFade(f32 fadeAmount, f32 time, bool fadeGUI) override;
	void ScreenFadeFromCurrent(f32 fadeAmount, f32 time, bool fadeGUI) override;
	bool IsFading() override;
	f32 GetCurrentFade() override { return currentScreenFade; }
	
	void SetBackgroundCol(const video::SColor &col) override { backgroundCol = col; }
	
	void RenderInvisible() override;
	
	// Task methods
	void Update(f32 dt) override;
};
