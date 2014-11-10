package com.smarthome.emulator.design.tree;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 * Created by Shu on 28.5.2014.
 */
public class JIOTTreeNode extends DefaultMutableTreeNode {
    private ImageIcon icon;

    public JIOTTreeNode(TreeIcon icon, Object userObject) {
        super(userObject);
        if(icon != null)
            this.icon = new ImageIcon(getClass().getResource(icon.getPath()));
    }

    public JIOTTreeNode(TreeIcon icon, Object userObject, boolean allowsChildren) {
        super(userObject, allowsChildren);
        if(icon != null)
            this.icon = new ImageIcon(getClass().getResource(icon.getPath()));
    }

    public ImageIcon getIcon() {
        return icon;
    }

    public void setIcon(ImageIcon icon) {
        this.icon = icon;
    }
}
