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

void FenPrincipale::on_nouveau_triggered() {
	if (this->confirm_close()) {
		this->init();
		this->update();
	}
}

void FenPrincipale::on_ouvrir_triggered() {
	if (this->confirm_close()) {
		QString name = QFileDialog::getOpenFileName(
			this,
			tr("Ouvrir un classement"),
			"",
			tr("Classement (*.race ou *.txt);;Tous les fichiers (*.*)")
		);
		this->ouvrir(name);
	}
}

bool FenPrincipale::ouvrir(QString name) {
	if (name.isEmpty()) { return false; }
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qApp->restoreOverrideCursor();
		this->msg(
			tr("Erreur"),
			tr("Impossible d'ouvrir le classement. Vérifiez que le fichier est accessible en lecture."),
			"x"
		);
		return false;
	}
	QByteArray cl = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(cl);
	if (doc.isNull()) {
		qApp->restoreOverrideCursor();
		this->msg(
			tr("Erreur"),
			tr("Impossible de lire le classement. Vérifiez que le fichier peut être ouvert par DériVoile calc'."),
			"x"
		);
		return false;
	}
	this->init();
	QJsonObject obj = this->json_compatibilite(doc.object());
	QJsonValue v;
	v = obj.value("nomRegate");
	if (!v.isString()) { this->nomRegate = ""; }
	else { this->nomRegate = v.toString(); }
	v = obj.value("classement");
	if (!v.isString()) { this->typeClmt = CLMT_TEMPS; }
	else {
		this->typeClmt = v.toString();
		QList<QString> l;
		l << CLMT_TEMPS << CLMT_SCRATCH;
		if (!l.contains(this->typeClmt)) { this->typeClmt = CLMT_TEMPS; }
	}
	if (this->typeClmt == CLMT_TEMPS) {
		v = obj.value("ratings");
		if (!v.isString()) { this->typeRt = RT_FFV; }
		else {
			this->typeRt = v.toString();
			QList<QString> l;
			l << RT_FFV << RT_RYA << RT_DERI;
			if (!l.contains(this->typeRt)) { this->typeRt = RT_FFV; }
		}
		if (this->typeRt == RT_FFV) {
			v = obj.value("bateaux");
			if (!v.isString()) { this->typeBt = BT_DER; }
			else {
				this->typeBt = v.toString();
				QList<QString> l;
				l << BT_MUL << BT_DER << BT_QUI << BT_HAB << BT_DER_QUI_HAB;
				if (!l.contains(this->typeBt)) { this->typeBt = BT_DER; }
			}
		}
		else if (this->typeRt == RT_RYA) {
			v = obj.value("bateaux");
			if (!v.isString()) { this->typeBt = BT_DER; }
			else {
				this->typeBt = v.toString();
				QList<QString> l;
				l << BT_MUL << BT_DER << BT_QUI << BT_MUL_DER_QUI;
				if (!l.contains(this->typeBt)) { this->typeBt = BT_DER; }
			}
		}
		else if (this->typeRt == RT_DERI) {
			v = obj.value("bateaux");
			if (!v.isString()) { this->typeBt = BT_MUL_DER_QUI; }
			else {
				this->typeBt = v.toString();
				QList<QString> l;
				l << BT_MUL_DER_QUI << BT_ALL;
				if (!l.contains(this->typeBt)) { this->typeBt = BT_MUL_DER_QUI; }
			}
		}
	}
	v = obj.value("nbEquipages");
	if (v.isDouble()) { this->nbEquipages = v.toInt(); }
	v = obj.value("nbManches");
	if (v.isDouble()) { this->nbManches = v.toInt(); }
	v = obj.value("manchesRetirees");
	if (v.isDouble()) { this->manchesRetirees = v.toInt(); }
	v = obj.value("manchesRetireesMin");
	if (v.isDouble()) { this->manchesRetireesMin = v.toInt(); }
	v = obj.value("equipages");
	if (v.isArray()) {
		QJsonArray a = v.toArray();
		for (int i = 0; i < this->nbEquipages; ++i) {
			if (a.at(i).isUndefined()) { continue; }
			QJsonObject e = a.at(i).toObject();
			Equipage eq;
			v = e.value("nom");
			if (!v.isString()) { eq.nom = ""; }
			else { eq.nom = v.toString(); }
			v = e.value("rating");
			if (v.isString()) { eq.rating = v.toString(); }
			else if (v.isDouble()) { eq.rating = QString::number(v.toDouble()); }
			else { eq.rating = ""; }
			v = e.value("bateau");
			if (!v.isString()) { eq.bateau = ""; }
			else { eq.bateau = v.toString(); }
			v = e.value("manches");
			if (v.isArray()) {
				QJsonArray aa = v.toArray();
				for (int j = 0; j < this->nbManches; ++j) {
					if (aa.at(j).isUndefined()) { continue; }
					QJsonObject m = aa.at(j).toObject();
					Manche ma;
					v = m.value("pl");
					if (v.isDouble()) { ma.pl = v.toInt(); }
					v = m.value("h");
					if (v.isDouble()) { ma.h = v.toInt(); }
					v = m.value("min");
					if (v.isDouble()) { ma.min = v.toInt(); }
					v = m.value("s");
					if (v.isDouble()) { ma.s = v.toInt(); }
					v = m.value("abr");
					if (v.isString()) { ma.abr = v.toString(); }
					eq.manches.insert(j, ma);
				}
			}
			this->equipages.insert(i, eq);
		}
	}
	this->filename = QString(name);
	this->update();
	// Mise à jour des fichiers récemment ouverts
	if (this->filename != this->preferences->value("recent1", "").toString()) {
		if (this->filename == this->preferences->value("recent2", "").toString()) {
			this->preferences->setValue("recent2",
				this->preferences->value("recent1", "").toString());
			this->preferences->setValue("recent1", this->filename);
		}
		else if (this->filename == this->preferences->value("recent3", "").toString()) {
			this->preferences->setValue("recent3",
				this->preferences->value("recent2", "").toString());
			this->preferences->setValue("recent2",
				this->preferences->value("recent1", "").toString());
			this->preferences->setValue("recent1", this->filename);
		}
		else if (this->filename == this->preferences->value("recent4", "").toString()) {
			this->preferences->setValue("recent4",
				this->preferences->value("recent3", "").toString());
			this->preferences->setValue("recent3",
				this->preferences->value("recent2", "").toString());
			this->preferences->setValue("recent2",
				this->preferences->value("recent1", "").toString());
			this->preferences->setValue("recent1", this->filename);
		}
		else {
			this->preferences->setValue("recent5",
				this->preferences->value("recent4", "").toString());
			this->preferences->setValue("recent4",
				this->preferences->value("recent3", "").toString());
			this->preferences->setValue("recent3",
				this->preferences->value("recent2", "").toString());
			this->preferences->setValue("recent2",
				this->preferences->value("recent1", "").toString());
			this->preferences->setValue("recent1", this->filename);
		}
		this->load_recents();
	}
	qApp->restoreOverrideCursor();
	return true;
}

