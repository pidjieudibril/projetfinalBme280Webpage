/*
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


setInterval(function ()
{
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function()
    {
        if(this.readyState == 4 && this.status == 200)
        {
         
            document.getElementById("humidity").innerHTML = this.responseText;
           
        }
    };

   
    xhttp.open("GET", "/lireHumidity", true);
    
    xhttp.send();
}, 2000);
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
    xhttp.open("GET", "/lirePresure", true);;
   
    xhttp.send();
}, 2000);

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

*/



// Function to get current readings on the webpage when it loads for the first time

function getReadings() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      document.getElementById("temperature").innerHTML = myObj.temperature;
      document.getElementById("humidity").innerHTML = myObj.humidity;
      document.getElementById("presure").innerHTML = myObj.pressure;
      document.getElementById("altitude").innerHTML = myObj.altitude;
      
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}
setInterval(getReadings,2000)

