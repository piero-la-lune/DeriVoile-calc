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


	var version = "v6-5";

	var nbManche = 0;
	var nbEquipage = 0;

	var ratings = []; // Tous les bateaux et leurs coefficients calculés
	var selects = {}; // Code HTML de la liste des bateaux
	var listeBateaux = {}; // Liste des bateaux disponibles
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

$.update_ratings = function() {

	var data = $.parseJSON(FenPrincipale.get_ratings());

	var bateau = {};
	var ffv = null;
	var ffvc = null;
	var rya = null;
	var deri = null;
	var SM = null;

	ratings = [];
	selects = {
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
	};
	listeBateaux = {
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
	};

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
		ratings[bateau.code] = {
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
		ratings[bateau.code] = {
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
		ratings[bateau.code] = {
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
		ratings["#"+bateau.id] = {
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
	$.setSelectBateaux();

};

$.setSelectBateaux = function() {
	var bateaux = [];
	typeRatings = $("#typeRatings").get("value");
	if (typeRatings == "ffv") {
		$("#typeBateauxRya").set("$", "+hidden");
		$("#typeBateauxDeri").set("$", "+hidden");
		typeBateaux = $("#typeBateauxFfv").set("$", "-hidden").get("value");
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
		$("#typeBateauxFfv").set("$", "+hidden");
		$("#typeBateauxDeri").set("$", "+hidden");
		typeBateaux = $("#typeBateauxRya").set("$", "-hidden").get("value");
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
		$("#typeBateauxFfv").set("$", "+hidden");
		$("#typeBateauxRya").set("$", "+hidden");
		typeBateaux = $("#typeBateauxDeri").set("$", "-hidden").get("value");
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
	var nv = $$(".selectBateau").cloneNode(false);
	nv.innerHTML = '<div class="recents">Récents</div>'+bateaux+'<div></div>'+
		'<span class="visible" data-match="← rentrer un coefficient" '+
		'data-id="">← Rentrer un coefficient</span>';
	$$(".selectBateau").parentNode.replaceChild(nv, $$(".selectBateau"));
	recents = [];
	$(".rating").each(function(i) {
		$(i).trigger("keyup");
		if ($(i).trav("nextSibling", ".tpBateau", 1).get("value") === "") {
			$(i).set("value", "");
		}
	});
	$(".selectBateau").on("mousedown", function(event) {
		var e = $(event.srcElement);
		var id = e.get("%id");
		if (id !== null) {
			FenPrincipale.modif();
			selectTdEquipage.select(".tpBateau").set("value", e.text());
			selectTdEquipage.select(".rating").set("value", id);
			if (id === "") {
				selectTdEquipage.select(".rating")[0].focus();
				return false; // .tpBateau va perdre le focus (↑)
			}
			else { $.addRecent(id, e.text()); }
			return true; // .tpBateau va perdre le focus
		}
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
		$(".selectBateau .recents").addAfter(EE("span", {
			"%id": id, "%match": serie.toLowerCase(), "className": "visible"
		}, serie));
	}
};

$.on_create_h = function(e) {
	e.on("|keyup", function() {
		if (e.get("value").length >= 2 && !isNaN(e.get("value"))) {
			$$(e.trav("nextSibling", ".min", 1)).focus();
		}
		FenPrincipale.modif();
	});
};
$.on_create_min = function(e) {
	e.on("|keyup", function() {
		if (e.get("value").length >= 2 && !isNaN(e.get("value"))) {
			$$(e.trav("nextSibling", ".s", 1)).focus();
		}
		FenPrincipale.modif();
	});
};
$.on_create_s = function(e) {
	e.on("|keyup", function() {
		if (e.get("value").length >= 2 && !isNaN(e.get("value"))) {
			var tr = e.trav("parentNode", "tr", 1).trav("nextSibling", "tr", 1);
			var nb = e.trav("parentNode", "td", 1)
				.trav("previousSibling", "td").length;
			if (tr.length > 0) {
				tr.select(".h")[nb-1].focus();
			}
		}
		FenPrincipale.modif();
	});
};
$.on_create_pl = function(e) {
	e.on("|keyup", FenPrincipale.modif);
};
$.on_create_nom = function(e) {
	e.on("|keyup", FenPrincipale.modif);
};
$.on_create_rm_eqg = function(e) {
	e.on("click", function() {
		e.trav("parentNode", "tr", 1).remove();
		nbEquipage--;
		FenPrincipale.modif();
	});
};
$.on_create_rating = function(e) {
	e.on("|keyup", function() {
		var tpBateauInput = e.trav("parentNode", "td", 1)
			.select(".tpBateau");
		var code = e.get("value");
		var tpBateau = $.isBateauOk(code);
		if (tpBateau !== false) {
			tpBateauInput.set("value", tpBateau);
		}
		else if (code.match(/[0-9](\.|,)[0-9]*$/) !== null) {
			tpBateauInput.set("value", "← Rentrer un coefficient");
		}
		else {
			tpBateauInput.set("value", "");
		}
		FenPrincipale.modif();
	});
	e.on("|change", function() {
		e.trigger("keyup");
		var tpBateau = $.isBateauOk(e.get("value"));
		if (tpBateau !== false) {
			$.addRecent(e.get("value"), tpBateau);
			// Normalement .tpBateau doit déjà avoir la bonne valeur
			// On ne modifie pas `recents` avec l'évènement keyup,
			// car plusieurs bateaux ont leur code qui commence par les
			// même caractères
		}
	});
};
$.on_create_tpBateau = function(e) {
	e.on("|focus", function() {
		selectTdEquipage = e.trav("parentNode", "td", 1);
		var code = selectTdEquipage.select(".rating");
		var pos = $$(code).getBoundingClientRect();
		$$(".selectBateau").style.left = pos.left+"px";
		$$(".selectBateau").style.top = (pos.bottom)+"px";
		oldBateau = e.get("value");
		oldCode = code.get("value");
		code.set("value", "");
		e.trigger("keyup");
	});
	e.on("|blur", function() {
		$$(".selectBateau").scrollTop = 0;
		$$(".selectBateau").style.left = "-"+999+"px";
		$$(".selectBateau").style.top = "-"+999+"px";
		if (oldBateau == e.get("value") &&
			selectTdEquipage.select(".rating").get("value") === ""
		) {
			// Pas de changement : on remet l'ancien code
			selectTdEquipage.select(".rating").set("value", oldCode);
		}
	});
	e.on("|keydown", function() {
		btLength = e.get("value").length;
	});
	e.on("|keyup", function() {
		var val = e.get("value").toLowerCase();
		var len = val.length;
		if (len > 1) {
			if ($$(e).selectionStart != len) {
				$(".selectBateau span").each(function(i) {
					if ($(i).get("%match").indexOf(val) === -1) {
						i.className = "hidden";
					}
					else { i.className = "visible"; }
				});
			}
			else {
				if (len > btLength) {
					$(".selectBateau .visible").each(function(i) {
						if ($(i).get("%match").indexOf(val) === -1) {
							i.className = "hidden";
						}
					});
				}
				else {
					$(".selectBateau .hidden").each(function(i) {
						if ($(i).get("%match").indexOf(val) !== -1) {
							i.className = "visible";
						}
					});
				}
			}
		}
		else {
			$(".selectBateau .hidden").each(function(i) {
				i.className = "visible";
			});
		}
	});
};
$.on_create_rm_manche = function(e) {
	e.on("click", function() {
		var no = e.trav("parentNode", "td", 1)
			.trav("previousSibling", "td").length;
		$(".tpsInputs").each(function(i, index) {
			if ((index % nbManche) == (no-1)) { $(i).remove(); }
		});
		$($("#etape2 thead td")[nbManche]).remove();
		$($("#showManche option")[nbManche]).remove();
		nbManche--;
		if ($("#showManche").get("value") == no) {
			$("#showManche").set("value", nbManche);
		}
		$("#showManche").trigger("change");
		FenPrincipale.modif();
	});
};

$.getTpsInputs = function(arr) {
	var h = "";
	var min = "";
	var s = "";
	var pl = "";
	if (arr) {
		h = arr.h;
		min = arr.min;
		s = arr.s;
		pl = arr.pl;
	}
	return EE("td", {"className": "tpsInputs"}, [
		EE("span", {"className": "tps"}, [
			EE("input", {
				"type": "text", "className": "h", "placeholder": "h",
				"@tabindex": 2, "value": h
			}, [], $.on_create_h),
			EE("span", {"className": "sep"}, " : "),
			EE("input", {
				"type": "text", "className": "min", "placeholder": "min",
				"@tabindex": 2, "value": min
			}, [], $.on_create_min),
			EE("span", {"className": "sep"}, " : "),
			EE("input", {
				"type": "text", "className": "s", "placeholder": "s",
				"@tabindex": 2, "value": s
			}, [], $.on_create_s)
		]),
		EE("span", {"className": "place"},
			EE("input", {
				"type": "text", "className": "pl", "@tabindex": 2, "value": pl
			}, [], $.on_create_pl)
		)
	]);
};

$.getEquipageInputs = function(arr) {
	var nom = "";
	var rating = "";
	if (arr) {
		nom = arr.nom;
		rating = arr.rating;
	}
	return EE("td", {"className": "eqgInputs"}, [
		EE("input", {
			"type": "text", "className": "nom",
			"placeholder": "Nom de l'équipage", "@tabindex": 1, "value": nom
		}, [], $.on_create_nom),
		EE("span", {
			"className": "rm", "title": "Supprimer cet équipage", "@tabindex": 0
		}, "x", $.on_create_rm_eqg),
		EE("br"),
		EE("input", {
			"type": "text", "className": "rating",
			"placeholder": "Code", "@tabindex": 1, "value": rating
		}, [], $.on_create_rating),
		" ",
		EE("input", {
			"type": "text", "className": "tpBateau",
			"placeholder": "rechercher bateau...", "@tabindex": -1
		}, [], $.on_create_tpBateau)
	]);
};

$.getMancheHead = function(no) {
	return EE("td", {"className": "tpsHeader"}, [
		"M"+no,
		EE("span", {
			"className": "rm", "title": "Supprimer cette manche", "@tabindex": 0
		}, "x", $.on_create_rm_manche)
	]);
};

$.getManchePick = function(no) {
	return EE("option", {
		"value": no, "className": "manchePick"
	}, "la manche n°"+no);
};

$.addEquipage = function(nb) {
	var tds = [$.getEquipageInputs()];
	for (i=0; i<nbManche; i++) { tds.push($.getTpsInputs()); }
	var trs = [];
	for (i=0; i<nb; i++) {
		nbEquipage++;
		trs.push(EE("tr", tds));
	}
	$("#etape2 table tbody").add(trs);
	$("#showManche").trigger("change");
};

$.addManche = function(nb) {
	var tds = [];
	var tdsHead = [];
	var opts = [];
	for (i=0; i<nb; i++) {
		nbManche++;
		tdsHead.push($.getMancheHead(nbManche));
		tds.push($.getTpsInputs());
		opts.push($.getManchePick(nbManche));
	}
	$("#etape2 thead tr").add(tdsHead);
	$("#etape2 tbody tr").add(tds);
	$($("#etape2 tfoot tr")[0]).add(tds);
	$("#showManche").add(opts);
	if ($("#showManche").get("value") != 'all') {
		$("#showManche").set("value", nbManche);
	}
	$("#showManche").trigger("change");
};

$.gotoEtape = function(etape) {
	if (etapeActuelle == etape) { return false; }
	$("body").set("$", "-etape1 -etape2 -etape3 -etape4 +etape"+etape);
	etapeActuelle = etape;
	if (etapeActuelle == 4) {
		$("#calculer").set("$", "+hidden");
	}
	else {
		$("#calculer").set("$", "-hidden");
	}
	$.resizeTable();
	FenPrincipale.set_etape(etape);
};

$.resizeTable = function() {
	if (etapeActuelle == 2) {
		if (nbEquipage > 0 && nbManche > 0 &&
			$("#typeClassement").get("value") == "temps"
		) {
			$(".tps").set("$", "-multilines");
			if ($(".tps")[0].offsetHeight > 30) {
				$(".tps").set("$", "+multilines");
			}
		}
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
		$("#nomRegate").set("value", donnees["nomRegate"]);
			// Type de classement
		if (!("classement" in donnees)) { donnees["classement"] = "temps"; }
		switch (donnees["classement"]) {
			case "scratch":
				$("#typeClassement").set("value", "scratch").trigger("change");
				typeClassement = "scratch";
				break;
			default:
				$("#typeClassement").set("value", "temps").trigger("change");
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
						$("#typeRatings").set("value", "rya").trigger("change");
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
						$("#typeRatings").set("value", "deri").trigger("change");
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
						$("#typeRatings").set("value", "ffv").trigger("change");
				}
		}
			// Manches retirées
		if (!("manchesRetirees" in donnees)) { donnees["manchesRetirees"] = 0; }
		$("#manchesRetirees").set("value", donnees["manchesRetirees"]);
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
		$("#etape2 table tfoot .tpsInputs").each(function(td, index) {
			if (index in donnees["manches"]) {
				if ("h" in donnees["manches"][index]) {
					$(td).select(".h")
						.set("value", donnees["manches"][index]["h"]);
				}
				if ("min" in donnees["manches"][index]) {
					$(td).select(".min")
						.set("value", donnees["manches"][index]["min"]);
				}
				if ("s" in donnees["manches"][index]) {
					$(td).select(".s")
						.set("value", donnees["manches"][index]["s"]);
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
				var tds = [];
				var eqg = donnees["equipages"][i];
				if (!("nom" in eqg)) { eqg["nom"] = ""; }
				if (!("rating" in eqg)) { eqg["rating"] = ""; }
				tds.push($.getEquipageInputs(
					{nom: eqg["nom"], rating: eqg["rating"]}
				));
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
							tds.push($.getTpsInputs(obj));
						}
						else {
							tds.push($.getTpsInputs());
						}
					}
				}
				else {
					for (var k=0; k<nbManche; k++) {
						tds.push($.getTpsInputs());
					}
				}
				trs.push(EE("tr", tds));
			}
			i++;
			FenPrincipale.progression(parseInt((i+1)*70/nbEquipage, 10)+25);
			setTimeout(ouverture7, 0);
		}
		else {
			FenPrincipale.progression(98);
			$("#etape2 table tbody").fill(trs);
			$(".rating").trigger("change");
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

$.enregistrer = function() {
	var donnees = {
		version: version,
		nomRegate: $("#nomRegate").get("value"),
		nbEquipages: nbEquipage,
		nbManches: nbManche,
		classement: $("#typeClassement").get("value"),
		rating: typeRatings,
		bateaux: typeBateaux,
		manchesRetirees: $("#manchesRetirees").get("value"),
		manches: [],
		equipages: []
	};
	for (var i=0; i<nbManche; i++) {
		donnees.manches.push({
			h: $($("#etape2 table tfoot .h")[i]).get("value"),
			min: $($("#etape2 table tfoot .min")[i]).get("value"),
			s: $($("#etape2 table tfoot .s")[i]).get("value")
		});
	}
	$("#etape2 table tbody tr").each(function(i) {
		var manches = [];
		for (var j=0; j<nbManche; j++) {
			manches.push({
				h: $($(i).select(".h")[j]).get("value"),
				min: $($(i).select(".min")[j]).get("value"),
				s: $($(i).select(".s")[j]).get("value"),
				pl: $($(i).select(".pl")[j]).get("value")
			});
		}
		donnees.equipages.push({
			nom: $(i).select(".nom").get("value"),
			bateau: $(i).select(".tpBateau").get("value"),
			rating: $(i).select(".rating").get("value"),
			manches: manches
		});
	});
	FenPrincipale.enregistrer_callback($.toJSON(donnees));
};

$.html = function() {
	FenPrincipale.html_callback($("#results .visible")[0].innerHTML);
};
$.pdf = function() {
	FenPrincipale.pdf_callback($("#results .visible")[0].innerHTML);
};




$(".navEtapes li").on("click", function(i, index) {
	$.gotoEtape(index+1);
});

window.onresize = $.resizeTable;
window.onorientationchange = $.resizeTable;

$.update_ratings();




/* ########################## Étape 1 ########################## */

$("#nomRegate").on("keyup", FenPrincipale.modif);
$("#typeClassement").on("change", function() {
	if ($("#typeClassement").get("value") == 'temps') {
		$("#etapes").set("$", "-clmScratch +clmTemps");
	}
	else {
		$("#etapes").set("$", "-clmTemps +clmScratch");
		$(".multilines").set("$", "-multilines");
	}
	FenPrincipale.modif();
});

$("#typeRatings").on("change", function() {
	var val = $("#typeRatings").get("value");
	if (val == "ffv") {
		if (typeBateaux == "multicoques" || typeBateaux == "deriveurs" ||
			typeBateaux == "quillards" || typeBateaux == "habitables" ||
			typeBateaux == "deriveurs_habitables"
		) {
			$("#typeBateauxFfv").set("value", typeBateaux);
		}
	}
	else if (val == "rya") {
	if (typeBateaux == "multicoques" || typeBateaux == "deriveurs" ||
			typeBateaux == "quillards" || typeBateaux == "deriveurs_multicoques"
		) {
			$("#typeBateauxRya").set("value", typeBateaux);
		}
	}
	else {
		if (typeBateaux == "deriveurs_multicoques" || typeBateaux == "tous") {
			$("#typeBateauxDeri").set("value", typeBateaux);
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

$.addManche(1);
$.addEquipage(1);
$("#addManche").on("click", function() {
	$.addManche(1);
	FenPrincipale.modif();
});
$("#addEquipage").on("click", function() {
	$.addEquipage(1);
	FenPrincipale.modif();
});

$("#showManche").on("change", function() {
	var val = $("#showManche").get("value");
	if (val == "all") {
		$("#etape2 table tr td").set("$", "-hidden");
	}
	else {
		val = parseInt(val, 10);
		$("#etape2 table tr td").each(function(e, index) {
			var nb = index % (nbManche+1);
			if (nb > 0 && nb != val) { $(e).set("$", "+hidden"); }
			else { $(e).set("$", "-hidden"); }
		});
	}
	$.resizeTable();
});




/* ########################## Étape 3 ########################## */

$("#manchesRetirees").on("|keyup", FenPrincipale.modif);




/* ########################## Étape 4 ########################## */

$("#calculer").on("click", FenPrincipale.calculer);
$("#showResult").on("change", function() {
	$(".clm").set("$", "+hidden -visible");
	$("#"+$("#showResult").get("value")).set("$", "-hidden +visible");
});

});






// FIXME :
//	- la navigation au clavier ne fonctionne pas avec la version de Webkit
//		fournie avec Qt : `onclick` n'est pas déclenché lorsqu'on appuie sur
//		Entrée et qu'un élément est sélectionné, alors que cela devrai être le
//		cas (fonctionne sous Opéra). POURQUOI ?
