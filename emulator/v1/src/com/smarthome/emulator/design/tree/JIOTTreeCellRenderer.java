package com.smarthome.emulator.design.tree;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.basis.Value;

import javax.swing.*;
import javax.swing.tree.DefaultTreeCellRenderer;
import java.awt.*;

/**
 * Created by Shu on 28.5.2014.
 */
public class JIOTTreeCellRenderer extends DefaultTreeCellRenderer {

    public JIOTTreeCellRenderer() {

    }

    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
        super.getTreeCellRendererComponent(tree, value, leaf, expanded, leaf, row, hasFocus);
        JIOTTreeNode node = (JIOTTreeNode) value;
        if(node.getIcon() != null)
            setIcon(node.getIcon());
        else
            setIcon(getIcon());
        if(leaf && node.getUserObject() instanceof Value){
            String name = ((Value) node.getUserObject()).getName();
            name = name.replace(":","");
            setText(name);
        }
        /*if (!leaf) {
            JIOTTreeNode node = (JIOTTreeNode) value;
            if (node.getIcon() != null) {
                System.out.println(node.getIcon().toString());
                setClosedIcon(node.getIcon());
                setOpenIcon(node.getIcon());
            } else {
                setClosedIcon(getDefaultClosedIcon());
                setClosedIcon(getDefaultOpenIcon());
                setOpenIcon(getDefaultOpenIcon());
            }
        }*/
        return this;
    }

    @Override
    public Color getBackgroundNonSelectionColor() {
        return (null);
    }

    @Override
    public Color getBackgroundSelectionColor() {
        return null;
        //return new Color(-5841665);
    }

    @Override
    public Color getBackground() {
        return (null);
    }

    @Override
    public Color getBorderSelectionColor(){
        return null;
    }

}
