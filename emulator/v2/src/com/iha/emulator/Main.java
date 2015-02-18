package com.iha.emulator;

import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import javafx.application.Application;
import javafx.scene.image.Image;
import javafx.stage.Stage;

import java.io.File;
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
        DetailedSimulationPresenter dPresenter = new DetailedSimulationPresenter(primaryStage);
        primaryStage.setTitle("IHA Emulator");
        primaryStage.setScene(dPresenter.loadView());
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon32x32.png")));
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon16x16.png")));
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
}
