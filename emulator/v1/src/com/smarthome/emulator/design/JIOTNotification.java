package com.smarthome.emulator.design;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Created by Shu on 3.6.2014.
 */
public class JIOTNotification  extends JFrame implements MouseListener, ActionListener,FocusListener {
    private static final float BACKGROUND_ALPHA = 0.5f;
    private static final Color BACKGROUND_COLOR = new Color(0.65f,0.86f,1f,1f/2f);
    private static final Color FOREGROUND_COLOR = new Color(129,129,129);
    private static final int BORDER_WIDTH = 2;
    private static final int WIDTH = 150;
    private static final int HEIGHT = 30;

    private JFrame parent = null;
    private JLabel text = null;
    private Timer timer = new Timer(3000,this);

    private boolean seen = false;

    public JIOTNotification(JFrame parent,String text,Dimension dimension) {
        super(text);
        setUndecorated(true);
        Rectangle pos = parent.getBounds();
        setBounds(pos.x+(pos.getSize().width-WIDTH-30),pos.y+(pos.getSize().height-HEIGHT-30),dimension.width,dimension.height);
        this.parent = parent;
        create();
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setText(text);
        //pack();
        addMouseListener(this);
        setVisible(true);
        timer.start();

        //parent.add(this);
    }

    public void setText(String text){
        this.text.setText(text);
    }

    private void create(){
        //setOpaque(true);
        setBackground(BACKGROUND_COLOR);
        setForeground(FOREGROUND_COLOR);
        //setBorder(BorderFactory.createLineBorder(FOREGROUND_COLOR,BORDER_WIDTH));

        text = new JLabel("Notification");
        add(text);
    }

    private void setWidthAndHeight(Dimension dimension){
        setPreferredSize(dimension);
        setMinimumSize(dimension);
        setMaximumSize(dimension);
        validate();
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        this.setSeen(true);
        System.out.println("seen");
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    private static void setTranslucency( Window window){
        try {
            Class<?> awtUtilitiesClass = Class.forName("com.sun.awt.AWTUtilities");
            Method mSetWindowOpacity = awtUtilitiesClass.getMethod("setWindowOpacity", Window.class, float.class);
            mSetWindowOpacity.invoke(null, window, Float.valueOf(0.75f));
        } catch (NoSuchMethodException ex) {
            ex.printStackTrace();
        } catch (SecurityException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (IllegalArgumentException ex) {
            ex.printStackTrace();
        } catch (InvocationTargetException ex) {
            ex.printStackTrace();
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(isSeen()){
            dispose();
        }
    }

    public boolean isSeen(){
        return this.seen;
    }

    public void setSeen(boolean b){
        this.seen = b;
    }

    @Override
    public void focusGained(FocusEvent e) {

    }

    @Override
    public void focusLost(FocusEvent e) {

    }
}