QJsonObject FenPrincipale::json_compatibilite(QJsonObject obj) {
	// on fixe un numéro de version par défaut, car les fichiers enregistrés
	// avec les premières versions ne le contenaient pas
	QString version = "v6-0";
	QJsonValue v;
	v = obj.value("version");
	if (v.isString()) {
		version = v.toString();
	}
	if (this->version_greater(version, "v7-0")) { return obj; }
	// on remplace les objets indexés par des entiers par des tableaux
	v = obj.value("equipages");
	if (v.isObject()) {
		obj.insert("equipages", this->json_toArray(v.toObject()));
	}
	v = obj.value("equipages");
	if (v.isArray()) {
		QJsonArray a = v.toArray();
		for (int i = 0; i < a.size(); i++) {
			if (a[i].isObject()) {
				v = a[i].toObject().value("manches");
				if (v.isObject()) {
					QJsonObject o = a[i].toObject();
					o.insert("manches", this->json_toArray(v.toObject()));
					a[i] = o;
				}
			}
		}
		obj.insert("equipages", a);
	}
	// on corrige les chaines de caractère qui devraient être des entiers
	v = obj.value("nbEquipages");
	if (v.isString()) { obj.insert("nbEquipages", v.toString().toInt()); }
	v = obj.value("nbManches");
	if (v.isString()) { obj.insert("nbManches", v.toString().toInt()); }
	v = obj.value("manchesRetirees");
	if (v.isString()) { obj.insert("manchesRetirees", v.toString().toInt()); }
	v = obj.value("manchesRetireesMin");
	if (v.isString()) { obj.insert("manchesRetireesMin", v.toString().toInt()); }
	// on corrige la façon de rentrer les temps / places / abréviations
	v = obj.value("classement");
	bool is_scratch = false;
	if (v.isString() && v.toString() == CLMT_SCRATCH) {
		is_scratch = true;
	}
	v = obj.value("equipages");
	if (v.isArray()) {
		QJsonArray a = v.toArray();
		for (int i = 0; i < a.size(); i++) {
			if (a[i].isObject()) {
				v = a[i].toObject().value("manches");
				if (v.isArray()) {
					QJsonArray b = v.toArray();
					for (int j = 0; j < b.size(); j++) {
						if (b[j].isObject()) {
							QJsonObject result;
							int pos = 0;
							if (is_scratch) {
								QIntValidator vv(this);
								vv.setBottom(1);
								QString pl;
								v = b[j].toObject().value("pl");
								if (v.isDouble()) { pl = QString::number(v.toInt()); }
								else if (v.isString()) { pl = v.toString(); }
								else {
									v = b[j].toObject().value("min");
									if (v.isDouble()) { pl = QString::number(v.toInt()); }
									else if (v.isString()) { pl = v.toString(); }
								}
								if (vv.validate(pl, pos) == QValidator::Acceptable) {
									result.insert("pl", pl.toInt());
								}
								else {
									result.insert("abr", pl);
								}
							}
							else {
								QIntValidator vv(0, 59, this);
								QString h;
								v = b[j].toObject().value("h");
								if (v.isDouble()) { h = QString::number(v.toInt()); }
								else if (v.isString()) { h = v.toString(); }
								if (vv.validate(h, pos) == QValidator::Acceptable) {
									result.insert("h", h.toInt());
								}
								else if (!h.isEmpty()) { result.insert("abr", h); }
								QString min;
								v = b[j].toObject().value("min");
								if (v.isDouble()) { min = QString::number(v.toInt()); }
								else if (v.isString()) { min = v.toString(); }
								if (vv.validate(min, pos) == QValidator::Acceptable) {
									result.insert("min", min.toInt());
								}
								else if (!min.isEmpty()) { result.insert("abr", min); }
								QString s;
								v = b[j].toObject().value("s");
								if (v.isDouble()) { s = QString::number(v.toInt()); }
								else if (v.isString()) { s = v.toString(); }
								if (vv.validate(s, pos) == QValidator::Acceptable) {
									result.insert("s", s.toInt());
								}
								else if (!s.isEmpty()) { result.insert("abr", s); }
							}
							b[j] = result;
						}
					}
					QJsonObject o = a[i].toObject();
					o.insert("manches", b);
					a[i] = o;
				}
			}
		}
		obj.insert("equipages", a);
	}
	return obj;
}

