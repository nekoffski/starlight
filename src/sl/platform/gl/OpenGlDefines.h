#define STARL_FALSE 0
#define STARL_TRUE 1

/* Data types */
#define STARL_BYTE 0x1400
#define STARL_UNSIGNED_BYTE 0x1401
#define STARL_SHORT 0x1402
#define STARL_UNSIGNED_SHORT 0x1403
#define STARL_INT 0x1404
#define STARL_UNSIGNED_INT 0x1405
#define STARL_FLOAT 0x1406
#define STARL_2_BYTES 0x1407
#define STARL_3_BYTES 0x1408
#define STARL_4_BYTES 0x1409
#define STARL_DOUBLE 0x140A

/* Primitives */
#define STARL_POINTS 0x0000
#define STARL_LINES 0x0001
#define STARL_LINE_LOOP 0x0002
#define STARL_LINE_STRIP 0x0003
#define STARL_TRIANGLES 0x0004
#define STARL_TRIANGLE_STRIP 0x0005
#define STARL_TRIANGLE_FAN 0x0006
#define STARL_QUADS 0x0007
#define STARL_QUAD_STRIP 0x0008
#define STARL_POLYGON 0x0009

#define STARL_COLOR_ATTACHMENT0 0x8CE0
#define STARL_COLOR_ATTACHMENT1 0x8CE1
#define STARL_COLOR_ATTACHMENT2 0x8CE2
#define STARL_COLOR_ATTACHMENT3 0x8CE3
#define STARL_COLOR_ATTACHMENT4 0x8CE4
#define STARL_COLOR_ATTACHMENT5 0x8CE5
#define STARL_COLOR_ATTACHMENT6 0x8CE6
#define STARL_COLOR_ATTACHMENT7 0x8CE7
#define STARL_COLOR_ATTACHMENT8 0x8CE8
#define STARL_COLOR_ATTACHMENT9 0x8CE9
#define STARL_COLOR_ATTACHMENT10 0x8CEA
#define STARL_COLOR_ATTACHMENT11 0x8CEB
#define STARL_COLOR_ATTACHMENT12 0x8CEC
#define STARL_COLOR_ATTACHMENT13 0x8CED
#define STARL_COLOR_ATTACHMENT14 0x8CEE
#define STARL_COLOR_ATTACHMENT15 0x8CEF
#define STARL_COLOR_ATTACHMENT16 0x8CF0
#define STARL_COLOR_ATTACHMENT17 0x8CF1
#define STARL_COLOR_ATTACHMENT18 0x8CF2
#define STARL_COLOR_ATTACHMENT19 0x8CF3
#define STARL_COLOR_ATTACHMENT20 0x8CF4
#define STARL_COLOR_ATTACHMENT21 0x8CF5
#define STARL_COLOR_ATTACHMENT22 0x8CF6
#define STARL_COLOR_ATTACHMENT23 0x8CF7
#define STARL_COLOR_ATTACHMENT24 0x8CF8
#define STARL_COLOR_ATTACHMENT25 0x8CF9
#define STARL_COLOR_ATTACHMENT26 0x8CFA
#define STARL_COLOR_ATTACHMENT27 0x8CFB
#define STARL_COLOR_ATTACHMENT28 0x8CFC
#define STARL_COLOR_ATTACHMENT29 0x8CFD
#define STARL_COLOR_ATTACHMENT30 0x8CFE
#define STARL_COLOR_ATTACHMENT31 0x8CFF

