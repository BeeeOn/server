package com.iha.emulator.ui.panels;

import javafx.scene.Node;

import java.io.IOException;

/**
 * Interface with methods needed to load View and bind Model. Every application Presenter implements this interface.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface PanelPresenter {
    /**
     * Loads view from FXML file, binds presenter and view, initializes presenter.
     * @return root GUI component
     * @throws IOException cannot load FXML file
     */
    public Node loadView() throws IOException;

    /**
     * Adds model to presenter
     * @param model model to be added
     */
    public void addModel(Object model);

    /**
     * Gets model
     * @return model
     */
    public Object getModel();
    /**
     * Returns view for presenter
     * @return view for presenter
     */
    public Node getView();

    /**
     * Clear GUI components
     */
    public void clear();
}
