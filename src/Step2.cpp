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

void FenPrincipale::reset_step2() {
	this->stopPropagation = false;
	ui->equipages->setColumnWidth(0, 30);
	ui->equipages->setColumnWidth(1, 200);
	ui->equipages->setColumnWidth(2, 60);
	ui->equipages->setColumnWidth(3, 200);
}

void FenPrincipale::goto_step2() {
	ui->btnStep1->setFlat(false);
	ui->btnStep2->setFlat(true);
	ui->btnStep3->setFlat(false);
	ui->btnStep4->setFlat(false);
	ui->btnStep5->setFlat(false);
	ui->step1->setVisible(false);
	ui->step2->setVisible(true);
	ui->step3->setVisible(false);
	ui->step4->setVisible(false);
	ui->step5->setVisible(false);
}

QList<QString> FenPrincipale::add_bateaux(QJsonArray bateaux) {
	QList<QString> list;
	for (int i = 0; i < bateaux.count(); ++i) {
		QJsonObject bateau = bateaux.at(i).toObject();
		QString code = bateau.value("code").toString();
		Bateau bt;
		bt.serie = bateau.value("serie").toString();
		bt.rating = 0;
		bt.coef = 0;
		bt.rya = 0;
		if (!bateau.value("rating").isNull()) {
			bt.rating = bateau.value("rating").toInt();
			if (this->typeRt == RT_FFV || this->typeRt == RT_DERI) {
				list << bt.serie;
			}
		}
		if (!bateau.value("coef").isNull()) {
			bt.coef = bateau.value("coef").toInt();
			if (this->typeRt == RT_FFV || this->typeRt == RT_DERI) {
				list << bt.serie;
			}
		}
		if (!bateau.value("rya").isNull()) {
			bt.rya = bateau.value("rya").toInt();
			if (this->typeRt == RT_RYA) {
				list << bt.serie;
			}
		}
		this->bateaux.insert(code, bt);
		this->bateauxInv.insert(bt.serie.toLower(), code);
	}
	return list;
}

void FenPrincipale::update_completer() {
	this->bateaux.clear();
	this->bateauxInv.clear();
	QList<QString> list;
	QJsonDocument doc = QJsonDocument::fromJson(this->ratings);
	if (this->typeBt == BT_MUL || this->typeBt == BT_MUL_DER_QUI
		|| this->typeBt == BT_ALL) {
		list << this->add_bateaux(doc.object().value("multicoques").toArray());
	}
	if (this->typeBt == BT_DER || this->typeBt == BT_MUL_DER_QUI
		|| this->typeBt == BT_DER_QUI_HAB || this->typeBt == BT_ALL) {
		list << this->add_bateaux(doc.object().value("deriveurs").toArray());
	}
	if (this->typeBt == BT_QUI || this->typeBt == BT_MUL_DER_QUI
		|| this->typeBt == BT_DER_QUI_HAB || this->typeBt == BT_ALL) {
		list << this->add_bateaux(doc.object().value("quillards").toArray());
	}
	if (this->typeBt == BT_HAB || this->typeBt == BT_DER_QUI_HAB
		|| this->typeBt == BT_ALL) {
		list << this->add_bateaux(doc.object().value("habitables").toArray());
	}
	this->completer = new QCompleter(list, this);
	this->completer->setCaseSensitivity(Qt::CaseInsensitive);
	this->completer->setFilterMode(Qt::MatchContains);
	for (int i = 0; i < this->nbEquipages; ++i) {
		qobject_cast<QLineEdit*>(ui->equipages->cellWidget(i, 3))->setCompleter(this->completer);
	}
}

void FenPrincipale::type_complete(QString type) {
	QString code = this->bateauxInv.value(type.toLower());
	if (!this->stopPropagation || !code.isEmpty()) {
		qobject_cast<QLineEdit*>(ui->equipages
			->cellWidget(sender()->property("rowIndex").toInt(), 2))->setText(code);
	}
}
void FenPrincipale::code_complete(QString code) {
	QString type = this->bateaux.value(type.toUpper()).serie;
	this->stopPropagation = true;
	qobject_cast<QLineEdit*>(ui->equipages
		->cellWidget(sender()->property("rowIndex").toInt(), 3))->setText(type);
	this->stopPropagation = false;
}