QJsonArray FenPrincipale::json_toArray(QJsonObject obj) {
	int i = 0;
	QJsonArray a;
	while (!obj.value(QString::number(i)).isUndefined()) {
		a.append(obj.value(QString::number(i)));
		++i;
	}
	return a;
}


void FenPrincipale::load_recents() {
	if (preferences->value("recent1", "").toString().isEmpty()) {
		ui->recent1->setVisible(false);
	}
	else {
		ui->recent1->setVisible(true);
		ui->recent1->setText(preferences->value("recent1").toString());
	}
	if (preferences->value("recent2", "").toString().isEmpty()) {
		ui->recent2->setVisible(false);
	}
	else {
		ui->recent2->setVisible(true);
		ui->recent2->setText(preferences->value("recent2").toString());
	}
	if (preferences->value("recent3", "").toString().isEmpty()) {
		ui->recent3->setVisible(false);
	}
	else {
		ui->recent3->setVisible(true);
		ui->recent3->setText(preferences->value("recent3").toString());
	}
	if (preferences->value("recent4", "").toString().isEmpty()) {
		ui->recent4->setVisible(false);
	}
	else {
		ui->recent4->setVisible(true);
		ui->recent4->setText(preferences->value("recent4").toString());
	}
	if (preferences->value("recent5", "").toString().isEmpty()) {
		ui->recent5->setVisible(false);
	}
	else {
		ui->recent5->setVisible(true);
		ui->recent5->setText(preferences->value("recent5").toString());
	}
}
void FenPrincipale::on_recent1_triggered() {
	this->ouvrir(this->preferences->value("recent1", "").toString());
}
void FenPrincipale::on_recent2_triggered() {
	this->ouvrir(this->preferences->value("recent2", "").toString());
}
void FenPrincipale::on_recent3_triggered() {
	this->ouvrir(this->preferences->value("recent3", "").toString());
}
void FenPrincipale::on_recent4_triggered() {
	this->ouvrir(this->preferences->value("recent4", "").toString());
}
void FenPrincipale::on_recent5_triggered() {
	this->ouvrir(this->preferences->value("recent5", "").toString());
}
void FenPrincipale::on_recents_clean_triggered() {
	this->preferences->setValue("recent1", "");
	ui->recent1->setVisible(false);
	this->preferences->setValue("recent2", "");
	ui->recent2->setVisible(false);
	this->preferences->setValue("recent3", "");
	ui->recent3->setVisible(false);
	this->preferences->setValue("recent4", "");
	ui->recent4->setVisible(false);
	this->preferences->setValue("recent5", "");
	ui->recent5->setVisible(false);
}

