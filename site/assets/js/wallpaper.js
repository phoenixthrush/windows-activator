const themes = {
    "https://files.tmaster055.com/touhou.png": "https://c4.wallpaperflare.com/wallpaper/760/803/807/anime-anime-girls-hakurei-reimu-touhou-wallpaper-preview.jpg",
    "https://files.tmaster055.com/tree.png": "https://wallpaperbat.com/img/8600050-japanese-cherry-blossom-landscape-ai.jpg",
    "https://files.tmaster055.com/chaya.png": "https://get.wallhere.com/photo/selective-coloring-anime-girls-simple-background-black-background-2204297.jpg"
}

function browser(link) {
    if (!link) {
        link = document.getElementById('background').alt
    }

    try {
        window.open_browser(link)
    } catch (e) {
        window.open(link, "_blank")
    }
}

function randomizeBackground() {
    const copyrightLink = document.getElementById('copyright-img')
    copyrightLink.textContent = "Image Source"

    const keys = Object.keys(themes)
    const img_source = keys[Math.floor(Math.random() * keys.length)]
    const img_credits = themes[img_source]

    const backgroundImg = document.getElementById('background')

    const testImage = new Image()
    testImage.src = img_source

    testImage.onload = function () {
        backgroundImg.src = img_source
        document.getElementById('background').style.display = 'inline-block';

        backgroundImg.alt = img_credits
        globalLink = img_credits
    }

    testImage.onerror = function () {
        backgroundImg.alt = img_credits
    }
}

window.onload = randomizeBackground