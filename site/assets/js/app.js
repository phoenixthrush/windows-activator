/**
 * Universal Activation Toolkit - Optimized Application Script
 * Combines audio, wallpaper, and app functionality
 */

// Configuration and constants
const CONFIG = {
    AUDIO: {
        VOLUME: 0.8,
        RETRY_TIMEOUT: 3000,
        PLAYLIST: [
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/01.%20Illusionary%20Night%20~%20Ghostly%20Eyes.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/02.%20Bloom%20Nobly%2C%20Ink-Black%20Cherry%20Blossom%20~%20Border%20of%20Life.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/03.%20Wind%20Circulation%20~%20Wind%20Tour.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/04.%20Paradise%20~%20Deep%20Mountain.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/05.%20Sleepless%20Night%20of%20the%20Eastern%20Country.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/06.%20The%20Maid%20and%20the%20Pocket%20Watch%20of%20Blood.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/07.%20Sakura%2C%20Sakura%20~%20Japanize%20Dream.mp3",
            "https://archive.org/download/as-if-looking-at-a-distant-place-demetori/08.%20Scarlet%20Tower%20~%20Eastern%20Dream.mp3"
        ]
    },
    THEMES: {
        "assets/images/touhou": {
            webp: "assets/images/touhou.webp",
            credit: "https://c4.wallpaperflare.com/wallpaper/760/803/807/anime-anime-girls-hakurei-reimu-touhou-wallpaper-preview.jpg"
        },
        "assets/images/tree": {
            webp: "assets/images/tree.webp",
            credit: "https://wallpaperbat.com/img/8600050-japanese-cherry-blossom-landscape-ai.jpg"
        },
        "assets/images/chaya": {
            webp: "assets/images/chaya.webp",
            credit: "https://get.wallhere.com/photo/selective-coloring-anime-girls-simple-background-black-background-2204297.jpg"
        }
    },
    KEYBOARD: {
        SPACE: 'Space',
        ARROW_LEFT: 'ArrowLeft',
        ARROW_RIGHT: 'ArrowRight',
        ARROW_UP: 'ArrowUp',
        ARROW_DOWN: 'ArrowDown'
    }
};

// Application state
const AppState = {
    currentTrack: 0,
    audioElement: null,
    backgroundElement: null,
    isAudioLoaded: false,
    isBackgroundLoaded: false
};

// Utility functions
const Utils = {
    /**
     * Safely get element by ID with error handling
     */
    getElement(id) {
        const element = document.getElementById(id);
        if (!element) {
            console.warn(`Element with ID '${id}' not found`);
        }
        return element;
    },

    /**
     * Debounce function to limit rapid function calls
     */
    debounce(func, wait) {
        let timeout;
        return function executedFunction(...args) {
            const later = () => {
                clearTimeout(timeout);
                func(...args);
            };
            clearTimeout(timeout);
            timeout = setTimeout(later, wait);
        };
    },

    /**
     * Check if network resource is accessible
     */
    async checkNetworkResource(url, timeout = 5000) {
        return new Promise((resolve) => {
            const timeoutId = setTimeout(() => {
                resolve(false);
            }, timeout);

            if (url.includes('archive.org')) {
                // Audio check
                const audio = new Audio();
                audio.oncanplaythrough = () => {
                    clearTimeout(timeoutId);
                    resolve(true);
                };
                audio.onerror = () => {
                    clearTimeout(timeoutId);
                    resolve(false);
                };
                audio.src = url;
            } else if (url.startsWith('assets/') || url.startsWith('./') || url.startsWith('/')) {
                // Local file check - assume accessible
                clearTimeout(timeoutId);
                resolve(true);
            } else {
                // External image check
                const img = new Image();
                img.onload = () => {
                    clearTimeout(timeoutId);
                    resolve(true);
                };
                img.onerror = () => {
                    clearTimeout(timeoutId);
                    resolve(false);
                };
                img.src = url;
            }
        });
    },

    /**
     * Open URL using available method
     */
    openUrl(url) {
        try {
            if (window.open_browser && typeof window.open_browser === 'function') {
                window.open_browser(url);
            } else {
                window.open(url, '_blank', 'noopener,noreferrer');
            }
        } catch (error) {
            console.error('Failed to open URL:', error);
            // Fallback: copy to clipboard if possible
            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(url).then(() => {
                    alert(`Link copied to clipboard: ${url}`);
                }).catch(() => {
                    alert(`Please manually navigate to: ${url}`);
                });
            } else {
                alert(`Please manually navigate to: ${url}`);
            }
        }
    }
};