void FenPrincipale::on_enregistrer_triggered() {
	this->enregistrer(this->filename);
}

void FenPrincipale::on_enregistrer_sous_triggered() {
	this->enregistrer("");
}

void FenPrincipale::enregistrer(QString name) {
	statusBar()->showMessage(tr("Enregistrement du classement en cours..."));
	if (name.isEmpty()) {
		name = QFileDialog::getSaveFileName(
			this,
			tr("Enregistrer le classement"),
			"",
			tr("Classement (*.race ou *.txt);;Tous les fichiers (*.*)")
		);
	}
	if (name.isEmpty()) {
		this->enregistrer_failed(false);
	}
	else {
		this->data = name;
		/*webFrame->evaluateJavaScript("$.enregistrer();");*/
	}
}

void FenPrincipale::enregistrer_callback(QString data) {
	QFile file(this->data);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		this->enregistrer_failed(true);
	}
	else {
		QTextStream flux(&file);
		flux.setCodec("UTF-8");
		flux << data;
		this->filename = this->data;
		this->set_titre();
		hasModif = false;
		statusBar()->showMessage(tr("Le classement a été enregistré."), 3000);
	}
	file.close();
}

void FenPrincipale::enregistrer_failed(bool msg) {
	if (msg) {
		this->msg(
			tr("Erreur"),
			tr("Impossible d'enregistrer le classement.\nVérifiez que le fichier est accessible en écriture."),
			"x"
		);
	}
	statusBar()->showMessage(
		tr("L'enregistrement du classement a échoué."),
		3000
	);
}

