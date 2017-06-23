function textCounter()
{
 var textarea = document.getElementById('textareaid');
 var textareacount = document.getElementById('textareacount');

 textareacount.textContent = (textarea.maxLength - textarea.textLength);
}
function validateForm(event) {
  if (document.forms["myForm"]["firstname"].value == "") {
      alert("Firstname must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["lastname"].value == "") {
      alert("Lastname must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["email"].value == "") {
      alert("Email must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["pwd"].value == "") {
      alert("Password must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["web"].value == "") {
      alert("Website must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["date"].value == "") {
      alert("Birthdate must be filled out");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["optradio"].value == "") {
      alert("What radio button do you like the most?");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"]["responsibility"].value == "") {
      alert("Who is responsible for you?");
      event.preventDefault();
      return false;
  }

  if (document.forms["myForm"].refered[1].checked == true) {
    return true;
  }else if (document.forms["myForm"].refered[2].checked) {
    return true;
  }else if (document.forms["myForm"].refered[3].checked) {
    return true;
  }else if (document.forms["myForm"].refered[4].checked) {
    return true;
  }else if (document.forms["myForm"].refered[5].checked) {
    return true;
  }else if (document.forms["myForm"].refered[0].checked) {
    return true;
  }else {
    alert("What checkboxes do you like the most?");
    event.preventDefault();
    return false;
  }
  return true;
}
var button = document.getElementById("submitButton");
button.addEventListener("click", function(event){validateForm(event)},false);
textarea.onkeypress = function(){textCounter()};