package com.iha.emulator.ui;

/**
 * Interface with methods needed to bind View and Presenter. Every application Presenter implements this interface
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Presenter {
    /**
     * Binds properties to appropriate filed, binds automatic validation and fills text fields with default values.
     */
    public void bind();
}