/* Vertex Arrays */
#define STARL_VERTEX_ARRAY 0x8074
#define STARL_NORMAL_ARRAY 0x8075
#define STARL_COLOR_ARRAY 0x8076
#define STARL_INDEX_ARRAY 0x8077
#define STARL_TEXTURE_COORD_ARRAY 0x8078
#define STARL_EDGE_FLAG_ARRAY 0x8079
#define STARL_VERTEX_ARRAY_SIZE 0x807A
#define STARL_VERTEX_ARRAY_TYPE 0x807B
#define STARL_VERTEX_ARRAY_STRIDE 0x807C
#define STARL_NORMAL_ARRAY_TYPE 0x807E
#define STARL_NORMAL_ARRAY_STRIDE 0x807F
#define STARL_COLOR_ARRAY_SIZE 0x8081
#define STARL_COLOR_ARRAY_TYPE 0x8082
#define STARL_COLOR_ARRAY_STRIDE 0x8083
#define STARL_INDEX_ARRAY_TYPE 0x8085
#define STARL_INDEX_ARRAY_STRIDE 0x8086
#define STARL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define STARL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define STARL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define STARL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define STARL_VERTEX_ARRAY_POINTER 0x808E
#define STARL_NORMAL_ARRAY_POINTER 0x808F
#define STARL_COLOR_ARRAY_POINTER 0x8090
#define STARL_INDEX_ARRAY_POINTER 0x8091
#define STARL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define STARL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define STARL_V2F 0x2A20
#define STARL_V3F 0x2A21
#define STARL_C4UB_V2F 0x2A22
#define STARL_C4UB_V3F 0x2A23
#define STARL_C3F_V3F 0x2A24
#define STARL_N3F_V3F 0x2A25
#define STARL_C4F_N3F_V3F 0x2A26
#define STARL_T2F_V3F 0x2A27
#define STARL_T4F_V4F 0x2A28
#define STARL_T2F_C4UB_V3F 0x2A29
#define STARL_T2F_C3F_V3F 0x2A2A
#define STARL_T2F_N3F_V3F 0x2A2B
#define STARL_T2F_C4F_N3F_V3F 0x2A2C
#define STARL_T4F_C4F_N3F_V4F 0x2A2D

/* Matrix Mode */
#define STARL_MATRIX_MODE 0x0BA0
#define STARL_MODELVIEW 0x1700
#define STARL_PROJECTION 0x1701
#define STARL_TEXTURE 0x1702

/* Points */
#define STARL_POINT_SMOOTH 0x0B10
#define STARL_POINT_SIZE 0x0B11
#define STARL_POINT_SIZE_GRANULARITY 0x0B13
#define STARL_POINT_SIZE_RANGE 0x0B12

/* Lines */
#define STARL_LINE_SMOOTH 0x0B20
#define STARL_LINE_STIPPLE 0x0B24
#define STARL_LINE_STIPPLE_PATTERN 0x0B25
#define STARL_LINE_STIPPLE_REPEAT 0x0B26
#define STARL_LINE_WIDTH 0x0B21
#define STARL_LINE_WIDTH_GRANULARITY 0x0B23
#define STARL_LINE_WIDTH_RANGE 0x0B22

/* Polygons */
#define STARL_POINT 0x1B00
#define STARL_LINE 0x1B01
#define STARL_FILL 0x1B02
#define STARL_CW 0x0900
#define STARL_CCW 0x0901
#define STARL_FRONT 0x0404
#define STARL_BACK 0x0405
#define STARL_POLYGON_MODE 0x0B40
#define STARL_POLYGON_SMOOTH 0x0B41
#define STARL_POLYGON_STIPPLE 0x0B42
#define STARL_EDGE_FLAG 0x0B43
#define STARL_CULL_FACE 0x0B44
#define STARL_CULL_FACE_MODE 0x0B45
#define STARL_FRONT_FACE 0x0B46
#define STARL_POLYGON_OFFSET_FACTOR 0x8038
#define STARL_POLYGON_OFFSET_UNITS 0x2A00
#define STARL_POLYGON_OFFSET_POINT 0x2A01
#define STARL_POLYGON_OFFSET_LINE 0x2A02
#define STARL_POLYGON_OFFSET_FILL 0x8037

/* Display Lists */
#define STARL_COMPILE 0x1300
#define STARL_COMPILE_AND_EXECUTE 0x1301
#define STARL_LIST_BASE 0x0B32
#define STARL_LIST_INDEX 0x0B33
#define STARL_LIST_MODE 0x0B30