void FenPrincipale::nom_changed(QString text) {
	this->equipages[sender()->property("rowIndex").toInt()].nom = text;
}
void FenPrincipale::code_changed(QString text) {
	this->equipages[sender()->property("rowIndex").toInt()].rating = text;
}
void FenPrincipale::type_changed(QString text) {
	this->equipages[sender()->property("rowIndex").toInt()].bateau = text;
}

void FenPrincipale::on_btnStep2_clicked() {
	this->goto_step2();
}

void FenPrincipale::on_addEquipage_clicked() {
		// ajout de la ligne dans "Équipages"
	QLineEdit *nom = new QLineEdit();
	QLineEdit *code = new QLineEdit();
	QLineEdit *type = new QLineEdit();
	QPushButton *del = new QPushButton(QIcon(":/icones/close.png"), "");
	type->setCompleter(this->completer);
	int row = this->nbEquipages;
	ui->equipages->insertRow(row);
	ui->equipages->setCellWidget(row, 0, del);
	ui->equipages->setCellWidget(row, 1, nom);
	ui->equipages->setCellWidget(row, 2, code);
	ui->equipages->setCellWidget(row, 3, type);
	del->setProperty("rowIndex", row);
	nom->setProperty("rowIndex", row);
	code->setProperty("rowIndex", row);
	type->setProperty("rowIndex", row);
	connect(del, SIGNAL(clicked()), this, SLOT(deleteEquipage()));
	connect(code, SIGNAL(textEdited(QString)), this, SLOT(code_complete(QString)));
	connect(type, SIGNAL(textChanged(QString)), this, SLOT(type_complete(QString)));
	connect(nom, SIGNAL(textChanged(QString)), this, SLOT(nom_changed(QString)));
	connect(code, SIGNAL(textChanged(QString)), this, SLOT(code_changed(QString)));
	connect(type, SIGNAL(textChanged(QString)), this, SLOT(type_changed(QString)));
		// ajout de la ligne dans "Manches"
		// cette ligne est la row+1 (car il faut compter le header des manches)
	ui->manches->insertRow(row+1);
	QLabel *nom2 = new QLabel();
	nom2->setProperty("type", "header");
	nom2->setProperty("left", true);
	nom2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	ui->manches->setCellWidget(row+1, 0, nom2);
	for (int i = 0; i < this->nbManches; ++i) {
		this->add_manche_inputs(this->nbEquipages, i);
	}
	++this->nbEquipages;
	QWidget *h;
	QWidget *min;
	QWidget *s;
	for (int i = 0; i < this->nbManches; ++i) {
		for (int j = 0; j < this->nbEquipages; ++j) {
			h = ui->manches->cellWidget(j+1, i+1)->layout()->itemAt(0)->widget();
			min = ui->manches->cellWidget(j+1, i+1)->layout()->itemAt(2)->widget();
			if (i != 0 || j != 0) {
				setTabOrder(s, h);
			}
			s = ui->manches->cellWidget(j+1, i+1)->layout()->itemAt(4)->widget();
			setTabOrder(h, min);
			setTabOrder(min, s);
		}
	}
}

void FenPrincipale::deleteEquipage() {
	int nb = sender()->property("rowIndex").toInt();
	ui->equipages->removeRow(nb);
	ui->manches->removeRow(nb+1);
	for (int i = (nb+1); i < this->nbEquipages; ++i) {
		ui->equipages->cellWidget(i-1, 0)->setProperty("rowIndex", i-1);
		ui->equipages->cellWidget(i-1, 2)->setProperty("rowIndex", i-1);
		ui->equipages->cellWidget(i-1, 3)->setProperty("rowIndex", i-1);
		for (int j = 0; j < this->nbManches; ++j) {
			ui->manches->cellWidget(i, j+1)->setProperty("rowIndex", i-1);
		}
	}
	this->equipages.rm(nb);
	--this->nbEquipages;
}