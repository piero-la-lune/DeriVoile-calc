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


	var version = "v7-0";

	var nbManche = 0;
	var nbEquipage = 0;

	var ratings = {
		multicoques: [],
		deriveurs: [],
		quillards: [],
		habitables: []
	}; // Tous les bateaux et leurs coefficients calculés
	var selects = {
		ffv: {
			multicoques: "<div>Multicoques</div>",
			deriveurs: "<div>Dériveurs</div>",
			quillards: "<div>Quillards de sport</div>",
			habitables: "<div>Habitables</div>"
		},
		rya:{
			multicoques: "<div>Multicoques</div>",
			deriveurs: "<div>Dériveurs</div>",
			quillards: "<div>Quillards de sport</div>",
			habitables: "<div>Habitables</div>"
		}
	}; // Code HTML de la liste des bateaux
	var listeBateaux = {
		ffv: {
			multicoques: [],
			deriveurs: [],
			quillards: [],
			habitables: []
		},
		rya: {
			multicoques: [],
			deriveurs: [],
			quillards: [],
			habitables: []
		}
	}; // Liste des bateaux disponibles
	var listeBateauxC = []; // Liste des bateaux actuellement disponibles
	var recents = []; // Liste des bateaux récemment utilisés

	var typeRatings = ""; // Type de ratings actuel
	var typeBateaux = ""; // Type de bateaux actuel

	var selectTdEquipage = null; // TD actuel lorsque .selectBateau affiché
	var oldBateau = ""; // Ancienne valeur de .tpBateau
	var oldCode = ""; // Ancienne valeur de .rating
	var btLength = 0; // Ancienne longueur de .tpBateau

	var etapeActuelle = 1;

	Number.prototype.fround = function(precision) {
		return Math.round(this*(Math.pow(10, precision))) /
			Math.pow(10, precision);
	};

$(function() {

var data = $.parseJSON(FenPrincipale.get_ratings());

var bateau = {};
var ffv = null;
var ffvc = null;
var rya = null;
var deri = null;
var SM = null;

for (i=0; i<data.multicoques.length; i++) {
	bateau = data.multicoques[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1/bateau.rating).fround(4);
		deri = (1000/(bateau.rating*692.7)).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		deri = (1000/(692.7/bateau.coef)).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings["multicoques"][bateau.code] = {
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	};
	if (ffv !== null) {
		selects.ffv.multicoques += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.ffv.multicoques[bateau.code] = bateau.serie;
	}
	if (rya !== null) {
		selects.rya.multicoques += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.rya.multicoques[bateau.code] = bateau.serie;
	}
}
for (i=0; i<data.deriveurs.length; i++) {
	bateau = data.deriveurs[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1000/bateau.rating).fround(4);
		ffvc = (768/bateau.rating).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.rating/1000, 0.913))).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		ffvc = (768*bateau.coef/1000).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.coef, -0.913))).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings["deriveurs"][bateau.code] = {
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	};
	if (ffv !== null) {
		selects.ffv.deriveurs += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.ffv.deriveurs[bateau.code] = bateau.serie;
	}
	if (rya !== null) {
		selects.rya.deriveurs += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.rya.deriveurs[bateau.code] = bateau.serie;
	}
}
for (i=0; i<data.quillards.length; i++) {
	bateau = data.quillards[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	if (bateau.rating !== null) {
		ffv = (1000/bateau.rating).fround(4);
		ffvc = (768/bateau.rating).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.rating/1000, 0.913))).fround(4);
	}
	if (bateau.coef !== null) {
		ffv = bateau.coef;
		ffvc = (768*bateau.coef/1000).fround(4);
		deri = (1000/(1011.2*Math.pow(bateau.coef, -0.913))).fround(4);
	}
	if (bateau.rya !== null) {
		rya = (1000/bateau.rya).fround(4);
	}
	ratings["quillards"][bateau.code] = {
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri
	};
	if (ffv !== null) {
		selects.ffv.quillards += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.ffv.quillards[bateau.code] = bateau.serie;
	}
	if (rya !== null) {
		selects.rya.quillards += '<span data-id="'+bateau.code+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.rya.quillards[bateau.code] = bateau.serie;
	}
}
for (i=0; i<data.habitables.length; i++) {
	bateau = data.habitables[i];
	ffv = null;
	ffvc = null;
	rya = null;
	deri = null;
	SM = null;
	if (bateau.groupe <= 11) { SM = 290-bateau.groupe*15; }
	else if (bateau.groupe <= 30) { SM = 235-bateau.groupe*10; }
	else if (bateau.groupe <= 45) { SM = 175-bateau.groupe*8; }
	else { SM = 85-bateau.groupe*6; }
	ffv = (600/(SM+683)).fround(4);
	ffvc = ffv;
	deri = (1000/(-11.742*bateau.groupe+1163.1)).fround(4);
	ratings["habitables"]["#"+bateau.id] = {
		serie: bateau.serie,
		ffv: ffv,
		ffvc: ffvc,
		rya: rya,
		deri: deri,
		cvl: bateau.cvl
	};
	if (ffv !== null) {
		selects.ffv.habitables += '<span data-id="#'+bateau.id+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.ffv.habitables["#"+bateau.id] = bateau.serie;
	}
	if (rya !== null) {
		selects.ffv.habitables += '<span data-id="#'+bateau.id+
			'" data-match="'+bateau.serie.toLowerCase()+'" class="visible">'+
			bateau.serie+'</span>';
		listeBateaux.rya.habitables["#"+bateau.id] = bateau.serie;
	}
}

