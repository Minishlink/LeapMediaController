#include "LeapListener.h"
#define DEG2RAD 0.01745329251f

double lastProgress;

void LeapListener::onInit(const Controller& controller) 
{
	std::cout << "Initialized" << std::endl;

	/*Config cfg = controller.config;
	cfg.setFloat(Gesture.Swipe.MinLength, */
}

void LeapListener::onConnect(const Controller& controller) 
{
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_SWIPE);
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.setPolicyFlags(Controller::POLICY_BACKGROUND_FRAMES);

	lastProgress = 0;
	showInfoDialog = false;
}

void LeapListener::onDisconnect(const Controller& controller) 
{
	std::cout << "Disconnected" << std::endl;
}

void LeapListener::onExit(const Controller& controller) 
{
	std::cout << "Exited" << std::endl;
}

void LeapListener::onFrame(const Controller& controller) 
{
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();

	// Get gestures
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) 
	{
		Gesture gesture = gestures[g];
		if(gesture.isValid())
		{
			if(gesture.type() == Gesture::TYPE_SWIPE && gesture.state() == gesture.STATE_STOP) // swipe
			{
				SwipeGesture swipe = gesture;
				if(swipe.position().distanceTo(swipe.startPosition()) > 200 || 1)
				{
					if(swipe.direction().angleTo(swipe.direction().right()) < 20*DEG2RAD)
					{
						pressKey(VK_MEDIA_NEXT_TRACK);
						showMsg("LeapMediaController", "Next track");
					}
					else if(swipe.direction().angleTo(swipe.direction().left()) < 20*DEG2RAD)
					{
						pressKey(VK_MEDIA_PREV_TRACK);
						showMsg("LeapMediaController", "Previous track");
					}
				}
				
				if(swipe.position().distanceTo(swipe.startPosition()) > 50 || 1)
				{
					if(swipe.direction().angleTo(swipe.direction().up()) < 30*DEG2RAD || swipe.direction().angleTo(swipe.direction().down()) < 30*DEG2RAD)
					{
						pressKey(VK_MEDIA_PLAY_PAUSE);
						showMsg("LeapMediaController", "Play/Pause");
					}
				}
			}
			else if(gesture.type() == Gesture::TYPE_CIRCLE) //circle
			{
				CircleGesture circle = gesture;
				bool addVolume = (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) ? true : false;

				if(floor(circle.progress()) > lastProgress)
				{
					if(addVolume)
					{
						pressKey(VK_VOLUME_UP);
						showMsg("LeapMediaController", "Volume up");
					}
					else
					{
						pressKey(VK_VOLUME_DOWN);
						showMsg("LeapMediaController", "Volume down");
					}
				}

				lastProgress = floor(circle.progress());
			}
		}
	}
}

/*
VK_VOLUME_MUTE        -> fist
VK_VOLUME_UP          -> swype up
VK_VOLUME_DOWN        -> swype down
VK_MEDIA_NEXT_TRACK   -> swype right
VK_MEDIA_PREV_TRACK   -> swype left
VK_MEDIA_STOP         -> ?
VK_MEDIA_PLAY_PAUSE   -> swipe down
*/

int pressKey(byte keyCode) 
{
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = keyCode;

	// press key
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	// release key
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	return 0;
}

void LeapListener::showMsg(const char* title, const char* msg)
{
	if(showInfoDialog)
		_trayIcon->showMessage(title, msg);
}