// Audio Manager
const AudioManager = {
    init() {
        AppState.audioElement = Utils.getElement('audio-player');
        if (!AppState.audioElement) return;

        this.setupEventListeners();
        this.loadRandomTrack();
    },

    setupEventListeners() {
        if (!AppState.audioElement) return;

        AppState.audioElement.addEventListener('ended', () => this.nextTrack());
        AppState.audioElement.addEventListener('error', () => this.handleAudioError());
        AppState.audioElement.addEventListener('canplaythrough', () => {
            AppState.isAudioLoaded = true;
        });
    },

    async loadRandomTrack() {
        const randomIndex = Math.floor(Math.random() * CONFIG.AUDIO.PLAYLIST.length);
        await this.loadTrack(randomIndex);
    },

    async loadTrack(index) {
        if (!AppState.audioElement || index < 0 || index >= CONFIG.AUDIO.PLAYLIST.length) return;

        AppState.currentTrack = index;
        const url = CONFIG.AUDIO.PLAYLIST[index];

        try {
            const isAccessible = await Utils.checkNetworkResource(url);

            if (isAccessible) {
                AppState.audioElement.removeAttribute('loop');
                AppState.audioElement.src = url;
                AppState.audioElement.volume = CONFIG.AUDIO.VOLUME;

                // Update credit link
                const creditElement = Utils.getElement('audio-credit');
                if (creditElement) {
                    creditElement.textContent = 'Audio Source';
                    creditElement.style.opacity = '1';
                }

                await AppState.audioElement.play();
            } else {
                this.useFallbackAudio();
            }
        } catch (error) {
            console.warn('Failed to load online audio, using fallback:', error);
            this.useFallbackAudio();
        }
    },

    useFallbackAudio() {
        if (!AppState.audioElement) return;

        AppState.audioElement.setAttribute('loop', 'true');
        AppState.audioElement.volume = CONFIG.AUDIO.VOLUME * 0.7; // Lower volume for fallback

        const creditElement = Utils.getElement('audio-credit');
        if (creditElement) {
            creditElement.textContent = 'Local Audio';
            creditElement.style.opacity = '0.7';
        }

        AppState.audioElement.play().catch(error => {
            console.warn('Fallback audio failed to play:', error);
        });
    },

    nextTrack() {
        const nextIndex = (AppState.currentTrack + 1) % CONFIG.AUDIO.PLAYLIST.length;
        this.loadTrack(nextIndex);
    },

    previousTrack() {
        const prevIndex = AppState.currentTrack === 0
            ? CONFIG.AUDIO.PLAYLIST.length - 1
            : AppState.currentTrack - 1;
        this.loadTrack(prevIndex);
    },

    handleAudioError() {
        console.warn('Audio error occurred, trying next track...');
        setTimeout(() => this.nextTrack(), CONFIG.AUDIO.RETRY_TIMEOUT);
    },

    togglePlayPause() {
        if (!AppState.audioElement) return;

        if (AppState.audioElement.paused) {
            AppState.audioElement.play().catch(error => {
                console.warn('Failed to play audio:', error);
            });
        } else {
            AppState.audioElement.pause();
        }
    },

    adjustVolume(delta) {
        if (!AppState.audioElement) return;

        const newVolume = Math.max(0, Math.min(1, AppState.audioElement.volume + delta));
        AppState.audioElement.volume = newVolume;
    },

    seek(position) {
        if (!AppState.audioElement) return;

        if (position === 'start') {
            AppState.audioElement.currentTime = 0;
        } else if (position === 'end') {
            AppState.audioElement.currentTime = AppState.audioElement.duration || 0;
        }
    }
};

