// obtenir les valeur de temperature apres chaque deux seconde 
setInterval(function ()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("temperature").innerHTML = this.responseText;
        
        }
    };

    xhttp.open("GET", "/lireTemperature", true);
   
    xhttp.send();
}, 2000);
//lecture de la presion en utilisant ajax apres chaque deux seconde
setInterval(function ()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            
            document.getElementById("presure").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/LirePresure", true);;
   
    xhttp.send();
}, 2000);
// lecture de l'altitude apres chaque deux seconde 
setInterval(function ()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
            document.getElementById("altitude").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/lireAltitude", true);
    xhttp.send();
}, 2000);





// Function to get current readings on the webpage when it loads for the first time
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

