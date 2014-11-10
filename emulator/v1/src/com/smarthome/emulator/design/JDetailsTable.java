package com.smarthome.emulator.design;

import javax.swing.*;
import javax.swing.table.*;
/**
 * Creates table for displaying information
 */
public class JDetailsTable extends JTable
{
    /** row editor model */
    protected JRowEditorModel rm;
    /**
     * Creates default table from {@link javax.swing.JTable}
     */
    public JDetailsTable()
    {
        super();
            rm = null;
    }
    /**
     * Sets row editor model
     * @param rm row editor model
     */
    public void setRowEditorModel(JRowEditorModel rm)
    {
        this.rm = rm;
    }
    /**
     * Returns row editor model
     * @return row editor model
     */
    public JRowEditorModel getRowEditorModel()
    {
        return rm;
    }
    /**
     * Returns cell editor for given row and column
     * @param row desired row
     * @param col desired column
     * @return cell editor for given row and column
     */
    public TableCellEditor getCellEditor(int row, int col)
    {
        TableCellEditor tmpEditor = null;
        if (rm!=null)
            tmpEditor = rm.getEditor(row);
        if (tmpEditor!=null)
            return tmpEditor;
        return super.getCellEditor(row,col);
    }
}