// Background Manager
const BackgroundManager = {
    init() {
        AppState.backgroundElement = Utils.getElement('background');
        if (!AppState.backgroundElement) return;

        // Add load event listener to ensure image becomes visible
        AppState.backgroundElement.addEventListener('load', () => {
            AppState.backgroundElement.classList.add('loaded');
            AppState.backgroundElement.style.opacity = '1';
        });

        // Add error event listener
        AppState.backgroundElement.addEventListener('error', () => {
            console.warn('Background image failed to load');
            AppState.backgroundElement.style.opacity = '1'; // Show anyway
            AppState.backgroundElement.classList.add('loaded');
        });

        // Ensure background is visible immediately
        AppState.backgroundElement.classList.add('loaded');
        AppState.backgroundElement.style.opacity = '1';

        // Load random theme
        this.loadRandomBackground();
    },

    async loadRandomBackground() {
        const themes = Object.keys(CONFIG.THEMES);
        const randomTheme = themes[Math.floor(Math.random() * themes.length)];
        await this.loadBackground(randomTheme);
    },

    async loadBackground(themeKey) {
        if (!AppState.backgroundElement) return;

        const theme = CONFIG.THEMES[themeKey];
        if (!theme) return;

        try {
            // Use WebP directly
            const imageUrl = theme.webp;
            const isAccessible = await Utils.checkNetworkResource(imageUrl);

            if (isAccessible) {
                AppState.backgroundElement.src = imageUrl;
                AppState.backgroundElement.alt = theme.credit;
                AppState.backgroundElement.classList.add('loaded');

                // Update credit link
                const creditElement = Utils.getElement('image-credit');
                if (creditElement) {
                    creditElement.textContent = 'Image Source';
                    creditElement.style.opacity = '1';
                }

                AppState.isBackgroundLoaded = true;
            } else {
                this.useFallbackBackground(theme.credit);
            }
        } catch (error) {
            console.warn('Failed to load background:', error);
            this.useFallbackBackground(theme.credit);
        }
    },

    useFallbackBackground(creditUrl) {
        if (!AppState.backgroundElement) return;

        // Make sure the element is visible
        AppState.backgroundElement.style.opacity = '1';
        AppState.backgroundElement.alt = creditUrl || '';
        AppState.backgroundElement.classList.add('loaded');

        const creditElement = Utils.getElement('image-credit');
        if (creditElement) {
            creditElement.textContent = 'Local Image';
            creditElement.style.opacity = '0.7';
        }

        AppState.isBackgroundLoaded = true;
    }
};

// Event handlers for global functions
window.browser = function (url) {
    if (!url && AppState.backgroundElement) {
        url = AppState.backgroundElement.alt;
    }
    if (url) {
        Utils.openUrl(url);
    }
};

window.browser_audio = function (url) {
    if (!url && AppState.audioElement) {
        url = AppState.audioElement.src;
    }
    if (url) {
        Utils.openUrl(url);
    }
};

// Keyboard controls
const KeyboardManager = {
    init() {
        document.addEventListener('keydown', this.handleKeyDown.bind(this));
        // Disable context menu
        document.addEventListener('contextmenu', (e) => e.preventDefault());
    },

    handleKeyDown(event) {
        // Prevent default browser behavior for media keys
        event.preventDefault();

        switch (event.code) {
            case CONFIG.KEYBOARD.SPACE:
                AudioManager.togglePlayPause();
                break;
            case CONFIG.KEYBOARD.ARROW_RIGHT:
                AudioManager.seek('end');
                break;
            case CONFIG.KEYBOARD.ARROW_LEFT:
                AudioManager.seek('start');
                break;
            case CONFIG.KEYBOARD.ARROW_UP:
                AudioManager.adjustVolume(0.1);
                break;
            case CONFIG.KEYBOARD.ARROW_DOWN:
                AudioManager.adjustVolume(-0.1);
                break;
        }
    }
};

// Performance optimizations
const PerformanceManager = {
    init() {
        // Preload critical resources
        this.preloadCriticalResources();

        // Setup intersection observer for lazy loading
        this.setupIntersectionObserver();

        // Setup performance monitoring
        this.setupPerformanceMonitoring();
    },

    preloadCriticalResources() {
        // Preload WebP images directly
        const link = document.createElement('link');
        link.rel = 'preload';
        link.href = 'assets/images/tree.webp';
        link.as = 'image';
        document.head.appendChild(link);
    },

    setupIntersectionObserver() {
        if ('IntersectionObserver' in window) {
            const observer = new IntersectionObserver((entries) => {
                entries.forEach(entry => {
                    if (entry.isIntersecting) {
                        entry.target.classList.add('visible');
                    }
                });
            });

            document.querySelectorAll('.btn').forEach(btn => {
                observer.observe(btn);
            });
        }
    },

    setupPerformanceMonitoring() {
        if ('PerformanceObserver' in window) {
            try {
                const observer = new PerformanceObserver((list) => {
                    list.getEntries().forEach((entry) => {
                        if (entry.entryType === 'navigation') {
                            console.log(`Page load time: ${entry.loadEventEnd - entry.loadEventStart}ms`);
                        }
                    });
                });
                observer.observe({ entryTypes: ['navigation'] });
            } catch (error) {
                console.warn('Performance monitoring not available:', error);
            }
        }
    }
};

// Application initialization
const App = {
    init() {
        // Wait for DOM to be ready
        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', () => this.start());
        } else {
            this.start();
        }
    },

    start() {
        console.log('Universal Activation Toolkit - Starting...');

        try {
            // Initialize all managers
            PerformanceManager.init();
            KeyboardManager.init();
            BackgroundManager.init();
            AudioManager.init();

            console.log('Universal Activation Toolkit - Ready!');
        } catch (error) {
            console.error('Failed to initialize application:', error);
        }
    }
};

// Start the application
App.init();
