package com.smarthome.emulator.design;

import javax.swing.*;
import javax.swing.plaf.metal.MetalScrollBarUI;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * Created by Shu on 31.5.2014.
 */
public class JIOTScrollBarUI extends MetalScrollBarUI {
    private Image imageThumb, imageTrack;

    private final static int SCROLLBAR_WIDTH = 17;
    private final static int SCROLLBAR_HEIGHT = 30;
    private final static Color COLOR_THUMB = new Color(129,129,129);
    private final static Color COLOR_TRACK = new Color(219,219,219);
    private JButton b = new JButton() {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(0, 0);
        }

    };
    public JIOTScrollBarUI() {
        imageThumb = GraphicImage.create(SCROLLBAR_WIDTH, SCROLLBAR_HEIGHT, COLOR_THUMB);
        imageTrack = GraphicImage.create(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH, COLOR_TRACK);
    }

    @Override
    protected void paintThumb(Graphics g, JComponent c, Rectangle r) {
        g.setColor(Color.blue);
        ((Graphics2D) g).drawImage(imageThumb,
                r.x, r.y, r.width, r.height, null);
    }

    @Override
    protected void paintTrack(Graphics g, JComponent c, Rectangle r) {
        ((Graphics2D) g).drawImage(imageTrack,
                r.x, r.y, r.width, r.height, null);
    }

    @Override
    protected JButton createDecreaseButton(int orientation) {
        return b;
    }

    @Override
    protected JButton createIncreaseButton(int orientation) {
        return b;
    }

    private static class GraphicImage {
        static public Image create(int w, int h, Color c) {
            BufferedImage bi = new BufferedImage(
                    w, h, BufferedImage.TYPE_INT_ARGB);
            Graphics2D g2d = bi.createGraphics();
            g2d.setPaint(c);
            g2d.fillRect(0, 0, w, h);
            g2d.dispose();
            return bi;
        }
    }
}
