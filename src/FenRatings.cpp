/*

###################    DériVoile calc' - Français    ###################

Version : v7-1
Date : 2014-03-22
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

Version : v7-1
Date : 2014-03-22
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

#include "FenRatings.h"
#include "ui_FenRatings.h"

FenRatings::FenRatings(QWidget *parent, QString fileName, FenPrincipale *fenPrincipale) :
	QMainWindow(parent),
	ui(new Ui::FenRatings)
{

	this->oldFileName = fileName;
	this->fenPrincipale = fenPrincipale;

	ui->setupUi(this);

	if (!this->load_ratings(fileName)) {
		this->fenPrincipale->msg(
			tr("Erreur"),
			tr("Impossible de lire les ratings."),
			"x"
		);
		ui->cancel->click();
	}
	else {
		this->on_show_currentIndexChanged(0);
	}
	
}

FenRatings::~FenRatings() {
	delete ui;
}

bool FenRatings::load_ratings(QString fileName) {
	QFile file(fileName);
	if (!file.exists()) { return false; }
	file.open(QIODevice::ReadOnly);
	QByteArray ratings = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(ratings);
	if (doc.isNull()) { return false; }

	if (!doc.object().value("version").isString()) { return false; }

	QJsonArray multicoques = doc.object().value("multicoques").toArray();
	ui->tableMulticoques->setSortingEnabled(false);
	ui->tableMulticoques->setRowCount(multicoques.count());
	for (int i = 0; i < multicoques.count(); ++i) {
		QJsonObject bateau = multicoques.at(i).toObject();
		QTableWidgetItem *code = new QTableWidgetItem();
		if (bateau.value("code").isString()) {
			code->setText(bateau.value("code").toString());
		}
		QTableWidgetItem *serie = new QTableWidgetItem();
		if (bateau.value("serie").isString()) {
			serie->setText(bateau.value("serie").toString());
		}
		QTableWidgetItem *cat = new QTableWidgetItem();
		if (bateau.value("cat").isString()) {
			cat->setText(bateau.value("cat").toString());
		}
		QTableWidgetItem *rating = new QTableWidgetItem();
		if (bateau.value("rating").isDouble()) {
			rating->setText(QString::number(bateau.value("rating").toDouble()));
		}
		QTableWidgetItem *coef = new QTableWidgetItem();
		if (bateau.value("coef").isDouble()) {
			coef->setText(QString::number(bateau.value("coef").toDouble()));
		}
		QTableWidgetItem *rya = new QTableWidgetItem();
		if (bateau.value("rya").isDouble()) {
			rya->setText(QString::number(bateau.value("rya").toDouble()));
		}
		ui->tableMulticoques->setItem(i, 0, code);
		ui->tableMulticoques->setItem(i, 1, serie);
		ui->tableMulticoques->setItem(i, 2, cat);
		ui->tableMulticoques->setItem(i, 3, rating);
		ui->tableMulticoques->setItem(i, 4, coef);
		ui->tableMulticoques->setItem(i, 5, rya);
	}
	ui->tableMulticoques->setSortingEnabled(true);
	ui->tableMulticoques->horizontalHeader()
		->setSectionResizeMode(1, QHeaderView::Stretch);

	QJsonArray deriveurs = doc.object().value("deriveurs").toArray();
	ui->tableDeriveurs->setSortingEnabled(false);
	ui->tableDeriveurs->setRowCount(deriveurs.count());
	for (int i = 0; i < deriveurs.count(); ++i) {
		QJsonObject bateau = deriveurs.at(i).toObject();
		QTableWidgetItem *code = new QTableWidgetItem();
		if (bateau.value("code").isString()) {
			code->setText(bateau.value("code").toString());
		}
		QTableWidgetItem *serie = new QTableWidgetItem();
		if (bateau.value("serie").isString()) {
			serie->setText(bateau.value("serie").toString());
		}
		QTableWidgetItem *cat = new QTableWidgetItem();
		if (bateau.value("cat").isString()) {
			cat->setText(bateau.value("cat").toString());
		}
		QTableWidgetItem *rating = new QTableWidgetItem();
		if (bateau.value("rating").isDouble()) {
			rating->setText(QString::number(bateau.value("rating").toDouble()));
		}
		QTableWidgetItem *coef = new QTableWidgetItem();
		if (bateau.value("coef").isDouble()) {
			coef->setText(QString::number(bateau.value("coef").toDouble()));
		}
		QTableWidgetItem *rya = new QTableWidgetItem();
		if (bateau.value("rya").isDouble()) {
			rya->setText(QString::number(bateau.value("rya").toDouble()));
		}
		ui->tableDeriveurs->setItem(i, 0, code);
		ui->tableDeriveurs->setItem(i, 1, serie);
		ui->tableDeriveurs->setItem(i, 2, cat);
		ui->tableDeriveurs->setItem(i, 3, rating);
		ui->tableDeriveurs->setItem(i, 4, coef);
		ui->tableDeriveurs->setItem(i, 5, rya);
	}
	ui->tableDeriveurs->setSortingEnabled(true);
	ui->tableDeriveurs->horizontalHeader()
		->setSectionResizeMode(1, QHeaderView::Stretch);

	QJsonArray quillards = doc.object().value("quillards").toArray();
	ui->tableQuillards->setSortingEnabled(false);
	ui->tableQuillards->setRowCount(quillards.count());
	for (int i = 0; i < quillards.count(); ++i) {
		QJsonObject bateau = quillards.at(i).toObject();
		QTableWidgetItem *code = new QTableWidgetItem();
		if (bateau.value("code").isString()) {
			code->setText(bateau.value("code").toString());
		}
		QTableWidgetItem *serie = new QTableWidgetItem();
		if (bateau.value("serie").isString()) {
			serie->setText(bateau.value("serie").toString());
		}
		QTableWidgetItem *rating = new QTableWidgetItem();
		if (bateau.value("rating").isDouble()) {
			rating->setText(QString::number(bateau.value("rating").toDouble()));
		}
		QTableWidgetItem *coef = new QTableWidgetItem();
		if (bateau.value("coef").isDouble()) {
			coef->setText(QString::number(bateau.value("coef").toDouble()));
		}
		QTableWidgetItem *rya = new QTableWidgetItem();
		if (bateau.value("rya").isDouble()) {
			rya->setText(QString::number(bateau.value("rya").toDouble()));
		}
		ui->tableQuillards->setItem(i, 0, code);
		ui->tableQuillards->setItem(i, 1, serie);
		ui->tableQuillards->setItem(i, 2, rating);
		ui->tableQuillards->setItem(i, 3, coef);
		ui->tableQuillards->setItem(i, 4, rya);
	}
	ui->tableQuillards->setSortingEnabled(true);
	ui->tableQuillards->horizontalHeader()
		->setSectionResizeMode(1, QHeaderView::Stretch);

	QJsonArray habitables = doc.object().value("habitables").toArray();
	ui->tableHabitables->setSortingEnabled(false);
	ui->tableHabitables->setRowCount(habitables.count());
	for (int i = 0; i < habitables.count(); ++i) {
		QJsonObject bateau = habitables.at(i).toObject();
		QTableWidgetItem *code = new QTableWidgetItem();
		if (bateau.value("id").isDouble()) {
			code->setText("#"+QString::number(bateau.value("id").toDouble()));
		}
		QTableWidgetItem *serie = new QTableWidgetItem();
		if (bateau.value("serie").isString()) {
			serie->setText(bateau.value("serie").toString());
		}
		QTableWidgetItem *cat = new QTableWidgetItem();
		if (bateau.value("cat").isString()) {
			cat->setText(bateau.value("cat").toString());
		}
		QTableWidgetItem *groupe = new QTableWidgetItem();
		if (bateau.value("groupe").isDouble()) {
			groupe->setText(QString::number(bateau.value("groupe").toDouble()));
		}
		QTableWidgetItem *cvl = new QTableWidgetItem();
		if (bateau.value("cvl").isDouble()) {
			cvl->setText(QString::number(bateau.value("cvl").toDouble()));
		}
		ui->tableHabitables->setItem(i, 0, code);
		ui->tableHabitables->setItem(i, 1, serie);
		ui->tableHabitables->setItem(i, 2, cat);
		ui->tableHabitables->setItem(i, 3, groupe);
		ui->tableHabitables->setItem(i, 4, cvl);
	}
	ui->tableHabitables->setSortingEnabled(true);
	ui->tableHabitables->horizontalHeader()
		->setSectionResizeMode(1, QHeaderView::Stretch);

	this->fileName = fileName;
	this->version = doc.object().value("version").toString();

	if (this->fileName == ":/ratings.json") {
		ui->fichier_i->setText(tr("par défaut"));
	}
	else {
		ui->fichier_i->setText(this->fileName);
	}
	ui->version_i->setText(this->version);
	ui->show->setCurrentIndex(0);

	return true;
}

void FenRatings::on_show_currentIndexChanged(int index) {
	ui->tableMulticoques->hide();
	ui->tableDeriveurs->hide();
	ui->tableQuillards->hide();
	ui->tableHabitables->hide();
	if (index == 0) {
		ui->tableMulticoques->show();
	}
	else if (index == 1) {
		ui->tableDeriveurs->show();
	}
	else if (index == 2) {
		ui->tableQuillards->show();
	}
	else {
		ui->tableHabitables->show();
	}
}

void FenRatings::on_fichier_choisir_clicked() {
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Choisir un fichier de ratings"),
		this->fileName,
		tr("Fichier JSON (*.json);;Tous les fichiers (*.*)")
	);
	if (!fileName.isEmpty()) {
		if (!this->load_ratings(fileName)) {
			this->fenPrincipale->msg(
				tr("Erreur"),
				tr("Impossible de lire les ratings.\nVérifiez que le fichier est accessible en lecture et correspond bien à un fichier de ratings."),
				"x"
			);
		}
	}
}

void FenRatings::on_fichier_defaut_clicked() {
	this->load_ratings(":/ratings.json");
}

void FenRatings::on_cancel_clicked() {
	this->close();
}

void FenRatings::on_save_clicked() {
	if (this->oldFileName != this->fileName) {
		if (!fenPrincipale->load_ratings(this->fileName)) {
			this->fenPrincipale->msg(
				tr("Erreur"),
				tr("Impossible de lire les ratings.\nVérifiez que le fichier est accessible en lecture et correspond bien à un fichier de ratings."),
				"x"
			);
		}
		else {
			/*this->fenPrincipale->update_ratings_js();*/
		}
	}
	this->close();
}
