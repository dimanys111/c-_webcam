/****************************************************************************
 *  webcam
 *
 *  Copyright (c) 2012 by Nikita Belov <null@deltaz.org>
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "webcam.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	webCam wc;
	wc.show();

	return a.exec();
}
