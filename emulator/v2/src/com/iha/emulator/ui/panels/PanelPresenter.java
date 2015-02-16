package com.iha.emulator.ui.panels;

import javafx.scene.Node;

import java.io.IOException;

/**
 * Created by Shu on 26.11.2014.
 */
public interface PanelPresenter {
    public Node loadView() throws IOException;
    public void addModel(Object model);
    public Object getModel();
    public Node getView();
    public void clear();
}
