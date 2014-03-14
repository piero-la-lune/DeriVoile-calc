/*

###################    DériVoile calc' - Français    ###################

Version : v7-0
Date : 2014-03-14
Licence : dans le fichier « COPYING »
Site web : http://calc.derivoile.fr

Copyright © 2011-2014 Pierre Monchalin, http://derivoile.fr
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

Version : v7-0
Date : 2014-03-14
Licence : see file “COPYING”
Web site : http://calc.derivoile.fr

Copyright © 2011-2014 Pierre Monchalin, http://derivoile.fr
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

#include "FenAide.h"
#include "ui_FenAide.h"
/*#include <QWebInspector>*/

FenAide::FenAide(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenAide) {

	ui->setupUi(this);

	webView = new QWebView;

	webView->load(QUrl("qrc:/doc/index.html"));
	webView->setContextMenuPolicy(Qt::NoContextMenu);
	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

/*	webView->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	QWebInspector *inspector = new QWebInspector;
	inspector->setPage(webView->page());
	inspector->setVisible(true);*/

	setCentralWidget(webView);

	connect(
		webView,
		SIGNAL(linkClicked(QUrl)),
		this,
		SLOT(link_clicked(QUrl))
	);

}

FenAide::~FenAide() {
	delete ui;
}

void FenAide::link_clicked(QUrl url) {
	QDesktopServices::openUrl(url);
}