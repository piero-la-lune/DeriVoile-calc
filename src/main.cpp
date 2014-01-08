/*

###################    DériVoile calc' - Français    ###################

Version : v6-5
Date : 2013-06-19
Licence : dans le fichier « COPYING »
Site web : http://calc.derivoile.fr

Copyright © 2011-2012 Pierre Monchalin, http://derivoile.fr
Pensez à me laisser un petit mot si vous distribuez une copie !

DériVoile calc' est un logiciel libre : vous pouvez le redistribuer ou
le modifier selon les termes de la GNU General Public Licence tels que
publiés par la Free Software Foundation : à votre choix, soit la
version 3 de la licence, soit une version ultérieure quelle qu'elle
soit.

DériVoile calc' est distribué dans l'espoir qu'il sera utile, mais SANS
AUCUNE GARANTIE ; sans même la garantie implicite de QUALITÉ MARCHANDE
ou D'ADÉQUATION À UNE UTILISATION PARTICULIÈRE. Pour plus de détails,
reportez-vous à la GNU General Public License.

Vous devez avoir reçu une copie de la GNU General Public License avec
DériVoile calc'. Si ce n'est pas le cas, consultez
<http://www.gnu.org/licenses/>

###################    DériVoile calc' - English    ###################

Version : v6-5
Date : 2013-06-19
Licence : see file “COPYING”
Web site : http://calc.derivoile.fr

Copyright © 2011-2012 Pierre Monchalin, http://derivoile.fr
Remember to leave me a note if you distribute a copy!

DériVoile calc' is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

DériVoile calc' is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with DériVoile calc'. If not, see
<http://www.gnu.org/licenses/>

*/

#include "FenPrincipale.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QSplashScreen>

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	int exit_code = FenPrincipale::EXIT_CODE_REBOOT;
	while (exit_code == FenPrincipale::EXIT_CODE_REBOOT) {

		QSplashScreen splash(QPixmap(":/img/splash.png"));
		splash.showMessage(
			"Chargement des préférences...",
			Qt::AlignHCenter | Qt::AlignBottom
		);
		qApp->processEvents();
		splash.show();
		qApp->processEvents();

		QSettings preferences(
			QDir::currentPath() + "/preferences.ini",
			QSettings::IniFormat
		);

		QTranslator translator;
		translator.load(preferences.value("language", "FR").toString(), ":/translations");
		app.installTranslator(&translator);

		QFile style("C:\\Users\\pierre\\Documents\\Programmation\\DeriVoile-calc\\DeriVoile-calc\\src\\style.css");
		style.open(QIODevice::ReadOnly);
		qApp->setStyleSheet(style.readAll());
		style.close();

		FenPrincipale window(&preferences, &splash);

		exit_code = app.exec();

	}

    return exit_code;
}
