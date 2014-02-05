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

void FenPrincipale::reset_step1() {
	ui->nomRegate->setText(this->nomRegate);
	ui->typeClassement->setCurrentIndex(this->clmt_textToInt(this->typeClmt));
	ui->typeRatings->setCurrentIndex(this->rt_textToInt(this->typeRt));
	QComboBox *bt;
	if (this->typeRt == RT_FFV) { bt = ui->typeBateauxFfv; }
	else if (this->typeRt == RT_RYA) { bt = ui->typeBateauxRya; }
	else { bt = ui->typeBateauxDeri; }
	bt->setCurrentIndex(this->bt_textToInt(this->typeBt));
}

void FenPrincipale::goto_step1() {
	ui->btnStep1->setFlat(true);
	ui->btnStep2->setFlat(false);
	ui->btnStep3->setFlat(false);
	ui->btnStep4->setFlat(false);
	ui->btnStep5->setFlat(false);
	ui->step1->setVisible(true);
	ui->step2->setVisible(false);
	ui->step3->setVisible(false);
	ui->step4->setVisible(false);
	ui->step5->setVisible(false);
}

void FenPrincipale::on_nomRegate_textChanged(QString text) {
	this->nomRegate = text;
}

void FenPrincipale::on_typeClassement_currentIndexChanged(int nb) {
	this->typeClmt = this->clmt_intToText(nb);
	ui->pTypeRatings->setVisible(this->typeClmt == CLMT_TEMPS);
	ui->typeRatings->setVisible(this->typeClmt == CLMT_TEMPS);
	ui->pTypeBateaux->setVisible(this->typeClmt == CLMT_TEMPS);
	ui->typeBateauxFfv->setVisible(this->typeClmt == CLMT_TEMPS && this->typeRt == RT_FFV);
	ui->typeBateauxRya->setVisible(this->typeClmt == CLMT_TEMPS	&& this->typeRt == RT_RYA);
	ui->typeBateauxDeri->setVisible(this->typeClmt == CLMT_TEMPS && this->typeRt == RT_DERI);

}

void FenPrincipale::on_typeRatings_currentIndexChanged(int nb) {
	this->typeRt = this->rt_intToText(nb);
	ui->typeBateauxFfv->setVisible(this->typeRt == RT_FFV);
	ui->typeBateauxRya->setVisible(this->typeRt == RT_RYA);
	ui->typeBateauxDeri->setVisible(this->typeRt == RT_DERI);
	// on est obligé de modifier la valeur du champ de sélection du type de bateaux
	// pour qu'il corresponde à typeBt avec une valeur possible
	if (this->typeRt == RT_FFV) {
		ui->typeBateauxFfv->setCurrentIndex(this->bt_textToInt(this->typeBt));
	}
	else if (this->typeRt == RT_RYA) {
		ui->typeBateauxRya->setCurrentIndex(this->bt_textToInt(this->typeBt));
	}
	else {
		ui->typeBateauxDeri->setCurrentIndex(this->bt_textToInt(this->typeBt));
	}
}

void FenPrincipale::on_typeBateauxFfv_currentIndexChanged(int nb) {
	this->typeBt = this->bt_intToText(nb);
	this->update_completer();
}
void FenPrincipale::on_typeBateauxRya_currentIndexChanged(int nb) {
	this->typeBt = this->bt_intToText(nb);
	this->update_completer();
}
void FenPrincipale::on_typeBateauxDeri_currentIndexChanged(int nb) {
	this->typeBt = this->bt_intToText(nb);
	this->update_completer();
}

void FenPrincipale::on_btnStep1_clicked() {
	this->goto_step1();
}



// Correspondance entre chaine de caractère et numéro d'items dans les listes de sélection

int FenPrincipale::clmt_textToInt(QString text) {
	if (text == CLMT_TEMPS) { return 0; }
	return 1;
}
QString FenPrincipale::clmt_intToText(int nb) {
	if (nb == 0) { return CLMT_TEMPS; }
	return CLMT_SCRATCH;
}
int FenPrincipale::rt_textToInt(QString text) {
	if (text == RT_FFV) { return 0; }
	if (text == RT_RYA) { return 1; }
	return 2;
}
QString FenPrincipale::rt_intToText(int nb) {
	if (nb == 0) { return RT_FFV; }
	if (nb == 1) { return RT_RYA; }
	return RT_DERI;
}
int FenPrincipale::bt_textToInt(QString text) {
	if (this->typeRt == RT_FFV) {
		if (text == BT_MUL) { return 0; }
		if (text == BT_DER) { return 1; }
		if (text == BT_QUI) { return 2; }
		if (text == BT_HAB) { return 3; }
		return 4;
	}
	if (this->typeRt == RT_RYA) {
		if (text == BT_MUL) { return 0; }
		if (text == BT_DER) { return 1; }
		if (text == BT_QUI) { return 2; }
		return 3;
	}
	if (text == BT_ALL) { return 1; }
	return 0;
}
QString FenPrincipale::bt_intToText(int nb) {
	if (this->typeRt == RT_FFV) {
		if (nb == 0) { return BT_MUL; }
		if (nb == 1) { return BT_DER; }
		if (nb == 2) { return BT_QUI; }
		if (nb == 3) { return BT_HAB; }
		return BT_DER_QUI_HAB;
	}
	if (this->typeRt == RT_RYA) {
		if (nb == 0) { return BT_MUL; }
		if (nb == 1) { return BT_DER; }
		if (nb == 2) { return BT_QUI; }
		return BT_MUL_DER_QUI;
	}
	if (nb == 1) { return BT_ALL; }
	return BT_MUL_DER_QUI;
}