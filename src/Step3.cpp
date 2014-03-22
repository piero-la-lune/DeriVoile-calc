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

void FenPrincipale::reset_step3() {
	QLabel *vide = new QLabel();
	vide->setProperty("type", "coin");
	ui->manches->setCellWidget(0, 0, vide);
	while (ui->manches->columnCount() > 1) {
		ui->manches->removeColumn(1);
	}
	int nb = this->nbManches;
	this->nbManches = 0; // car on va l'augmenter juste après
	for (int i = 0; i < nb; ++i) {
		this->progression(40+50*i/nb);
		this->on_addManche_clicked();
		for (int j = 0; j < this->nbEquipages; ++j) {
			Manche m = this->equipages[j].manches[i];
			if (m.abr.isEmpty()) {
				if (this->typeClmt == CLMT_SCRATCH && m.pl > 0) {
					qobject_cast<MyLineEdit*>(ui->manches->cellWidget(j+2, i+1)
						->layout()->itemAt(2)->widget())
						->setText(QString::number(m.pl));
				}
				else if (this->typeClmt == CLMT_TEMPS) {
					if (m.h > 0) {
						qobject_cast<MyLineEdit*>(ui->manches->cellWidget(j+2, i+1)
							->layout()->itemAt(0)->widget())
							->setText(QString::number(m.h));
					}
					if (m.min > 0) {
						qobject_cast<MyLineEdit*>(ui->manches->cellWidget(j+2, i+1)
							->layout()->itemAt(2)->widget())
							->setText(QString::number(m.min));
					}
					if (m.s > 0) {
						qobject_cast<MyLineEdit*>(ui->manches->cellWidget(j+2, i+1)
							->layout()->itemAt(4)->widget())
							->setText(QString::number(m.s));
					}
				}
			}
			else {
				qobject_cast<MyLineEdit*>(ui->manches->cellWidget(j+2, i+1)
					->layout()->itemAt(2)->widget())
					->setText(this->equipages[j].manches[i].abr);
			}
		}
		if (this->typeClmt == CLMT_TEMPS) {
			Manche m = this->manches[i];
			if (m.h > 0) {
				qobject_cast<MyLineEdit*>(ui->manches->cellWidget(1, i+1)
					->layout()->itemAt(0)->widget())
					->setText(QString::number(m.h));
			}
			if (m.min > 0) {
				qobject_cast<MyLineEdit*>(ui->manches->cellWidget(1, i+1)
					->layout()->itemAt(2)->widget())
					->setText(QString::number(m.min));
			}
			if (m.s > 0) {
				qobject_cast<MyLineEdit*>(ui->manches->cellWidget(1, i+1)
					->layout()->itemAt(4)->widget())
					->setText(QString::number(m.s));
			}
		}
	}
	QLabel *depart = new QLabel(tr("Heure de départ"));
	depart->setProperty("type", "header");
	depart->setProperty("left", true);
	depart->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	ui->manches->setCellWidget(1, 0, depart);
}

void FenPrincipale::on_btnStep3_clicked() {
	this->goto_step3();
}
void FenPrincipale::goto_step3() {
	this->leave_step1();
	this->leave_step2();
	this->leave_step4();
	this->leave_step5();
	ui->btnStep3->setFlat(true);
	ui->step3->setVisible(true);
	for (int i = 0; i < this->nbEquipages; ++i) {
		qobject_cast<QLabel*>(ui->manches->cellWidget(i+2, 0))
			->setText(this->equipages[i].nom);
	}
	ui->manches->resizeColumnsToContents();
}
void FenPrincipale::leave_step3() {
	ui->btnStep3->setFlat(false);
	ui->step3->setVisible(false);
}

void FenPrincipale::init_step3() {
	this->rmMancheTimer = new QTimer();
	connect(this->rmMancheTimer, SIGNAL(timeout()), this, SLOT(disable_rmManche()));
}

