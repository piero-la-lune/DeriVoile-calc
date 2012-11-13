/*

###################    DériVoile calc' - Français    ###################

Version : v6-3
Date : 2012-10-26
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

Version : v6-3
Date : 2012-10-26
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

var FenPrincipale = {
	classement: '',
	hasModif: '',
	etapeActuelle: 0,
	dialog: { draggable: false, modal: true, resizable: false, width: 'auto', minHeight: 'auto' },

	addProgressBar: function(texte) {
		$("#d1 span").text(texte);
		$("#d1 #progressbar div").width(0);
		$("#d1").show();
	},

	removeProgressBar: function() {
		$("#d1").hide();
	},

	progression: function(nb) {
		$("#d1 #progressbar div").width(nb+'%');
	},

	modif: function() {
		this.hasModif = true;
	},

	setEtape: function(nb) {
		this.etapeActuelle = nb;
		if (nb == 4) {
			$("#mExportHTML").removeClass('disabled');
			$("#mPrint").removeClass('disabled');
		}
		else {
			$("#mExportHTML").addClass('disabled');
			$("#mPrint").addClass('disabled');
		}
	},

	setTitre: function() {
		if (this.classement == "") {
			$("#filename").html("Nouveau classement");
		}
		else {
			$("#filename").html(this.classement);
		}
	},

	calculer: function() {
		$(".divMain").hide();
		this.addProgressBar("Calcul du classement :");
		var c = new Calcul();
		/*c.initialisation(nbManche, nbEquipage, ratings);*/
		c.initialisation();
	},

	calculM: function(message) {
		if (message != "") {
			alert(message);
		}
		this.removeProgressBar();
		$(".divMain").show();
	},

	on_actionNouveau_triggered: function() {
		if (!this.hasModif || window.confirm("Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?")) {
			FenPrincipale.hasModif = false; // pour ne pas afficher d'autre message de confirmation
			window.location.reload();
		}
	},

	on_actionOuvrir_triggered: function() {
		if (this.hasModif && !window.confirm("Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?")) {
			return false;
		}
		$(".divMain").hide();
		$("#d2").slideDown();
		$("#d2 .annuler").click(function() {
			$("#d2").hide();
			$(".divMain").slideDown();
		});
		$("#d2 form").submit(function() {
			$("#d2").hide();
			FenPrincipale.addProgressBar("Ouverture du classement :");
			FenPrincipale.progression(1);
		});
	},

	ouvrirClassement: function(txt) {
		$.ouvrir(txt);
	},

	ouvrir: function(message) {
		if (message != "") {
			alert(message);
		}
		else {
			this.hasModif = false;
			this.classement = $("#file").val();
			this.setTitre();
		}
		this.removeProgressBar();
		$(".divMain").show();
	},

	on_actionEnregistrer_triggered: function() {
		$.enregistrer();
	},

	enregistrer: function(donnees) {
		$("#d4 #donnees").val(donnees);
		$("#d4 #nom").val(this.classement);
		$(".divMain").hide();
		$("#d4").slideDown();
		$("#d4 .annuler").click(function() {
			$("#d4").hide();
			$(".divMain").slideDown();
		});
		$("#d4 form").submit(function() {
			FenPrincipale.classement = $("#d4 #nom").val()+'.race';
			FenPrincipale.hasModif = false;
			FenPrincipale.setTitre();
			$("#d4 .annuler").click();
		});
	},

	on_actionExporter_en_HTML_triggered: function() {
		if (this.etapeActuelle != 4) {
			alert('Cliquez d\'abord sur « Classement ».');
		}
		else {
			$.exporterHTML();
		}
	},

	exporterHTML: function(html) {
		$("#d5 #donnees2").val(html);
		$(".divMain").hide();
		$("#d5").slideDown();
		$("#d5 .annuler").click(function() {
			$("#d5").hide();
			$(".divMain").slideDown();
		});
		$("#d5 form").submit(function() {
			$("#d5 .annuler").click();
		});
	},

	on_actionImprimer_triggered: function() {
		if (this.etapeActuelle != 4) {
			alert('Cliquez d\'abord sur « Classement ».');
		}
		else {
			window.print();
		}
	},

	on_actionOpen_popup_triggered: function(url, titre) {
		window.open(url, 'popup', 'width=700,height=500,scrollbars=yes,resizable=yes');
	}
}

$(document).ready(function() {

/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------- MENU ----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

var menuAffiche = false;

$("nav li > a").bind('click', function() {
	var a = $(this);
	if (a.hasClass('mSelected')) { $(document).click(); return false; }
	$(document).click();
	var div = a.parent().find('div');
	div.css('top', a.outerHeight());
	div.slideDown(50);
	a.addClass('mSelected');
	menuAffiche = true;
	return false;
});
$("nav li > a").bind('mouseenter', function() {
	if (menuAffiche && !$(this).hasClass('mSelected')) {
		$(this).click();
	}
	return false;
});
$(document).bind('click', function() {
	if (!menuAffiche) { return true; }
	$("nav div").slideUp(50);
	$("nav a").removeClass('mSelected');
	menuAffiche = false;
});
$("nav div a").click(function() {
	if ($(this).hasClass('disabled')) { return false; }
	$(document).click();
	$("nav div").hide(); // Pour IE6 qui fait encore des siennes...
	var id = $(this).attr('id');
	if (id == "mNouveau") { FenPrincipale.on_actionNouveau_triggered(); }
	else if (id == "mOpen") { FenPrincipale.on_actionOuvrir_triggered(); }
	else if (id == "mSave") { FenPrincipale.on_actionEnregistrer_triggered(); }
	else if (id == "mExportHTML") { FenPrincipale.on_actionExporter_en_HTML_triggered(); }
	else if (id == "mPrint") { FenPrincipale.on_actionImprimer_triggered(); }
	else if (id == "mAide") { FenPrincipale.on_actionOpen_popup_triggered('./html/aide.html', 'Aide'); }
	else if (id == "mRatings") { FenPrincipale.on_actionOpen_popup_triggered('./html/ratings.html', 'Consulter les ratings'); }
	else if (id == "mRegles") { FenPrincipale.on_actionOpen_popup_triggered('./html/regles.html', 'Extraits des Règles de Course'); }
	else if (id == "mInfos") { FenPrincipale.on_actionOpen_popup_triggered('./html/apropos.html', 'À propos...'); }
	return false;
});

window.onbeforeunload = function() {
	if (FenPrincipale.hasModif) return "Certaines données n'ont pas été enregistrées et vont être perdues.\nVoulez-vous vraiment continuer ?";
}

});