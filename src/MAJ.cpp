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

QNetworkReply *FenPrincipale::download(QString url, const char *slot) {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, slot);
	return manager->get(QNetworkRequest(QUrl(url)));
}

bool FenPrincipale::check_reply(QNetworkReply *reply) {
	int code = reply->error();
	if (code == QNetworkReply::NoError) {
		int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
			.toInt();
		if (status == 200 && reply->isReadable()) { return true; }
	}
	return false;
}



/* ############################ DériVoile calc' ############################ */

void FenPrincipale::on_maj_deri_triggered() {
	statusBar()->showMessage(tr("Recherche de mises à jour..."));
	this->progress->setMinimum(0);
	this->progress->setMaximum(0);
	this->download(
		"http://calc.derivoile.fr/maj/version.txt",
		SLOT(maj_deri_done(QNetworkReply*))
	);
}
void FenPrincipale::maj_deri_done(QNetworkReply *reply) {
	statusBar()->showMessage("");
	this->progress->setMaximum(100);
	if (check_reply(reply)) {
		QString version = QString::fromUtf8(reply->readAll().data());
		if (this->version_greater(version, FenPrincipale::VERSION, true)) {
			this->msg(
				tr("Mise à jour disponible"),
				tr("Une mise à jour de DériVoile calc' est disponible.<br /><br /><a href='http://calc.derivoile.fr/download.html'>Télécharger cette nouvelle version</a>."),
				"i"
			);
		}
		else {
			this->msg(
				tr("Aucune mise à jour disponible"),
				tr("Cette version de DériVoile calc' est à jour."),
				"i"
			);
		}
	}
	else {
		this->msg(
			tr("Erreur"),
			tr("Impossible de vérifier l'existence d'une mise à jour.<br /><br />Rendez-vous sur <a href='http://calc.derivoile.fr'>calc.derivoile.fr</a> pour vérifier manuellement."),
			"x"
		);
	}
}

bool FenPrincipale::version_greater(QString a, QString b, bool strict) {
	if (strict) {
		return (a.mid(1, 1).toInt()*10+a.mid(3, 1).toInt()
		> b.mid(1, 1).toInt()*10+b.mid(3, 1).toInt());
	}
	return (a.mid(1, 1).toInt()*10+a.mid(3, 1).toInt()
		>= b.mid(1, 1).toInt()*10+b.mid(3, 1).toInt());
}

void FenPrincipale::update_deri() {
	this->download(
		"http://calc.derivoile.fr/maj/version.txt",
		SLOT(update_deri_done(QNetworkReply*))
	);
}
void FenPrincipale::update_deri_done(QNetworkReply *reply) {
	if (check_reply(reply)) {
		QString version = QString::fromUtf8(reply->readAll().data());
		if (this->version_greater(version, FenPrincipale::VERSION, true)) {
			this->msg(
				tr("Mise à jour disponible"),
				tr("Une mise à jour de DériVoile calc' est disponible.<br /><br /><a href='http://calc.derivoile.fr/download.html'>Télécharger cette nouvelle version</a>."),
				"i"
			);
		}
	}
	this->splash->clearMessage();
	this->splashDone--;
}



/* ############################ Ratings ############################ */

void FenPrincipale::on_maj_ratings_triggered() {
	statusBar()->showMessage(tr("Recherche de mises à jour..."));
	this->progress->setMinimum(0);
	this->progress->setMaximum(0);
	this->download(
		"http://calc.derivoile.fr/maj/version_ratings.txt",
		SLOT(maj_ratings_done(QNetworkReply*))
	);
}
void FenPrincipale::maj_ratings_done(QNetworkReply *reply) {
	statusBar()->showMessage("");
	this->progress->setMaximum(100);
	if (check_reply(reply)) {
		QString version = QString::fromUtf8(reply->readAll().data());
		if (version != this->version_ratings) {
			if (this->confirm(
				tr("Mise à jour disponible"),
				tr("Une mise à jour des ratings est disponible.<br /><br />Voulez-vous la télécharger ?"),
				"?"
			)) {
				download_ratings();
			}
		}
		else {
			this->msg(
				tr("Aucune mise à jour disponible"),
				tr("Les ratings sont à jour."),
				"i"
			);
		}
	}
	else {
		this->msg(
			tr("Erreur"),
			tr("Impossible de vérifier l'existence d'une mise à jour."),
			"x"
		);
	}
}