$.setSelectBateaux = function() {
	var bateaux = [];
	typeRatings = $("#typeRatings").val();
	if (typeRatings == "ffv") {
		$("#typeBateauxRya").addClass("hidden");
		$("#typeBateauxDeri").addClass("hidden");
		typeBateaux = $("#typeBateauxFfv").removeClass("hidden").val();
		if (typeBateaux == "multicoques") {
			bateaux = selects.ffv.multicoques;
			listeBateauxC = [listeBateaux.ffv.multicoques];
		}
		else if (typeBateaux == "deriveurs") {
			bateaux = selects.ffv.deriveurs;
			listeBateauxC = [listeBateaux.ffv.deriveurs];
		}
		else if (typeBateaux == "quillards") {
			bateaux = selects.ffv.quillards;
			listeBateauxC = [listeBateaux.ffv.quillards];
		}
		else if (typeBateaux == "habitables") {
			bateaux = selects.ffv.habitables;
			listeBateauxC = [listeBateaux.ffv.habitables];
		}
		else {
			bateaux = selects.ffv.deriveurs+selects.ffv.quillards+
				selects.ffv.habitables;
			listeBateauxC = [listeBateaux.ffv.deriveurs,
				listeBateaux.ffv.quillards, listeBateaux.ffv.habitables];
		}
	}
	else if (typeRatings == "rya") {
		$("#typeBateauxFfv").addClass("hidden");
		$("#typeBateauxDeri").addClass("hidden");
		typeBateaux = $("#typeBateauxRya").removeClass("hidden").val();
		if (typeBateaux == "multicoques") {
			bateaux = selects.rya.multicoques;
			listeBateauxC = [listeBateaux.rya.multicoques];
		}
		else if (typeBateaux == "deriveurs") {
			bateaux = selects.rya.deriveurs;
			listeBateauxC = [listeBateaux.rya.deriveurs];
		}
		else if (typeBateaux == "quillards") {
			bateaux = selects.rya.quillards;
			listeBateauxC = [listeBateaux.rya.quillards];
		}
		else {
			bateaux = selects.rya.multicoques+selects.rya.deriveurs+
				selects.rya.quillards;
			listeBateauxC = [listeBateaux.rya.multicoques,
				listeBateaux.rya.deriveurs, listeBateaux.rya.quillards];
		}
	}
	else {
		$("#typeBateauxFfv").addClass("hidden");
		$("#typeBateauxRya").addClass("hidden");
		typeBateaux = $("#typeBateauxDeri").removeClass("hidden").val();
		if (typeBateaux == "deriveurs_multicoques") {
			bateaux = selects.ffv.multicoques+selects.ffv.deriveurs+
				selects.ffv.quillards;
			listeBateauxC = [listeBateaux.ffv.multicoques,
				listeBateaux.ffv.deriveurs, listeBateaux.ffv.quillards];
		}
		else {
			bateaux = selects.ffv.multicoques+selects.ffv.deriveurs+
				selects.ffv.quillards+selects.ffv.habitables;
			listeBateauxC = [listeBateaux.ffv.multicoques,
				listeBateaux.ffv.deriveurs, listeBateaux.ffv.quillards,
				listeBateaux.ffv.habitables];
		}
	}
	// Much faster...
	$(".selectBateau").replaceWith(
		'<div class="selectBateau">'+
			'<div class="recents">Récents</div>'+bateaux+'<div></div>'+
			'<span class="visible" data-match="← rentrer un coefficient" '+
				'data-id="">← Rentrer un coefficient</span>'+
		'</div>'
	);
	recents = [];
	$(".rating").each(function() {
		$(this).trigger("keyup");
		if ($(this).next(".tpBateau").val() === "") { $(this).val(""); }
	});
};

