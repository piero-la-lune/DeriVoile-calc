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

#include "FenAide.h"
#include "ui_FenAide.h"

FenAide::FenAide(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenAide) {

	ui->setupUi(this);

	tree = new QTreeWidget;
	tree->setColumnCount(1);
	tree->setHeaderHidden(true);
	tree->setFixedWidth(200);

	aide = new QTreeWidgetItem;
	aide->setText(0, tr("Aide"));

	debut = new QTreeWidgetItem;
	debut->setText(0, tr("Première utilisation"));
	aide->addChild(debut);

	etapes = new QTreeWidgetItem;
	etapes->setText(0, tr("Étapes"));
	etape1 = new QTreeWidgetItem;
	etape1->setText(0, tr("Étape 1"));
	etapes->addChild(etape1);
	etape2 = new QTreeWidgetItem;
	etape2->setText(0, tr("Étape 2"));
	etapes->addChild(etape2);
	etape3 = new QTreeWidgetItem;
	etape3->setText(0, tr("Étape 3"));
	etapes->addChild(etape3);
	aide->addChild(etapes);

	ratings = new QTreeWidgetItem;
	ratings->setText(0, tr("Ratings"));
	aide->addChild(ratings);

	astuces = new QTreeWidgetItem;
	astuces->setText(0, tr("Astuces"));
	aide->addChild(astuces);

	maj = new QTreeWidgetItem;
	maj->setText(0, tr("Mises à jour"));
	aide->addChild(maj);

	regles = new QTreeWidgetItem;
	regles->setText(0, tr("Extraits des Règles de Course"));

	tree->addTopLevelItem(aide);
	tree->addTopLevelItem(regles);
	tree->expandAll();
	ui->toolBar->addWidget(tree);

	webView = new QWebView;

	webView->load(QUrl("qrc:/doc/aide.html"));
	webView->setContextMenuPolicy(Qt::NoContextMenu);

	setCentralWidget(webView);

	connect(
		tree,
		SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
		this,
		SLOT(change_page(QTreeWidgetItem*, QTreeWidgetItem*))
	);

	connect(
		webView,
		SIGNAL(urlChanged(QUrl)),
		this,
		SLOT(link_clicked(QUrl))
	);

}

FenAide::~FenAide() {
	delete ui;
}

void FenAide::change_page(QTreeWidgetItem *item, QTreeWidgetItem *) {
	if (item == this->aide) {
		this->webView->load(QUrl("qrc:/doc/aide.html"));
	}
	if (item == this->debut) {
		this->webView->load(QUrl("qrc:/doc/debut.html"));
	}
	if (item == this->etapes) {
		this->webView->load(QUrl("qrc:/doc/etapes.html"));
	}
	if (item == this->etape1) {
		this->webView->load(QUrl("qrc:/doc/etape1.html"));
	}
	if (item == this->etape2) {
		this->webView->load(QUrl("qrc:/doc/etape2.html"));
	}
	if (item == this->etape3) {
		this->webView->load(QUrl("qrc:/doc/etape3.html"));
	}
	if (item == this->ratings) {
		this->webView->load(QUrl("qrc:/doc/ratings.html"));
	}
	if (item == this->astuces) {
		this->webView->load(QUrl("qrc:/doc/astuces.html"));
	}
	if (item == this->maj) {
		this->webView->load(QUrl("qrc:/doc/maj.html"));
	}
	if (item == this->regles) {
		this->webView->load(QUrl("qrc:/doc/regles.html"));
	}
}

void FenAide::link_clicked(QUrl url) {
	QString urlStr = url.toDisplayString();
	this->aide->setSelected(urlStr == "qrc:/doc/aide.html");
	this->debut->setSelected(urlStr == "qrc:/doc/debut.html");
	this->etapes->setSelected(urlStr == "qrc:/doc/etapes.html");
	this->etape1->setSelected(urlStr == "qrc:/doc/etape1.html");
	this->etape2->setSelected(urlStr == "qrc:/doc/etape2.html");
	this->etape3->setSelected(urlStr == "qrc:/doc/etape3.html");
	this->ratings->setSelected(urlStr == "qrc:/doc/ratings.html");
	this->astuces->setSelected(urlStr == "qrc:/doc/astuces.html");
	this->maj->setSelected(urlStr == "qrc:/doc/maj.html");
	this->regles->setSelected(urlStr == "qrc:/doc/regles.html");
}