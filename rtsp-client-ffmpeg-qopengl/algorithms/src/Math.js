.pragma library

function formatDuration(ms) {
    const hours = Math.floor(ms / 3600000);
    ms %= 3600000;

    const minutes = Math.floor(ms / 60000);
    ms %= 60000;

    const seconds = Math.floor(ms / 1000);
    const millis = ms % 1000;

    return String(hours).padStart(2, '0') + ":" + String(minutes).padStart(2, '0') + ":" + String(seconds).padStart(2, '0') + ":" + String(millis).padStart(3, '0');
}
