// Fragment shader supporting YUV420P and NV12 planar formats
// - Supports color conversion matrices for BT.601 and BT.709
// - Supports limited range (video) and full range (PC) YCbCr ranges
// - Handles both YUV420P (three separate planes: Y, U, V)
//   and NV12 (two planes: Y, interleaved UV)
//
// Uniforms / samplers expected by the host application:
// uniform int u_format; // 0 = YUV420P, 1 = NV12
// uniform int u_colorspace; // 0 = BT.601, 1 = BT.709
// uniform int u_range; // 0 = Limited (16..235/16..240), 1 = Full (0..255)
// uniform sampler2D s_y; // Y plane (R8 texture)
// uniform sampler2D s_u; // U plane (R8) for YUV420P OR interleaved UV packed into RG for NV12 (use .rg)
// uniform sampler2D s_v; // V plane (R8) for YUV420P. Not used for NV12.

precision highp float;
varying vec2 v_texCoord;

uniform int u_format;
uniform int u_colorspace;
uniform int u_range;
uniform sampler2D s_y;
uniform sampler2D s_u;
uniform sampler2D s_v;

// Convert sampled normalized texture value into a signed chroma or luma value
// depending on full vs limited range.
// Input: sampled value in [0,1]. Output: luma in [0,1] (normalized) and chroma centered at 0.

// For limited range (video):
//   Y'  encoded in [16,235] -> normalized Y_n = (Y_e - 16)/219
//   Cb/Cr encoded in [16,240] -> normalized C_n = (C_e - 128)/224
// For full range (PC):
//   Y' in [0,255] -> Y_n = Y_e/255
//   Cb/Cr in [0,255] -> C_n = (C_e - 128)/255

vec3 yuv_to_rgb_matrix(float y, float u, float v, int colorspace) {
    // Define conversion matrices multiplied by [Y, U, V] where U and V are centered at 0.
    // These matrices assume Y is luma in [0,1] and U/V are in [-0.5,0.5] or similar scaled ranges.
    // We'll scale U/V appropriately before calling this.

    mat3 m601 = mat3(
        1.0,     0.0,       1.402,
        1.0,    -0.344136, -0.714136,
        1.0,     1.772,     0.0
    );

    mat3 m709 = mat3(
        1.0,     0.0,       1.5748,
        1.0,    -0.187324, -0.468124,
        1.0,     1.8556,    0.0
    );

    vec3 yuv = vec3(y, u, v);
    if (colorspace == 0) {
        return m601 * yuv;
    } else {
        return m709 * yuv;
    }
}

void main() {
    // Sample Y plane
    float Ys = texture2D(s_y, v_texCoord).r; // encoded Y in [0,1]

    float Y;
    if (u_range == 0) {
        // Limited range: map [16/255..235/255] -> [0..1]
        Y = (Ys * 255.0 - 16.0) / 219.0;
    } else {
        // Full range: map [0..255] -> [0..1]
        Y = Ys;
    }

    float U = 0.0;
    float V = 0.0;

    if (u_format == 0) {
        // YUV420P: separate U and V planes. U and V are subsampled by 2 in each dimension
        float Us = texture2D(s_u, v_texCoord).r;
        float Vs = texture2D(s_v, v_texCoord).r;
        if (u_range == 0) {
            U = (Us * 255.0 - 128.0) / 224.0; // limited chroma range center 128 and width 224
            V = (Vs * 255.0 - 128.0) / 224.0;
        } else {
            U = (Us * 255.0 - 128.0) / 255.0;
            V = (Vs * 255.0 - 128.0) / 255.0;
        }
    } else {
        // NV12: U and V are interleaved in UV plane stored as RG: U=R, V=G
        vec2 uv = texture2D(s_u, v_texCoord).rg;
        float Us = uv.r;
        float Vs = uv.g;
        if (u_range == 0) {
            U = (Us * 255.0 - 128.0) / 224.0;
            V = (Vs * 255.0 - 128.0) / 224.0;
        } else {
            U = (Us * 255.0 - 128.0) / 255.0;
            V = (Vs * 255.0 - 128.0) / 255.0;
        }
    }

    // Now Y in [0,1], U and V approximately in [-0.5,0.5] scaled depending on range mapping.
    vec3 rgb = yuv_to_rgb_matrix(Y, U, V, u_colorspace);

    // Clamp to [0,1]
    rgb = clamp(rgb, 0.0, 1.0);

    gl_FragColor = vec4(rgb, 1.0);
}