void FenPrincipale::update_ratings() {
	this->download(
		"http://calc.derivoile.fr/maj/version_ratings.txt",
		SLOT(update_ratings_done(QNetworkReply*))
	);
}
void FenPrincipale::update_ratings_done(QNetworkReply *reply) {
	if (check_reply(reply)) {
		QString version = QString::fromUtf8(reply->readAll().data());
		if (version != this->version_ratings) {
			if (this->confirm(
				tr("Mise à jour disponible"),
				tr("Une mise à jour des ratings est disponible.<br /><br />Voulez-vous la télécharger ?"),
				"?"
			)) {
				download_ratings();
			}
		}
	}
	this->splash->clearMessage();
	this->splashDone--;
}



/* ########################## Download ratings ########################## */

void FenPrincipale::download_ratings() {
	this->splashDone++;
	this->progressDialog = new QProgressDialog(
		tr("Téléchargement des nouveaux ratings en cours..."),
		tr("Annuler"),
		0,
		100,
		this
	);
	this->progressDialog->setWindowTitle(tr("Téléchargement en cours"));
	QNetworkReply *reply = this->download(
		"http://calc.derivoile.fr/maj/ratings.json",
		SLOT(download_ratings_done(QNetworkReply*))
	);
	connect(
		reply,
		SIGNAL(downloadProgress(qint64, qint64)),
		this,
		SLOT(download_progress(qint64, qint64))
	);
	connect(progressDialog, SIGNAL(canceled()),	reply, SLOT(abort()));
}
void FenPrincipale::download_ratings_done(QNetworkReply *reply) {
	if (check_reply(reply)) {
		this->progressDialog->done(0);
		QString filename = QDir::currentPath() + "/ratings.json";
		QFile file_ratings(filename);
		file_ratings.open(QIODevice::WriteOnly);
		file_ratings.write(reply->readAll());
		file_ratings.close();
		if (!this->load_ratings(filename)) {
			this->msg(
				tr("Erreur"),
				tr("La lecture des ratings a échoué.<br /><br />Vous pouvez réessayer ou <a href='http://calc.derivoile.fr/maj/ratings.json'>télécharger manuellement le fichier</a> puis l'importer via le menu « Préférences »."),
				"x"
			);
		}
		else {
			/*this->update_ratings_js();*/
			this->msg(
				tr("Terminé"),
				tr("Les ratings sont maintenant à jour."),
				"i"
			);
		}
		this->splashDone--;
	}
	else {
		download_canceled();
	}
}
void FenPrincipale::download_progress(qint64 value, qint64 max) {
	this->progressDialog->setValue(value * 100 / max);
}
void FenPrincipale::download_canceled() {
	this->progressDialog->done(0);
	this->msg(
		tr("Erreur"),
		tr("Le téléchargement des ratings a échoué.<br /><br />Vous pouvez <a href='http://calc.derivoile.fr/maj/ratings.json'>télécharger manuellement le fichier</a> puis l'importer via le menu « Préférences »."),
		"x"
	);
	this->splashDone--;
}



/* ############################ Preferences ############################ */

void FenPrincipale::on_maj_deri_auto_triggered() {
	preferences->setValue("auto_maj_deri", true);
}
void FenPrincipale::on_maj_deri_manu_triggered() {
	preferences->setValue("auto_maj_deri", false);
}
void FenPrincipale::on_maj_ratings_auto_triggered() {
	preferences->setValue("auto_maj_ratings", true);
}
void FenPrincipale::on_maj_ratings_manu_triggered() {
	preferences->setValue("auto_maj_ratings", false);
}