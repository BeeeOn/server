package com.iha.emulator.utilities;

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
 * Created by Shu on 25.11.2014.
 */
@Plugin(name = "TextAreaAppender", category = "Core", elementType = "appender")
public final class TextAreaAppender extends AbstractAppender {

    private static volatile TextArea text;
    private static final Logger logger = LogManager.getLogger("AppLog");
    DateFormat dateFormat = new SimpleDateFormat("dd/MM/yy-HH:mm:ss");

    protected TextAreaAppender(String name, Filter filter, Layout<? extends Serializable> layout) {
        super(name, filter, layout);
    }

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

    public static void setTextFlow(final TextArea textArea){
        TextAreaAppender.text = textArea;
    }

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
            // ignore case when the platform hasn't yet been iniitialized
        }
    }
}