/* Depth buffer */
#define STARL_NEVER 0x0200
#define STARL_LESS 0x0201
#define STARL_EQUAL 0x0202
#define STARL_LEQUAL 0x0203
#define STARL_GREATER 0x0204
#define STARL_NOTEQUAL 0x0205
#define STARL_GEQUAL 0x0206
#define STARL_ALWAYS 0x0207
#define STARL_DEPTH_TEST 0x0B71
#define STARL_DEPTH_BITS 0x0D56
#define STARL_DEPTH_CLEAR_VALUE 0x0B73
#define STARL_DEPTH_FUNC 0x0B74
#define STARL_DEPTH_RANGE 0x0B70
#define STARL_DEPTH_WRITEMASK 0x0B72
#define STARL_DEPTH_COMPONENT 0x1902

/* Lighting */
#define STARL_LIGHTING 0x0B50
#define STARL_LIGHT0 0x4000
#define STARL_LIGHT1 0x4001
#define STARL_LIGHT2 0x4002
#define STARL_LIGHT3 0x4003
#define STARL_LIGHT4 0x4004
#define STARL_LIGHT5 0x4005
#define STARL_LIGHT6 0x4006
#define STARL_LIGHT7 0x4007
#define STARL_SPOT_EXPONENT 0x1205
#define STARL_SPOT_CUTOFF 0x1206
#define STARL_CONSTANT_ATTENUATION 0x1207
#define STARL_LINEAR_ATTENUATION 0x1208
#define STARL_QUADRATIC_ATTENUATION 0x1209
#define STARL_AMBIENT 0x1200
#define STARL_DIFFUSE 0x1201
#define STARL_SPECULAR 0x1202
#define STARL_SHININESS 0x1601
#define STARL_EMISSION 0x1600
#define STARL_POSITION 0x1203
#define STARL_SPOT_DIRECTION 0x1204
#define STARL_AMBIENT_AND_DIFFUSE 0x1602
#define STARL_COLOR_INDEXES 0x1603
#define STARL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define STARL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define STARL_LIGHT_MODEL_AMBIENT 0x0B53
#define STARL_FRONT_AND_BACK 0x0408
#define STARL_SHADE_MODEL 0x0B54
#define STARL_FLAT 0x1D00
#define STARL_SMOOTH 0x1D01
#define STARL_COLOR_MATERIAL 0x0B57
#define STARL_COLOR_MATERIAL_FACE 0x0B55
#define STARL_COLOR_MATERIAL_PARAMETER 0x0B56
#define STARL_NORMALIZE 0x0BA1

/* User clipping planes */
#define STARL_CLIP_PLANE0 0x3000
#define STARL_CLIP_PLANE1 0x3001
#define STARL_CLIP_PLANE2 0x3002
#define STARL_CLIP_PLANE3 0x3003
#define STARL_CLIP_PLANE4 0x3004
#define STARL_CLIP_PLANE5 0x3005

/* Accumulation buffer */
#define STARL_ACCUM_RED_BITS 0x0D58
#define STARL_ACCUM_GREEN_BITS 0x0D59
#define STARL_ACCUM_BLUE_BITS 0x0D5A
#define STARL_ACCUM_ALPHA_BITS 0x0D5B
#define STARL_ACCUM_CLEAR_VALUE 0x0B80
#define STARL_ACCUM 0x0100
#define STARL_ADD 0x0104
#define STARL_LOAD 0x0101
#define STARL_MULT 0x0103
#define STARL_RETURN 0x0102

/* Alpha testing */
#define STARL_ALPHA_TEST 0x0BC0
#define STARL_ALPHA_TEST_REF 0x0BC2
#define STARL_ALPHA_TEST_FUNC 0x0BC1

