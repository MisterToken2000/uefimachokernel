#include "../../gnu-efi/inc/efi.h"
#include "../../gnu-efi/inc/efilib.h"

extern void ScrollUp(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 bg_color);
extern UINT32 ScreenHeight;
extern UINT32 ScreenWidth;

extern UINT16 unknown_symbol[16];
extern UINT16 space[16];
extern UINT16 a_upper[16];
extern UINT16 a_lower[16];
extern UINT16 b_upper[16];
extern UINT16 b_lower[16];
extern UINT16 c_upper[16];
extern UINT16 c_lower[16];
extern UINT16 d_upper[16];
extern UINT16 d_lower[16];
extern UINT16 e_upper[16];
extern UINT16 e_lower[16];
extern UINT16 f_upper[16];
extern UINT16 f_lower[16];
extern UINT16 g_upper[16];
extern UINT16 g_lower[16];
extern UINT16 h_upper[16];
extern UINT16 h_lower[16];
extern UINT16 i_upper[16];
extern UINT16 i_lower[16];
extern UINT16 j_upper[16];
extern UINT16 j_lower[16];
extern UINT16 k_upper[16];
extern UINT16 k_lower[16];
extern UINT16 l_upper[16];
extern UINT16 l_lower[16];
extern UINT16 m_upper[16];
extern UINT16 m_lower[16];
extern UINT16 n_upper[16];
extern UINT16 n_lower[16];
extern UINT16 o_upper[16];
extern UINT16 o_lower[16];
extern UINT16 p_upper[16];
extern UINT16 p_lower[16];
extern UINT16 q_upper[16];
extern UINT16 q_lower[16];
extern UINT16 r_upper[16];
extern UINT16 r_lower[16];
extern UINT16 s_upper[16];
extern UINT16 s_lower[16];
extern UINT16 t_upper[16];
extern UINT16 t_lower[16];
extern UINT16 u_upper[16];
extern UINT16 u_lower[16];
extern UINT16 v_upper[16];
extern UINT16 v_lower[16];
extern UINT16 w_upper[16];
extern UINT16 w_lower[16];
extern UINT16 x_upper[16];
extern UINT16 x_lower[16];
extern UINT16 y_upper[16];
extern UINT16 y_lower[16];
extern UINT16 z_upper[16];
extern UINT16 z_lower[16];
extern UINT16 null_number[16];
extern UINT16 one_number[16];
extern UINT16 two_number[16];
extern UINT16 three_number[16];
extern UINT16 four_number[16];
extern UINT16 five_number[16];
extern UINT16 six_number[16];
extern UINT16 seven_number[16];
extern UINT16 eight_number[16];
extern UINT16 nine_number[16];
extern UINT16 comma_symbol[16];
extern UINT16 dot_symbol[16];
extern UINT16 exclamation_mark_symbol[16];
extern UINT16 question_mark_symbol[16];
extern UINT16 colon_symbol[16];
extern UINT16 hyphen_symbol[16];

UINTN Index = 0;

UINTN Pixels;

UINT32 y;

