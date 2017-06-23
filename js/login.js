function checkInputes(event)
{
 var username = document.getElementById("username");
 var password = document.getElementById("pwd");

 if (username.value.length < 7) {
   alert("Minimum username characters required: 7")
   event.preventDefault();
   return false;
 } else {
   if (password.value.length < 7) {
     alert("Minimum password characters required: 7")
     event.preventDefault();
     return false;
   } else {
     return true;
   }
 }
}

var button = document.getElementById("submitButton");
document.getElementById("username").focus();
button.addEventListener("click", function(event){checkInputes(event)},false);