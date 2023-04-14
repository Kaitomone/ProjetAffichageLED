/**
    Script pour le service WEB
    @file script.js
    @author Enzo Richard
    @version 1.0 14/04/23 
*/

// function setAdresseIP()
// {
//     var adresseIP = "172.16.20.10";
//     var xhttp = new XMLHttpRequest();
//     adresseIPNew = document.getElementById("adresseIP").innerHTML;
//     if(this.status == 200) {
//         document.getElementById("adresseIP").innerHTML = this.responseText;
//         console.log(this.responseText);
//     };
//     xhttp.open("GET", "setAdresseIP?adresseIP=" + adresseIPNew, true);
//     xhttp.send();
// }

function getAdresseIP(){
    var xhttp = new XMLHttpRequest();
    adresseIPNew = document.getElementById("adresseIP").innerHTML;
    xhttp.onreadystatechange = function () {
        if(this.status == 200) {
            document.getElementById("adresseIP").innerHTML = this.responseText;
            console.log(this.responseText);
        }
        }   ;
    xhttp.open("GET", "adresseIP?adresseIP=" + adresseIPNew, true);
    xhttp.send();
}