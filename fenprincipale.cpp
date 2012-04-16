/*

#-#-#-#-#-#-#-#-#-#-#-#    DériVoile calc'    #-#-#-#-#-#-#-#-#-#-#-#

VERSION : @v
DATE : @date
LICENCE : ./COPYING
SITE WEB : http://calc.derivoile.fr

Copyright (C) 2011-2012 Pierre Monchalin, http://derivoile.fr
Vous êtes libre de distribuer et modifier ce programme.
Pensez à me laisser un petit mot si vous distribuez une copie !

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "popup.h"
#include "fenprincipale.h"
#include "ui_fenprincipale.h"

FenPrincipale::FenPrincipale(QWidget *parent) : QMainWindow(parent), ui(new Ui::FenPrincipale) {

	version = "@v";

	ui->setupUi(this);
	showMaximized();

	classement = QString("");
	hasModif = false;

	pageWeb = new QWebView;
	pageWebFrame = pageWeb->page()->mainFrame();

	attachObject();
	connect(pageWeb, SIGNAL(loadFinished(bool)), this, SLOT(chargementTermine(bool)));
	connect(pageWebFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()));

	pageWeb->load(QUrl("qrc:/html/index.html"));
	pageWeb->setContextMenuPolicy(Qt::NoContextMenu);
	setCentralWidget(pageWeb);

	progres = new QProgressBar;
	progres->setMaximumWidth(150);
	texteProgres = new QLabel("");
	statusBar()->addPermanentWidget(texteProgres);
	statusBar()->addPermanentWidget(progres);
	this->setTitre();

	statusBar()->showMessage("DériVoile calc' est un programme proposé par DériVoile (http://derivoile.fr). © 2011-2012 Pierre Monchalin");

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updater(QNetworkReply*)));
	manager->get(QNetworkRequest(QUrl("http://calc.derivoile.fr/common/version.txt")));

//	ui->actionNouveau->setIcon(QIcon::fromTheme("document-new"));
//	ui->actionOuvrir->setIcon(QIcon::fromTheme("document-open"));
//	ui->actionEnregistrer->setIcon(QIcon::fromTheme("document-save"));
//	ui->actionEnregistrer_sous->setIcon(QIcon::fromTheme("document-save-as"));
//	ui->actionImprimer->setIcon(QIcon::fromTheme("document-print"));
//	ui->actionExporter_en_HTML->setIcon(QIcon::fromTheme("text-html"));
//	ui->actionAide->setIcon(QIcon::fromTheme("help-contents"));
//	ui->action_propos->setIcon(QIcon::fromTheme("help-about"));
}

FenPrincipale::~FenPrincipale() {
	delete ui;
}

void FenPrincipale::updater(QNetworkReply *reply) {
	QByteArray rep = reply->readAll();
	QString reponse = rep;
	if (reponse != version) {
		QMessageBox::information(this, "Nouvelle version disponible", "<p>Une nouvelle version (v"+reponse+") de DeriVoile calc' est disponible !</p><p>Vous devriez faire la mise à jour, car d'importants bugs ont peut-être été corrigés : <a href='http://calc.derivoile.fr'>http://calc.derivoile.fr</a></p>");
	}
}

void FenPrincipale::attachObject() { pageWebFrame->addToJavaScriptWindowObject("FenPrincipale", this); }

void FenPrincipale::chargementTermine(bool ok) {
	QStringList args = QCoreApplication::arguments();
	if (args.count() > 1 && classement.isEmpty()) { // si c'est réellement l'ouverture de l'application
		this->addProgressBar("Ouverture du classement :");
		this->ouvrirClassement(args.at(1));
	}
}

void FenPrincipale::addProgressBar(QString texte) {
	texteProgres->setText(texte);
}

void FenPrincipale::removeProgressBar() {
	texteProgres->setText("");
	progres->reset();
}

void FenPrincipale::progression(int nb) {
	progres->setValue(nb);
	QApplication::processEvents();
}

void FenPrincipale::modif() {
	hasModif = true;
}

void FenPrincipale::setEtape(int nb) {
	etapeActuelle = nb;
	if (nb == 4) {
		ui->actionExporter->setEnabled(true);
		ui->actionExporter_en_HTML->setEnabled(true);
		ui->actionImprimer->setEnabled(true);
	}
	else {
		ui->actionExporter->setEnabled(false);
		ui->actionExporter_en_HTML->setEnabled(false);
		ui->actionImprimer->setEnabled(false);
	}
}

void FenPrincipale::setTitre() {
	if (classement == "") {
		this->setWindowTitle("Nouveau classement – DériVoile calc'");
	}
	else {
		QFileInfo infos(classement);
		this->setWindowTitle(QString(infos.fileName())+" – DériVoile calc'");
	}
}

void FenPrincipale::calculer() {
	this->addProgressBar("Calcul du classement :");
	QApplication::processEvents();
	pageWebFrame->evaluateJavaScript("var c = new Calcul(); c.initialisation();");
	QApplication::processEvents();
}

void FenPrincipale::calculM(QString message) {
	if (!message.isEmpty()) {
		QMessageBox::warning(this, "Erreur", message);
	}
	this->removeProgressBar();
}

void FenPrincipale::on_actionNouveau_triggered() {
	if (!hasModif || QMessageBox::warning(this, "Fermeture de ce classement", "Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
		pageWeb->reload();
		hasModif = false;
		classement = "";
		this->setTitre();
		ui->actionExporter->setEnabled(false);
		ui->actionExporter_en_HTML->setEnabled(false);
		ui->actionImprimer->setEnabled(false);
	}
}

bool FenPrincipale::on_actionOuvrir_triggered() {
	if (hasModif && QMessageBox::warning(this, "Fermeture de ce classement", "Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
		return false;
	}
	this->addProgressBar("Ouverture du classement :");
	QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir un classement", "", "Classement (*.race *.txt);;Tous les fichiers (*.*)");
	if (fileName.isEmpty()) {
		this->removeProgressBar();
		statusBar()->showMessage("L'ouverture du classement a échouée.", 3000);
		return false;
	}
	this->ouvrirClassement(fileName);
	return true;
}

bool FenPrincipale::ouvrirClassement(QString fileName) {
	if (!fileName.contains(".race") && !fileName.contains(".txt") && QMessageBox::warning(this, "Attention !", "Ce fichier ne semble pas être un classement créé par DériVoile calc'.\nVoulez-vous vraiment essayer d'ouvrir ce fichier ?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
		this->removeProgressBar();
		statusBar()->showMessage("L'ouverture du classement a échouée.", 3000);
		return false;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le classement. Vérifiez que le fichier est accessible en lecture.");
		file.close();
		this->removeProgressBar();
		statusBar()->showMessage("L'ouverture du classement a échouée.", 3000);
		return false;
	}
	QTextStream flux(&file);
	QString contenu = flux.readAll();
	file.close();
	classement = fileName;
	pageWebFrame->evaluateJavaScript("$.ouvrir('"+contenu+"');");
	return true;
}

void FenPrincipale::ouvrir(QString message) {
	if (!message.isEmpty()) {
		QMessageBox::warning(this, "Erreur", message);
		classement = QString("");
		statusBar()->showMessage("L'ouverture du classement a échouée.", 3000);
	}
	else {
		statusBar()->showMessage("Le classement est prêt à être modifié.", 3000);
		hasModif = false;
		this->setTitre();
	}
	this->removeProgressBar();
}

void FenPrincipale::on_actionEnregistrer_triggered() {
	statusBar()->showMessage("Enregistrement du classement en cours...");
	pageWebFrame->evaluateJavaScript("$.enregistrer();");
}

void FenPrincipale::on_actionEnregistrer_sous_triggered() {
	statusBar()->showMessage("Enregistrement du classement en cours...");
	classement = QString("");
	pageWebFrame->evaluateJavaScript("$.enregistrer();");
}

bool FenPrincipale::enregistrer(QString donnees) {
	QString fileName = classement;
	if (fileName.isEmpty()) {
		fileName = QFileDialog::getSaveFileName(this, "Enregistrer le classement", "", "Classement (*.race *.txt);;Tous les fichiers (*.*)");
	}
	if (fileName.isEmpty()) {
		statusBar()->showMessage("L'enregistrement du classement a échoué.", 3000);
		return false;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer le classement. Vérifiez que le fichier est accessible en écriture.");
		file.close();
		statusBar()->showMessage("L'enregistrement du classement a échoué.", 3000);
		return false;
	}
	QTextStream flux(&file);
	flux << donnees;
	file.close();
	classement = fileName;
	hasModif = false;
	this->setTitre();
	statusBar()->showMessage("Le classement a été enregistré.", 3000);
	return true;
}

void FenPrincipale::closeEvent(QCloseEvent * event) {
	if (hasModif && QMessageBox::warning(this, "Fermeture de ce classement", "Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
		event->ignore();
	}
}

void FenPrincipale::on_actionAide_triggered() {
	popup *aide = new popup("qrc:/html/aide.html", "Aide – DériVoile calc'", this);
	aide->exec();
}

void FenPrincipale::on_actionConsulter_les_ratings_triggered() {
	popup *ratings = new popup("qrc:/html/ratings.html", "Consulter les ratings – DériVoile calc'", this);
	ratings->exec();
}

void FenPrincipale::on_actionExtraits_des_R_gles_de_Course_triggered() {
	popup *regles = new popup("qrc:/html/regles.html", "Extraits des Règles de Course – DériVoile calc'", this);
	regles->exec();
}

void FenPrincipale::on_action_propos_triggered() {
	popup *apropos = new popup("qrc:/html/apropos.html", "À propos – DériVoile calc'", this);
	apropos->exec();
}

void FenPrincipale::on_actionImprimer_triggered() {
	if (etapeActuelle != 4) { QMessageBox::warning(this, "Action impossible", "Cliquez d'abord sur « Classement »."); }
	else {
		QPrinter printer;
		printer.setPageSize(QPrinter::A4);
		QPrintDialog printDialog(&printer, this);
		if (printDialog.exec() == QDialog::Accepted) {
			pageWeb->print(&printer);
			statusBar()->showMessage("Le classement a été imprimé.", 3000);
		}
	}
}

void FenPrincipale::on_actionExporter_triggered() {
	if (etapeActuelle != 4) { QMessageBox::warning(this, "Action impossible", "Cliquez d'abord sur « Classement »."); }
	else {
		QPrinter printer;
		printer.setPageSize(QPrinter::A4);
		printer.setOutputFormat(QPrinter::PdfFormat);

		QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF...", "", "Fichier PDF (*.pdf)");

		if (!fileName.isEmpty()) {
			printer.setOutputFileName(fileName);
			pageWeb->print(&printer);
			statusBar()->showMessage("Le classement a été exporté en PDF.", 3000);
		}
	}
}

void FenPrincipale::on_actionExporter_en_HTML_triggered() {
	if (etapeActuelle != 4) { QMessageBox::warning(this, "Action impossible", "Cliquez d'abord sur « Classement »."); }
	else {
		pageWebFrame->evaluateJavaScript("$.exporterHTML();");
	}
}

bool FenPrincipale::exporterHTML(QString html) {
	QString fileName = QFileDialog::getSaveFileName(this, "Exporter en HTML...", "", "Fichier HTML (*.html)");
	if (fileName.isEmpty()) {
		statusBar()->showMessage("L'export a échoué.", 3000);
		return false;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Erreur", "Impossible d'exporter le classement. Vérifiez que le fichier est accessible en écriture.");
		file.close();
		statusBar()->showMessage("L'export a échoué.", 3000);
		return false;
	}
	QTextStream flux(&file);
	flux << html;
	file.close();
	statusBar()->showMessage("Le classement a été exporté en HTML.", 3000);
	return true;
}
