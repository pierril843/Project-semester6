var currentTime = new Date()

document.getElementById("date").innerHTML = currentTime;
document.getElementById("copyright").innerHTML = ("This Page is &copy By JRR - " + currentTime.getFullYear());