void FenPrincipale::h_changed(QString text) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	int pos = 0;
	QIntValidator v(0, 59, this);
	if (v.validate(text, pos) == QValidator::Acceptable) {
		if (row == -1) {
			this->manches[col].h = text.toInt();
		}
		else {
			this->equipages[row].manches[col].h = text.toInt();
			this->equipages[row].manches[col].abr = "";
		}
		if (text.length() == 2) {
			ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(2)
				->widget()->setFocus();
		}
	}
	else if (row != -1) {
		this->equipages[row].manches[col].h = -1;
		this->equipages[row].manches[col].abr = text;
	}
	this->modif();
}
void FenPrincipale::min_changed(QString text) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	if (this->typeClmt == CLMT_SCRATCH) {
		int pos = 0;
		QIntValidator v(this);
		v.setBottom(1);
		if (v.validate(text, pos) == QValidator::Acceptable) {
			this->equipages[row].manches[col].pl = text.toInt();
			this->equipages[row].manches[col].abr = "";
		}
		else {
			this->equipages[row].manches[col].pl = -1;
			this->equipages[row].manches[col].abr = text;
		}
	}
	else {
		int pos = 0;
		QIntValidator v(0, 59, this);
		if (v.validate(text, pos) == QValidator::Acceptable) {
			if (row == -1) {
				this->manches[col].min = text.toInt();
			}
			else {
				this->equipages[row].manches[col].min = text.toInt();
				this->equipages[row].manches[col].abr = "";
			}
			if (text.length() == 2) {
				ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(4)
					->widget()->setFocus();
			}
		}
		else if (row != -1) {
			this->equipages[row].manches[col].min = -1;
			this->equipages[row].manches[col].abr = text;
		}
	}
	this->modif();
}
void FenPrincipale::s_changed(QString text) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	int pos = 0;
	QIntValidator v(0, 59, this);
	if (v.validate(text, pos) == QValidator::Acceptable) {
		if (row == -1) {
			this->manches[col].s = text.toInt();
		}
		else {
			this->equipages[row].manches[col].s = text.toInt();
			this->equipages[row].manches[col].abr = "";
		}
		if (text.length() == 2) {
			if (row < this->nbEquipages-1) {
				ui->manches->cellWidget(row+3, col+1)->layout()->itemAt(0)
					->widget()->setFocus();
			}
			else if (col < this->nbManches-1) {
				ui->manches->cellWidget(2, col+2)->layout()->itemAt(0)
					->widget()->setFocus();
			}
		}
	}
	else if (row != -1) {
		this->equipages[row].manches[col].s = -1;
		this->equipages[row].manches[col].abr = text;
	}
	this->modif();
}

