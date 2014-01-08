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

void FenPrincipale::reset_step3() {
}

void FenPrincipale::goto_step3() {
	ui->btnStep1->setFlat(false);
	ui->btnStep2->setFlat(false);
	ui->btnStep3->setFlat(true);
	ui->btnStep4->setFlat(false);
	ui->step1->setVisible(false);
	ui->step2->setVisible(false);
	ui->step3->setVisible(true);
	ui->step4->setVisible(false);
	for (int i = 0; i < this->nbEquipages; ++i) {
		QTableWidgetItem *nom = new QTableWidgetItem(this->equipages.value(i).nom);
		ui->manches->setVerticalHeaderItem(i, nom);
	}
}

void FenPrincipale::on_btnStep3_clicked() {
	this->goto_step3();
}

void FenPrincipale::h_changed(QString text) {
	int row = sender()->property("rowIndex").toInt();
	int col = sender()->property("colIndex").toInt();
	int pos = 0;
	QIntValidator v(0, 59, this);
	if (v.validate(text, pos) == QValidator::Acceptable) {
		this->equipages[row].manches[col].h = text.toInt();
		this->equipages[row].manches[col].abr = "";
		if (text.length() == 2) {
			this->tpsWdgs[row][col].min->setFocus();
		}
	}
	else {
		this->equipages[row].manches[col].h = 0;
		this->equipages[row].manches[col].abr = text;
	}
}
void FenPrincipale::min_changed(QString text) {
	int row = sender()->property("rowIndex").toInt();
	int col = sender()->property("colIndex").toInt();
	int pos = 0;
	QIntValidator v(0, 59, this);
	if (v.validate(text, pos) == QValidator::Acceptable) {
		this->equipages[row].manches[col].min = text.toInt();
		this->equipages[row].manches[col].abr = "";
		if (text.length() == 2) {
			this->tpsWdgs[row][col].s->setFocus();
		}
	}
	else {
		this->equipages[row].manches[col].min = 0;
		this->equipages[row].manches[col].abr = text;
	}
}
void FenPrincipale::s_changed(QString text) {
	int row = sender()->property("rowIndex").toInt();
	int col = sender()->property("colIndex").toInt();
	int pos = 0;
	QIntValidator v(0, 59, this);
	if (v.validate(text, pos) == QValidator::Acceptable) {
		this->equipages[row].manches[col].s = text.toInt();
		this->equipages[row].manches[col].abr = "";
		if (text.length() == 2) {
			if (row < this->nbEquipages-1) {
				this->tpsWdgs[row+1][col].h->setFocus();
			}
			else if (col < this->nbManches-1) {
				this->tpsWdgs[0][col+1].h->setFocus();
			}
		}
	}
	else {
		this->equipages[row].manches[col].s = 0;
		this->equipages[row].manches[col].abr = text;
	}
}

void FenPrincipale::on_addManche_clicked() {
	QTableWidgetItem *nom = new QTableWidgetItem("Manche n°"+QString::number(this->nbManches+1));
	ui->manches->insertColumn(this->nbManches);
	ui->manches->setHorizontalHeaderItem(this->nbManches, nom);
	for (int i = 0; i < this->nbEquipages; ++i) {
		this->add_manche_inputs(i, this->nbManches);
	}
	++this->nbManches;
}

void FenPrincipale::add_manche_inputs(int row, int col) {
	QWidget *wdg = new QWidget;
	QHBoxLayout *layout = new QHBoxLayout(wdg);
	QLineEdit *h = new QLineEdit;
	QLineEdit *min = new QLineEdit;
	QLineEdit *s = new QLineEdit;
	h->setAlignment(Qt::AlignHCenter);
	min->setAlignment(Qt::AlignHCenter);
	s->setAlignment(Qt::AlignHCenter);
	QLabel *points1 = new QLabel(":");
	QLabel *points2 = new QLabel(":");
	layout->addWidget(h);
	layout->addWidget(points1);
	layout->addWidget(min);
	layout->addWidget(points2);
	layout->addWidget(s);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	wdg->setLayout(layout);
	ui->manches->setCellWidget(row, col, wdg);
	h->setProperty("rowIndex", row);
	min->setProperty("rowIndex", row);
	s->setProperty("rowIndex", row);
	h->setProperty("colIndex", col);
	min->setProperty("colIndex", col);
	s->setProperty("colIndex", col);
	this->tpsWdgs[row][col].h = h;
	this->tpsWdgs[row][col].min = min;
	this->tpsWdgs[row][col].s = s;
	connect(h, SIGNAL(textChanged(QString)), this, SLOT(h_changed(QString)));
	connect(min, SIGNAL(textChanged(QString)), this, SLOT(min_changed(QString)));
	connect(s, SIGNAL(textChanged(QString)), this, SLOT(s_changed(QString)));
}