#ifndef LEAPMEDIACONTROLLER_H
#define LEAPMEDIACONTROLLER_H

#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include "ui_leapmediacontroller.h"

#include "LeapListener.h"

namespace Ui 
{
    class LeapMediaController;
}

class LeapMediaController : public QMainWindow
{
	Q_OBJECT

public:
	LeapMediaController(QWidget *parent = 0);
	~LeapMediaController();

private:
	void createActions();
	void createTrayIcon();
	void setIcon();
	void closeEvent(QCloseEvent *); // Overriding the window's close event
    
	Ui::LeapMediaControllerClass ui;

	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	QMenu *fileMenu;
 
	QAction *open;
	QAction *close;
	QAction *closeToTray;
	QAction *toggleListening;
	QAction *toggleInfoDialog;

	LeapListener listener;
	Controller controller;
	
private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
	void closeToTraySlot();
	void toggleListeningSlot();
	void toggleInfoDialogSlot();

};

#endif // LEAPMEDIACONTROLLER_H
