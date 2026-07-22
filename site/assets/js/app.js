/**
 * Controls the toolkit's decorative background, music, keyboard shortcuts,
 * native buttons, and credit links.
 *
 * The C application exposes native functions such as window.activate().
 * When this page is opened in a normal browser those functions do not exist,
 * so button clicks are ignored with a console warning instead of crashing.
 */

const AUDIO_VOLUME = 0.8;
const AUDIO_RETRY_DELAY_MS = 3000;
const FALLBACK_AUDIO_CREDIT = "https://www.youtube.com/watch?v=tPY-I3RX10c";

const PLAYLIST = [
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/01.%20Illusionary%20Night%20~%20Ghostly%20Eyes.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/02.%20Bloom%20Nobly%2C%20Ink-Black%20Cherry%20Blossom%20~%20Border%20of%20Life.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/03.%20Wind%20Circulation%20~%20Wind%20Tour.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/04.%20Paradise%20~%20Deep%20Mountain.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/05.%20Sleepless%20Night%20of%20the%20Eastern%20Country.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/06.%20The%20Maid%20and%20the%20Pocket%20Watch%20of%20Blood.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/07.%20Sakura%2C%20Sakura%20~%20Japanize%20Dream.mp3",
    "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/08.%20Scarlet%20Tower%20~%20Eastern%20Dream.mp3"
];

const BACKGROUNDS = [
    {
        image: "assets/images/touhou.webp",
        credit: "https://c4.wallpaperflare.com/wallpaper/760/803/807/anime-anime-girls-hakurei-reimu-touhou-wallpaper-preview.jpg"
    },
    {
        image: "assets/images/tree.webp",
        credit: "https://wallpaperbat.com/img/8600050-japanese-cherry-blossom-landscape-ai.jpg"
    },
    {
        image: "assets/images/chaya.webp",
        credit: "https://get.wallhere.com/photo/selective-coloring-anime-girls-simple-background-black-background-2204297.jpg"
    }
];

const elements = {
    audio: document.getElementById("audio-player"),
    audioCredit: document.getElementById("audio-credit"),
    background: document.getElementById("background"),
    imageCredit: document.getElementById("image-credit")
};

const state = {
    trackIndex: 0,
    failedTracks: 0,
    usingFallback: false,
    backgroundCredit: "",
    fallbackAudio: elements.audio.querySelector("source")?.src || ""
};

function randomIndex(items) {
    return Math.floor(Math.random() * items.length);
}

function openUrl(url) {
    if (!url) return;

    try {
        if (typeof window.open_browser === "function") {
            window.open_browser(url);
        } else {
            window.open(url, "_blank", "noopener,noreferrer");
        }
    } catch (error) {
        console.error("Could not open link:", error);
    }
}

function safelyPlayAudio() {
    elements.audio.play().catch((error) => {
        // Browsers may block autoplay. A later Space key press can still start it.
        if (error.name !== "NotAllowedError") {
            console.warn("Could not play audio:", error);
        }
    });
}

function loadTrack(index) {
    state.usingFallback = false;
    state.trackIndex = (index + PLAYLIST.length) % PLAYLIST.length;
    elements.audio.loop = false;
    elements.audio.src = PLAYLIST[state.trackIndex];
    elements.audio.volume = AUDIO_VOLUME;
    elements.audio.load();
    safelyPlayAudio();

    elements.audioCredit.textContent = "Audio Source";
    elements.audioCredit.dataset.url = PLAYLIST[state.trackIndex];
}

function useFallbackAudio() {
    if (!state.fallbackAudio) {
        elements.audioCredit.textContent = "Audio Unavailable";
        elements.audioCredit.removeAttribute("data-url");
        return;
    }

    state.usingFallback = true;
    elements.audio.loop = true;
    elements.audio.src = state.fallbackAudio;
    elements.audio.volume = AUDIO_VOLUME * 0.7;
    elements.audio.load();
    safelyPlayAudio();

    elements.audioCredit.textContent = "Local Audio";
    elements.audioCredit.dataset.url = FALLBACK_AUDIO_CREDIT;
}

function handleAudioError() {
    if (state.usingFallback) {
        elements.audioCredit.textContent = "Audio Unavailable";
        elements.audioCredit.removeAttribute("data-url");
        return;
    }

    state.failedTracks += 1;

    if (state.failedTracks >= PLAYLIST.length) {
        useFallbackAudio();
        return;
    }

    window.setTimeout(() => {
        loadTrack(state.trackIndex + 1);
    }, AUDIO_RETRY_DELAY_MS);
}

function toggleAudio() {
    if (elements.audio.paused) {
        safelyPlayAudio();
    } else {
        elements.audio.pause();
    }
}

function adjustVolume(change) {
    elements.audio.volume = Math.max(
        0,
        Math.min(1, elements.audio.volume + change)
    );
}

function seekAudio(toEnd) {
    elements.audio.currentTime = toEnd
        ? elements.audio.duration || 0
        : 0;
}

function loadRandomBackground() {
    const background = BACKGROUNDS[randomIndex(BACKGROUNDS)];
    state.backgroundCredit = background.credit;
    elements.background.src = background.image;
    elements.background.classList.add("loaded");
    elements.imageCredit.dataset.url = background.credit;
}

function runNativeAction(actionName) {
    const action = window[actionName];

    if (typeof action === "function") {
        action();
    } else {
        console.warn(
            "Native action '" + actionName + "' is available only in the desktop app."
        );
    }
}

function handleKeyboard(event) {
    // Do not replace the normal keyboard behavior of focused controls.
    if (event.target.closest("button, a, input, select, textarea")) return;

    const actions = {
        Space: () => toggleAudio(),
        ArrowLeft: () => seekAudio(false),
        ArrowRight: () => seekAudio(true),
        ArrowUp: () => adjustVolume(0.1),
        ArrowDown: () => adjustVolume(-0.1)
    };
    const action = actions[event.code];

    if (action) {
        event.preventDefault();
        action();
    }
}

function initializeButtons() {
    document.querySelectorAll("[data-action]").forEach((button) => {
        button.addEventListener("click", () => {
            runNativeAction(button.dataset.action);
        });
    });

    document.querySelectorAll("[data-url]").forEach((button) => {
        button.addEventListener("click", () => {
            openUrl(button.dataset.url);
        });
    });

    elements.imageCredit.addEventListener("click", () => {
        openUrl(state.backgroundCredit);
    });

    elements.audioCredit.addEventListener("click", () => {
        openUrl(elements.audioCredit.dataset.url);
    });
}

function initialize() {
    initializeButtons();
    loadRandomBackground();

    state.trackIndex = randomIndex(PLAYLIST);
    loadTrack(state.trackIndex);

    elements.audio.addEventListener("ended", () => {
        state.failedTracks = 0;
        loadTrack(state.trackIndex + 1);
    });
    elements.audio.addEventListener("error", handleAudioError);
    elements.background.addEventListener("load", () => {
        elements.background.classList.add("loaded");
    });
    elements.background.addEventListener("error", () => {
        console.warn("Background image could not be loaded.");
    });

    document.addEventListener("keydown", handleKeyboard);
    document.addEventListener("contextmenu", (event) => {
        event.preventDefault();
    });
}

initialize();
