#ifndef MP5_NATIVE_WIDESCREEN_H
#define MP5_NATIVE_WIDESCREEN_H

/*
 * Adaptive widescreen on the native port, after the Mario Party 4 port "Party Board"
 * (github.com/zeranemesis/Marioparty4, include/port/widescreen*.h*).
 *
 * The 640x480 EFB stays as is and Aurora's STRETCH policy scales it to the window,
 * so everything is drawn anamorphically: 3D cameras widen their aspect by
 * HuWideFactor() (Hor+, same vertical FOV) and 2D projections widen their ortho
 * width around the centre by the same factor. HUD x positions (0..576) are then
 * moved out towards the window edges by HuWideHudShift. On the GameCube the
 * macros expand to the original expressions.
 */

#ifdef TARGET_PC
#ifdef __cplusplus
extern "C" {
#endif

/* how many times wider than 4:3 the window is, 1..4 (48:9); 1 when widescreen is off,
 * the 4:3 lock is on or the window is narrower than 4:3 (then letterboxed) */
float HuWideFactor(void);
/* how far HUD x (0..576) moves towards the window edges */
float HuWideHudShift(float hudX);
/* HUD x over picture x (0..576 across the widened 3D picture), and back for HUD point (x, y) */
float HuWideHudX(float pictureX);
float HuWidePictureX(float hudX, float hudY);
/* per-scene fallback (hsfman.c): the cameras in cameraBit show only the 4:3 picture,
 * pillarboxed, for a scene whose geometry ends at the 4:3 edge; reset by Hu3DCameraCreate */
void Hu3DCameraPillarboxSet(int cameraBit, int onF);
/* a pillarboxed camera exists: the HUD uses the centred layout and 2D is clipped to 4:3 */
int Hu3DCameraPillarboxActive(void);
/* hsfman.c: the cameras in cameraBit keep the 4:3 projection (an image only read back by copies) */
void Hu3DCameraNoWideSet(int cameraBit, int onF);
int Hu3DCameraNoWideCheck(int cameraNo);
/* a sprite (or esprite) set over a 3D object, or drawn as a shape, stays put: the HUD
 * layout doesn't move it and it is never stretched as a backdrop */
void HuSprWideFixSet(int sprId);
void espWideFixSet(int espId);

#ifdef __cplusplus
}
#endif

#define HU_WIDE(aspect) ((aspect) * HuWideFactor())
/* left/right edges of a widened ortho over 0..w */
#define HU_WIDE_L(w) ((w) * 0.5f * (1.0f - HuWideFactor()))
#define HU_WIDE_R(w) ((w) * 0.5f * (1.0f + HuWideFactor()))
#define HU_WIDE_HUD_X(x) HuWideHudX(x)
#define HU_WIDE_PICTURE_X(x, y) HuWidePictureX(x, y)
#define HU_WIDE_PILLARBOX(cameraBit, onF) Hu3DCameraPillarboxSet(cameraBit, onF)
#define HU_WIDE_FIX_ESP(espId) espWideFixSet(espId)
#else
#define HU_WIDE(aspect) (aspect)
#define HU_WIDE_L(w) 0
#define HU_WIDE_R(w) (w)
#define HU_WIDE_HUD_X(x) (x)
#define HU_WIDE_PICTURE_X(x, y) (x)
#define HU_WIDE_PILLARBOX(cameraBit, onF)
#define HU_WIDE_FIX_ESP(espId)
#endif

#endif
