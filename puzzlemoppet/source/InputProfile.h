
#include "IInputProfile.h"
#include <map>
#include "Buttons.h"
#include "IWantEvents.h"

class IEngine;

// The lingo:
// - a Button is a keyboard key, a mouse button, a joystick button...
// - an id, is a virtual button which is bound to a particular physical button.
class InputProfile : public IInputProfile, public IWantEvents
{
	IEngine *engine;
	
	// New events generated by input profile.
	// Flushed in ProcessAndNotify
	std::vector<Event> profileEventCache;
	
	ButtonStates lastRealButtonStates;
	
	// physical button mapped to an id
	std::map<s32, s32> buttonBindings;
	std::map<s32, s32> axisBindings;
	std::map<s32, s32> axisAsButtonBindings;
	std::map<s32, f32> axisAsButtonDeltas; // axis deltas for real buttons
	// whether a axis mapping should be inverted. may not be present if false. (default)
	std::map<s32, bool> axisInverted;
	
	// id mapped to physical button
	// may be multiple buttons mapped to an id...
	std::map<s32, Set<s32> > reverseButtonBindings;
	std::map<s32, Set<s32> > reverseAxisBindings;
	std::map<s32, Set<s32> > reverseAxisAsButtonBindings;
	
	// Is the virtual button or axis enabled?
	// May not contain an entry if the enabled state has never been modified.
	// (in which case it should be considered enabled by default)
	std::map<s32, bool> buttonEnabled;
	std::map<s32, bool> axisEnabled;
	
	// Returns -1 if no binding exists
	// Gets the id that a particular button is bound to.
	s32 GetButtonBinding(s32 button);
	s32 GetAxisBinding(s32 axis);
	
	// Returns the actual physical button bound to an id.
	// Returns a vector as an id may have several buttons bound to it.
	// (the opposite of GetButtonBinding)
	// returns an empty vector if no binding exists.
	Set<s32> GetReverseButtonBinding(s32 id);
	Set<s32> GetReverseAxisBinding(s32 id);
	
	// Button ids that are forced to a particular state.
	std::map<s32, bool> forcedVirtualButtons;
	
	// Return -1 or 1 for this real (not virtual) axis inversion.
	f32 GetAxisInversionAsFloat(s32 axis);
	
	void NotifyAxisChanged(s32 id, f32 delta, const Set<IWantInput *> &subscribers);
	void NotifyButtonDown(s32 id, const Set<IWantInput *> &subscribers);
	void NotifyButtonUp(s32 id, const Set<IWantInput *> &subscribers);
	
	bool IsButtonEnabled(s32 id);
	bool IsAxisEnabled(s32 id);
	
protected:
	void OnPause() override;
	void OnResume() override;
	
public:
	InputProfile(s32 buttonCount, s32 axesCount, IEngine *engine);
	~InputProfile();
	
	void BindButton(s32 id, s32 button) override;
	
	void BindAxis(s32 id, s32 axis, bool inverted) override;
	
	void BindButtonAsAxis(s32 id, s32 button, f32 delta) override;
	
	// This returns state from engine.GetButtonState
	bool IsButtonDown(s32 id) override;
	
	void ForceButtonState(s32 id, bool state) override;
	
	void SetButtonEnabled(s32 id, bool enabled) override;
	
	void SetAxisEnabled(s32 id, bool enabled) override;
	
	void OnEvent(const Event &event) override;
	
	void ProcessAndNotify(const Set<IWantInput *> &subscribers) override;
};

