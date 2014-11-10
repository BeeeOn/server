package com.smarthome.emulator.design;

import javax.swing.JTextField;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.PlainDocument;

public class JLimitedTextField extends JTextField {
    private static int limit;

    public JLimitedTextField(){
    }

    public JLimitedTextField(int limit) {
        setLimit(limit);
    }

    public int getLimit() {
        return limit;
    }

    public void setLimit(int limit) {
        this.limit = limit;
    }

    protected Document createDefaultModel() {
        return new LimitedLengthDocument();
    }

    static class LimitedLengthDocument extends PlainDocument {
        public void insertString(int offs, String str, AttributeSet a)
                throws BadLocationException {
            if( str == null )
                return;

            if( (getLength() + str.length()) <= limit )
                super.insertString(offs, str, a);

        }
    }
}