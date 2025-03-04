const playlist = [
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/01.%20Illusionary%20Night%20~%20Ghostly%20Eyes.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/02.%20Bloom%20Nobly%2C%20Ink-Black%20Cherry%20Blossom%20~%20Border%20of%20Life.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/03.%20Wind%20Circulation%20~%20Wind%20Tour.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/04.%20Paradise%20~%20Deep%20Mountain.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/05.%20Sleepless%20Night%20of%20the%20Eastern%20Country.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/06.%20The%20Maid%20and%20the%20Pocket%20Watch%20of%20Blood.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/07.%20Sakura%2C%20Sakura%20~%20Japanize%20Dream.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/08.%20Scarlet%20Tower%20~%20Eastern%20Dream.mp3"
];

let currentTrack = 0;
const audio = document.getElementById("audio-player");

async function checkNetwork(url) {
    return new Promise((resolve) => {
        const audioTest = new Audio(url);

        audioTest.oncanplaythrough = () => resolve(true);
        audioTest.onerror = () => resolve(false);
    });
}

async function setAudioSource() {
    const online = await checkNetwork(playlist[currentTrack]);

    const copyrightLink = document.getElementById('copyright-audio')
    copyrightLink.textContent = "Audio Source";

    if (online) {
        audio.removeAttribute("loop");
        audio.src = playlist[currentTrack];
        audio.volume = 0.8;
        audio.play();
    } else {
        // does not work
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

function browser_audio(link) {
    if (!link) {
        link = document.getElementById('audio-player').src
    }

    try {
        window.open_browser(link)
    } catch (e) {
        window.open(link, "_blank")
    }
}

audio.onended = nextTrack;
shuffleTrack();