/* Blending */
#define STARL_BLEND 0x0BE2
#define STARL_BLEND_SRC 0x0BE1
#define STARL_BLEND_DST 0x0BE0
#define STARL_ZERO 0
#define STARL_ONE 1
#define STARL_SRC_COLOR 0x0300
#define STARL_ONE_MINUS_SRC_COLOR 0x0301
#define STARL_SRC_ALPHA 0x0302
#define STARL_ONE_MINUS_SRC_ALPHA 0x0303
#define STARL_DST_ALPHA 0x0304
#define STARL_ONE_MINUS_DST_ALPHA 0x0305
#define STARL_DST_COLOR 0x0306
#define STARL_ONE_MINUS_DST_COLOR 0x0307
#define STARL_SRC_ALPHA_SATURATE 0x0308

/* Render Mode */
#define STARL_FEEDBACK 0x1C01
#define STARL_RENDER 0x1C00
#define STARL_SELECT 0x1C02

/* Feedback */
#define STARL_2D 0x0600
#define STARL_3D 0x0601
#define STARL_3D_COLOR 0x0602
#define STARL_3D_COLOR_TEXTURE 0x0603
#define STARL_4D_COLOR_TEXTURE 0x0604
#define STARL_POINT_TOKEN 0x0701
#define STARL_LINE_TOKEN 0x0702
#define STARL_LINE_RESET_TOKEN 0x0707
#define STARL_POLYGON_TOKEN 0x0703
#define STARL_BITMAP_TOKEN 0x0704
#define STARL_DRAW_PIXEL_TOKEN 0x0705
#define STARL_COPY_PIXEL_TOKEN 0x0706
#define STARL_PASS_THROUGH_TOKEN 0x0700
#define STARL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define STARL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define STARL_FEEDBACK_BUFFER_TYPE 0x0DF2

/* Selection */
#define STARL_SELECTION_BUFFER_POINTER 0x0DF3
#define STARL_SELECTION_BUFFER_SIZE 0x0DF4

/* Fog */
#define STARL_FOG 0x0B60
#define STARL_FOG_MODE 0x0B65
#define STARL_FOG_DENSITY 0x0B62
#define STARL_FOG_COLOR 0x0B66
#define STARL_FOG_INDEX 0x0B61
#define STARL_FOG_START 0x0B63
#define STARL_FOG_END 0x0B64
#define STARL_LINEAR 0x2601
#define STARL_EXP 0x0800
#define STARL_EXP2 0x0801

/* Logic Ops */
#define STARL_LOGIC_OP 0x0BF1
#define STARL_INDEX_LOGIC_OP 0x0BF1
#define STARL_COLOR_LOGIC_OP 0x0BF2
#define STARL_LOGIC_OP_MODE 0x0BF0
#define STARL_CLEAR 0x1500
#define STARL_SET 0x150F
#define STARL_COPY 0x1503
#define STARL_COPY_INVERTED 0x150C
#define STARL_NOOP 0x1505
#define STARL_INVERT 0x150A
#define STARL_AND 0x1501
#define STARL_NAND 0x150E
#define STARL_OR 0x1507
#define STARL_NOR 0x1508
#define STARL_XOR 0x1506
#define STARL_EQUIV 0x1509
#define STARL_AND_REVERSE 0x1502
#define STARL_AND_INVERTED 0x1504
#define STARL_OR_REVERSE 0x150B
#define STARL_OR_INVERTED 0x150D

/* Stencil */
#define STARL_STENCIL_BITS 0x0D57
#define STARL_STENCIL_TEST 0x0B90
#define STARL_STENCIL_CLEAR_VALUE 0x0B91
#define STARL_STENCIL_FUNC 0x0B92
#define STARL_STENCIL_VALUE_MASK 0x0B93
#define STARL_STENCIL_FAIL 0x0B94
#define STARL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define STARL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define STARL_STENCIL_REF 0x0B97
#define STARL_STENCIL_WRITEMASK 0x0B98
#define STARL_STENCIL_INDEX 0x1901
#define STARL_KEEP 0x1E00
#define STARL_REPLACE 0x1E01
#define STARL_INCR 0x1E02
#define STARL_DECR 0x1E03