$.isBateauOk = function(code) {
	for (var i=0; i<listeBateauxC.length; i++) {
		if (code in listeBateauxC[i]) { return listeBateauxC[i][code]; }
	}
	return false;
};

$.addRecent = function(id, serie) {
	if (!(id in recents)) {
		recents[id] = true;
		$(".selectBateau .recents").addAfter('<span data-id="'+id+'" '+
			'data-match="'+serie.toLowerCase()+'" class="visible">'+
			serie+'</span>');
	}
};

$.getTpsInputs = function(arr) {
	var h = (arr) ? arr.h : "";
	var min = (arr) ? arr.min : "";
	var s = (arr) ? arr.s : "";
	var pl = (arr) ? arr.pl : "";
	return '<td class="tpsInputs">'+
		'<span class="tps">'+
			'<input type="text" class="h" placeholder="h" tabindex="2" '+
				'value="'+h+'" />'+
			'<span class="sep"> : </span>'+
			'<input type="text" class="min" placeholder="min" tabindex="2" '+
				'value="'+min+'" />'+
			'<span class="sep"> : </span>'+
			'<input type="text" class="s" placeholder="s" tabindex="2" '+
				'value="'+s+'" />'+
		'</span>'+
		'<span class="place">'+
			'<input type="text" class="pl" tabindex="2" value="'+pl+'" />'+
		'</span>'+
	'</td>';
};

$.getEquipageInputs = function(arr) {
	var nom = (arr) ? arr.nom : "";
	var rating = (arr) ? arr.rating : "";
	return '<td class="eqgInputs">'+
		'<input type="text" class="nom" placeholder="Nom de l\'équipage" '+
			'tabindex="1" value="'+nom+'" />'+
		'<span class="rm" title="Supprimer cet équipage" tabindex="0">x</span>'+
		'<br />'+
		'<input type="text" class="rating" placeholder="Code" tabindex="1" '+
			'value="'+rating+'" />'+
		' '+
		'<input type="text" class="tpBateau" '+
			'placeholder="rechercher bateau..." tabindex="-1" />'+
	'</td>';
};

$.getMancheHead = function(no) {
	return '<td class="tpsHeader">'+
		'M'+no+
		'<span class="rm" title="Supprimer cette manche" tabindex="0">x</span>'+
	'</td>';
};

$.getManchePick = function(no) {
	return '<option value="'+no+'" class="manchePick">'+
		'la manche n°'+no+
	'</option>';
};

$.addEquipage = function(nb) {
	var tds = $.getEquipageInputs();
	for (i=0; i<nbManche; i++) { tds += $.getTpsInputs(); }
	var trs = "";
	for (i=0; i<nb; i++) {
		nbEquipage++;
		trs += "<tr>"+tds+"</tr>";
	}
	$("#etape2 table tbody").append(trs);
	$("#showManche").trigger("change");
	FenPrincipale.modif();
};

$.addManche = function(nb) {
	var tds = "";
	var tdsHead = "";
	var opts = "";
	for (i=0; i<nb; i++) {
		nbManche++;
		tdsHead += $.getMancheHead(nbManche);
		tds += $.getTpsInputs();
		opts += $.getManchePick(nbManche);
	}
	$("#etape2 thead tr").append(tdsHead);
	$("#etape2 tbody tr").append(tds);
	$("#etape2 tfoot tr").append(tds);
	$("#showManche").append(opts);
	if ($("#showManche").val() != 'all') {
		$("#showManche").val(nbManche);
	}
	$("#showManche").trigger("change");
};

