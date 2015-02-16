package com.iha.emulator.ui.panels.adapter;

import com.iha.emulator.control.AdapterController;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.Button;

/**
 * Created by Shu on 27.11.2014.
 */
public class AdapterButton extends Button{

    private static final String[] BTN_MAIN_CSS = {"adapterListBtn","adapterBtn"};
    private static final String ACTIVE_CLASS = "activeAdapterBtn";
    private static final String INACTIVE_CLASS = "inactiveAdapterBtn";
    private static final String SELECTED_CLASS = "selectedAdapterBtn";

    private AdapterController controller;

    public AdapterButton(AdapterController controller){
        super(controller.getAdapter().getId() + " / " + controller.getAdapter().getName());
        this.controller = controller;
        addCss();
        addGraphicChangeListener(controller);
    }

    public void setSelected(boolean b){
        if(b) getStyleClass().add(SELECTED_CLASS);
        else getStyleClass().remove(SELECTED_CLASS);
    }

    private void addCss(){
        for(String cssClass:BTN_MAIN_CSS){
            this.getStyleClass().add(cssClass);
        }
        this.getStyleClass().add(INACTIVE_CLASS);
    }

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

    public AdapterController getController() {
        return controller;
    }
}
