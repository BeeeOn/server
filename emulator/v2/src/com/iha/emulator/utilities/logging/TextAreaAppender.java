package com.iha.emulator.utilities.logging;

import javafx.application.Platform;
import javafx.scene.control.TextArea;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.Filter;
import org.apache.logging.log4j.core.Layout;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.appender.AbstractAppender;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginAttribute;
import org.apache.logging.log4j.core.config.plugins.PluginElement;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;
import org.apache.logging.log4j.core.layout.PatternLayout;

import java.io.Serializable;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

/**
 * Class used as plugin by log4j2 to use GUI component {@link javafx.scene.control.TextArea} as output.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
@Plugin(name = "TextAreaAppender", category = "Core", elementType = "appender")
public final class TextAreaAppender extends AbstractAppender {
    /** GUI component */
    private static volatile TextArea text;
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger("AppLog");
    /** date formatter */
    DateFormat dateFormat = new SimpleDateFormat("dd/MM/yy-HH:mm:ss");

    /**
     * Creates new instance of TextAreaAppender with given name, filter and layout used by {@link org.apache.logging.log4j.core.appender.AbstractAppender}
     * @param name appender name
     * @param filter appender filter
     * @param layout appender layout
     */
    protected TextAreaAppender(String name, Filter filter, Layout<? extends Serializable> layout) {
        super(name, filter, layout);
    }

    /**
     * Creates and returns new instance of TextAreaAppender with given name, filter and layout used by
     * {@link org.apache.logging.log4j.core.appender.AbstractAppender}
     * @param name appender name
     * @param layout appender layout
     * @param filter appender filer
     * @return new appender
     */
    @PluginFactory
    public static TextAreaAppender createAppender(@PluginAttribute("name") String name,
                                                  @PluginElement("PatternLayout")PatternLayout layout,
                                                  @PluginElement("Filters") Filter filter ){
        if(name == null){
            logger.error("No name provided for TextAreaAppender");
            return null;
        }
        if(layout==null){
            layout = PatternLayout.createDefaultLayout();
        }
        return new TextAreaAppender(name,filter,layout);
    }

    /**
     * Set text area as destination component
     * @param textArea destination component
     */
    public static void setTextFlow(final TextArea textArea){
        TextAreaAppender.text = textArea;
    }

    /**
     * Append log message contained in given logEvent.
     * @param logEvent event containing log message information
     */
    @Override
    public void append(LogEvent logEvent) {
        // Append formatted message to text area using the Thread.
        try {
            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (text != null) {
                            text.appendText(dateFormat.format(logEvent.getTimeMillis()) + " - " +logEvent.getLevel().toString() + " - " + logEvent.getMessage().getFormattedMessage() + "\n");
                        }
                    } catch (final Throwable t) {
                        logger.error("Unable to append log to text area: "
                                + t.getMessage());
                    }
                }
            });
        } catch (final IllegalStateException e) {
            // ignore case when the platform hasn't yet been initialized
        }
    }
}
