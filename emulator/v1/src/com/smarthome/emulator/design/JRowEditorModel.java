package com.smarthome.emulator.design;

/**
 * Row editor model for {@link com.smarthome.emulator.design.JDetailsTable} stores editors in {@link java.util.Hashtable}
 */
import javax.swing.table.*;
import java.util.*;
public class JRowEditorModel
{
    /** row editors */
    private Hashtable data;
    /** initializes row editors */
    public JRowEditorModel()
    {
        data = new Hashtable();
    }
    /** insert new row editor to given row
     * @param row row for new editor
     * @param e editor
     */
     public void addEditorForRow(int row, TableCellEditor e )
    {
        data.put(new Integer(row), e);
    }
    /** removes row editor to given row
     * @param row from which should be editor deleted from
    */
    public void removeEditorForRow(int row)
    {
        data.remove(new Integer(row));
    }
    /**
     * Returns cell editor for given row
     * @param row editor's row
     * @return cell editor for given row
     */
    public TableCellEditor getEditor(int row)
    {
        return (TableCellEditor)data.get(new Integer(row));
    }
}
