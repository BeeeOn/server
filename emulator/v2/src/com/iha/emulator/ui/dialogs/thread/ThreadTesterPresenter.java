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
 * Class providing logic to user interactions for "Thread tester dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ThreadTesterPresenter implements Presenter,PanelPresenter{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ThreadTesterPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "ThreadTester.fxml";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** class providing logic of tester */
    private MaxThreadTester tester;
    /**
     * Interface implemented by "Thread tester dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(ThreadTesterPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public TextArea getTextArea();
    }

    /**
     * Creates new "Thread tester dialog" presenter.
     * @param window parent window
     */
    public ThreadTesterPresenter(Stage window) {
        this.window = window;
    }

    /**
     * Starts thread test
     */
    public void start(){
        if(tester!= null) tester.startTest();
        else logger.error("No tester");
    }
    /**
     * Closes dialog
     */
    public void close(){
        window.hide();
    }

    /**
     * {@inheritDoc}
     */
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
    /**
     * Initializes dialog. Fills components with data and sets validation options.
     */
    public void init(){
        this.tester = new MaxThreadTester(view.getTextArea());
    }

    /**
     * {@inheritDoc}
     *
     * Empty
     */
    @Override
    public void addModel(Object model) {

    }

    /**
     * {@inheritDoc}
     *
     * Empty
     * @return null
     */
    @Override
    public Object getModel() {
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Node getView() {
        return view.getView();
    }

    /**
     * {@inheritDoc}
     *
     * Empty
     */
    @Override
    public void clear() {

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
