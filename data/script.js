/*******************  obtenir les valeur de temperature apres chaque deux seconde ****************************/

//fonction qui permet de faire afficher les valeur de temperature a chaque deux seconde 
setInterval(function ()
{
    // ici quant la fonction setInterval est appele on va creer une variable xhttp qui est une requette http 
    var xhttp = new XMLHttpRequest();
     //quant la requette  est egale a 4 (prete) et le status 200 pour dire correct 
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
             //on remplace le texte qui a pour identifiant la valeur de la temperature par la reponse de esp32 (les donnees du capteur)
            document.getElementById("temperature").innerHTML = this.responseText;
        
        }
    };
//on ouvre le lien lireTemperature en utilisant la methode GET et true pour dire que la requette est asynchrone
    xhttp.open("GET", "/lireTemperature", true);
   //en envoie vers la page demande
    xhttp.send();
}, 2000);

/***********************lecture de la presion en utilisant ajax apres chaque deux seconde***********************/
setInterval(function ()
{
    // ici quant la fonction setInterval est appele on va creer une variable xhttp qui est une requette http
    var xhttp = new XMLHttpRequest();
    //quant la requette  est egale a 4 (prete) et le status 200 pour dire correct 
    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            //on remplace le texte qui a identifiant valeur de la pression par la reponse de esp32
            document.getElementById("presure").innerHTML = this.responseText;
        }
    };
    //on ouvre le lien lirePresure en utilisant la methode GET et true pour dire que la requette est asynchrone
    xhttp.open("GET", "/LirePresure", true);;
   
    xhttp.send();
}, 2000);
/************************************ lecture de l'altitude apres chaque deux seconde *****************************/
setInterval(function ()
{
    // ici quant la fonction setInterval est appele on va creer une variable xhttp qui est une requette http
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        //quant la requette  est egale a 4 (prete) et le status 200 pour dire correct 
        if(this.readyState == 4 && this.status == 200)
        {   //on remplace le texte qui a identifiant valeur de la altitude par la reponse de esp32
            document.getElementById("altitude").innerHTML = this.responseText;
        }
    };
    //on ouvre le lien lirePresure en utilisant la methode GET et true pour dire que la requette est asynchrone
    xhttp.open("GET", "/lireAltitude", true);
    xhttp.send();
}, 2000);


/*********************************Examinons maintenant la fonction getReadings. Il envoie une requête GET au
 *  serveur sur l’URL /readings et gère la réponse, une chaîne JSON contenant les lectures du capteur. 
 * Il place également les valeurs de température, d’humidité et de pression sur les éléments HTML avec les identifiants correspondants.
 * les donnees sont ensuite imprime dans un navigateur apres chaque deux seconde  */
function getReadings(){
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var myObj = JSON.parse(this.responseText);
        console.log(myObj);
        var temp = myObj.temperature;
        var pres = myObj.presure;
        var alt = myObj.altitude;
        temperature.value = temp;
        presure.value = pres;
        altitude.value = alt;
        
      }
    }; 
    xhr.open("GET", "/readings", true);
    xhr.send();
  }
  
setInterval(getReadings,2000)

