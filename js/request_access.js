function textCounter()
{
 var textarea = document.getElementById('textareaid');
 var textareacount = document.getElementById('textareacount');

 textareacount.textContent = (textarea.maxLength - textarea.textLength);
}


textarea = document.getElementById('textareaid');
textareacount = document.getElementById('textareacount');

textarea.onkeypress = function(){textCounter()};