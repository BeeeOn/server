package com.iha.emulator.ui.panels.adapter;

import com.iha.emulator.control.AdapterController;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.Button;

/**
 * Graphical component class extending {@link javafx.scene.control.Button} displaying information about adapter, it's
 * ID, name and status.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AdapterButton extends Button{
    /** main button CSS class */
    private static final String[] BTN_MAIN_CSS = {"adapterListBtn","adapterBtn"};
    /** active adapter button CSS class */
    private static final String ACTIVE_CLASS = "activeAdapterBtn";
    /** inactive adapter button CSS class */
    private static final String INACTIVE_CLASS = "inactiveAdapterBtn";
    /** selected adapter button CSS class */
    private static final String SELECTED_CLASS = "selectedAdapterBtn";
    /** adapter for which is button created */
    private AdapterController controller;

    /**
     * Creates button for given adapter. Assigns CSS classes and change listeners to button.
     * @param controller adapter with information to be displayed
     */
    public AdapterButton(AdapterController controller){
        //create button
        super(controller.getAdapter().getId() + " / " + controller.getAdapter().getName());
        this.controller = controller;
        //add CSS classes
        addCss();
        //add change listeners
        addGraphicChangeListener(controller);
    }

    /**
     * Sets, if adapter owning this button is selected
     * @param b <code>true</code> adapter selected, <code>false</code> otherwise
     */
    public void setSelected(boolean b){
        //if selected, set selected CSS class, else remove selected CSS class
        if(b) getStyleClass().add(SELECTED_CLASS);
        else getStyleClass().remove(SELECTED_CLASS);
    }

    /**
     * Adds default CSS classes to button
     */
    private void addCss(){
        for(String cssClass:BTN_MAIN_CSS){
            this.getStyleClass().add(cssClass);
        }
        this.getStyleClass().add(INACTIVE_CLASS);
    }

    /**
     * Adds change listener responsible for changing active and inactive CSS classes to button
     * @param controller adapter for which is listener added
     */
    private void addGraphicChangeListener(AdapterController controller){
        controller.getAdapter().statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    getStyleClass().remove(INACTIVE_CLASS);
                    getStyleClass().add(ACTIVE_CLASS);
                }else {
                    getStyleClass().remove(ACTIVE_CLASS);
                    getStyleClass().add(INACTIVE_CLASS);
                }
            }
        });
    }

    /**
     * Gets adapter owning button
     * @return adapter owning button
     */
    public AdapterController getController() {
        return controller;
    }
}