$.gotoEtape = function(etape) {
	if (etapeActuelle == etape) { return false; }
	$("body").removeClass("etape1 etape2 etape3 etape4")
		.addClass("etape"+etape);
	etapeActuelle = etape;
	$.resizeTable();
	FenPrincipale.set_etape(etape);
};

$.resizeTable = function() {
	if (etapeActuelle == '2') {
		if (nbEquipage > 0 && nbManche > 0 &&
			$("#typeClassement").val() == "temps"
		) {
			var tds = $("etape2 table tbody tr").eq(0).find(".tps")
				.removeClass("multilines");
			if (tds.height() > 30) {
				$(".tps").addClass("multilines");
			}
			else {
				$(".tps").removeClass("multilines");
			}
		}
	}
	else if (etapeActuelle == '4') {
/*		t = etapeActuelle.find("#results");
		if (t.css('display') != 'none') { t.attr('data-width', t.outerWidth()); }
		if (etapeActuelle.innerWidth() < parseInt(t.attr('data-width'))) {
			$("#results").hide();
			$("#resultsSmall").show();
			$("#manches table").addClass('restricted');
		}
		else {
			$("#resultsSmall").hide();
			$("#results").show();
			$("#manches table").removeClass('restricted');
		}*/
	}
};

$.ouvrir = function() {
	var donnees;
	var typeClassement = "temps";

		// Vérification et ouverture du fichier
	ouverture1 = function() {
		FenPrincipale.progression(1);
		var data = FenPrincipale.get_data();
		if (!data || data === "") {
			FenPrincipale.ouvrir_callback(false);
			return false;
		}
		FenPrincipale.progression(2);
		donnees = $.parseJSON(data);
		if (!donnees) {
			FenPrincipale.ouvrir_callback(false);
			return false;
		}
		FenPrincipale.progression(3);
		setTimeout(ouverture2, 0);
	};

		// Remise à zéro
	ouverture2 = function() {
		$("tfoot .tpsInputs").remove();
		$(".tpsHeader").remove();
		$(".manchePick").remove();
		nbEquipage = 0;
		nbManche = 0;
		FenPrincipale.progression(4);
		setTimeout(ouverture3, 0);
	};

		// Récupération des données principales
	ouverture3 = function() {
			// Nom de la régate
		if (!("nomRegate" in donnees)) { donnees["nomRegate"] = ""; }
		$("#nomRegate").val(donnees["nomRegate"]);
			// Type de classement
		if (!("classement" in donnees)) { donnees["classement"] = "temps"; }
		switch (donnees["classement"]) {
			case "scratch":
				$("#typeClassement").val("scratch").trigger("change");
				typeClassement = "scratch";
				break;
			default:
				$("#typeClassement").val("temps").trigger("change");
				typeClassement = "temps";
				if (!("ratings" in donnees)) { donnees["ratings"] = "ffv"; }
				switch (donnees["ratings"]) {
					// #typeBateaux est rempli par `change` de #typeRatings
					case "rya":
						if (!("bateaux" in donnees)) {
							donnees["bateaux"] = "multicoques";
						}
						switch (donnees["bateaux"]) {
							case "deriveurs":
							case "quillards":
							case "deriveurs_multicoques":
								typeBateaux = donnees["bateaux"];
								break;
							default:
								typeBateaux = "multicoques";
						}
						$("#typeRatings").val("rya").trigger("change");
						break;
					case "deri":
						if (!("bateaux" in donnees)) {
							donnees["bateaux"] = "deriveurs_multicoques";
						}
						switch (donnees["bateaux"]) {
							case "tous":
								typeBateaux = "tous";
								break;
							default:
								typeBateaux = "deriveurs_multicoques";
						}
						$("#typeRatings").val("deri").trigger("change");
						break;
					default:
						if (!("bateaux" in donnees)) {
							donnees["bateaux"] = "multicoques";
						}
						switch (donnees["bateaux"]) {
							case "deriveurs":
							case "quillards":
							case "habitables":
							case "deriveurs_habitables":
								typeBateaux = donnees["bateaux"];
								break;
							default:
								typeBateaux = "multicoques";
						}
						$("#typeRatings").val("ffv").trigger("change");
				}
		}
			// Manches retirées
		if (!("manchesRetirees" in donnees)) { donnees["manchesRetirees"] = 0; }
		$("#manchesRetirees").val("0");
		FenPrincipale.progression(10);
		setTimeout(ouverture4, 0);
	};

		// Ajout du bon nombre de manches
	ouverture4 = function() {
		if (!("nbManches" in donnees)) { donnees["nbManches"] = 0; }
		var nb = parseInt(donnees["nbManches"], 10);
		if (isNaN(nb)) { nb = 0; }
		$.addManche(nb);
		FenPrincipale.progression(15);
		setTimeout(ouverture5, 0);
	};

		// Ajout du bon nombre d'équipages
	ouverture5 = function() {
		if (!("nbEquipages" in donnees)) { donnees["nbEquipages"] = 0; }
		nbEquipage = parseInt(donnees["nbEquipages"], 10);
		if (isNaN(nbEquipage)) { nbEquipage = 0; }
		FenPrincipale.progression(20);
		setTimeout(ouverture6, 0);
	};

		// Ajout des infos sur les manches
	ouverture6 = function() {
		$("#etape2 table tfoot .tpsInputs").each(function(index) {
			if (index in donnees["manches"]) {
				if ("h" in donnees["manches"][index]) {
					$(this).find(".h").val(donnees["manches"][index]["h"]);
				}
				if ("min" in donnees["manches"][index]) {
					$(this).find(".min").val(donnees["manches"][index]["min"]);
				}
				if ("s" in donnees["manches"][index]) {
					$(this).find(".s").val(donnees["manches"][index]["s"]);
				}
			}
		});
		FenPrincipale.progression(25);
		setTimeout(ouverture7, 0);
	};

		// Entrée des informations sur les équipages
	var i = 0;
	var trs = [];
	ouverture7 = function() {
		if (i < nbEquipage) {
			if (i in donnees["equipages"]) {
				var tds = "";
				var eqg = donnees["equipages"][i];
				if (!("nom" in eqg)) { eqg["nom"] = ""; }
				if (!("rating" in eqg)) { eqg["rating"] = ""; }
				tds += $.getEquipageInputs(
					{nom: eqg["nom"], rating: eqg["rating"]}
				);
				if ("manches" in eqg) {
					for (var j=0; j<nbManche; j++) {
						if (j in eqg["manches"]) {
							var obj = {h: "", min: "", s: ""};
							if (typeClassement == "temps") {
								if ("h" in eqg["manches"][j]) {
									obj.h = eqg["manches"][j]["h"];
								}
								if ("min" in eqg["manches"][j]) {
									obj.min = eqg["manches"][j]["min"];
								}
								if ("s" in eqg["manches"][j]) {
									obj.s = eqg["manches"][j]["s"];
								}
							}
							else {
								if ("min" in eqg["manches"][j]) {
									obj.pl = eqg["manches"][j]["min"];
								}
								if ("pl" in eqg["manches"][j]) {
									obj.pl = eqg["manches"][j]["pl"];
								}
							}
							tds += $.getTpsInputs(obj);
						}
						else {
							tds += $.getTpsInputs();
						}
					}
				}
				else {
					for (var k=0; k<nbManche; k++) {
						tds += $.getTpsInputs();
					}
				}
				trs += "<tr>"+tds+"</td>";
			}
			i++;
			FenPrincipale.progression(parseInt((i+1)*70/nbEquipage, 10)+25);
			setTimeout(ouverture7, 0);
		}
		else {
			FenPrincipale.progression(98);
			$("#etape2 table tbody").replaceWith("<tbody>"+trs+"</tbody");
			setTimeout(ouverture8, 0);
		}
	};

	ouverture8 = function() {
		FenPrincipale.progression(100);
		FenPrincipale.ouvrir_callback(true);
		$.gotoEtape(1);
	};

	setTimeout(ouverture1, 0);
};