/* Buffers, Pixel Drawing/Reading */
#define STARL_NONE 0
#define STARL_LEFT 0x0406
#define STARL_RIGHT 0x0407
/*STARL_FRONT					0x0404 */
/*STARL_BACK					0x0405 */
/*STARL_FRONT_AND_BACK				0x0408 */
#define STARL_FRONT_LEFT 0x0400
#define STARL_FRONT_RIGHT 0x0401
#define STARL_BACK_LEFT 0x0402
#define STARL_BACK_RIGHT 0x0403
#define STARL_AUX0 0x0409
#define STARL_AUX1 0x040A
#define STARL_AUX2 0x040B
#define STARL_AUX3 0x040C
#define STARL_COLOR_INDEX 0x1900
#define STARL_RED 0x1903
#define STARL_GREEN 0x1904
#define STARL_BLUE 0x1905
#define STARL_ALPHA 0x1906
#define STARL_LUMINANCE 0x1909
#define STARL_LUMINANCE_ALPHA 0x190A
#define STARL_ALPHA_BITS 0x0D55
#define STARL_RED_BITS 0x0D52
#define STARL_GREEN_BITS 0x0D53
#define STARL_BLUE_BITS 0x0D54
#define STARL_INDEX_BITS 0x0D51
#define STARL_SUBPIXEL_BITS 0x0D50
#define STARL_AUX_BUFFERS 0x0C00
#define STARL_READ_BUFFER 0x0C02
#define STARL_DRAW_BUFFER 0x0C01
#define STARL_DOUBLEBUFFER 0x0C32
#define STARL_STEREO 0x0C33
#define STARL_BITMAP 0x1A00
#define STARL_COLOR 0x1800
#define STARL_DEPTH 0x1801
#define STARL_STENCIL 0x1802
#define STARL_DITHER 0x0BD0
#define STARL_RGB 0x1907
#define STARL_RGBA 0x1908
#define STARL_RGBA32F 0x8814
#define STARL_RGB32F 0x8815
#define STARL_RGBA16F 0x881A
#define STARL_RGB16F 0x881B

/* Implementation limits */
#define STARL_MAX_LIST_NESTING 0x0B31
#define STARL_MAX_EVAL_ORDER 0x0D30
#define STARL_MAX_LIGHTS 0x0D31
#define STARL_MAX_CLIP_PLANES 0x0D32
#define STARL_MAX_TEXTURE_SIZE 0x0D33
#define STARL_MAX_PIXEL_MAP_TABLE 0x0D34
#define STARL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define STARL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define STARL_MAX_NAME_STACK_DEPTH 0x0D37
#define STARL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define STARL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define STARL_MAX_VIEWPORT_DIMS 0x0D3A
#define STARL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B

/* Gets */
#define STARL_ATTRIB_STACK_DEPTH 0x0BB0
#define STARL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define STARL_COLOR_CLEAR_VALUE 0x0C22
#define STARL_COLOR_WRITEMASK 0x0C23
#define STARL_CURRENT_INDEX 0x0B01
#define STARL_CURRENT_COLOR 0x0B00
#define STARL_CURRENT_NORMAL 0x0B02
#define STARL_CURRENT_RASTER_COLOR 0x0B04
#define STARL_CURRENT_RASTER_DISTANCE 0x0B09
#define STARL_CURRENT_RASTER_INDEX 0x0B05
#define STARL_CURRENT_RASTER_POSITION 0x0B07
#define STARL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define STARL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define STARL_CURRENT_TEXTURE_COORDS 0x0B03
#define STARL_INDEX_CLEAR_VALUE 0x0C20
#define STARL_INDEX_MODE 0x0C30
#define STARL_INDEX_WRITEMASK 0x0C21
#define STARL_MODELVIEW_MATRIX 0x0BA6
#define STARL_MODELVIEW_STACK_DEPTH 0x0BA3
#define STARL_NAME_STACK_DEPTH 0x0D70
#define STARL_PROJECTION_MATRIX 0x0BA7
#define STARL_PROJECTION_STACK_DEPTH 0x0BA4
#define STARL_RENDER_MODE 0x0C40
#define STARL_RGBA_MODE 0x0C31
#define STARL_TEXTURE_MATRIX 0x0BA8
#define STARL_TEXTURE_STACK_DEPTH 0x0BA5
#define STARL_VIEWPORT 0x0BA2

