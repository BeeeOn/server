package com.smarthome.emulator.design.tree;

import javax.swing.*;
import java.awt.*;

/**
 * Created by Shu on 28.5.2014.
 */
public class JIOTTree extends JTree {

    private static final Color selectedColor = new Color(-5841665);
    private static final Color selectedBorderColor = new Color(-5841665);
    protected void paintComponent(Graphics g) {

        // paint background
        g.setColor(getBackground());
        g.fillRect(0, 0, getWidth(), getHeight());

        // paint selected node's background and border
        int fromRow = getRowForPath( getSelectionPath());
        if (fromRow != -1) {
            int toRow = fromRow + 1;
            Rectangle fromBounds = getRowBounds(fromRow);
            Rectangle toBounds = getRowBounds(toRow - 1);
            if (fromBounds != null && toBounds != null) {
                g.setColor(selectedColor);
                g.fillRect(0, fromBounds.y, getWidth(), toBounds.y - fromBounds.y + toBounds.height);
                g.setColor(selectedBorderColor);
                g.drawRect(0, fromBounds.y, getWidth() - 1, toBounds.y - fromBounds.y + toBounds.height);
            }
        }

        // perform operation of superclass
        setOpaque(false); // trick not to paint background
        super.paintComponent(g);
        setOpaque(false);
    }
}
