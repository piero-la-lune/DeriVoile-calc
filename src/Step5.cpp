/*

###################    DériVoile calc' - Français    ###################

Version : v7-0
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

Version : v7-0
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

void FenPrincipale::reset_step5() {
}

void FenPrincipale::on_btnStep5_clicked() {
	this->goto_step5();
}
void FenPrincipale::goto_step5() {
	this->leave_step1();
	this->leave_step2();
	this->leave_step3();
	this->leave_step4();
	ui->btnStep5->setFlat(true);
	QLayoutItem *child;
	while ((child = ui->resultatsLayout->takeAt(0)) != 0) {
		delete child->widget();
	    delete child;
	}
	ui->choisirResultat->clear();
	ui->choisirResultat->insertItem(0, tr("Classement général"));
	this->calculer();
	ui->step5->setVisible(true);
}
void FenPrincipale::leave_step5() {
	ui->btnStep5->setFlat(false);
	ui->step5->setVisible(false);
}

void FenPrincipale::calculer() {
	this->addProgressBar(tr("Calcul du classement"));
	qApp->processEvents();
	if (this->verif()) {
		this->calcul_manches();
		this->calcul_retirermanches();
		this->calcul_general();
	}
	this->removeProgressBar();
}

bool FenPrincipale::verif() {
	for (int i = 0; i < this->nbEquipages; ++i) {
		// on met les variables utilisés pour produire le classement
		// à leur valeur par défaut
		QList<double> pointsOrdonnes;
		QList<double> pointsTries;
		QList<double> pointsRetires;
		this->equipages[i].points = 0.0;
		this->equipages[i].pointsOrdonnes = pointsOrdonnes;
		this->equipages[i].pointsTries = pointsTries;
		this->equipages[i].pointsRetires = pointsRetires;
		// on vérifie les données rentrées par l'utilisateur
		if (this->equipages[i].nom.isEmpty()) {
			this->msg(tr("Erreur"), tr("Vous devez donner un nom à tous les équipages."), "x");
			this->goto_step2();
			return false;
		}
		if (this->typeClmt == CLMT_TEMPS) {
			QString rating = this->equipages[i].rating.toUpper();
			if (this->bateaux.value(rating).serie.isEmpty()) {
				QRegExp rx("^[0-9](\\.|,)[0-9]*$");
				if (rating.contains(rx)) {
					this->equipages[i].coef = rating.replace(",", ".").toDouble();
				}
				else {
					this->msg(
						tr("Erreur"),
						tr("Le code bateau « %1 » n'existe pas.").arg(rating),
						"x"
					);
					this->goto_step2();
					return false;
				}
			}
			else {
				this->equipages[i].coef = this->get_coef(this->bateaux.value(rating));
			}
		}
		else {
			this->equipages[i].coef = 1;
		}
		for (int j = 0; j < this->nbManches; ++j) {
			// on met les variables utilisés pour produire le classement
			// à leur valeur par défaut
			this->equipages[i].manches[j].points = 0.0;
			this->equipages[i].manches[j].tpsReel = -1;
			this->equipages[i].manches[j].tpsCompense = -1.0;
			// on calcul place / temps réel et temps compensé
			Manche m = this->equipages[i].manches[j];
			if (!m.abr.isEmpty()) {
				// pas de temps ou place à calculer
				continue;
			}
			if (this->typeClmt == CLMT_TEMPS) {
				if (m.h < 0) { m.h = 0; }
				if (m.min < 0) { m.min = 0; }
				if (m.s < 0) { m.s = 0; }
				int tps = m.h*3600+m.min*60+m.s;
				if (tps == 0) {
					this->equipages[i].manches[j].abr = "DNF";
				}
				else {
					double tpsCompense = tps*this->equipages[i].coef;
					this->equipages[i].manches[j].tpsReel = tps;
					this->equipages[i].manches[j].tpsCompense = tpsCompense;
				}
			}
			else {
				if (m.pl > 0) {
					this->equipages[i].manches[j].tpsReel = m.pl;
					this->equipages[i].manches[j].tpsCompense = m.pl;
				}
				else {
					this->equipages[i].manches[j].abr = "DNF";
				}
			}
		}
	}
	this->progression(5);
	return true;
}

void FenPrincipale::calcul_manches() {
	for (int j = 0;  j < this->nbManches; ++j) {
			// on affiche la table qui va contenir les résultats de la manche
		QTableWidget *table = new QTableWidget();
		table->verticalHeader()->hide();
		table->setSelectionMode(QAbstractItemView::NoSelection);
		table->setProperty("table", "manches");
		QList<QString> labels;
		labels << "Place" << "Équipage" << "Points";
		if (this->typeClmt == CLMT_TEMPS) {
			table->setColumnCount(5);
			labels << "Temps réel" << "Temps compensé";
			table->setMaximumWidth(560 + 20);
			table->setMinimumWidth(560 + 20);
			table->setColumnWidth(3, 120);
			table->setColumnWidth(4, 120);
		}
		else {
			table->setColumnCount(3);
			table->setMaximumWidth(320 + 20);
			table->setMinimumWidth(320 + 20);
		}
		table->setColumnWidth(0, 60);
		table->setColumnWidth(1, 200);
		table->setColumnWidth(2, 60);
		table->setHorizontalHeaderLabels(labels);
		table->setRowCount(this->nbEquipages);
		ui->choisirResultat->insertItem(j+1,
			tr("Résultats de la manche %1").arg(QString::number(j+1)));
		int nbAffiches = 0; // pour savoir à quelle ligne on en est
			// on traite et affiche les équipages
		// on traite chaque manche pour attribuer les points aux équipages
		for (int i = 0; i < this->nbEquipages; ++i) {
			// on recherche tous les équipages non encore traités pour cette
			// manches qui ont un tpsCompense minimal
			int min = 0;
			QList<int> ids;
			for (int k = 0; k < this->nbEquipages; ++k) {
				Manche m = this->equipages[k].manches[j];
				if (m.tpsCompense < 0 || m.points > 0 ) {
					// cet équipage a déjà été traité ou n'a pas de place/temps
					// (DNF, DNS, OCS, ...)
					continue;
				}
				if (m.tpsCompense < min || min == 0) {
					min = m.tpsCompense;
					ids.clear();
					ids.append(k);
				}
				else if (m.tpsCompense == min) {
					ids.append(k);
				}
			}
			if (min == 0) {
				// on n'a pas trouvé d'équipage à traiter (se produit s'il y a
				// des équipages DNS, DNF, OCS, ...)
				break;
			}
			for (int l = 0; l < ids.size(); ++l) {
				double points = (ids.size()-1.0)/2.0+i+1.0;
				this->equipages[ids.at(l)].points += points;
				this->equipages[ids.at(l)].pointsOrdonnes.prepend(points);
				this->equipages[ids.at(l)].pointsTries.append(points);
				this->equipages[ids.at(l)].manches[j].points = points;
				// on affiche ces équipages
				Equipage e = this->equipages[ids.at(l)];
				Manche m = e.manches[j];
				QLabel *place = new QLabel(QString::number(i+1));
				table->setCellWidget(i+l, 0, place);
				QLabel *nom = new QLabel(e.nom);
				table->setCellWidget(i+l, 1, nom);
				QLabel *pointsi = new QLabel(QString::number(m.points));
				table->setCellWidget(i+l, 2, pointsi);
				if (this->typeClmt == CLMT_TEMPS) {
					QLabel *tpsReel = new QLabel(this->formate_tps(m.tpsReel));
					table->setCellWidget(i+l, 3, tpsReel);
					QLabel *tpsCompense = new QLabel(this->formate_tps(qRound(m.tpsCompense)));
					table->setCellWidget(i+l, 4, tpsCompense);
				}
				++nbAffiches;
			}
			i = i+ids.size()-1;
		}
		// on traite les équipages qui n'ont pas de place/temps
		for (int i = 0; i < this->nbEquipages; ++i) {
			if (this->equipages[i].manches[j].tpsCompense < 0) {
				double points = this->nbEquipages+1.0;
				this->equipages[i].points += points;
				this->equipages[i].pointsOrdonnes.prepend(points);
				this->equipages[i].pointsTries.append(points);
				this->equipages[i].manches[j].points = points;
				// on affiche ces équipages
				Equipage e = this->equipages[i];
				Manche m = e.manches[j];
				QLabel *place = new QLabel(m.abr);
				table->setCellWidget(nbAffiches, 0, place);
				QLabel *nom = new QLabel(e.nom);
				table->setCellWidget(nbAffiches, 1, nom);
				QLabel *pointsi = new QLabel(QString::number(m.points));
				table->setCellWidget(nbAffiches, 2, pointsi);
				if (this->typeClmt == CLMT_TEMPS) {
					QLabel *tpsReel = new QLabel(m.abr);
					table->setCellWidget(nbAffiches, 3, tpsReel);
					QLabel *tpsCompense = new QLabel(m.abr);
					table->setCellWidget(nbAffiches, 4, tpsCompense);
				}
				++nbAffiches;
			}
		}
		ui->resultatsLayout->addWidget(table);
		table->hide();
		this->progression(5+j*75/this->nbManches);
	}
	// on trie les liste pointsTries
	for (int i = 0; i < this->nbEquipages; ++i) {
		qSort(this->equipages[i].pointsTries);
	}
}

void FenPrincipale::calcul_retirermanches() {
	if (this->nbManches >= this->manchesRetireesMin) {
		for (int i = 0; i < this->nbEquipages; ++i) {
			for (int j = 0; j < this->manchesRetirees; ++j) {
				// On récupère et supprime le dernier élément
				double points = this->equipages[i].pointsTries.takeLast();
				this->equipages[i].points -= points;
				this->equipages[i].pointsRetires.append(points);
			}
		}
	}
	this->progression(85);
}

void FenPrincipale::calcul_general() {
	QTableWidget *table = new QTableWidget();
	table->verticalHeader()->hide();
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->setColumnCount(3+this->nbManches);
	table->setRowCount(this->nbEquipages);
	table->setColumnWidth(0, 80);
	table->setColumnWidth(1, 200);
	table->setColumnWidth(2, 60);
	int size = 80 + 200 + 60;
	QList<QString> labels;
	labels << "Position" << "Équipage" << "Points";
	for (int j = 0; j < this->nbManches; ++j) {
		labels << "M"+QString::number(j+1);
		size += 50;
		table->setColumnWidth(3+j, 50);
	}
	table->setMinimumWidth(size + 20);
	table->setMaximumWidth(size + 20);
	table->setHorizontalHeaderLabels(labels);
	ui->resultatsLayout->insertWidget(0, table);
	for (int i = 0; i < this->nbEquipages; ++i) {
		QList<int> ids;
		Equipage e1;
		for (int k = 0; k < this->nbEquipages; ++k) {
			Equipage e2 = this->equipages[k];
			if (e2.points <= 0) {
				// cet équipage a déjà été affiché
				continue;
			}
			if (ids.isEmpty() || e2.points < e1.points) {
				ids.clear();
				ids.append(k);
				e1 = e2;
			}
			else if (e2.points == e1.points) {
				// égalité de points
				// pour départager les équipages, on confronte leurs meilleures
				// manches (sans celles retirées)
				for (int j = 0; j < e1.pointsTries.size(); ++j) {
					if (e2.pointsTries[j] < e1.pointsTries[j]) {
						ids.clear();
						ids.append(k);
						e1 = e2;
						break;
					}
					else if (e2.pointsTries[j] > e1.pointsTries[j]) {
						break;
					}
					else if (j == e1.pointsTries.size()-1) {
						// égalité des manches
						// pour départager les équipages, on confronte toutes
						// leur manches dans l'ordre en partant de la dernière
						for (int l = 0; l < e1.pointsOrdonnes.size(); ++l) {
							if (e2.pointsOrdonnes[l] < e1.pointsOrdonnes[l]) {
								ids.clear();
								ids.append(k);
								e1 = e2;
								break;
							}
							else if (e2.pointsOrdonnes[l] > e1.pointsOrdonnes[l]) {
								break;
							}
							else if (l = e1.pointsOrdonnes.size()-1) {
								// égalité parfaite
								ids.append(k);
							}
						}
					}
				}
			}
		}
		for (int k = 0; k < ids.size(); ++k) {
			Equipage e = this->equipages[ids[k]];
			QLabel *pos = new QLabel(QString::number(i+1));
			QWidget *nomWidget = new QWidget();
			QVBoxLayout *nomLayout = new QVBoxLayout();
			QLabel *nom = new QLabel(e.nom);
			nom->setProperty("label", "nom");
			nomLayout->addWidget(nom);
			if (this->typeClmt == CLMT_TEMPS) {
				QLabel *bateau = new QLabel();
				bateau->setText(this->bateaux.value(e.rating).serie
					+" ("+QString::number(e.coef)+")");
				bateau->setProperty("label", "bateau");
				nomLayout->addWidget(bateau);
				table->setRowHeight(i+k, 40);
			}
			nomLayout->setContentsMargins(0, 0, 0, 0);
			nomLayout->setSpacing(0);
			nomWidget->setLayout(nomLayout);
			QLabel *points = new QLabel(QString::number(e.points));
			table->setCellWidget(i+k, 0, pos);
			table->setCellWidget(i+k, 1, nomWidget);
			table->setCellWidget(i+k, 2, points);
			for (int j = 0; j < this->nbManches; ++j) {
				Manche m = e.manches[j];
				QLabel *la = new QLabel();
				if (m.tpsCompense < 0) {
					la->setText(m.abr);
				}
				else {
					la->setText(QString::number(m.points));
				}
				for (int l = 0; l < e.pointsRetires.size(); ++l) {
					if (e.pointsRetires[l] == m.points) {
						la->setText("("+la->text()+")");
						e.pointsRetires.removeAt(l);
						break;
					}
				}
				table->setCellWidget(i+k, j+3, la);
			}
			this->equipages[ids[k]].points = 0;
		}
		i += ids.size()-1;
	}
	this->progression(95);
}

void FenPrincipale::on_choisirResultat_currentIndexChanged(int index) {
	if (index >= 0) {
		for (int i = 0; i < ui->resultatsLayout->count(); ++i) {
			ui->resultatsLayout->itemAt(i)->widget()->hide();
		}
		if (ui->resultatsLayout->count() >= index+1) {
			ui->resultatsLayout->itemAt(index)->widget()->show();
		}
	}
}

double FenPrincipale::get_coef(Bateau bt) {
	if (bt.type == BT_MUL) {
		if (this->typeRt == RT_FFV) {
			if (bt.rating != 0) {
				double coef = 1.0/bt.rating;
				return qRound(10000.0*coef)/10000.0;
			}
			else {
				double coef = 1.0*bt.coef;
				return qRound(10000.0*coef)/10000.0;
			}
		}
		else if (this->typeRt == RT_RYA) {
			double coef = 1000.0/bt.rya;
			return qRound(10000.0*coef)/10000.0;
		}
		else {
			if (bt.rating != 0) {
				double coef = 1000.0/692.7/bt.rating;
				return qRound(10000.0*coef)/10000.0;
			}
			else {
				double coef = 1000.0/692.7*bt.coef;
				return qRound(10000.0*coef)/10000.0;
			}

		}
	}
	else if (bt.type == BT_DER || bt.type == BT_QUI) {
		if (this->typeRt == RT_FFV && this->typeBt == BT_DER_QUI_HAB) {
			if (bt.rating != 0) {
				double coef = 768.0/bt.rating;
				return qRound(10000.0*coef)/10000.0;
			}
			else {
				double coef = 768.0*bt.coef/1000.0;
				return qRound(10000.0*coef)/10000.0;
			}
		}
		else if (this->typeRt == RT_FFV) {
			if (bt.rating != 0) {
				double coef = 1000.0/bt.rating;
				return qRound(10000.0*coef)/10000.0;
			}
			else {
				double coef = 1.0*bt.coef;
				return qRound(10000.0*coef)/10000.0;
			}
		}
		else if (this->typeRt == RT_RYA) {
			double coef = 1000.0/bt.rya;
			return qRound(10000.0*coef)/10000.0;
		}
		else {
			if (bt.rating != 0) {
				double coef = 1000.0/1011.2/qPow(bt.rating/1000.0, 0.913);
				return qRound(10000.0*coef)/10000.0;
			}
			else {
				double coef = 1000.0/1011.2/qPow(bt.coef, -0.913);
				return qRound(10000.0*coef)/10000.0;
			}
		}
	}
	else {
		if (bt.groupe <= 11) {
			double coef = 600.0/(290.0-bt.groupe*15.0+683.0);
			return qRound(10000.0*coef)/10000.0;
		}
		else if (bt.groupe <= 30) {
			double coef = 600.0/(235.0-bt.groupe*10.0+683.0);
			return qRound(10000.0*coef)/10000.0;
		}
		else if (bt.groupe <= 45) {
			double coef = 600.0/(175.0-bt.groupe*8.0+683.0);
			return qRound(10000.0*coef)/10000.0;
		}
		else {
			double coef = 600.0/(85.0-bt.groupe*6.0+683.0);
			return qRound(10000.0*coef)/10000.0;
		}
	}
	return 1.0;
}

QString FenPrincipale::formate_tps(int tps) {
	QString temps;
	int h = tps/3600;
	int min = (tps-h*3600)/60;
	int s = tps-h*3600-min*60;
	if (h < 10) { temps = "0"+QString::number(h); }
	else { temps = QString::number(h); }
	if (min < 10) { temps += ":0"+QString::number(min); }
	else { temps += ":"+QString::number(min); }
	if (s < 10) { temps += ":0"+QString::number(s); }
	else { temps += ":"+QString::number(s); }
	return temps;
}


// this->addProgressBar(tr("Calcul du classement :"));
// qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
// qApp->restoreOverrideCursor();