package com.smarthome.emulator.design;

/**
 * Created by Shu on 8.3.2014.
 */
import java.util.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;

public class JCustomTabbedPaneUI extends BasicTabbedPaneUI {

    private Color selectColor;
    private Color deSelectColor;
    private int inclTab = 4;
    private int anchoFocoV = inclTab;
    private int anchoFocoH = 4;
    private int anchoCarpetas = 18;
    private Polygon shape;
    private static Color borderColor = new Color(239,239,239);

    public static ComponentUI createUI(JComponent c) {
        return new JCustomTabbedPaneUI();
    }

    @Override
    protected void installDefaults() {
        super.installDefaults();
        selectColor = new Color(166,220,255);
        deSelectColor = new Color(198,198,198);
        tabAreaInsets.right = anchoCarpetas;
        highlight = new Color(239,239,239);
        lightHighlight = new Color(239,239,239);
        shadow = new Color(239,239,239);
        darkShadow = new Color(239,239,239);
        focus = new Color(239,239,239);
    }

    @Override
    protected void paintTabArea(Graphics g, int tabPlacement, int selectedIndex) {
        if (runCount > 1) {
            int lines[] = new int[runCount];
            for (int i = 0; i < runCount; i++) {
                lines[i] = rects[tabRuns[i]].y + (tabPlacement == TOP ? maxTabHeight : 0);
            }
            Arrays.sort(lines);
            if (tabPlacement == TOP) {
                int fila = runCount;
                for (int i = 0; i < lines.length - 1; i++, fila--) {
                    Polygon carp = new Polygon();
                    carp.addPoint(0, lines[i]);
                    carp.addPoint(tabPane.getWidth() - 2 * fila - 2, lines[i]);
                    carp.addPoint(tabPane.getWidth() - 2 * fila, lines[i] + 3);
                    if (i < lines.length - 2) {
                        carp.addPoint(tabPane.getWidth() - 2 * fila, lines[i + 1]);
                        carp.addPoint(0, lines[i + 1]);
                    } else {
                        carp.addPoint(tabPane.getWidth() - 2 * fila, lines[i] + rects[selectedIndex].height);
                        carp.addPoint(0, lines[i] + rects[selectedIndex].height);
                    }
                    carp.addPoint(0, lines[i]);
                    g.setColor(hazAlfa(fila));
                    g.fillPolygon(carp);
                    g.setColor(Color.RED);
                    g.drawPolygon(carp);
                }
            } else {
                int fila = 0;
                for (int i = 0; i < lines.length - 1; i++, fila++) {
                    Polygon carp = new Polygon();
                    carp.addPoint(0, lines[i]);
                    carp.addPoint(tabPane.getWidth() - 2 * fila - 1, lines[i]);
                    carp.addPoint(tabPane.getWidth() - 2 * fila - 1, lines[i + 1] - 3);
                    carp.addPoint(tabPane.getWidth() - 2 * fila - 3, lines[i + 1]);
                    carp.addPoint(0, lines[i + 1]);
                    carp.addPoint(0, lines[i]);
                    g.setColor(hazAlfa(fila + 2));
                    g.fillPolygon(carp);
                    g.setColor(darkShadow.darker());
                    g.drawPolygon(carp);
                }
            }
        }
        super.paintTabArea(g, tabPlacement, selectedIndex);
    }

    @Override
    protected void paintTabBackground(Graphics g, int tabPlacement, int tabIndex, int x, int y, int w, int h, boolean isSelected) {
        Graphics2D g2D = (Graphics2D) g;
        GradientPaint gradientShadow;
        int xp[] = null;
        int yp[] = null;
        switch (tabPlacement) {
            case LEFT:
                xp = new int[]{x, x, x + w, x + w, x};
                yp = new int[]{y, y + h - 3, y + h - 3, y, y};
                gradientShadow = new GradientPaint(x, y, new Color(100, 100, 255), x, y + h, Color.ORANGE);
                break;
            case RIGHT:
                xp = new int[]{x, x, x + w - 2, x + w - 2, x};
                yp = new int[]{y, y + h - 3, y + h - 3, y, y};
                gradientShadow = new GradientPaint(x, y, new Color(100, 100, 255), x, y + h, new Color(153, 186, 243));
                break;
            case BOTTOM:
                xp = new int[]{x, x, x + 3, x + w - inclTab - 6, x + w - inclTab - 2, x + w - inclTab, x + w - 3, x};
                yp = new int[]{y, y + h - 3, y + h, y + h, y + h - 1, y + h - 3, y, y};
                gradientShadow = new GradientPaint(x, y, new Color(100, 100, 255), x, y + h, Color.BLUE);
                break;
            case TOP:
            default:
                xp = new int[]{x, x, x, x + w - inclTab, x + w - inclTab, x + w - inclTab, x + w - inclTab, x};
                yp = new int[]{y + h, y, y, y, y , y, y + h, y + h};
                gradientShadow = new GradientPaint(0, 0, new Color(166,220,255), 0, y + h / 2, new Color(166,220,255));
                break;
        }
        // ;
        shape = new Polygon(xp, yp, xp.length);
        if (isSelected) {
            g2D.setColor(selectColor);
            g2D.setPaint(gradientShadow);
        } else {
            if (tabPane.isEnabled() && tabPane.isEnabledAt(tabIndex)) {
                g2D.setColor(deSelectColor);
                GradientPaint gradientShadowTmp = new GradientPaint(0, 0, new Color(198,198,198), 0, y + h / 2, new Color(198,198,198));
                g2D.setPaint(gradientShadowTmp);
            } else {
                GradientPaint gradientShadowTmp = new GradientPaint(0, 0, new Color(198,198,198), 0, y + 15 + h / 2, new Color(198, 198, 198));
                g2D.setPaint(gradientShadowTmp);
            }
        }
        //selectColor = new Color(255, 255, 200);
        //deSelectColor = new Color(240, 255, 210);
        g2D.fill(shape);
        if (runCount > 1) {
            g2D.setColor(hazAlfa(getRunForTab(tabPane.getTabCount(), tabIndex) - 1));
            g2D.fill(shape);
        }
        g2D.fill(shape);
    }

    @Override
    protected int calculateTabWidth(int tabPlacement, int tabIndex, FontMetrics metrics) {
        return 20 + inclTab + super.calculateTabWidth(tabPlacement, tabIndex, metrics);
    }

    @Override
    protected int calculateTabHeight(int tabPlacement, int tabIndex, int fontHeight) {
        if (tabPlacement == LEFT || tabPlacement == RIGHT) {
            return super.calculateTabHeight(tabPlacement, tabIndex, fontHeight);
        } else {
            return anchoFocoH + super.calculateTabHeight(tabPlacement, tabIndex, fontHeight);
        }
    }

    @Override
    protected void paintTabBorder(Graphics g, int tabPlacement, int tabIndex, int x, int y, int w, int h, boolean isSelected) {
    }

    @Override
    protected void paintFocusIndicator(Graphics g, int tabPlacement, Rectangle[] rects, int tabIndex, Rectangle iconRect, Rectangle textRect, boolean isSelected) {
        if (tabPane.hasFocus() && isSelected) {
            g.setColor(UIManager.getColor("ScrollBar.thumbShadow"));
            g.drawPolygon(shape);
        }
    }

    protected Color hazAlfa(int fila) {
        int alfa = 0;
        if (fila >= 0) {
            alfa = 50 + (fila > 7 ? 70 : 10 * fila);
        }
        return new Color(0, 0, 0, alfa);
    }
}
