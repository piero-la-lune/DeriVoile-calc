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


	var version = "v6-3";

	var ratings = {'deriveurs': {}, 'multicoques': {}};
	var oldratings = {'deriveurs': {}, 'multicoques': {}};
	var recents = new Object();
	var selects = new Object();
	var select = 'deriveurs';
	var placeholder = 'min';

	var nbManche = 1;
	var nbEquipage = 0;

	var etapeActuelle;

(function($) {

	$.exporterHTML = function() {
var style = "\
body {\
	font-family: 'OpenSans', 'Trebuchet MS', Tahoma, Helvetica, arial, Sans-Serif;\
	text-align: center;\
}\
h1 { text-align: center; }\
#contenu { padding: 0 40px; }\
a {\
	color: #005FA3;\
	text-decoration: none;\
}\
a:hover {\
	text-decoration: underline;\
}\
table {\
	margin: auto;\
	border: 1px solid #C9D7F1;\
	border-collapse: collapse;\
	color: #005FA3;\
}\
thead td {\
	background-color: #D9E4F8;\
	font-weight: bold;\
}\
td {\
	border: 1px solid #C9D7F1;\
	text-align: center;\
	padding: 5px; \
}";
var clone = $("#etape4").clone();
clone.find("#resultsSmall").remove();
clone.find("#results").show();
		FenPrincipale.exporterHTML('<html><head><meta charset="utf-8" /><style>'+style+'</style></head><body>'+clone.html()+'</body></html>');
	}

	$.enregistrer = function() {
		var donnees = new Object();
		donnees['nomRegate'] = $("#nomRegate").val();
		donnees['nbEquipages'] = nbEquipage;
		donnees['nbManches'] = nbManche;
		donnees['bateaux'] = select;
		donnees['classement'] = $("#typeClassement").val();
		donnees['manchesRetirees'] = $("#manchesRetirees").val();
		donnees['manches'] = new Object();
		for (i=0; i<nbManche; i++) {
			donnees['manches'][i] = {
				h: $("#etape2 table tfoot .h").eq(i).val(),
				min: $("#etape2 table tfoot .min").eq(i).val(),
				s: $("#etape2 table tfoot .s").eq(i).val()
			};
		}
		donnees['equipages'] = new Object();
		$("#etape2 table tbody tr").each(function(i) {
			donnees['equipages'][i] = new Object();
			donnees['equipages'][i]['nom'] = $(this).find(".nom").val();
			donnees['equipages'][i]['bateau'] = $(this).find("select").val();
			donnees['equipages'][i]['rating'] = $(this).find(".rating").val();
			donnees['equipages'][i]['manches'] = new Object();
			for (j=0; j<nbManche; j++) {
				donnees['equipages'][i]['manches'][j] = {
					h: $(this).find(".h").eq(j).val(),
					min: $(this).find(".min").eq(j).val(),
					s: $(this).find(".s").eq(j).val()
				};
			}
		});
		var texte = JSON.stringify(donnees);
		FenPrincipale.enregistrer(texte);
		return texte;
	};

	$.ouvrir = function(texte) {
		var donnees;
		var nb = 0;
		var tr;

			// Vérification et ouverture du fichier
			ouverture1 = function(texte) {
		FenPrincipale.progression(1);
		if (!texte || texte == '') { FenPrincipale.ouvrir('Le fichier est vide.'); return false; }
		FenPrincipale.progression(2);
		donnees = JSON.parse(texte);
		if (!donnees) {
			FenPrincipale.ouvrir('Erreur lors de la lecture du classement : vérifiez que le fichier est valide...');
			return false;
		}
		else if ('erreur' in donnees) {
			FenPrincipale.ouvrir(donnees['erreur']);
			return false;
		}
		FenPrincipale.progression(3);
		setTimeout(function() { ouverture2(); }, 0);
			};

			// Remise à zéro
			ouverture2 = function() {
		$("#etape2 table tbody").html(' ');
		$("#etape2 table thead").html('<tr><td>Équipage</td></tr>');
		$("#etape2 table tfoot td:gt(0)").remove();
		$("#showManche option:gt(0)").remove();
			// Récupération des données principales
		if ('nomRegate' in donnees) {
			$("#nomRegate").val(donnees['nomRegate']);
		}
		else {
			$("#nomRegate").val('');
		}
		if ('bateaux' in donnees && donnees['bateaux'] in ratings) {
			$("#typeBateaux").val(donnees['bateaux']).change();
		}
		else {
			$("#typeBateaux").val('deriveurs');
		}
		if ('classement' in donnees
			&& (donnees['classement'] == 'temps' || donnees['classement'] == 'scratch'))
		{
			$("#typeClassement").val(donnees['classement']).change();
		}
		else {
			$("#typeClassement").val('temps');
		}
		if ('manchesRetirees' in donnees) {
			$("#manchesRetirees").val(donnees['manchesRetirees']);
		}
		else {
			$("#manchesRetirees").val('0');
		}
		FenPrincipale.progression(4);
		nbManche = 0;
		nbEquipage = 0;
		if ('nbManches' in donnees) {
			// car nbManche est augmenté dans $.addManche
			nb = parseInt(donnees['nbManches']);
			if (isNaN(nb)) { nb = 0; }
		}
		if ('nbEquipages' in donnees) {
			nbEquipage =  parseInt(donnees['nbEquipages']);
			if (isNaN(nbEquipage)) { nbEquipage = 0; }
		}
		setTimeout(function() { ouverture3(0); }, 0);
			};

			// Ajout du bon nombre de manches
			ouverture3 = function(i) {
		$.addManche();
		if (i in donnees['manches']) {
			if ('h' in donnees['manches'][i]) {
				$("#etape2 table tfoot .h").eq(i).val(donnees['manches'][i]['h']);
			}
			if ('min' in donnees['manches'][i]) {
				$("#etape2 table tfoot .min").eq(i).val(donnees['manches'][i]['min']);
			}
			if ('s' in donnees['manches'][i]) {
				$("#etape2 table tfoot .s").eq(i).val(donnees['manches'][i]['s']);
			}
		}
		FenPrincipale.progression(parseInt((i+1)*16/nb)+4);
		if (++i < nb) { setTimeout(function() { ouverture3(i); }, 0); }
		else { setTimeout(function() { ouverture4(); }, 0); }
			};

			// Ajout du bon nombre d'équipages
			ouverture4 = function() {
		var tab = ''; var txt = $.getEquipage();
		for (var i=0; i<nbEquipage; i++) { tab += txt; }
		$("#etape2 table tbody").html(tab);
		tr = $("#etape2 table tbody tr:eq(0)");
		setTimeout(function() { ouverture5(0); }, 0);
			};

			// Entrée des informations sur les équipages
			ouverture5 = function(i) {
		if (i in donnees['equipages']) {
			var e = donnees['equipages'][i];
			if ('nom' in e) {
				tr.find(".nom").val(e['nom']);
			}
			if ('bateau' in e) {
				if (e['bateau'] in ratings[select]) {
					tr.find("select").val(e['bateau']).change();
				}
				else if (e['bateau'] == 'autre') {
					tr.find("select").val('autre');
					if ('rating' in e) {
						tr.find(".rating").val(e['rating']);
					}
				}
				else if (e['bateau'] in oldratings[select]) {
					// Parce qu'avant on n'utilisait pas les abréviations pour les multicoques
					tr.find("select").val(oldratings[select][e['bateau']]).change();
				}
			}
			for (var j=0; j<nbManche; j++) {
				if (j in e['manches']) {
					if ('h' in e['manches'][j]) {
						tr.find(".h").eq(j).val(e['manches'][j]['h']);
					}
					if ('min' in e['manches'][j]) {
						tr.find(".min").eq(j).val(e['manches'][j]['min']);
					}
					if ('s' in e['manches'][j]) {
						tr.find(".s").eq(j).val(e['manches'][j]['s']);
					}
				}
			}
		}
		tr = tr.next();
		FenPrincipale.progression(parseInt((i+1)*78/nbEquipage)+20);
		if (++i < nbEquipage) { setTimeout(function() { ouverture5(i); }, 0); }
		else { setTimeout(function() { ouverture6(); }, 0); }
			};

			ouverture6 = function() {
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); } // Si le classement est au scratch
		FenPrincipale.progression(100);
		FenPrincipale.ouvrir("");
		$("#etape0 input").click();
		return true;
			};
		
		setTimeout(function() { ouverture1(texte); }, 0);
	};

	$.getEquipage = function() {
		var ligne = '<tr><td><input type="text" value="" class="nom" placeholder="Nom de l\'équipage" /><a href="#" title="Supprimer cet équipage" class="removeEquipage">w</a><span class="tpBateau">'+$.getSelect()+'<input type="text" placeholder="Code" class="rating" /></span></td>';
		for (i=0; i<nbManche; i++) { ligne += $.getManche(placeholder); }
		ligne += '</tr>';
		return ligne;
	};

	$.getManche = function(txt) {
		return '<td class="tps"><input type="text" value="" class="h" size="2" placeholder="h" pattern="[0-9]*" /><br /><input type="text" value="" class="min" size="3" placeholder="'+txt+'" pattern="[0-9]*" /><br /><input type="text" value="" class="s" size="2" placeholder="s" pattern="[0-9]*" /></td>';
	};

	$.addEquipage = function() {
		nbEquipage++;
		$("#etape2 table tbody").append($.getEquipage);
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); }
		if ($("#showManche").val() != 'all') { $("#showManche").change(); }
	};

	$.removeEquipage = function(num) {
		nbEquipage--;
		$("#etape2 tbody tr").eq(num).remove();
	};

	$.addManche = function() {
		nbManche++;
		$("#etape2 tbody tr").each(function() {
			$(this).append($.getManche(placeholder));
		});
		$("#etape2 thead tr").append('<td>M'+nbManche+'<a href="#" title="Supprimer cette manche" class="removeManche">w</a></td>');
		$("#etape2 tfoot tr").append($.getManche('min'));
		if ($("#typeClassement").val() != 'temps') { $("#etape1 #typeClassement").change(); }
		$("#showManche").append('<option value="'+nbManche+'">la manche n°'+nbManche+'</option>');
		$(window).resize();
		if ($("#showManche").val() != 'all') { $("#showManche").val(nbManche).change(); }
	};
	
	$.removeManche = function(num) {
		nbManche--;
		$("#etape2 thead td").eq(num).remove();
		$("#etape2 tbody tr").each(function() {
			$(this).find("td").eq(num).remove();
		});
		$("#etape2 tfoot td").eq(num-1).remove();
		var i = 1;
		$("#etape2 thead td:gt(0)").each(function() {
			$(this).html('M'+i+'<a href="#" title="Supprimer cette manche" class="removeManche">w</a>');
			i++;
		});
		if ($("#showManche").val() != 'all') { $("#showManche").val('1').change(); }
		$("#showManche option:last").remove();
	};

	$.gotoEtape = function(etape) {
		if (etapeActuelle.attr('id') == 'etape'+etape) { return false; }
		var a = $(".navEtapes a:eq("+(parseInt(etape)-1)+")");
		$("#etapes").css('height', $("#etapes").height());
		etapeActuelle.fadeOut('fast', function() {
			etapeActuelle = $("#etape"+etape);
			$("#etapes").animate({height: $("#etape"+etape).height()});
			etapeActuelle.fadeIn(function() {
				$("#etapes").css('height', 'auto');
				$.resizeTable();
			});
		});
		$("#etape2 ol").hide();
		$(".navEtapes a").removeClass('selected');
		a.addClass('selected');
		FenPrincipale.setEtape(etape);
	};

	$.getSelect = function() {
		var selection = '<select><option value="default" selected="selected">Choisir (ou tapper code →)</option><option value="autre">Rentrer un coefficient :</option>';
		if ('default' in recents) { rct = recents['default']; } else { rct = ''; }
		selection += '<optgroup label="Récents :">'+rct+'</optgroup>';
		selection += '<optgroup label="Tous :">'+selects[select]+'</optgroup>';
		return selection;
	};

	$.resizeTable = function() {
		if (etapeActuelle.attr('id') == 'etape4') {
			t = etapeActuelle.find("#results");
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
			}
		}
		else if (etapeActuelle.attr('id') == 'etape2') {
			t = etapeActuelle.find("table");
			if ($("#showManche").val() == 'all' && etapeActuelle.innerWidth() < t.outerWidth()) {
				$("#showManche").val(nbManche).change();
			}
		}
	};

})(jQuery)