void FenPrincipale::h_pressed(QKeyEvent *ev) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	if (ev->key() == Qt::Key_Up) {
		if ((this->typeClmt == CLMT_TEMPS && row > -1)
			|| (this->typeClmt == CLMT_SCRATCH && row > 0)
		) {
			ui->manches->cellWidget(row+1, col+1)->layout()->itemAt(0)
				->widget()->setFocus();
		}
	}
	else if (ev->key() == Qt::Key_Down && row < this->nbEquipages-1) {
		ui->manches->cellWidget(row+3, col+1)->layout()->itemAt(0)
			->widget()->setFocus();
	}
	else if (ev->key() == Qt::Key_Left && col > 0) {
		ui->manches->cellWidget(row+2, col)->layout()->itemAt(4)
			->widget()->setFocus();
	}
	else if (ev->key() == Qt::Key_Right) {
		ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(2)
			->widget()->setFocus();
	}
}
void FenPrincipale::min_pressed(QKeyEvent *ev) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	if (ev->key() == Qt::Key_Up) {
		if ((this->typeClmt == CLMT_TEMPS && row > -1)
			|| (this->typeClmt == CLMT_SCRATCH && row > 0)
		) {
			ui->manches->cellWidget(row+1, col+1)->layout()->itemAt(2)
				->widget()->setFocus();
		}
	}
	else if (ev->key() == Qt::Key_Down && row < this->nbEquipages-1) {
		ui->manches->cellWidget(row+3, col+1)->layout()->itemAt(2)
			->widget()->setFocus();
	}
	else if (ev->key() == Qt::Key_Left) {
		if (this->typeClmt == CLMT_SCRATCH) {
			if (col > 0) {
				ui->manches->cellWidget(row+2, col)->layout()->itemAt(2)
					->widget()->setFocus();
			}
		}
		else {
			ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(0)
				->widget()->setFocus();
		}
	}
	else if (ev->key() == Qt::Key_Right) {
		if (this->typeClmt == CLMT_SCRATCH) {
			if (col < this->nbManches-1) {
				ui->manches->cellWidget(row+2, col+2)->layout()->itemAt(2)
					->widget()->setFocus();
			}
		}
		else {
			ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(4)
				->widget()->setFocus();
		}
	}
}
void FenPrincipale::s_pressed(QKeyEvent *ev) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	if (ev->key() == Qt::Key_Up) {
		if ((this->typeClmt == CLMT_TEMPS && row > -1)
			|| (this->typeClmt == CLMT_SCRATCH && row > 0)
		) {
			ui->manches->cellWidget(row+1, col+1)->layout()->itemAt(4)
				->widget()->setFocus();
		}
	}
	else if (ev->key() == Qt::Key_Down && row < this->nbEquipages-1) {
		ui->manches->cellWidget(row+3, col+1)->layout()->itemAt(4)
			->widget()->setFocus();
	}
	else if (ev->key() == Qt::Key_Left) {
		ui->manches->cellWidget(row+2, col+1)->layout()->itemAt(2)
			->widget()->setFocus();
	}
	else if (ev->key() == Qt::Key_Right && col < this->nbManches-1) {
		ui->manches->cellWidget(row+2, col+2)->layout()->itemAt(0)
			->widget()->setFocus();
	}
}

void FenPrincipale::tps_focussed(bool hasFocus) {
	int row = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("rowIndex").toInt();
	int col = qobject_cast<QWidget*>(sender())
		->parentWidget()->property("colIndex").toInt();
	if (ui->manches->columnCount() > col+1) {
		QWidget *wdg1 = ui->manches->cellWidget(0, col+1);
		wdg1->setProperty("selected", hasFocus);
		wdg1->style()->unpolish(wdg1);
		wdg1->style()->polish(wdg1);
		wdg1->update();
	}
	if (ui->manches->rowCount() > row+2) {
		QWidget *wdg2 = ui->manches->cellWidget(row+2, 0);
		wdg2->setProperty("selected", hasFocus);
		wdg2->style()->unpolish(wdg2);
		wdg2->style()->polish(wdg2);
		wdg2->update();
	}
	this->currentManche = col;
	if (hasFocus) {
		this->rmMancheTimer->stop();
		ui->rmManche->setEnabled(true);
	}
	else {
		// on a besoin du Timer, car sinon l'événement "clicked" n'est pas
		// déclenché
		this->rmMancheTimer->start(10);
	}
}

void FenPrincipale::manche_focussed(bool hasFocus) {
	this->currentManche = qobject_cast<QWidget*>(sender())
		->property("colIndex").toInt();
	if (hasFocus) {
		this->rmMancheTimer->stop();
		ui->rmManche->setEnabled(true);
	}
	else {
		// on a besoin du Timer, car sinon l'événement "clicked" n'est pas
		// déclenché
		this->rmMancheTimer->start(10);
	}
}

void FenPrincipale::disable_rmManche() { ui->rmManche->setEnabled(false); }