$(".navEtapes li").on("click", function(event) {
	var e = $(event.srcElement);
	$.gotoEtape(e.index()+1);
});

$(window).on("resize", $.resizeTable);
$(window).on("orientationchange", $.resizeTable);




/* ########################## Étape 1 ########################## */

$("#nomRegate").on("keyup", FenPrincipale.modif);
$("#typeClassement").on("change", function() {
	if ($("#typeClassement").val() == 'temps') {
		$("#etapes").removeClass("clmScratch").addClass("clmTemps");
	}
	else {
		$("#etapes").removeClass("clmTemps").addClass("clmScratch");
		$(".multilines").removeClass("multilines");
	}
	FenPrincipale.modif();
});

$.setSelectBateaux();
$("#typeRatings").on("change", function() {
	var val = $("#typeRatings").val();
	if (val == "ffv") {
		if (typeBateaux == "multicoques" || typeBateaux == "deriveurs" ||
			typeBateaux == "quillards" || typeBateaux == "habitables" ||
			typeBateaux == "deriveurs_habitables"
		) {
			$("#typeBateauxFfv").val(typeBateaux);
		}
	}
	else if (val == "rya") {
	if (typeBateaux == "multicoques" || typeBateaux == "deriveurs" ||
			typeBateaux == "quillards" || typeBateaux == "deriveurs_multicoques"
		) {
			$("#typeBateauxRya").val(typeBateaux);
		}
	}
	else {
		if (typeBateaux == "deriveurs_multicoques" || typeBateaux == "tous") {
			$("#typeBateauxDeri").val(typeBateaux);
		}
	}
	$.setSelectBateaux();
	FenPrincipale.modif();
});
$("#typeBateauxFfv").on("change", function() {
	$.setSelectBateaux();
	FenPrincipale.modif();
});
$("#typeBateauxRya").on("change", function() {
	$.setSelectBateaux();
	FenPrincipale.modif();
});
$("#typeBateauxDeri").on("change", function() {
	$.setSelectBateaux();
	FenPrincipale.modif();
});




