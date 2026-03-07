.pragma library

// Ensure with different index, function return different color
function colorFromIndex(index) {
    var hue = (index * 137.508) % 360; // Use golden angle approximation
    return Qt.hsla(hue / 360, 0.5, 0.5, 1);
}
