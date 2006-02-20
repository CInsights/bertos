/**
 * \file
 * <!--
 * Copyright 2006 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2000, 2001 Bernardo Innocenti <bernie@codewiz.org>
 * This file is part of DevLib - See README.devlib for information.
 * -->
 *
 * \version $Id$
 *
 * \author Bernardo Innocenti <bernie@develer.com>
 *
 * \brief Qt-based emulator framework for embedded applications (implementation)
 */

/*#*
 *#* $Log$
 *#* Revision 1.3  2006/02/20 02:00:40  bernie
 *#* Port to Qt 4.1.
 *#*
 *#* Revision 1.2  2006/01/16 03:51:51  bernie
 *#* Fix boilerplate.
 *#*
 *#* Revision 1.1  2006/01/16 03:37:12  bernie
 *#* Add emulator skeleton.
 *#*
 *#*/

#include "emul.h"
#include "emulwin.h"
#include <cfg/module.h>

#include <cstdlib> // std::exit()

#if _QT < 4
	#include <qapplication.h>
#else
	#include <QtGui/qapplication.h>
#endif


/// The global emulator instance.
Emulator *emul;

Emulator::Emulator(int &argc, char **argv) :
	emulApp(new QApplication(argc, argv)),
	emulWin(new EmulWin(this))
{
	#if QT_VERSION < ((4 << 16) + (0 << 8) + 0)
		emulApp->setMainWidget(emulWin);
	#endif
	emulWin->show();
}


Emulator::~Emulator()
{
	// we don't delete emulWin because it automatically
	// deletes itself when closed
	delete emulApp;
}


NORETURN void Emulator::quit()
{
	// WHAT A KLUDGE!
	this->~Emulator();
	emul = NULL;

	// do we have a better way to shut down the emulation?
	exit(0);
}

MOD_DEFINE(emul)

/// Main emulator entry point.
extern "C" void emul_init(int *argc, char *argv[])
{
	// setup global emulator pointer
	emul = new Emulator(*argc, argv);

	MOD_INIT(emul);
}

extern "C" void emul_cleanup()
{
	MOD_CLEANUP(emul);

	delete emul;
	emul = NULL;
}

extern "C" void emul_idle()
{
	// We process GUI events when the application is idle.
	emul->emulApp->processEvents();
}