void FenPrincipale::on_addManche_clicked() {
	MyLabel *nom = new MyLabel(tr("Manche n°%1").arg(QString::number(this->nbManches+1)));
	nom->setProperty("type", "header");
	nom->setProperty("top", true);
	nom->setProperty("colIndex", this->nbManches);
	nom->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	nom->setFocusPolicy(Qt::ClickFocus);
	connect(nom, SIGNAL(focussed(bool)), this, SLOT(manche_focussed(bool)));
	ui->manches->insertColumn(this->nbManches+1);
	ui->manches->setCellWidget(0, this->nbManches+1, nom);
	if (!this->manches.contains(this->nbManches)) {
		Manche m;
		this->manches[this->nbManches] = m;
	}
	this->add_manche_inputs(-1, this->nbManches);
	for (int i = 0; i < this->nbEquipages; ++i) {
		if (!this->equipages[i].manches.contains(this->nbManches)) {
			Manche m;
			this->equipages[i].manches[this->nbManches] = m;
		}
		this->add_manche_inputs(i, this->nbManches);
	}
	ui->manches->resizeColumnsToContents();
	++this->nbManches;
	this->modif();
}

void FenPrincipale::add_manche_inputs(int row, int col) {
	QWidget *wdg = new QWidget(this);
	QHBoxLayout *layout = new QHBoxLayout(wdg);
	MyLineEdit *h = new MyLineEdit(wdg);
	MyLineEdit *min = new MyLineEdit(wdg);
	MyLineEdit *s = new MyLineEdit(wdg);
	h->setAlignment(Qt::AlignHCenter);
	min->setAlignment(Qt::AlignHCenter);
	s->setAlignment(Qt::AlignHCenter);
	QLabel *points1 = new QLabel(":", wdg);
	QLabel *points2 = new QLabel(":", wdg);
	layout->addWidget(h);
	layout->addWidget(points1);
	layout->addWidget(min);
	layout->addWidget(points2);
	layout->addWidget(s);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	wdg->setProperty("rowIndex", row);
	wdg->setProperty("colIndex", col);
	wdg->setLayout(layout);
	ui->manches->setCellWidget(row+2, col+1, wdg);
	// on cache les bons champs si classement scratch
	if (this->typeClmt == CLMT_SCRATCH) {
		h->hide();
		points1->hide();
		points2->hide();
		s->hide();
		min->setProperty("place", true);
		min->style()->unpolish(min);
		min->style()->polish(min);
		min->update();
	}
	connect(h, SIGNAL(textChanged(QString)), this, SLOT(h_changed(QString)));
	connect(min, SIGNAL(textChanged(QString)), this, SLOT(min_changed(QString)));
	connect(s, SIGNAL(textChanged(QString)), this, SLOT(s_changed(QString)));
	connect(h, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(h_pressed(QKeyEvent*)));
	connect(min, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(min_pressed(QKeyEvent*)));
	connect(s, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(s_pressed(QKeyEvent*)));
	connect(h, SIGNAL(focussed(bool)), this, SLOT(tps_focussed(bool)));
	connect(min, SIGNAL(focussed(bool)), this, SLOT(tps_focussed(bool)));
	connect(s, SIGNAL(focussed(bool)), this, SLOT(tps_focussed(bool)));
}

void FenPrincipale::on_rmManche_pressed() {
	if (this->nbManches > 0 &&
		this->confirm(
			tr("Supprimer une manche"),
			tr("Voulez-vous vraiment supprimer la manche n°%1 ?")
				.arg(QString::number(this->currentManche+1)),
			"!"
		)
	) {
		ui->manches->removeColumn(this->currentManche+1);
		this->manches.rm(this->currentManche);
		for (int i = 0; i < this->nbEquipages; ++i) {
			for (int j = (this->currentManche+1); j < this->nbManches; ++j) {
				ui->manches->cellWidget(i+2, j)->setProperty("colIndex", j-1);
			}
			this->equipages[i].manches.rm(this->currentManche);
		}
		if (this->typeClmt == CLMT_TEMPS) {
			for (int j = (this->currentManche+1); j < this->nbManches; ++j) {
				ui->manches->cellWidget(1, j)->setProperty("colIndex", j-1);
			}
		}
		for (int i = (this->currentManche+1); i < this->nbManches; ++i) {
			QLabel *nom = qobject_cast<QLabel*>(ui->manches->cellWidget(0, i));
			nom->setText(tr("Manche n°%1").arg(QString::number(i)));
			nom->setProperty("colIndex", i-1);

		}
		--this->nbManches;
		this->modif();
	}
}