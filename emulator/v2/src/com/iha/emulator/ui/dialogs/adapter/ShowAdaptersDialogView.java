package com.iha.emulator.ui.dialogs.adapter;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TableView;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Class providing GUI components for "Show adapters in database dialog". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ShowAdaptersDialogView implements Initializable,ShowAdaptersDialogPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ShowAdaptersDialogView.class);
    /** presenter */
    private ShowAdaptersDialogPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private TableView table;

    @FXML
    public void handleRefresh(ActionEvent event){
        logger.trace("Refresh button clicked");
        presenter.refresh();
    }

    @FXML
    public void handleClose(ActionEvent event){
        logger.trace("Close button clicked");
        presenter.close();
    }

    @Override
    public ProgressIndicator getIndicator() {
        return indicator;
    }

    @Override
    public Label getStatus() {
        return status;
    }

    @Override
    public TableView getTable() {
        return table;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ShowAdaptersDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
