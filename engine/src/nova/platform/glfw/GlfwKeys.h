/* The unknown key */
#define NOVA_KEY_UNKNOWN -1

/* Printable keys */
#define NOVA_KEY_SPACE 32
#define NOVA_KEY_APOSTROPHE 39 /* ' */
#define NOVA_KEY_COMMA 44      /* , */
#define NOVA_KEY_MINUS 45      /* - */
#define NOVA_KEY_PERIOD 46     /* . */
#define NOVA_KEY_SLASH 47      /* / */
#define NOVA_KEY_0 48
#define NOVA_KEY_1 49
#define NOVA_KEY_2 50
#define NOVA_KEY_3 51
#define NOVA_KEY_4 52
#define NOVA_KEY_5 53
#define NOVA_KEY_6 54
#define NOVA_KEY_7 55
#define NOVA_KEY_8 56
#define NOVA_KEY_9 57
#define NOVA_KEY_SEMICOLON 59 /* ; */
#define NOVA_KEY_EQUAL 61     /* = */
#define NOVA_KEY_A 65
#define NOVA_KEY_B 66
#define NOVA_KEY_C 67
#define NOVA_KEY_D 68
#define NOVA_KEY_E 69
#define NOVA_KEY_F 70
#define NOVA_KEY_G 71
#define NOVA_KEY_H 72
#define NOVA_KEY_I 73
#define NOVA_KEY_J 74
#define NOVA_KEY_K 75
#define NOVA_KEY_L 76
#define NOVA_KEY_M 77
#define NOVA_KEY_N 78
#define NOVA_KEY_O 79
#define NOVA_KEY_P 80
#define NOVA_KEY_Q 81
#define NOVA_KEY_R 82
#define NOVA_KEY_S 83
#define NOVA_KEY_T 84
#define NOVA_KEY_U 85
#define NOVA_KEY_V 86
#define NOVA_KEY_W 87
#define NOVA_KEY_X 88
#define NOVA_KEY_Y 89
#define NOVA_KEY_Z 90
#define NOVA_KEY_LEFT_BRACKET 91  /* [ */
#define NOVA_KEY_BACKSLASH 92     /* \ */
#define NOVA_KEY_RIGHT_BRACKET 93 /* ] */
#define NOVA_KEY_GRAVE_ACCENT 96  /* ` */
#define NOVA_KEY_WORLD_1 161      /* non-US #1 */
#define NOVA_KEY_WORLD_2 162      /* non-US #2 */

/* Function keys */
#define NOVA_KEY_ESCAPE 256
#define NOVA_KEY_ENTER 257
#define NOVA_KEY_TAB 258
#define NOVA_KEY_BACKSPACE 259
#define NOVA_KEY_INSERT 260
#define NOVA_KEY_DELETE 261
#define NOVA_KEY_RIGHT 262
#define NOVA_KEY_LEFT 263
#define NOVA_KEY_DOWN 264
#define NOVA_KEY_UP 265
#define NOVA_KEY_PAGE_UP 266
#define NOVA_KEY_PAGE_DOWN 267
#define NOVA_KEY_HOME 268
#define NOVA_KEY_END 269
#define NOVA_KEY_CAPS_LOCK 280
#define NOVA_KEY_SCROLL_LOCK 281
#define NOVA_KEY_NUM_LOCK 282
#define NOVA_KEY_PRINT_SCREEN 283
#define NOVA_KEY_PAUSE 284
#define NOVA_KEY_F1 290
#define NOVA_KEY_F2 291
#define NOVA_KEY_F3 292
#define NOVA_KEY_F4 293
#define NOVA_KEY_F5 294
#define NOVA_KEY_F6 295
#define NOVA_KEY_F7 296
#define NOVA_KEY_F8 297
#define NOVA_KEY_F9 298
#define NOVA_KEY_F10 299
#define NOVA_KEY_F11 300
#define NOVA_KEY_F12 301
#define NOVA_KEY_F13 302
#define NOVA_KEY_F14 303
#define NOVA_KEY_F15 304
#define NOVA_KEY_F16 305
#define NOVA_KEY_F17 306
#define NOVA_KEY_F18 307
#define NOVA_KEY_F19 308
#define NOVA_KEY_F20 309
#define NOVA_KEY_F21 310
#define NOVA_KEY_F22 311
#define NOVA_KEY_F23 312
#define NOVA_KEY_F24 313
#define NOVA_KEY_F25 314
#define NOVA_KEY_KP_0 320
#define NOVA_KEY_KP_1 321
#define NOVA_KEY_KP_2 322
#define NOVA_KEY_KP_3 323
#define NOVA_KEY_KP_4 324
#define NOVA_KEY_KP_5 325
#define NOVA_KEY_KP_6 326
#define NOVA_KEY_KP_7 327
#define NOVA_KEY_KP_8 328
#define NOVA_KEY_KP_9 329
#define NOVA_KEY_KP_DECIMAL 330
#define NOVA_KEY_KP_DIVIDE 331
#define NOVA_KEY_KP_MULTIPLY 332
#define NOVA_KEY_KP_SUBTRACT 333
#define NOVA_KEY_KP_ADD 334
#define NOVA_KEY_KP_ENTER 335
#define NOVA_KEY_KP_EQUAL 336
#define NOVA_KEY_LEFT_SHIFT 340
#define NOVA_KEY_LEFT_CONTROL 341
#define NOVA_KEY_LEFT_ALT 342
#define NOVA_KEY_LEFT_SUPER 343
#define NOVA_KEY_RIGHT_SHIFT 344
#define NOVA_KEY_RIGHT_CONTROL 345
#define NOVA_KEY_RIGHT_ALT 346
#define NOVA_KEY_RIGHT_SUPER 347
#define NOVA_KEY_MENU 348

