// Vertex shader used by QSGMaterialShader / QSGGeometryNode
// Expects Qt's built-in vertex attributes: qt_Vertex (vec4) and qt_MultiTexCoord0 (vec2)
// Outputs texture coordinates to the fragment shader.

uniform mat4 qt_Matrix;
attribute vec4 qt_Vertex;
attribute vec2 qt_MultiTexCoord0;
varying vec2 v_texCoord;

void main() {
    gl_Position = qt_Matrix * qt_Vertex;
    v_texCoord = qt_MultiTexCoord0;
}
