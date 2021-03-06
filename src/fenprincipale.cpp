/*

###################    DériVoile calc' - Français    ###################

Version : v7-2
Date : 2014-06-18
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

Version : v7-2
Date : 2014-06-18
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

#include "FenPrincipale.h"
#include "ui_FenPrincipale.h"

#include "FenAide.h"
#include "FenPropos.h"
#include "FenRatings.h"

int const FenPrincipale::EXIT_CODE_REBOOT = -123456789;
QString const FenPrincipale::VERSION = "v7-2";

QString const FenPrincipale::CLMT_TEMPS = "temps";
QString const FenPrincipale::CLMT_SCRATCH = "scratch";
QString const FenPrincipale::RT_FFV = "ffv";
QString const FenPrincipale::RT_RYA = "rya";
QString const FenPrincipale::RT_DERI = "deri";
QString const FenPrincipale::BT_MUL = "multicoques";
QString const FenPrincipale::BT_DER = "deriveurs";
QString const FenPrincipale::BT_QUI = "quillards";
QString const FenPrincipale::BT_HAB = "habitables";
QString const FenPrincipale::BT_MUL_DER_QUI = "deriveurs_multicoques";
QString const FenPrincipale::BT_DER_QUI_HAB = "deriveurs_habitables";
QString const FenPrincipale::BT_ALL = "tous";
QString const FenPrincipale::ABRS = "DNC,DNS,OCS,BFD,DNF,RAF,DSQ";

FenPrincipale::FenPrincipale(
	QSettings *preferences, QSplashScreen *splash, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FenPrincipale)
{

	this->preferences = preferences;
	this->splash = splash;
	this->splashDone = 0;

	this->abrs = ABRS.split(",");

	ui->setupUi(this);
	QActionGroup *group_langue = new QActionGroup(this);
	group_langue->addAction(ui->francais);
	group_langue->addAction(ui->english);
	if (preferences->value("language", "FR").toString() == "EN") {
		ui->english->setChecked(true);
	}
	QActionGroup *group_maj_ratings = new QActionGroup(this);
	group_maj_ratings->addAction(ui->maj_ratings_auto);
	group_maj_ratings->addAction(ui->maj_ratings_manu);
	if (preferences->value("auto_maj_ratings", true).toBool() == false) {
		ui->maj_ratings_manu->setChecked(true);
	}
	QActionGroup *group_maj_deri = new QActionGroup(this);
	group_maj_deri->addAction(ui->maj_deri_auto);
	group_maj_deri->addAction(ui->maj_deri_manu);
	if (preferences->value("auto_maj_deri", true).toBool() == false) {
		ui->maj_deri_manu->setChecked(true);
	}

	this->load_recents();

	QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
	QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Black.ttf");
	QFont font("Source Sans Pro", 10);
	QApplication::setFont(font);

	splash->showMessage(
		tr("Chargement des ratings..."),
		Qt::AlignHCenter | Qt::AlignBottom
	);

	if (!load_ratings(
		preferences->value("ratings", ":/ratings.json").toString()
	)) {
		load_ratings(":/ratings.json");
	}

	splash->showMessage(
		tr("Chargement..."),
		Qt::AlignHCenter | Qt::AlignBottom
	);

	this->progressText = new QLabel("");
	statusBar()->addPermanentWidget(progressText);
	this->progress = new QProgressBar;
	this->progress->setMaximumWidth(150);
	statusBar()->addPermanentWidget(progress);
	
	if (preferences->value("auto_maj_ratings", true).toBool()) {
		splash->showMessage(
			tr("Recherche de mises à jour..."),
			Qt::AlignHCenter | Qt::AlignBottom
		);
		qApp->processEvents();
		this->splashDone++;
		update_ratings();
	}
	if (preferences->value("auto_maj_deri", true).toBool()) {
		splash->showMessage(
			tr("Recherche de mises à jour..."),
			Qt::AlignHCenter | Qt::AlignBottom
		);
		qApp->processEvents();
		this->splashDone++;
		update_deri();
	}

	// Initialisation
	this->init_step3();
	this->init();
	this->update();

	// Ouverture du fichier si nécessaire
	QStringList args = QCoreApplication::arguments();
	if (args.count() > 1) {
		this->ouvrir(args.at(1));
	}

	while (this->splashDone > 0) { qApp->processEvents(); }

	splash->hide();
	this->showMaximized();

}

FenPrincipale::~FenPrincipale() {
	delete ui;
}

void FenPrincipale::init() {
	this->filename = QString("");
	this->hasModif = false;
	this->nomRegate = "";
	this->typeClmt = CLMT_TEMPS;
	this->typeRt = RT_FFV;
	this->typeBt = BT_DER;
	this->manchesRetirees = 0;
	this->manchesRetireesMin = 0;
	this->nbManches = 0;
	this->nbEquipages = 0;
	this->equipages.clear();
}
void FenPrincipale::update() {
	ui->centralwidget->hide();
	statusBar()->showMessage(
		tr("DériVoile calc' est un programme proposé par DériVoile (http://derivoile.fr). © 2011-2014 Pierre Monchalin")
	);
	this->addProgressBar(tr("Ouverture du fichier :"));
	this->reset_step1();
	this->reset_step2();
	this->reset_step3();
	this->reset_step4();
	this->hasModif = false;
	this->set_titre();
	this->goto_step1();
	ui->centralwidget->show();
	this->removeProgressBar();
}

void FenPrincipale::set_titre() {
	if (this->filename == "") {
		this->setWindowTitle(tr("Nouveau classement – DériVoile calc'"));
	}
	else {
		QFileInfo infos(filename);
		if (this->hasModif) {
			this->setWindowTitle(QString(infos.fileName())+"* – DériVoile calc'");
		}
		else {
			this->setWindowTitle(QString(infos.fileName())+" – DériVoile calc'");
		}
	}
}

void FenPrincipale::modif() {
	this->hasModif = true;
	this->set_titre();
}

void FenPrincipale::addProgressBar(QString text) {
	this->progressText->setText(text);
	this->progress->reset();
}

void FenPrincipale::removeProgressBar() {
	this->progressText->setText("");
	this->progress->reset();
}

void FenPrincipale::progression(int nb) {
	this->progress->setValue(nb);
	qApp->processEvents();
}

bool FenPrincipale::load_ratings(QString filename) {
	QFile file_ratings(filename);
		// Check if the file exists
	if (!file_ratings.exists()) { return false; }
		// Read the file
	file_ratings.open(QIODevice::ReadOnly);
	QByteArray ratings = file_ratings.readAll();
	file_ratings.close();
		// Check the json structure
	QJsonDocument doc_ratings = QJsonDocument::fromJson(ratings);
	if (doc_ratings.isNull()) { return false; }
	this->ratings = ratings;
	this->file_ratings = filename;
	this->version_ratings = doc_ratings.object().value("version").toString();
	preferences->setValue("ratings", filename);
	return true;
}

bool FenPrincipale::confirm(QString title, QString text, QString icon) {
	return this->confirm(title, text, icon, tr("Oui"));
}
bool FenPrincipale::confirm(QString title, QString text, QString icon, QString oui) {
	QMessageBox msgBox;
	msgBox.setWindowTitle(title);
	msgBox.setText(text);
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setButtonText(QMessageBox::Yes, oui);
	msgBox.setButtonText(QMessageBox::Cancel, tr("Annuler"));
	msgBox.setDefaultButton(QMessageBox::Cancel);
	if (icon == "!") {
		msgBox.setIcon(QMessageBox::Warning);
	}
	else {
		msgBox.setIcon(QMessageBox::Question);
	}
	return (msgBox.exec() == QMessageBox::Yes);
}
void FenPrincipale::msg(QString title, QString text, QString icon) {
	QMessageBox msgBox;
	msgBox.setWindowTitle(title);
	msgBox.setText(text);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setButtonText(QMessageBox::Ok, tr("OK"));
	msgBox.setDefaultButton(QMessageBox::Ok);
	if (icon == "i") {
		msgBox.setIcon(QMessageBox::Information);
	}
	else if (icon == "!") {
		msgBox.setIcon(QMessageBox::Warning);
	}
	else {
		msgBox.setIcon(QMessageBox::Critical);
	}
	msgBox.exec();
}
int FenPrincipale::getInt(QString title, QString text, int value, int min, int max) {
	QInputDialog msgBox;
	msgBox.setInputMode(QInputDialog::IntInput);
	msgBox.setWindowTitle(title);
	msgBox.setLabelText(text);
	msgBox.setIntRange(min, max);
	msgBox.setIntValue(value);
	msgBox.setCancelButtonText(tr("Annuler"));
	msgBox.setOkButtonText(tr("Continuer"));
	bool ok = (msgBox.exec() == QDialog::Accepted);
	if (ok) {
		return msgBox.intValue();
	}
	return -1;
}

void FenPrincipale::on_francais_triggered() {
	preferences->setValue("language", "FR");
	this->restart();
}
void FenPrincipale::on_english_triggered() {
	preferences->setValue("language", "EN");
	this->restart();
}

void FenPrincipale::on_ratings_triggered() {
	fenRatings = new FenRatings(this, this->file_ratings, this);
	fenRatings->show();
}

void FenPrincipale::on_aide_triggered() {
	fenAide = new FenAide(this);
	fenAide->show();
}
void FenPrincipale::on_site_web_triggered() {
	QDesktopServices::openUrl(QUrl("http://calc.derivoile.fr"));
}
void FenPrincipale::on_propos_triggered() {
    FenPropos window(this);
    window.exec();
}

bool FenPrincipale::confirm_close() {
	return !this->hasModif || this->confirm(
		tr("Fermeture du classement"),
		tr("Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?"),
		"!");
}

void FenPrincipale::restart() {
	if (this->confirm(
			tr("Redémarrage nécéssaire"),
			tr("L'application doit être redémarrée pour que les changements s'appliquent.\nVoulez-vous continuer ?"),
			"!")
	) {
		if (this->confirm_close()) {
			qApp->exit(FenPrincipale::EXIT_CODE_REBOOT);
		}
	}
}

void FenPrincipale::closeEvent(QCloseEvent * event) {
	if (!this->confirm_close()) {
		event->ignore();
	}
}




#include "MAJ.cpp"
#include "Classement.cpp"
#include "Step1.cpp"
#include "Step2.cpp"
#include "Step3.cpp"
#include "Step4.cpp"
#include "Step5.cpp"