#define NOVA_KEY_LAST NOVA_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define NOVA_MOD_SHIFT 0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 *
 *  If this bit is set one or more Control keys were held down.
 */
#define NOVA_MOD_CONTROL 0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 *
 *  If this bit is set one or more Alt keys were held down.
 */
#define NOVA_MOD_ALT 0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 *
 *  If this bit is set one or more Super keys were held down.
 */
#define NOVA_MOD_SUPER 0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
 *
 *  If this bit is set the Caps Lock key is enabled and the @ref
 *  NOVA_LOCK_KEY_MODS input mode is set.
 */
#define NOVA_MOD_CAPS_LOCK 0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
 *
 *  If this bit is set the Num Lock key is enabled and the @ref
 *  NOVA_LOCK_KEY_MODS input mode is set.
 */
#define NOVA_MOD_NUM_LOCK 0x0020

/*! @} */

/*! @defgroup buttons Mouse buttons
 *  @brief Mouse button IDs.
 *
 *  See [mouse button input](@ref input_mouse_button) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define NOVA_MOUSE_BUTTON_1 0
#define NOVA_MOUSE_BUTTON_2 1
#define NOVA_MOUSE_BUTTON_3 2
#define NOVA_MOUSE_BUTTON_4 3
#define NOVA_MOUSE_BUTTON_5 4
#define NOVA_MOUSE_BUTTON_6 5
#define NOVA_MOUSE_BUTTON_7 6
#define NOVA_MOUSE_BUTTON_8 7
#define NOVA_MOUSE_BUTTON_LAST NOVA_MOUSE_BUTTON_8
#define NOVA_MOUSE_BUTTON_LEFT NOVA_MOUSE_BUTTON_1
#define NOVA_MOUSE_BUTTON_RIGHT NOVA_MOUSE_BUTTON_2
#define NOVA_MOUSE_BUTTON_MIDDLE NOVA_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
 *  @brief Joystick IDs.
 *
 *  See [joystick input](@ref joystick) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define NOVA_JOYSTICK_1 0
#define NOVA_JOYSTICK_2 1
#define NOVA_JOYSTICK_3 2
#define NOVA_JOYSTICK_4 3
#define NOVA_JOYSTICK_5 4
#define NOVA_JOYSTICK_6 5
#define NOVA_JOYSTICK_7 6
#define NOVA_JOYSTICK_8 7
#define NOVA_JOYSTICK_9 8
#define NOVA_JOYSTICK_10 9
#define NOVA_JOYSTICK_11 10
#define NOVA_JOYSTICK_12 11
#define NOVA_JOYSTICK_13 12
#define NOVA_JOYSTICK_14 13
#define NOVA_JOYSTICK_15 14
#define NOVA_JOYSTICK_16 15
#define NOVA_JOYSTICK_LAST NOVA_JOYSTICK_16
/*! @} */

/*! @defgroup gamepad_buttons Gamepad buttons
 *  @brief Gamepad buttons.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define NOVA_GAMEPAD_BUTTON_A 0
#define NOVA_GAMEPAD_BUTTON_B 1
#define NOVA_GAMEPAD_BUTTON_X 2
#define NOVA_GAMEPAD_BUTTON_Y 3
#define NOVA_GAMEPAD_BUTTON_LEFT_BUMPER 4
#define NOVA_GAMEPAD_BUTTON_RIGHT_BUMPER 5
#define NOVA_GAMEPAD_BUTTON_BACK 6
#define NOVA_GAMEPAD_BUTTON_START 7
#define NOVA_GAMEPAD_BUTTON_GuiDE 8
#define NOVA_GAMEPAD_BUTTON_LEFT_THUMB 9
#define NOVA_GAMEPAD_BUTTON_RIGHT_THUMB 10
#define NOVA_GAMEPAD_BUTTON_DPAD_UP 11
#define NOVA_GAMEPAD_BUTTON_DPAD_RIGHT 12
#define NOVA_GAMEPAD_BUTTON_DPAD_DOWN 13
#define NOVA_GAMEPAD_BUTTON_DPAD_LEFT 14
#define NOVA_GAMEPAD_BUTTON_LAST NOVA_GAMEPAD_BUTTON_DPAD_LEFT

#define NOVA_GAMEPAD_BUTTON_CROSS NOVA_GAMEPAD_BUTTON_A
#define NOVA_GAMEPAD_BUTTON_CIRCLE NOVA_GAMEPAD_BUTTON_B
#define NOVA_GAMEPAD_BUTTON_SQUARE NOVA_GAMEPAD_BUTTON_X
#define NOVA_GAMEPAD_BUTTON_TRIANGLE NOVA_GAMEPAD_BUTTON_Y
/*! @} */

/*! @defgroup gamepad_axes Gamepad axes
 *  @brief Gamepad axes.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define NOVA_GAMEPAD_AXIS_LEFT_X 0
#define NOVA_GAMEPAD_AXIS_LEFT_Y 1
#define NOVA_GAMEPAD_AXIS_RIGHT_X 2
#define NOVA_GAMEPAD_AXIS_RIGHT_Y 3
#define NOVA_GAMEPAD_AXIS_LEFT_TRIGGER 4
#define NOVA_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define NOVA_GAMEPAD_AXIS_LAST NOVA_GAMEPAD_AXIS_RIGHT_TRIGGER