/* Evaluators */
#define STARL_AUTO_NORMAL 0x0D80
#define STARL_MAP1_COLOR_4 0x0D90
#define STARL_MAP1_INDEX 0x0D91
#define STARL_MAP1_NORMAL 0x0D92
#define STARL_MAP1_TEXTURE_COORD_1 0x0D93
#define STARL_MAP1_TEXTURE_COORD_2 0x0D94
#define STARL_MAP1_TEXTURE_COORD_3 0x0D95
#define STARL_MAP1_TEXTURE_COORD_4 0x0D96
#define STARL_MAP1_VERTEX_3 0x0D97
#define STARL_MAP1_VERTEX_4 0x0D98
#define STARL_MAP2_COLOR_4 0x0DB0
#define STARL_MAP2_INDEX 0x0DB1
#define STARL_MAP2_NORMAL 0x0DB2
#define STARL_MAP2_TEXTURE_COORD_1 0x0DB3
#define STARL_MAP2_TEXTURE_COORD_2 0x0DB4
#define STARL_MAP2_TEXTURE_COORD_3 0x0DB5
#define STARL_MAP2_TEXTURE_COORD_4 0x0DB6
#define STARL_MAP2_VERTEX_3 0x0DB7
#define STARL_MAP2_VERTEX_4 0x0DB8
#define STARL_MAP1_GRID_DOMAIN 0x0DD0
#define STARL_MAP1_GRID_SEGMENTS 0x0DD1
#define STARL_MAP2_GRID_DOMAIN 0x0DD2
#define STARL_MAP2_GRID_SEGMENTS 0x0DD3
#define STARL_COEFF 0x0A00
#define STARL_ORDER 0x0A01
#define STARL_DOMAIN 0x0A02

/* Hints */
#define STARL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define STARL_POINT_SMOOTH_HINT 0x0C51
#define STARL_LINE_SMOOTH_HINT 0x0C52
#define STARL_POLYGON_SMOOTH_HINT 0x0C53
#define STARL_FOG_HINT 0x0C54
#define STARL_DONT_CARE 0x1100
#define STARL_FASTEST 0x1101
#define STARL_NICEST 0x1102

/* Scissor box */
#define STARL_SCISSOR_BOX 0x0C10
#define STARL_SCISSOR_TEST 0x0C11

/* Pixel Mode / Transfer */
#define STARL_MAP_COLOR 0x0D10
#define STARL_MAP_STENCIL 0x0D11
#define STARL_INDEX_SHIFT 0x0D12
#define STARL_INDEX_OFFSET 0x0D13
#define STARL_RED_SCALE 0x0D14
#define STARL_RED_BIAS 0x0D15
#define STARL_GREEN_SCALE 0x0D18
#define STARL_GREEN_BIAS 0x0D19
#define STARL_BLUE_SCALE 0x0D1A
#define STARL_BLUE_BIAS 0x0D1B
#define STARL_ALPHA_SCALE 0x0D1C
#define STARL_ALPHA_BIAS 0x0D1D
#define STARL_DEPTH_SCALE 0x0D1E
#define STARL_DEPTH_BIAS 0x0D1F
#define STARL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define STARL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define STARL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define STARL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define STARL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define STARL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define STARL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define STARL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define STARL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define STARL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define STARL_PIXEL_MAP_S_TO_S 0x0C71
#define STARL_PIXEL_MAP_I_TO_I 0x0C70
#define STARL_PIXEL_MAP_I_TO_R 0x0C72
#define STARL_PIXEL_MAP_I_TO_G 0x0C73
#define STARL_PIXEL_MAP_I_TO_B 0x0C74
#define STARL_PIXEL_MAP_I_TO_A 0x0C75
#define STARL_PIXEL_MAP_R_TO_R 0x0C76
#define STARL_PIXEL_MAP_G_TO_G 0x0C77
#define STARL_PIXEL_MAP_B_TO_B 0x0C78
#define STARL_PIXEL_MAP_A_TO_A 0x0C79
#define STARL_PACK_ALIGNMENT 0x0D05
#define STARL_PACK_LSB_FIRST 0x0D01
#define STARL_PACK_ROW_LENGTH 0x0D02
#define STARL_PACK_SKIP_PIXELS 0x0D04
#define STARL_PACK_SKIP_ROWS 0x0D03
#define STARL_PACK_SWAP_BYTES 0x0D00
#define STARL_UNPACK_ALIGNMENT 0x0CF5
#define STARL_UNPACK_LSB_FIRST 0x0CF1
#define STARL_UNPACK_ROW_LENGTH 0x0CF2
#define STARL_UNPACK_SKIP_PIXELS 0x0CF4
#define STARL_UNPACK_SKIP_ROWS 0x0CF3
#define STARL_UNPACK_SWAP_BYTES 0x0CF0
#define STARL_ZOOM_X 0x0D16
#define STARL_ZOOM_Y 0x0D17