$(document).ready(function() {

	etapeActuelle = $("#etape0");

	$("#ratingsDeriveurs tbody tr").each(function() {
		var code = $(this).children().eq(0).text();
		var nom = $(this).children().eq(1).text();
		var coef = $(this).children().eq(2).text();
		selects['deriveurs'] += '<option value="'+code+'">'+nom+'</option>';
		ratings['deriveurs'][code] = coef;
		oldratings['deriveurs'][nom] = code;
	});
	$("#ratingsMulticoques tbody tr").each(function() {
		var code = $(this).children().eq(0).text();
		var nom = $(this).children().eq(1).text();
		var coef = $(this).children().eq(2).text();
		selects['multicoques'] += '<option value="'+code+'">'+nom+'</option>';
		ratings['multicoques'][code] = coef;
		oldratings['multicoques'][nom] = code;
	});
	$.addEquipage();


/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ NAVIGATION -------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(".navEtapes a:eq(0)").click(function() { $.gotoEtape('1'); return false; });
$(".navEtapes a:eq(1)").click(function() { $.gotoEtape('2'); return false; });
$(".navEtapes a:eq(2)").click(function() { $.gotoEtape('3'); return false; });

$("#etape0 input").click(function() {
	$(".navEtapes").slideDown();
	$(".btCalculer").slideDown();
	$.gotoEtape('1'); return false;
});



/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ FORMULAIRE -------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$("#formulaire select").live('change', function() { FenPrincipale.modif(); });
$("#formulaire input").live('keyup', function() { FenPrincipale.modif(); });

$("#addEquipage").click(function() {
	$.addEquipage();
	return false;
});

$("#addManche").click(function() {
	$.addManche();
	return false;
});

$(".removeEquipage").live('click', function() {
	if (nbEquipage < 2) { return false; }
	$.removeEquipage($(this).parent().parent().index());
	return false;
});

$(".removeManche").live('click', function() {
	if (nbManche < 2) { return false; }
	$.removeManche($(this).parent().index());
	return false;
});

$("#calculer").click(function() {
	FenPrincipale.calculer();
});

$("tbody .tps input").live('keyup', function() {
	if ($(this).val().length >= 2 && !isNaN($(this).val())) {
		if ($(this).index() < 4) {
			$(this).next().next().focus();
		}
		else if ($(this).closest("tr").index() < nbEquipage-1) {
			$(this).closest("tr").next().find("td").eq($(this).parent().index()).find("input").eq(0).focus();
		}
		else {
			$(this).closest("tbody").find("tr").eq(0).find("td").eq($(this).parent().index()+1).find("input").eq(0).focus();
		}
	}
});
$("tfoot .tps input").live('keyup', function() {
	if ($(this).val().length >= 2 && !isNaN($(this).val())) {
		if ($(this).index() < 4) {
			$(this).next().next().focus();
		}
		else {
			$(this).closest("tr").find("td").eq($(this).parent().index()+1).find("input").eq(0).focus();
		}
	}
});

$("#etape2 table select").live('change', function() {
	if (!($(this).val() in recents) && $(this).val() != 'default' && $(this).val() != 'autre') {
		var txt = '<option value="'+$(this).val()+'">'+$(this).find(':selected').text()+'</option>';
		$("#etape2 select").each(function() { $(this).find("optgroup:eq(0)").prepend(txt); });
		$(this).find("optgroup:eq(0) option:eq(0)").attr("selected","selected");
		recents[$(this).val()] = true;
		recents['default'] += txt;
	}
	if ($(this).val() != 'default' && $(this).val() != 'autre') {
		$(this).closest("td").find(".rating").val($(this).val());
	}
});
$(".rating").live('keyup', function() {
	var sel = $(this).closest("td").find("select");
	var val = $(this).val();
	if (val in ratings[select]) {
		sel.val(val).change(); // pour mettre à jours les bateaux récents.
	}
	else if (val.match(/[0-9](\.|,)[0-9]*$/) != null) {
		sel.val('autre');
	}
	else {
		sel.val('default');
	}
});

$("#etape2 a:eq(0)").click(function() {
	var val = $("#typeClassement").val();
	if (val == 'temps') {
		$("#etape2 ol:eq(0)").toggle();
	}
	else {
		$("#etape2 ol:eq(1)").toggle();
	}
	return false
});
$("#typeClassement").change(function() {
	var val = $(this).val();
	if (val == 'temps') {
		$("#pTypeBateau").show();
		$("#etape2 table tfoot").show();
		$("#etape2 table .tpBateau").show();
		$("#etape2 table .tps").find("*").show();
		placeholder = 'min';
		$("#etape2 table .tps").find(".min").attr('placeholder', placeholder);
	}
	else {
		$("#pTypeBateau").hide();
		$("#etape2 table tfoot").hide();
		$("#etape2 table .tpBateau").hide();
		$("#etape2 table .tps").find("*:not(.min)").hide();
		placeholder = 'n°';
		$("#etape2 table .tps").find(".min").attr('placeholder', placeholder);
	}
	$("#showManche").val('all').change();
});
$("#typeBateaux").change(function() {
	if ($(this).val() in ratings) {
		select = $(this).val();
		recents = new Array();
		$("#etape2 table select").remove();
		$("#etape2 table tbody tr").each(function() {
			$(this).find(".tpBateau").prepend($.getSelect());
			$(this).find(".rating").val('');
		});
	}
});
$("#showManche").change(function() {
	if ($(this).val() == 'all') {
		$("#etape2 table .tps").show();
		$("#etape2 table thead td:gt(0)").show();
	}
	else {
		var nb = parseInt($(this).val());
		$("#etape2 table tr .tps").hide();
		$("#etape2 table thead td:gt(0)").hide();
		$("#etape2 table tbody tr").each(function() { $(this).find(".tps").eq(nb-1).show(); });
		$("#etape2 table thead td").eq(nb).show();
		$("#etape2 table tfoot .tps").eq(nb-1).show()
	}
});



/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------- POPUP ---------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(".question").click(function() {
	$(this).next(".reponse").toggle();
});



/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------- TABLES --------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

$(window).bind("orientationchange resize", function(){
	$.resizeTable();
});



});