package com.iha.emulator.ui.dialogs.thread;

import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.MaxThreadTester;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 3.2.2015.
 */
public class ThreadTesterPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(ThreadTesterPresenter.class);
    private static final String FXML_PATH = "ThreadTester.fxml";
    private static final double DEFAULT_REFRESH_MILLIS = 2000;

    private Display view;
    private Stage window;

    private MaxThreadTester tester;

    public interface Display {
        public Node getView();
        public void setPresenter(ThreadTesterPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public TextArea getTextArea();
    }

    public ThreadTesterPresenter(Stage window) {
        this.window = window;
    }

    public void start(){
        if(tester!= null) tester.startTest();
        else logger.error("No tester");
    }

    public void close(){
        window.hide();
    }


    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ThreadTesterView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            // bind view and presenter
            bind();
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    public void init(){
        this.tester = new MaxThreadTester(view.getTextArea());
    }

    @Override
    public void addModel(Object model) {

    }

    @Override
    public Object getModel() {
        return null;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
