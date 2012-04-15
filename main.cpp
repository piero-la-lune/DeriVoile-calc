/*

#-#-#-#-#-#-#-#-#-#-#-#    DériVoile calc'    #-#-#-#-#-#-#-#-#-#-#-#

VERSION : @v
DATE : @date
LICENCE : ./COPYING
SITE WEB : http://calc.derivoile.fr

Copyright (C) 2011-2012 Pierre Monchalin, http://derivoile.fr
Vous êtes libre de distribuer et modifier ce programme.
Pensez à me laisser un petit mot si vous distribuez une copie !

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <QApplication>
#include <QtGui>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QSplashScreen>
#include <QPixmap>
#include "fenprincipale.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&translator);

	// Ouverture de la fenêtre principale du navigateur
	FenPrincipale principale;
	principale.show();

	return app.exec();
}
