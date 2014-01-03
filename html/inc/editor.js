$(function() {

var data = $.parseJSON(FenRatings.get_ratings());

var bateau;
var rating;
var coef;
var rya;

var trs = [];
for (i=0; i<data.multicoques.length; i++) {
	bateau = data.multicoques[i];
	trs.push(EE("tr", [
		EE("td", {"@contenteditable": true}, bateau.code),
		EE("td", {"@contenteditable": true}, bateau.serie),
		EE("td", {"@contenteditable": true}, bateau.cat),
		EE("td", {"@contenteditable": true}, bateau.rating),
		EE("td", {"@contenteditable": true}, bateau.coef),
		EE("td", {"@contenteditable": true}, bateau.rya)
	]));
}
$("#multicoques tbody").fill(trs);

trs = [];
for (i=0; i<data.deriveurs.length; i++) {
	bateau = data.deriveurs[i];
	trs.push(EE("tr", [
		EE("td", {"@contenteditable": true}, bateau.code),
		EE("td", {"@contenteditable": true}, bateau.serie),
		EE("td", {"@contenteditable": true}, bateau.cat),
		EE("td", {"@contenteditable": true}, bateau.rating),
		EE("td", {"@contenteditable": true}, bateau.coef),
		EE("td", {"@contenteditable": true}, bateau.rya)
	]));
}
$("#deriveurs tbody").fill(trs);
$("#deriveurs").set("$", "+hidden");

trs = [];
for (i=0; i<data.quillards.length; i++) {
	bateau = data.quillards[i];
	trs.push(EE("tr", [
		EE("td", {"@contenteditable": true}, bateau.code),
		EE("td", {"@contenteditable": true}, bateau.serie),
		EE("td", {"@contenteditable": true}, bateau.rating),
		EE("td", {"@contenteditable": true}, bateau.coef),
		EE("td", {"@contenteditable": true}, bateau.rya)
	]));
}
$("#quillards tbody").fill(trs);
$("#quillards").set("$", "+hidden");

trs = [];
for (i=0; i<data.habitables.length; i++) {
	bateau = data.habitables[i];
	trs.push(EE("tr", [
		EE("td", {"@contenteditable": true}, "#"+bateau.id),
		EE("td", {"@contenteditable": true}, bateau.serie),
		EE("td", {"@contenteditable": true}, bateau.cat),
		EE("td", {"@contenteditable": true}, bateau.groupe),
		EE("td", {"@contenteditable": true}, bateau.cvl)
	]));
}
$("#habitables tbody").fill(trs);
$("#habitables").set("$", "+hidden");

});