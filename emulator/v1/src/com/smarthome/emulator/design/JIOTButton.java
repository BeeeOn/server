package com.smarthome.emulator.design;

import javax.swing.*;
import java.awt.*;

/**
 * Created by Shu on 28.5.2014.
 */
public class JIOTButton extends JButton {

    private static final int DEFAULT_WIDTH = 130;
    private static final int DEFAULT_HEIGHT = 24;
    private static final Color ACTIVE_BACKGROUND = new Color(-5841665);
    private static final Color ACTIVE_FOREGROUND = new Color(-12303292);
    private static final Color INACTIVE_BACKGROUND = new Color(-3750202);
    private static final Color INACTIVE_FOREGROUND = new Color(-1);
    private static final Color BORDER_COLOR = new Color(220, 220, 220);
    private static final int BORDE_WIDTH = 1;
    private boolean activity = false;

    public JIOTButton() {
        this(false);
    }
    public JIOTButton(boolean activity) {
        super("New Button");
        setDefaultLayout();
        setDimensions(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        setActivity(activity);
    }
    public JIOTButton(String title) {
        this(title,false);
    }
    public JIOTButton(String title,boolean activity) {
        super(title);
        setDefaultLayout();
        setDimensions(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        setActivity(activity);
    }
    public JIOTButton(int width,int height) {
        this(width, height,false);
    }
    public JIOTButton(int width,int height,boolean activity) {
        super("New Button");
        setDefaultLayout();
        setDimensions(width,height);
        setActivity(activity);
    }
    public JIOTButton(String title,int width,int height) {
        this(title, width, height,false);
    }

    public JIOTButton(String title,int width,int height,boolean activity) {
        super(title);
        setDefaultLayout();
        setDimensions(width,height);
        setActivity(activity);
    }

    public void switchActivity(){
        activity = !activity;
        if(activity)
            setActive();
        else
            setInactive();
    }

    public void setActivity(boolean b){
        if(b)
            setActive();
        else
            setInactive();
        this.activity = b;
    }

    public boolean isActive(){
        return this.activity;
    }

    private void setActive(){
        this.setBackground(ACTIVE_BACKGROUND);
        this.setForeground(ACTIVE_FOREGROUND);
    }

    private void setInactive(){
        this.setBackground(INACTIVE_BACKGROUND);
        this.setForeground(INACTIVE_FOREGROUND);
    }

    private void setDefaultLayout(){
        this.setBorderPainted(false);
        this.setFocusPainted(false);
        this.setHorizontalAlignment(0);
        this.setMargin(new Insets(2, 0, 2, 0));
        setBorder(BorderFactory.createLineBorder(BORDER_COLOR,BORDE_WIDTH));

    }

    private void setDimensions(int width,int height){
        this.setMaximumSize(new Dimension(width, height));
        this.setMinimumSize(new Dimension(width, height));
        this.setPreferredSize(new Dimension(width, height));
    }
}