EFI_STATUS Draw_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, CHAR16 Char, UINT32 x, UINT32 y, UINT32 Color) {
    UINT32 Pitch = GraphicsOutput.Mode->Info->PixelsPerScanLine;
    UINT16 *char_bitmap = space;

    if (Char == ' ') char_bitmap = space;
    else if (Char == 'A') char_bitmap = a_upper;
    else if (Char == 'a') char_bitmap = a_lower;
    else if (Char == 'B') char_bitmap = b_upper;
    else if (Char == 'b') char_bitmap = b_lower;
    else if (Char == 'C') char_bitmap = c_upper;
    else if (Char == 'c') char_bitmap = c_lower;
    else if (Char == 'D') char_bitmap = d_upper;
    else if (Char == 'd') char_bitmap = d_lower;
    else if (Char == 'E') char_bitmap = e_upper;
    else if (Char == 'e') char_bitmap = e_lower;
    else if (Char == 'F') char_bitmap = f_upper;
    else if (Char == 'f') char_bitmap = f_lower;
    else if (Char == 'G') char_bitmap = g_upper;
    else if (Char == 'g') char_bitmap = g_lower;
    else if (Char == 'H') char_bitmap = h_upper;
    else if (Char == 'h') char_bitmap = h_lower;
    else if (Char == 'I') char_bitmap = i_upper;
    else if (Char == 'i') char_bitmap = i_lower;
    else if (Char == 'J') char_bitmap = j_upper;
    else if (Char == 'j') char_bitmap = j_lower;
    else if (Char == 'K') char_bitmap = k_upper;
    else if (Char == 'k') char_bitmap = k_lower;
    else if (Char == 'L') char_bitmap = l_upper;
    else if (Char == 'l') char_bitmap = l_lower;
    else if (Char == 'M') char_bitmap = m_upper;
    else if (Char == 'm') char_bitmap = m_lower;
    else if (Char == 'N') char_bitmap = n_upper;
    else if (Char == 'n') char_bitmap = n_lower;
    else if (Char == 'O') char_bitmap = o_upper;
    else if (Char == 'o') char_bitmap = o_lower;
    else if (Char == 'P') char_bitmap = p_upper;
    else if (Char == 'p') char_bitmap = p_lower;
    else if (Char == 'Q') char_bitmap = q_upper;
    else if (Char == 'q') char_bitmap = q_lower;
    else if (Char == 'R') char_bitmap = r_upper;
    else if (Char == 'r') char_bitmap = r_lower;
    else if (Char == 'S') char_bitmap = s_upper;
    else if (Char == 's') char_bitmap = s_lower;
    else if (Char == 'T') char_bitmap = t_upper;
    else if (Char == 't') char_bitmap = t_lower;
    else if (Char == 'U') char_bitmap = u_upper;
    else if (Char == 'u') char_bitmap = u_lower;
    else if (Char == 'V') char_bitmap = v_upper;
    else if (Char == 'v') char_bitmap = v_lower;
    else if (Char == 'W') char_bitmap = w_upper;
    else if (Char == 'w') char_bitmap = w_lower;
    else if (Char == 'X') char_bitmap = x_upper;
    else if (Char == 'x') char_bitmap = x_lower;
    else if (Char == 'Y') char_bitmap = y_upper;
    else if (Char == 'y') char_bitmap = y_lower;
    else if (Char == 'Z') char_bitmap = z_upper;
    else if (Char == 'z') char_bitmap = z_lower;
    else if (Char == '0') char_bitmap = null_number;
    else if (Char == '1') char_bitmap = one_number;
    else if (Char == '2') char_bitmap = two_number;
    else if (Char == '3') char_bitmap = three_number;
    else if (Char == '4') char_bitmap = four_number;
    else if (Char == '5') char_bitmap = five_number;
    else if (Char == '6') char_bitmap = six_number;
    else if (Char == '7') char_bitmap = seven_number;
    else if (Char == '8') char_bitmap = eight_number;
    else if (Char == '9') char_bitmap = nine_number;
    else if (Char == ',') char_bitmap = comma_symbol;
    else if (Char == '.') char_bitmap = dot_symbol;
    else if (Char == '?') char_bitmap = question_mark_symbol;
    else if (Char == '!') char_bitmap = exclamation_mark_symbol;
    else if (Char == ':') char_bitmap = colon_symbol;
    else if (Char == '-') char_bitmap = hyphen_symbol;
    else char_bitmap = unknown_symbol;

    for (UINTN Row = 0; Row < 16; Row++) {
        for (UINTN Col = 0; Col < 16; Col++) {
            if (char_bitmap[Row] & (2 << (14 - Col))) {
                UINT32 *Pixel = (UINT32 *)GraphicsOutput.Mode->FrameBufferBase;
                Pixel[(y + Row) * Pitch + (x + Col)] = Color;
            }
        }
    }

    return EFI_SUCCESS;
}

EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color) {
    Pixels = 0;
    for (int i = 0; Text[i] != '\0'; i++) {
	if (Text[i] == '\n') {
                Line += 18;
	        y += 18;
                Index = 0;
		continue;
        } else {
        	Draw_Unicode(GraphicsOutput, Text[i], Index, Line, Color);
        	Index += 8;
		    Pixels += 8;
	}
    }
    return EFI_SUCCESS;
}