/* ########################## Étape 2 ########################## */

$("#showManche").on("change", function() {
	var val = $("#showManche").val();
	if (val == "all") {
		$("#etape2 table tr td").removeClass("hidden");
	}
	else {
		val = parseInt(val, 10);
		$("#etape2 table tr td").each(function(index) {
			var nb = index % (nbManche+1);
			if (nb > 0 && nb != val) { $(this).addClass("hidden"); }
			else { $(this).removeClass("hidden"); }
		});
	}
	$.resizeTable();
});

$.addManche(1);
$.addEquipage(1);

$(document).on("mousedown", ".selectBateau", function(event) {
	var e = $(event.srcElement);
	var id = e.data("id");
	if (id !== null) {
		selectTdEquipage.find(".tpBateau").val(e.text());
		selectTdEquipage.find(".rating").val(id);
		if (id === "") {
			selectTdEquipage.find(".rating")[0].focus();
			return true; // .tpBateau va perdre le focus (↑)
		}
		else { $.addRecent(id, e.text()); }
		return false; // .tpBateau va perdre le focus
	}
});

$("#etape2 table").on("keyup", ".h", function(event) {
	var e = $(event.srcElement);
	if (e.val().length >= 2 && !isNaN(e.val())) {
		e.next(".min")[0].focus();
	}
	FenPrincipale.modif();
});
$("#etape2 table").on("keyup", ".min", function(event) {
	var e = $(event.srcElement);
	if (e.val().length >= 2 && !isNaN(e.val())) {
		e.next(".s")[0].focus();
	}
	FenPrincipale.modif();
});
$("#etape2 table").on("keyup", ".s", function(event) {
	var e = $(event.srcElement);
	if (e.val().length >= 2 && !isNaN(e.val())) {
		var tr = e.parent("tr").next("tr");
		var nb = e.parent("td").index();
		if (tr.length > 0) {
			tr.find(".h")[nb-1].focus();
		}
	}
	FenPrincipale.modif();
});
$("#etape2 table").on("keyup", ".pl", FenPrincipale.modif);
$("#etape2 table").on("keyup", ".nom", FenPrincipale.modif);
$("#etape2 table tbody").on("click", ".rm", function(event) {
	var e = $(event.srcElement);
	e.parent("tr").remove();
	nbEquipage--;
	FenPrincipale.modif();
});
$("#etape2 table").on("keyup", ".rating", function(event) {
	var e = $(event.srcElement);
	var tpBateauInput = e.next(".tpBateau");
	var code = e.val();
	var tpBateau = $.isBateauOk(code);
	if (tpBateau !== false) {
		tpBateauInput.val(tpBateau);
	}
	else if (code.match(/[0-9](\.|,)[0-9]*$/) !== null) {
		tpBateauInput.val("← Rentrer un coefficient");
	}
	else {
		tpBateauInput.val("");
	}
	FenPrincipale.modif();
});
$("#etape2 table").on("change", ".rating", function(event) {
	var e = $(event.srcElement);
	e.trigger("keyup");
	var tpBateau = $.isBateauOk(e.val());
	if (tpBateau !== false) {
		$.addRecent(e.val(), tpBateau);
		// Normalement .tpBateau doit déjà avoir la bonne valeur.
		// On ne modifie pas `recents` avec l'évènement keyup,
		// car plusieurs bateaux ont leur code qui commence par les
		// même caractères.
	}
});
$("#etape2 table").on("focus", ".tpBateau", function(event) {
	var e = $(event.srcElement);
	selectTdEquipage = e.parent("td");
	var rating = selectTdEquipage.find(".rating");
	$(".selectBateau").css({
		left: rating.offset().left,
		top: rating.offset().top+rating.offset.height
	});
	oldBateau = e.val();
	oldCode = rating.val();
	rating.val("");
	e.trigger("keyup");
});
$("#etape2 table").on("blur", ".tpBateau", function(event) {
	var e = $(event.srcElement);
	$(".selectBateau")[0].scrollTop = 0;
	$(".selectBateau").css({
		left: -999,
		top: -999
	});
	if (oldBateau == e.val() && selectTdEquipage.find(".rating").val() === "") {
		// Pas de changement : on remet l'ancien code
		selectTdEquipage.find(".rating").val(oldCode);
	}
});
$("#etape2 table").on("keydown", ".tpBateau", function(event) {
	var e = $(event.srcElement);
	btLength = e.val().length;
});
$("#etape2 table").on("keyup", ".tpBateau", function(event) {
	var e = $(event.srcElement);
	var val = e.val().toLowerCase();
	var len = val.length;
	if (len > 1) {
		if (e[0].selectionStart != len) {
			$(".selectBateau span").each(function(index, f) {
				if ($(this).data("match").indexOf(val) === -1) {
					f.className = "hidden";
				}
				else { f.className = "visible"; }
			});
		}
		else {
			if (len > btLength) {
				$(".selectBateau .visible").each(function(index, f) {
					if ($(this).data("match").indexOf(val) === -1) {
						f.className = "hidden";
					}
				});
			}
			else {
				$(".selectBateau .hidden").each(function(index, f) {
					if ($(this).data("match").indexOf(val) !== -1) {
						f.className = "visible";
					}
				});
			}
		}
		}
		else {
			$(".selectBateau .hidden").each(function(index, f) {
				f.className = "visible";
			});
		}
		FenPrincipale.modif();
});
$("#etape2 table thead").on("click", ".rm", function(event) {
	var e = $(event.srcElement);
	var no = e.parent("td").index();
	$(".tpsInputs").each(function(index) {
		if ((index % nbManche) == (no-1)) { $(this).remove(); }
	});
	$("#etape2 thead td").eq(nbManche).remove();
	$("#showManche option").eq(nbManche).remove();
	nbManche--;
	if ($("#showManche").val() == no) {
		$("#showManche").val(nbManche);
	}
	$("#showManche").trigger("change");
	FenPrincipale.modif();
});


$("#addManche").on("click", function() {
	$.addManche(1);
	FenPrincipale.modif();
});
$("#addEquipage").on("click", function() {
	$.addEquipage(1);
	FenPrincipale.modif();
});






/* ########################## Étape 3 ########################## */

$("#manchesRetirees").on("keyup", FenPrincipale.modif);




/* ########################## Étape 4 ########################## */

/*$("#calculer").on("click", FenPrincipale.calculer);*/

});






// FIXME :
//	- la navigation au clavier ne fonctionne pas avec la version de Webkit
//		fournie avec Qt : `onclick` n'est pas déclenché lorsqu'on appuie sur
//		Entrée et qu'un élément est sélectionné, alors que cela devrai être le
//		cas (fonctionne sous Opéra). POURQUOI ?
