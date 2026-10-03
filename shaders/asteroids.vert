#version 120
#extension GL_EXT_gpu_shader4 : enable

attribute float a_type;

// Hardcoded Shape Database (4 variants x 15 vertices)
const vec2 TEMPLATES[60] = vec2[] (
    // variant 0:
    vec2(0.34, -0.81), vec2(-0.14, -0.86), vec2(-0.57, -0.62), 
    vec2(-0.72, -0.19), vec2(-0.91, 0.14), vec2(-0.57, 0.43), vec2(-0.62, 0.72),
    vec2(-0.19, 0.57), vec2(0.24, 0.86), vec2(0.43, 0.53), vec2(0.77, 0.53), 
    vec2(0.48, 0.19), vec2(0.81, -0.14), vec2(0.53, -0.43), vec2(0.25, -0.34),

    // variant 1:
    vec2(0.35, -0.75), vec2(0.18, -0.90), vec2(-0.23, -0.81), 
    vec2(-0.53, -0.69), vec2(-0.54, -0.41), vec2(-0.71, -0.14), vec2(-0.69, 0.41),
    vec2(-0.44, 0.60), vec2(0.05, 0.37), vec2(0.50, 0.69), vec2(0.81, 0.32),
     vec2(0.71, -0.14), vec2(0.44, -0.23), vec2(0.69, -0.55), vec2(0.50, -0.81),

    // variant 2:
    vec2(0.35, -0.74), vec2(-0.29, -0.82), vec2(-0.35, -0.74), vec2(-0.39, -0.35),
    vec2(-0.74, -0.26), vec2(-0.82, 0.17), vec2(-0.48, 0.30), vec2(-0.69, 0.65), 
    vec2(-0.22, 0.56), vec2(0.13, 0.82), vec2(0.56, 0.65), vec2(0.52, 0.22), 
    vec2(0.82, -0.13), vec2(0.74, -0.30), vec2(0.35, -0.43),

    // variant 3:
    vec2(0.42, -0.80), vec2(0.05, -0.92), vec2(-0.42, -0.75), 
    vec2(-0.61, -0.28), vec2(-0.53, -0.14), vec2(-0.80, 0.14), vec2(-0.61, 0.70), 
    vec2(-0.14, 0.52), vec2(0.33, 0.80), vec2(0.80, 0.42), vec2(0.52, 0.14),
    vec2(0.92, -0.14), vec2(0.80, -0.42), vec2(0.71, -0.48), vec2(0.61, -0.61)
);

// Hardcoded Local Index Buffer Map for a single 15-triangle layout (45 slots)
const int TRIANGLE_MAP[45] = int[](
    0, 1, 2,    0, 2, 3,    0, 3, 4,    0, 4, 5,    0, 5, 6,
    0, 6, 7,    0, 7, 8,    0, 8, 9,    0, 9, 10,   0, 10, 11,
    0, 11, 12,  0, 12, 13,  0, 13, 14,  0, 14, 15,  0, 15, 1
);

void main()
{
    // Determine the raw overall vertex index from the draw sequence (0, 1, 2, 3...)
    int global_id = gl_VertexID;

    // Map this to a local step identifier within its specific 45-vertex asteroid sequence
    //int local_triangle_step = global_id - (45 * (global_id) / 45);
    int local_triangle_step = int(mod(global_id, 45));

    // Use our hardcoded map to see which perimeter point this vertex needs
    int vertex_idx = TRIANGLE_MAP[local_triangle_step];

    // Read transformations and type
    vec2 position   = gl_Vertex.xy; 
    float rotation  = gl_Vertex.z;  
    float scale     = gl_Vertex.w;  
    int type        = int(a_type); 

    // Look up the coordinate. If vertex_idx == 0, it stays at the center (0,0)
    vec2 local_pos = vec2(0.0, 0.0);
    if (vertex_idx > 0) {
        int lookup_index = (type * 15) + (vertex_idx - 1);
        local_pos = TEMPLATES[lookup_index];
    }

    // Standard local-to-world transformations
    local_pos *= scale;
    float cos_r = cos(rotation);
    float sin_r = sin(rotation);
    vec2 rotated_pos = vec2(
        local_pos.x * cos_r - local_pos.y * sin_r,
        local_pos.x * sin_r + local_pos.y * cos_r
    );
    vec2 world_pos = rotated_pos + position;

    gl_Position = vec4(world_pos, 0.0, 1.0);
    gl_FrontColor = gl_Color;
}