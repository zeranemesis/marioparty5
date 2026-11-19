package io.github.zeranemesis.mp5native;

import android.app.Presentation;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.hardware.display.DisplayManager;
import android.os.Bundle;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;

/**
 * Second screen (the AYN Thor's bottom display): the board seen from above, rendered live by the game's engine
 * (src/port/board_screen.cpp), with the players as tokens, their stars and coins, and a Settings button. Styled
 * like MP5 Native's own menus (purple panels, gold accents).
 */
public class BoardScreen extends Presentation {
    // Layout of the native snapshot, see board_screen.cpp.
    private static final int W = 620, H = 540, HEADER = 6, PLAYER = 7, PLAYERS = 4;

    private static native void nativeSetActive(boolean on);
    private static native boolean nativeFrame(float[] info, int[] pixels, int lastFrame);
    private static native void nativeOpenMenu();

    /** The first presentation display (the bottom screen), or null on a single-screen device. */
    public static Display findDisplay(Context context) {
        DisplayManager dm = (DisplayManager) context.getSystemService(Context.DISPLAY_SERVICE);
        Display[] displays = dm.getDisplays(DisplayManager.DISPLAY_CATEGORY_PRESENTATION);
        return displays.length > 0 ? displays[0] : null;
    }

    public BoardScreen(Context context, Display display) {
        super(context, display);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new BoardView(getContext()));
    }

    @Override
    protected void onStart() {
        super.onStart();
        nativeSetActive(true);
    }

    @Override
    protected void onStop() {
        nativeSetActive(false);
        super.onStop();
    }

    private static final class BoardView extends View {
        private static final int PURPLE_TOP = Color.rgb(104, 64, 196), PURPLE_BOTTOM = Color.rgb(58, 30, 128);
        private static final int GOLD = Color.rgb(255, 204, 51), CREAM = Color.rgb(255, 244, 214);

        private final float[] info = new float[HEADER + PLAYERS * PLAYER];
        private final int[] pixels = new int[W * H];
        private final Bitmap view = Bitmap.createBitmap(W, H, Bitmap.Config.ARGB_8888);
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        private final Paint bitmapPaint = new Paint(Paint.FILTER_BITMAP_FLAG);
        private final Path path = new Path();
        private final RectF rect = new RectF();
        private final RectF button = new RectF();
        private final Rect dst = new Rect();
        private int frame = -1;
        private boolean hasView;

        BoardView(Context context) {
            super(context);
            setBackgroundColor(Color.rgb(24, 14, 58));
            paint.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        }

        @Override
        protected void onDraw(Canvas canvas) {
            postInvalidateDelayed(33);
            final int w = getWidth(), h = getHeight();
            if (nativeFrame(info, pixels, frame)) {
                view.setPixels(pixels, 0, W, 0, 0, W, H);
                frame = (int) info[1];
                hasView = true;
            }
            final boolean onBoard = info[0] != 0;

            if (onBoard && hasView) {
                dst.set(0, 0, w, h);
                canvas.drawBitmap(view, null, dst, bitmapPaint);
                drawTokens(canvas, w, h);
                drawScores(canvas, w, h);
            } else {
                paint.setShader(new LinearGradient(0, 0, 0, h, PURPLE_TOP, PURPLE_BOTTOM, Shader.TileMode.CLAMP));
                canvas.drawRect(0, 0, w, h, paint);
                paint.setShader(null);
                paint.setColor(GOLD);
                paint.setTextAlign(Paint.Align.CENTER);
                paint.setTextSize(h * 0.08f);
                canvas.drawText("MP5 NATIVE", w / 2f, h * 0.42f, paint);
            }
            drawButton(canvas, w, h);
        }

        private void drawTokens(Canvas canvas, int w, int h) {
            final int turn = (int) info[4];
            final float r = h * 0.035f;
            for (int k = 0; k <= PLAYERS; k++) { // the player whose turn it is last, on top
                int p = k < PLAYERS ? k : turn;
                if (p < 0 || p >= PLAYERS || (k < PLAYERS && p == turn)) continue;
                int o = HEADER + p * PLAYER;
                if (info[o] < 0) continue;
                float x = info[o] * w, y = info[o + 1] * h;
                paint.setShadowLayer(r * 0.3f, 0, r * 0.15f, Color.argb(160, 0, 0, 0));
                paint.setStyle(Paint.Style.FILL);
                paint.setColor(p == turn ? GOLD : CREAM);
                canvas.drawCircle(x, y, r * 1.2f, paint);
                paint.clearShadowLayer();
                paint.setColor(playerColor(o));
                canvas.drawCircle(x, y, r, paint);
                paint.setColor(Color.WHITE);
                paint.setTextAlign(Paint.Align.CENTER);
                paint.setTextSize(r * 1.1f);
                paint.setShadowLayer(2f, 0, 1f, Color.argb(200, 0, 0, 0));
                canvas.drawText(String.valueOf(p + 1), x, y + r * 0.38f, paint);
                paint.clearShadowLayer();
            }
        }

        private int playerColor(int o) {
            return Color.rgb((int) info[o + 2], (int) info[o + 3], (int) info[o + 4]);
        }

        private void drawScores(Canvas canvas, int w, int h) {
            final float pad = h * 0.02f, cardW = (w - pad * 5) / 4f, cardH = h * 0.11f;
            final int turn = (int) info[4];
            for (int p = 0; p < PLAYERS; p++) {
                int o = HEADER + p * PLAYER;
                float left = pad + p * (cardW + pad), top = pad;
                rect.set(left, top, left + cardW, top + cardH);
                paint.setStyle(Paint.Style.FILL);
                paint.setShader(new LinearGradient(0, top, 0, top + cardH, PURPLE_TOP, PURPLE_BOTTOM,
                                                   Shader.TileMode.CLAMP));
                canvas.drawRoundRect(rect, cardH * 0.3f, cardH * 0.3f, paint);
                paint.setShader(null);
                paint.setStyle(Paint.Style.STROKE);
                paint.setStrokeWidth(cardH * 0.05f);
                paint.setColor(p == turn ? GOLD : CREAM);
                canvas.drawRoundRect(rect, cardH * 0.3f, cardH * 0.3f, paint);

                paint.setStyle(Paint.Style.FILL);
                float cy = top + cardH / 2f, dot = cardH * 0.28f;
                paint.setColor(playerColor(o));
                canvas.drawCircle(left + cardH * 0.45f, cy, dot, paint);
                paint.setColor(Color.WHITE);
                paint.setTextAlign(Paint.Align.CENTER);
                paint.setTextSize(dot * 1.1f);
                canvas.drawText(String.valueOf(p + 1), left + cardH * 0.45f, cy + dot * 0.38f, paint);

                float tx = left + cardH * 0.95f;
                drawStar(canvas, tx, cy - cardH * 0.17f, cardH * 0.13f);
                drawCoin(canvas, tx, cy + cardH * 0.2f, cardH * 0.1f);
                paint.setColor(CREAM);
                paint.setTextAlign(Paint.Align.LEFT);
                paint.setTextSize(cardH * 0.28f);
                canvas.drawText(String.valueOf((int) info[o + 6]), tx + cardH * 0.2f, cy - cardH * 0.07f, paint);
                canvas.drawText(String.valueOf((int) info[o + 5]), tx + cardH * 0.2f, cy + cardH * 0.3f, paint);
            }
            // turn counter
            String turnText = "TURN " + (int) info[2] + " / " + (int) info[3];
            paint.setTextSize(h * 0.04f);
            paint.setTextAlign(Paint.Align.LEFT);
            float tw = paint.measureText(turnText);
            rect.set(pad, h - pad - h * 0.08f, pad * 3 + tw, h - pad);
            paint.setColor(Color.argb(200, 40, 20, 90));
            canvas.drawRoundRect(rect, rect.height() / 2, rect.height() / 2, paint);
            paint.setColor(GOLD);
            canvas.drawText(turnText, pad * 2, rect.centerY() + h * 0.014f, paint);
        }

        private void drawButton(Canvas canvas, int w, int h) {
            final float bh = h * 0.08f, bw = h * 0.3f, pad = h * 0.02f;
            button.set(w - pad - bw, h - pad - bh, w - pad, h - pad);
            paint.setStyle(Paint.Style.FILL);
            paint.setShader(new LinearGradient(0, button.top, 0, button.bottom, Color.rgb(255, 222, 99),
                                               Color.rgb(240, 170, 20), Shader.TileMode.CLAMP));
            paint.setShadowLayer(bh * 0.15f, 0, bh * 0.06f, Color.argb(160, 0, 0, 0));
            canvas.drawRoundRect(button, bh / 2, bh / 2, paint);
            paint.clearShadowLayer();
            paint.setShader(null);
            paint.setColor(Color.rgb(70, 30, 110));
            paint.setTextAlign(Paint.Align.CENTER);
            paint.setTextSize(bh * 0.45f);
            canvas.drawText("SETTINGS", button.centerX(), button.centerY() + bh * 0.16f, paint);
        }

        private void drawStar(Canvas canvas, float cx, float cy, float r) {
            path.reset();
            for (int i = 0; i < 10; i++) {
                double a = -Math.PI / 2 + i * Math.PI / 5;
                float rr = i % 2 == 0 ? r : r * 0.45f;
                float x = cx + (float) Math.cos(a) * rr, y = cy + (float) Math.sin(a) * rr;
                if (i == 0) path.moveTo(x, y); else path.lineTo(x, y);
            }
            path.close();
            paint.setStyle(Paint.Style.FILL);
            paint.setColor(GOLD);
            canvas.drawPath(path, paint);
        }

        private void drawCoin(Canvas canvas, float cx, float cy, float r) {
            paint.setStyle(Paint.Style.FILL);
            paint.setColor(Color.rgb(240, 170, 20));
            canvas.drawCircle(cx, cy, r, paint);
            paint.setColor(GOLD);
            canvas.drawCircle(cx, cy, r * 0.7f, paint);
        }

        @Override
        public boolean onTouchEvent(MotionEvent e) {
            if (e.getAction() == MotionEvent.ACTION_UP && button.contains(e.getX(), e.getY())) {
                nativeOpenMenu();
                performClick();
            }
            return true;
        }

        @Override
        public boolean performClick() {
            return super.performClick();
        }
    }
}
