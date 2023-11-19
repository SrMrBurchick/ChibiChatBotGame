WorkerScript.onMessage = function(message) {
    console.log("Tobi pizda: ", message.page)
    WorkerScript.sendMessage({ 'reply': message.page })
}
