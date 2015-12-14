package com.smarthome.emulator.design;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

/**
 * Custom table cell renderer for displaying information about adapter, server and sensor.
 */
public class JDetailsTableCellRenderer extends DefaultTableCellRenderer {
    /** selected cell background color*/
    private Color selectedColor = new Color(166,220,255);
    /** selected cell text color*/
    private Color selectedFontColor = new Color(198,198,198);
    /** text color*/
    private Color fontColor = new Color(130,130,130);
    /** sensor,adapter,server activity color */
    private Color activeServerColor = new Color(101,247,77);
    /** sensor,adapter,server activity color */
    private Color inactiveServerColor = new Color(235,66,66);
    @Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        JLabel c = (JLabel)super.getTableCellRendererComponent( table, value, isSelected, hasFocus, row, column);
        setBorder(BorderFactory.createEmptyBorder(0,10,0,0));
        //properties aligned to left
        if(column==1){
            setHorizontalAlignment(SwingConstants.LEFT);
            if(hasFocus){
                setBackground(selectedColor);
                setFont(getFont().deriveFont(Font.BOLD));
                setForeground(Color.WHITE);
            }else{
                setBackground(Color.WHITE);
                if(value!= null){
                    if(value.toString().equals("ON") || value.toString().equals("Active")){
                        setFont(getFont().deriveFont(Font.BOLD));
                        setForeground(activeServerColor);
                    }else if (value.toString().equals("OFF") || value.toString().equals("Inactive")){
                        setFont(getFont().deriveFont(Font.BOLD));
                        setForeground(inactiveServerColor);
                    }else{
                        setForeground(fontColor);
                    }
                }
            }
        }else{
            //properties names aligned to right
            setBackground(Color.WHITE);
            setForeground(fontColor);
            setHorizontalAlignment(SwingConstants.RIGHT);
        }
        //TODO make own toolTip
        c.setToolTipText(c.getText());
        return c;
    }

    public void setTextColor(Color color){
        this.fontColor = color;
    }

}