/* Texture mapping */
#define STARL_TEXTURE_ENV 0x2300
#define STARL_TEXTURE_ENV_MODE 0x2200
#define STARL_TEXTURE_1D 0x0DE0
#define STARL_TEXTURE_2D 0x0DE1
#define STARL_TEXTURE_WRAP_S 0x2802
#define STARL_TEXTURE_WRAP_T 0x2803
#define STARL_TEXTURE_MAG_FILTER 0x2800
#define STARL_TEXTURE_MIN_FILTER 0x2801
#define STARL_TEXTURE_ENV_COLOR 0x2201
#define STARL_TEXTURE_GEN_S 0x0C60
#define STARL_TEXTURE_GEN_T 0x0C61
#define STARL_TEXTURE_GEN_R 0x0C62
#define STARL_TEXTURE_GEN_Q 0x0C63
#define STARL_TEXTURE_GEN_MODE 0x2500
#define STARL_TEXTURE_BORDER_COLOR 0x1004
#define STARL_TEXTURE_WIDTH 0x1000
#define STARL_TEXTURE_HEIGHT 0x1001
#define STARL_TEXTURE_BORDER 0x1005
#define STARL_TEXTURE_COMPONENTS 0x1003
#define STARL_TEXTURE_RED_SIZE 0x805C
#define STARL_TEXTURE_GREEN_SIZE 0x805D
#define STARL_TEXTURE_BLUE_SIZE 0x805E
#define STARL_TEXTURE_ALPHA_SIZE 0x805F
#define STARL_TEXTURE_LUMINANCE_SIZE 0x8060
#define STARL_TEXTURE_INTENSITY_SIZE 0x8061
#define STARL_NEAREST_MIPMAP_NEAREST 0x2700
#define STARL_NEAREST_MIPMAP_LINEAR 0x2702
#define STARL_LINEAR_MIPMAP_NEAREST 0x2701
#define STARL_LINEAR_MIPMAP_LINEAR 0x2703
#define STARL_OBJECT_LINEAR 0x2401
#define STARL_OBJECT_PLANE 0x2501
#define STARL_EYE_LINEAR 0x2400
#define STARL_EYE_PLANE 0x2502
#define STARL_SPHERE_MAP 0x2402
#define STARL_DECAL 0x2101
#define STARL_MODULATE 0x2100
#define STARL_NEAREST 0x2600
#define STARL_REPEAT 0x2901
#define STARL_CLAMP 0x2900
#define STARL_S 0x2000
#define STARL_T 0x2001
#define STARL_R 0x2002
#define STARL_Q 0x2003

/* Utility */
#define STARL_VENDOR 0x1F00
#define STARL_RENDERER 0x1F01
#define STARL_VERSION 0x1F02
#define STARL_EXTENSIONS 0x1F03

