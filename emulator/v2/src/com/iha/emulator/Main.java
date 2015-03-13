package com.iha.emulator;

import com.iha.emulator.ui.simulations.chooser.ChooserPresenter;
import javafx.application.Application;
import javafx.application.HostServices;
import javafx.scene.image.Image;
import javafx.stage.Stage;

public class Main extends Application {

    public enum SIM{
        DETAILED_SIMULATION,
        PERFORMANCE_SIMULATION
    }

    private static final String DEFAULT_SETTINGS_FILE = "default_settings.properties";
    private final HostServices hostServices = getHostServices();

    @Override
    public void start(Stage primaryStage) throws Exception{
        /** FIXGUI remove css from .fxml after done styling */
        ChooserPresenter chooserPresenter = new ChooserPresenter(primaryStage);
        chooserPresenter.setHostServices(hostServices);
        primaryStage.setTitle("IHA Emulator");
        primaryStage.setScene(chooserPresenter.loadView());
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon32x32.png")));
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon16x16.png")));
        primaryStage.show();
        chooserPresenter.init(DEFAULT_SETTINGS_FILE);

        /*DetailedSimulationPresenter dPresenter = new DetailedSimulationPresenter(primaryStage);
        primaryStage.setTitle("IHA Emulator");
        primaryStage.setScene(dPresenter.loadView());
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon32x32.png")));
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon16x16.png")));
        primaryStage.show();
        dPresenter.init(DEFAULT_SETTINGS_FILE);*/
    }


    public static void main(String[] args) {
        launch(args);
    }
}
