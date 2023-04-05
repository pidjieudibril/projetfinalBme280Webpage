
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

