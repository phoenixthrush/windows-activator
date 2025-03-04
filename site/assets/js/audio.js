let currentTrack = 0;
const audio = document.getElementById("audio-player");

async function checkNetwork() {
    return new Promise((resolve) => {
        const audioTest = new Audio("https://.mp3");

        audioTest.oncanplaythrough = () => resolve(true);
        audioTest.onerror = () => resolve(false);
    });
}

const playlist = [
    "https://download.samplelib.com/mp3/sample-3s.mp3",
    "https://download.samplelib.com/mp3/sample-6s.mp3",
    "https://download.samplelib.com/mp3/sample-9s.mp3"
];

async function setAudioSource() {
    const online = await checkNetwork();

    if (online) {
        audio.src = playlist[currentTrack];
        audio.volume = 0.8;
        audio.play();
    }
}

function nextTrack() {
    currentTrack = (currentTrack + 1) % playlist.length;
    setAudioSource();
}

function shuffleTrack() {
    let randomIndex;
    do {
        randomIndex = Math.floor(Math.random() * playlist.length);
    } while (randomIndex === currentTrack);

    currentTrack = randomIndex;
    setAudioSource();
}

audio.onended = nextTrack;

setAudioSource();
