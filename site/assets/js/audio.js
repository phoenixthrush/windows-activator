async function checkNetwork() {
    return new Promise((resolve) => {
        const audioTest = new Audio("https://.mp3");

        audioTest.oncanplaythrough = () => resolve(true);
        audioTest.onerror = () => resolve(false);
    });
}

async function setAudioSource() {
    const audio = document.getElementById("audio-player");
    const online = await checkNetwork();

    if (online) {
        audio.src = "https://.mp3";
    }

    audio.volume = 0.1;

    // Not needed because of autoplay
    //  try {
    //      await audio.play();
    //  } catch (e) {
    //      console.error("Autoplay blocked:", e);
    // }
}

setAudioSource();