void FenPrincipale::on_pdf_triggered() {
}
void FenPrincipale::pdf_callback(QString html) {
	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	printer.setOutputFormat(QPrinter::PdfFormat);
	QString name = QFileDialog::getSaveFileName(
		this,
		tr("Exporter en PDF..."),
		"",
		tr("Fichier PDF (*.pdf)")
	);
	if (!name.isEmpty()) {
		printer.setOutputFileName(name);
		QWebView printable;
		printable.setHtml(this->get_printed_html(html));
		printable.print(&printer);
		QDesktopServices::openUrl(QUrl(name));
		statusBar()->showMessage(
			tr("Le classement a été exporté en PDF."),
			3000
		);
	}
}
void FenPrincipale::on_html_triggered() {
}
void FenPrincipale::html_callback(QString html) {
	QString name = QFileDialog::getSaveFileName(
		this,
		tr("Exporter en HTML..."),
		"",
		tr("Fichier HTML (*.html)")
	);
	if (!name.isEmpty()) {
		QFile file(name);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			this->msg(
				tr("Erreur"),
				tr("Impossible d'exporter le classement.\nVérifiez que le fichier est accessible en écriture."),
				"x"
			);
			file.close();
			statusBar()->showMessage(tr("L'export a échoué."), 3000);
		}
		else {
			QTextStream flux(&file);
			flux.setCodec("UTF-8");
			flux << this->get_printed_html(html);
			file.close();
			QDesktopServices::openUrl(QUrl(name));
			statusBar()->showMessage(
				tr("Le classement a été exporté en HTML."),
				3000
			);
		}
	}
}

QString FenPrincipale::get_printed_html(QString html) {
	return "\
<html>\
	<head>\
		<meta charset=\"utf-8\" />\
		<style>\
		body {\
			font-family: \"Source Sans Pro\", \"Verdana\", \"Arial\", \"Helvetica\", sans-serif;\
			font-size: 16px;\
		}\
		h2 {\
			text-align: center;\
			font-weight: 900;\
			font-size: 2em;\
			margin: 24px 0 32px 0;\
		}\
		a {\
			color: #005FA3;\
			text-decoration: none;\
		}\
		a:hover {\
			text-decoration: underline;\
		}\
		#stats { text-align: center; margin: 24px 0 32px 0; }\
		table {\
			margin: 0 auto;\
			border-collapse: collapse;\
		}\
		td {\
			border: 1px solid #DDE6F6;\
			text-align: center;\
			padding: 5px;\
		}\
		td.b-top { border-top: none; }\
		td.b-bottom { border-bottom: none; }\
		thead td {\
			background: #DDE6F6;\
			color: #005FA3;\
			padding: 6px 10px;\
			font-weight: 900;\
		}\
		.eqg {\
			text-align: left;\
			min-width: 200px;\
		}\
		.bateau {\
			display: block;\
			margin-top: 4px;\
			padding: 4px 4px 0 0;\
			border-top: 1px dotted #C9D7F1;\
			font-style: italic;\
			font-size: 0.8em;\
			text-transform: uppercase;\
			text-align: right;\
		}\
		.rm-pts:before { content: \"(\"; }\
		.rm-pts:after { content: \")\"; }\
		.copy {\
			text-align: center;\
			margin-top: 32px;\
			font-style: italic;\
		}\
		</style>\
	</head>\
	<body>\
		"+html+"\
		<p class=\"copy\">Ces résultats ont été calculés avec DériVoile calc' (<a href=\"http://calc.derivoile.fr\">calc.derivoile.fr</a>), une alternative à FReg ultra simplifiée.</p>\
	</body>\
</html>\
	";
}

void FenPrincipale::on_quitter_triggered() {
	if (this->confirm_close()) {
		qApp->exit();
	}
}