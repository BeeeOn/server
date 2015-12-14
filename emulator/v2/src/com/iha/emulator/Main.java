package com.iha.emulator;

import com.iha.emulator.ui.simulations.chooser.ChooserPresenter;
import javafx.application.Application;
import javafx.application.HostServices;
import javafx.scene.image.Image;
import javafx.stage.Stage;

/**
 * Class implements application that can emulate end stations in intelligent home.<br>
 * <a href="https://ant-2.fit.vutbr.cz/">See more info</a>
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Main extends Application {
    /**
     * Enum with simulation types
     */
    public enum SIM{
        DETAILED_SIMULATION,
        PERFORMANCE_SIMULATION
    }
    /** default configuration filename */
    private static final String DEFAULT_SETTINGS_FILE = "default_settings.properties";
    /** application host services */
    private final HostServices hostServices = getHostServices();

    /**
     * {@inheritDoc}
     *
     * Method invokes {@link com.iha.emulator.ui.simulations.chooser.ChooserPresenter} which serves as entry point
     * of graphical interface of application.
     */
    @Override
    public void start(Stage primaryStage) throws Exception{
        //create presenter
        ChooserPresenter chooserPresenter = new ChooserPresenter(primaryStage);
        chooserPresenter.setHostServices(hostServices);
        primaryStage.setTitle("BeeeOn Emulator");
        //initiate scene
        primaryStage.setScene(chooserPresenter.loadView());
        //set icons
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon32x32.png")));
        primaryStage.getIcons().add(new Image(Main.class.getResourceAsStream("/com/iha/emulator/resources/images/emu_icon16x16.png")));
        //show application
        primaryStage.show();
        //initiate chooser
        chooserPresenter.init(DEFAULT_SETTINGS_FILE);
    }
    public static void main(String[] args) {
        launch(args);
    }
}
