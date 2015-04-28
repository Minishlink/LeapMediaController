#include "leapmediacontroller.h"
#include <QMenu>
#include <QCloseEvent>
#include <QMenuBar>

LeapMediaController::LeapMediaController(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	createActions();
	createTrayIcon();

	fileMenu = new QMenu(this);
	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(toggleListening);
	fileMenu->addAction(toggleInfoDialog);
	fileMenu->addSeparator();
	fileMenu->addAction(closeToTray);
	fileMenu->addAction(close);

	setIcon();

	trayIcon->show();

	listener._trayIcon = trayIcon;
	controller.addListener(listener);
	
}

LeapMediaController::~LeapMediaController()
{
	delete trayIcon;
	delete trayIconMenu;
	delete fileMenu;
	delete open;
	delete close;
	delete closeToTray;
	delete toggleListening;
	delete toggleInfoDialog;

	controller.removeListener(listener);
}

void LeapMediaController::createActions()
{
	open = new QAction(tr("&Open"), this);
	connect(open, SIGNAL(triggered()), this, SLOT(show()));

	close = new QAction(tr("&Quit"), this);
	connect(close, SIGNAL(triggered()), qApp, SLOT(quit()));

	closeToTray = new QAction(tr("&Close to tray"), this);
	connect(closeToTray, SIGNAL(triggered()), this, SLOT(closeToTraySlot()));

	toggleListening = new QAction(tr("&Listen to Leap"), this);
	toggleListening->setCheckable(true);
	toggleListening->setChecked(true);
	connect(toggleListening, SIGNAL(triggered()), this, SLOT(toggleListeningSlot()));

	toggleInfoDialog = new QAction(tr("&Show info dialogs"), this);
	toggleInfoDialog->setCheckable(true);
	toggleInfoDialog->setChecked(false);
	connect(toggleInfoDialog, SIGNAL(triggered()), this, SLOT(toggleInfoDialogSlot()));
}

void LeapMediaController::createTrayIcon()
{
	trayIconMenu = new QMenu(this);

	trayIconMenu->addAction(open);
	trayIconMenu->addAction(toggleListening);
	trayIconMenu->addAction(toggleInfoDialog);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(close);
 
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
 
	connect(
		trayIcon,
		SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this,
		SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
	);
}

void LeapMediaController::setIcon()
{
	trayIcon->setIcon(QIcon(":/LeapMediaController/icon"));
}

void LeapMediaController::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
		this->show();
}

void LeapMediaController::closeToTraySlot()
{
	// pas besoin de prévenir l'utilisateur que cela va se réduire dans la barre de taches puisque c'est lui qui le demande
	if (trayIcon->isVisible()) 
		hide();
}

void LeapMediaController::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) 
	{
		trayIcon->showMessage(tr("Background mode"),
		tr("Quit by right-clicking this icon, thanks."));
		
		hide();

		event->ignore(); // Don't let the event propagate to the base class
	}
}

void LeapMediaController::toggleListeningSlot()
{
	if(toggleListening->isChecked())
	{
		controller.addListener(listener);
	}
	else
	{
		controller.removeListener(listener);
	}
}

void LeapMediaController::toggleInfoDialogSlot()
{
	listener.showInfoDialog = (toggleInfoDialog->isChecked()) ? true : false;
}

