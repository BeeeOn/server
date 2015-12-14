package com.smarthome.emulator.simulations.panels;

import com.smarthome.emulator.design.JDetailsTable;
import com.smarthome.emulator.design.JDetailsTableCellRenderer;
import com.smarthome.emulator.simulations.basis.Server;

import javax.swing.table.DefaultTableModel;
import java.net.InetAddress;

/**
 * Sets model for table with {@link com.smarthome.emulator.simulations.basis.Server} information.
 * Updates this model with current information.
 */
public class ServerPanel {
    /** table showing model data */
    private JDetailsTable serverTable;
    /** column headers - currently not displaying */
    private static String[] server_col_names = {"Property", "Value"};
    /** table properties names */
    private static String[] property_names = {"Connection:", "Name:", "IP:", "Port:", ""};
    /** model to be assign to table */
    private DefaultTableModel serverModel;
    /** number of rows in table */
    private static final int ROW_COUNT = 4;
    /**
     * Assigns new data <code>model</code> given as parameter for {@link com.smarthome.emulator.design.JDetailsTable} <code>table</code> given as parameter
     * @param table table for which should be new data model assigned
     * @param model data model for table, null if create new model
     */
    public ServerPanel(JDetailsTable table,DefaultTableModel model) {
        this.serverTable = table;
        setServerTable();
        if(model == null){
            this.serverModel = createModel();
        }else{
            this.serverModel = model;
        }
        assignModelToTable();
    }
    /**
     * Sets data model to default state with <code>0</code> rows
     */
    public void delete(){
        serverModel.setRowCount(0);
        serverTable.validate();
    }
    /**
     * Creates new {@link javax.swing.table.DefaultTableModel} and returns it.
     * @return newly created data model for table
     */
    public static DefaultTableModel createModel(){
        DefaultTableModel model = new DefaultTableModel(server_col_names, ROW_COUNT) {
            public Object getValueAt(int row, int col) {
                if (col == 0)
                    return property_names[row];
                return super.getValueAt(row, col);
            }
            public boolean isCellEditable(int row, int col) {
                return false;
            }
        };
        return model;
    }

    /**
     * Assigns model to table
     */
    public void assignModelToTable(){
        serverTable.setModel(this.serverModel);
    }
    /**
     * Updates model's data with information from {@link com.smarthome.emulator.simulations.basis.Server} given as parameter.
     * @param server server information to be displayed
     */
    public void refreshTable(Server server){
        this.setConnection(server.isConnected());
        this.setName(server.getName());
        this.setPort(server.getPort());
        this.setIp(server.getIp());
    }
    //region Getters and Setters
    /**
     * Sets table's model information about <code>server</code>'s ip address
     * @param ip ip address
     */
    public void setIp(String ip) {
        serverModel.setValueAt(ip, 2, 1);
    }
    /**
     * Sets table's model information about <code>server</code>'s connection status
     * @param connection <code>server</code>'s connection status
     */
    public void setConnection(boolean connection) {
        String conn;
        if(connection)
            conn = "Active";
        else
            conn = "Inactive";
        serverModel.setValueAt(conn, 0, 1);
    }
    /**
     * Sets table's model information about <code>server</code>'s hostname
     * @param name <code>server</code>'s hostname
     */
    public void setName(String name) {
        serverModel.setValueAt(name, 1, 1);
    }
    /**
     * Sets table's model information about <code>server</code>'s ip address and hostname
     * @param address <code>server</code>'s address
     */
    public void setAddress(InetAddress address) {
        String ip = Server.getIpAsString(address);
        String name = address.getHostName();
        serverModel.setValueAt(ip, 2, 1);
        serverModel.setValueAt(name, 1, 1);
    }
    /**
     * Sets table's model information about <code>server</code>'s port
     * @param port <code>server</code>'s port
     */
    public void setPort(int port) {
        serverModel.setValueAt(port, 3, 1);
    }
    //endregion
    /**
     * Sets visual configuration for {@link com.smarthome.emulator.design.JDetailsTable} and assigns
     * renderer {@link com.smarthome.emulator.design.JDetailsTableCellRenderer} for table
     */
    private void setServerTable(){
        serverTable.setRowSelectionAllowed(false);
        serverTable.setColumnSelectionAllowed(false);
        serverTable.setDefaultRenderer(Object.class,new JDetailsTableCellRenderer());
    }
}
