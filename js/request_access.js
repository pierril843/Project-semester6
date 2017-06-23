function textCounter()
{
 var textarea = document.getElementById('textareaid');
 var textareacount = document.getElementById('textareacount');

 textareacount.textContent = (textarea.maxLength - textarea.textLength);
}

textarea.onkeypress = function(){textCounter()};