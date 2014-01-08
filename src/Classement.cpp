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

void FenPrincipale::on_nouveau_triggered() {
	if (this->confirm_close()) {
		webView->reload();
		this->hasModif = false;
		this->filename = "";
		this->set_titre();
		this->set_etape(1);
	}
}

void FenPrincipale::on_ouvrir_triggered() {
	if (this->confirm_close()) {
		this->addProgressBar(tr("Ouverture du classement :"));
		QString name = QFileDialog::getOpenFileName(
			this,
			tr("Ouvrir un classement"),
			"",
			tr("Classement (*.race ou *.txt);;Tous les fichiers (*.*)")
		);
		if (name.isEmpty()) { this->ouvrir_failed(false); }
		else { this->ouvrir(name); }
	}
}

void FenPrincipale::ouvrir(QString name) {
	this->webView->setVisible(false);
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		this->ouvrir_failed(true);
	}
	else {
		QTextStream flux(&file);
		flux.setCodec("UTF-8");
		this->data = flux.readAll();
		/*this->webFrame->evaluateJavaScript("$.ouvrir();");*/
		this->filename = name;
	}
	file.close();
}

void FenPrincipale::ouvrir_callback(bool ok) {
	if (ok) {
		this->removeProgressBar();
		statusBar()->showMessage(
			tr("Le classement est prêt à être modifié."),
			3000
		);
		this->hasModif = false;
		this->set_titre();
		if (this->filename !=
			this->preferences->value("recent1", "").toString()
		) {
			if (this->filename ==
				this->preferences->value("recent2", "").toString()
			) {
				this->preferences->setValue("recent2",
					this->preferences->value("recent1", "").toString());
				this->preferences->setValue("recent1", this->filename);
			}
			else if (this->filename ==
				this->preferences->value("recent3", "").toString()
			) {
				this->preferences->setValue("recent3",
					this->preferences->value("recent2", "").toString());
				this->preferences->setValue("recent2",
					this->preferences->value("recent1", "").toString());
				this->preferences->setValue("recent1", this->filename);
			}
			else if (this->filename ==
				this->preferences->value("recent4", "").toString()
			) {
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
	}
	else {
		this->filename = "";
		this->ouvrir_failed(true);
	}
	this->webView->setVisible(true);
	qApp->restoreOverrideCursor();
}

void FenPrincipale::ouvrir_failed(bool msg) {
	if (msg) {
		this->msg(
			tr("Erreur"),
			tr("Impossible d'ouvrir ou de lire le classement.\nVérifiez que le fichier est accessible en lecture et peut être ouvert avec DériVoile calc'."),
			"x"
		);
	}
	this->removeProgressBar();
	statusBar()->showMessage(
		tr("L'ouverture du classement a échouée."),
		3000
	);
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
	if (this->etapeActuelle == 4) {
		/*this->webFrame->evaluateJavaScript("$.pdf();");*/
	}
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
	if (this->etapeActuelle == 4) {
		/*this->webFrame->evaluateJavaScript("$.html();");*/
	}
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