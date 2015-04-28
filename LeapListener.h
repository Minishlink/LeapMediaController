#ifndef LEAPLISTENER_H
#define LEAPLISTENER_H

#include <Windows.h>
#include <QSystemTrayIcon>
#include "Leap.h"
using namespace Leap;

int pressKey(byte keyCode);

class LeapListener : public Listener 
{
	public:
		virtual void onInit(const Controller&);
		virtual void onConnect(const Controller&);
		virtual void onDisconnect(const Controller&);
		virtual void onExit(const Controller&);
		virtual void onFrame(const Controller&);
		
		QSystemTrayIcon *_trayIcon;		
		void showMsg(const char* title, const char* msg);
		bool showInfoDialog;
};

#endif // LEAPLISTENER_H