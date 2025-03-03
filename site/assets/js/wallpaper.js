const fallback = "chaya.png";

const themes = {
    "https://files.tmaster055.com/touhou.png": "https://c4.wallpaperflare.com/wallpaper/760/803/807/anime-anime-girls-hakurei-reimu-touhou-wallpaper-preview.jpg",
    "https://files.tmaster055.com/tree.png": "https://wallpaperbat.com/img/8600050-japanese-cherry-blossom-landscape-ai.jpg",
    [fallback]: "https://get.wallhere.com/photo/selective-coloring-anime-girls-simple-background-black-background-2204297.jpg"
};

function randomizeBackground() {
    const keys = Object.keys(themes);
    const randomKey = keys[Math.floor(Math.random() * keys.length)];
    const urlKey = themes[randomKey];

    const backgroundImg = document.getElementById('background');
    const copyrightLink = document.getElementById('copyright-img');

    copyrightLink.href = urlKey;
    copyrightLink.textContent = "Source";

    if (randomKey === fallback) return;

    backgroundImg.src = randomKey;
    backgroundImg.alt = randomKey;
}

window.onload = randomizeBackground;
