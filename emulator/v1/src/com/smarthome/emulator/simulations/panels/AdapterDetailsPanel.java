package com.smarthome.emulator.simulations.panels;

import com.smarthome.emulator.design.JDetailsTableCellRenderer;
import com.smarthome.emulator.design.JDetailsTable;
import com.smarthome.emulator.simulations.basis.Adapter;

import javax.swing.table.DefaultTableModel;

/**
 * Creates model for table with {@link com.smarthome.emulator.simulations.basis.Adapter} information.
 */
public class AdapterDetailsPanel {
    /** table for new table model */
    private JDetailsTable detailsTable;
    /** shown information names */
    private String[] property_names = {"Status:", "Id:", "Registered", "Active sen/act:", "Registered sen/act:", "Server protocol:", ""};
    /** newly created model for table */
    private DefaultTableModel detailsModel;
    /** number of columns for table */
    private static final int COLUMN_COUNT = 2;
    /** number of row for table */
    private static final int ROW_COUNT = 7;
    /**
     * Creates and assigns new data model fot {@link com.smarthome.emulator.design.JDetailsTable} given as parameter
     * @param table table for which should be new data model assigned
     */
    public AdapterDetailsPanel(JDetailsTable table) {
        this.detailsTable = table;
        createDetailsTable();
        assignModelToTable();
    }
    /**
     * Sets data model to default state with <code>0</code> rows
     */
    public void delete(){
        detailsModel.setRowCount(0);
        detailsTable.validate();
    }
    /**
     * Updates model's data with information from {@link com.smarthome.emulator.simulations.basis.Adapter} given as parameter.
     * @param adapter adapter information to be displayed
     */
    public void refreshTable(Adapter adapter){
        if (adapter != null) {
            this.setStatus(adapter.isActive() ? "Active" : "Inactive");
            this.setId(adapter.getAdapterId());
            this.setRegistered(adapter.isRegistered() ? "Yes" : "No");
            this.setActiveSensors(adapter.getActiveSensors());
            this.setRegisteredSensors(adapter.getRegisteredSensors());
            this.setServerProtocol(adapter.getServerProtocol());
        }
    }
    /**
     * Assigns model to table
     */
    public void assignModelToTable(){
        detailsTable.setModel(this.detailsModel);
    }
    /**
     * Sets table's model information about <code>adapter</code>'s activity status
     * @param s <code>adapter</code>'s activity status ( <code>Active</code> or <code>Inactive</code> )
     */
    public void setStatus(String s){
        detailsModel.setValueAt(s,0,1);
    }
    /**
     * Sets table's model information about <code>adapter</code>'s a id
     * @param i <code>adapter</code>'s id
     */
    public void setId(long i){
        detailsModel.setValueAt(i, 1, 1);
    }
    public void setRegistered(String s){
        detailsModel.setValueAt(s, 2, 1);
    }
    /**
     * Sets table's model information about <code>adapter</code>'s number of active sensors
     * @param a <code>adapter</code>'s number of active sensors
     */
    public void setActiveSensors(int a){
        detailsModel.setValueAt(a,3,1);
    }
    /**
     * Sets table's model information about <code>adapter</code>'s number of registered sensors
     * @param r <code>adapter</code>'s number of registered sensors
     */
    public void setRegisteredSensors(int r){
        detailsModel.setValueAt(r,4,1);
    }
    /**
     * Sets table's model information about <code>adapter</code>'s server <-> adapter protocol
     * @param p <code>adapter</code>'s protocol
     */
    public void setServerProtocol(float p){
        detailsModel.setValueAt(p,5,1);
    }
    /**
     * Creates new {@link javax.swing.table.DefaultTableModel} for {@link com.smarthome.emulator.design.JDetailsTable} given as constructor parameter.
     * Also assigns renderer {@link com.smarthome.emulator.design.JDetailsTableCellRenderer} for table
     */
    private void createDetailsTable(){
        detailsModel = new DefaultTableModel(ROW_COUNT, COLUMN_COUNT) {
            public Object getValueAt(int row, int col) {
                if (col == 0)
                    return property_names[row];
                return super.getValueAt(row, col);
            }

            public boolean isCellEditable(int row, int col) {
                return false;
            }
        };
        detailsTable.setRowSelectionAllowed(false);
        detailsTable.setColumnSelectionAllowed(false);
        detailsTable.setDefaultRenderer(Object.class,new JDetailsTableCellRenderer());
    }
}
