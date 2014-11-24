package com.iha.emulator;

import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;

public class Main extends Application {

    public enum SIM{
        DETAILED_SIMULATION,
        PERFORMANCE_SIMULATION;
    }

    private static final String DEFAULT_SETTINGS_FILE = "default_settings.properties";
    public static boolean FILE_OUTPUT = false;

    @Override
    public void start(Stage primaryStage) throws Exception{
        /** FIXGUI remove css from .fxml after done styling */
        DetailedSimulationPresenter dPresenter = new DetailedSimulationPresenter();
        primaryStage.setTitle("IHA Emulator");
        primaryStage.setScene(dPresenter.loadView());
        primaryStage.show();
        dPresenter.init(DEFAULT_SETTINGS_FILE);

        if (FILE_OUTPUT) {
            try {
                System.setOut(new PrintStream(new File("emu_out.txt")));
                System.setErr(new PrintStream(new File("emu_out.txt")));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


    public static void main(String[] args) {
        launch(args);
    }

    protected Object loadController(String url) throws IOException
    {
        InputStream fxmlStream = null;
        try
        {
            fxmlStream = getClass().getResourceAsStream(url);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            return loader.getController();
        }
        finally
        {
            if (fxmlStream != null)
            {
                fxmlStream.close();
            }
        }
    }
}