/* Errors */
#define STARL_NO_ERROR 0
#define STARL_INVALID_ENUM 0x0500
#define STARL_INVALID_VALUE 0x0501
#define STARL_INVALID_OPERATION 0x0502
#define STARL_STACK_OVERFLOW 0x0503
#define STARL_STACK_UNDERFLOW 0x0504
#define STARL_OUT_OF_MEMORY 0x0505

/* STARLPush/PopAttrib bits */
#define STARL_CURRENT_BIT 0x00000001
#define STARL_POINT_BIT 0x00000002
#define STARL_LINE_BIT 0x00000004
#define STARL_POLYGON_BIT 0x00000008
#define STARL_POLYGON_STIPPLE_BIT 0x00000010
#define STARL_PIXEL_MODE_BIT 0x00000020
#define STARL_LIGHTING_BIT 0x00000040
#define STARL_FOG_BIT 0x00000080
#define STARL_DEPTH_BUFFER_BIT 0x00000100
#define STARL_ACCUM_BUFFER_BIT 0x00000200
#define STARL_STENCIL_BUFFER_BIT 0x00000400
#define STARL_VIEWPORT_BIT 0x00000800
#define STARL_TRANSFORM_BIT 0x00001000
#define STARL_ENABLE_BIT 0x00002000
#define STARL_COLOR_BUFFER_BIT 0x00004000
#define STARL_HINT_BIT 0x00008000
#define STARL_EVAL_BIT 0x00010000
#define STARL_LIST_BIT 0x00020000
#define STARL_TEXTURE_BIT 0x00040000
#define STARL_SCISSOR_BIT 0x00080000
#define STARL_ALL_ATTRIB_BITS 0xFFFFFFFF

/* OpenSTARL 1.1 */
#define STARL_PROXY_TEXTURE_1D 0x8063
#define STARL_PROXY_TEXTURE_2D 0x8064
#define STARL_TEXTURE_PRIORITY 0x8066
#define STARL_TEXTURE_RESIDENT 0x8067
#define STARL_TEXTURE_BINDING_1D 0x8068
#define STARL_TEXTURE_BINDING_2D 0x8069
#define STARL_TEXTURE_INTERNAL_FORMAT 0x1003
#define STARL_ALPHA4 0x803B
#define STARL_ALPHA8 0x803C
#define STARL_ALPHA12 0x803D
#define STARL_ALPHA16 0x803E
#define STARL_LUMINANCE4 0x803F
#define STARL_LUMINANCE8 0x8040
#define STARL_LUMINANCE12 0x8041
#define STARL_LUMINANCE16 0x8042
#define STARL_LUMINANCE4_ALPHA4 0x8043
#define STARL_LUMINANCE6_ALPHA2 0x8044
#define STARL_LUMINANCE8_ALPHA8 0x8045
#define STARL_LUMINANCE12_ALPHA4 0x8046
#define STARL_LUMINANCE12_ALPHA12 0x8047
#define STARL_LUMINANCE16_ALPHA16 0x8048
#define STARL_INTENSITY 0x8049
#define STARL_INTENSITY4 0x804A
#define STARL_INTENSITY8 0x804B
#define STARL_INTENSITY12 0x804C
#define STARL_INTENSITY16 0x804D
#define STARL_R3_G3_B2 0x2A10
#define STARL_RGB4 0x804F
#define STARL_RGB5 0x8050
#define STARL_RGB8 0x8051
#define STARL_RGB10 0x8052
#define STARL_RGB12 0x8053
#define STARL_RGB16 0x8054
#define STARL_RGBA2 0x8055
#define STARL_RGBA4 0x8056
#define STARL_RGB5_A1 0x8057
#define STARL_RGBA8 0x8058
#define STARL_RGB10_A2 0x8059
#define STARL_RGBA12 0x805A
#define STARL_RGBA16 0x805B
#define STARL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define STARL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define STARL_ALL_CLIENT_ATTRIB_BITS 0xFFFFFFFF
#define STARL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF
