package com.smarthome.emulator.design;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;

/**
 * Extends {@link javax.swing.JTextField} providing field accepting only numerical input of
 * given size
 */
public class JNumberTextField extends JTextField
{
    private static final Color DISABLED_FONT_COLOR = new Color(-5841665);
    /** input types */
    public static final int INTEGER_NUMBER = 0;
    public static final int DECIMAL_NUMBER = 1;
    /** possible chars */
    private static final char DECIMAL_SEPARATOR = '.';
    private static final char NEGATIVE_SIGN = '-';
    /** allowed chars for integer number*/
    public static final String ALLOWED_INTEGER = "0123456789";
    /** allowed chars for decimal number*/
    public static final String ALLOWED_DECIMAL = ALLOWED_INTEGER + DECIMAL_SEPARATOR;
    /** default maximum length */
    private int maxLength = 0;
    /** default type */
    private int type = INTEGER_NUMBER;
    /** default negative chars */
    private String negativeChars = "";
    /** default allowed chars */
    private String allowedChars;
    private boolean negative;
    /** set default precision */
    private int precision;
    /** input filter */
    protected PlainDocument numberFieldFilter;
    /**
     * Creates {@link javax.swing.JTextField} accepting only integer input of max length
     * given as parameter
     * @param maxLen maximum length
     */
    public JNumberTextField( int maxLen )
    {
        this( maxLen, INTEGER_NUMBER ,true);
    }
    /**
     * Creates {@link javax.swing.JTextField} accepting either integer or decimal input of max length
     * given as parameter
     * @param maxLen maximum length
     * @param type either <code>INTEGER_NUMBER</code> or <code>DECIMAL_NUMBER</code>
     * @param negative <code>true</code> if negative numbers are allowed, <code>false</code> otherwise
     */
    public JNumberTextField( int maxLen, int type,boolean negative)
    {
        setDisabledTextColor(DISABLED_FONT_COLOR);
        setNegative(negative);
        setMaxLength( maxLen );
        setType(type);

        numberFieldFilter = new JNumberFieldFilter();
        super.setDocument( numberFieldFilter );
    }
    /**
     * Sets maximum length
     * @param maxLen maximum length
     */
    public void setMaxLength( int maxLen )
    {
        if (maxLen > 0)
            maxLength = maxLen;
        else
            maxLength = 0;
    }
    /**
     * Returns field text as integer
     * @return field text as integer
     */
    public long getLong()
    {
        return Long.parseLong( getText() );
    }
    /**
     * Sets field text from integer
     * @param value number to be added as text
     */
    public void setLong( long value )
    {
        setText( String.valueOf( value ) );
    }
    /**
     * Returns field text as integer
     * @return field text as integer
     */
    public int getInt()
    {
        return Integer.parseInt( getText() );
    }
    /**
     * Sets field text from integer
     * @param value number to be added as text
     */
    public void setInt( int value )
    {
        setText( String.valueOf( value ) );
    }
    /**
     * Returns field text as float
     * @return field text as float
     */
    public float getFloat()
    {
        return ( new Float( getText() ) ).floatValue();
    }
    /**
     * Sets field text from float
     * @param value number to be added as text
     */
    public void setFloat(float value)
    {
        setText( String.valueOf( value ) );
    }
    /**
     * Returns field text as double
     * @return field text as double
     */
    public double getDouble()
    {
        return ( new Double( getText() ) ).doubleValue();
    }
    /**
     * Sets field text from double
     * @param value number to be added as text
     */
    public void setDouble(double value)
    {
        setText( String.valueOf(value) );
    }
    /**
     * According to given type sets variables needed to validate input
     * @param type either <code>INTEGER_NUMBER</code> or <code>DECIMAL_NUMBER</code>
     */
    public void setType(int type)
    {
        if(type == INTEGER_NUMBER){
            this.type = INTEGER_NUMBER;
            this.precision = 0;
            this.allowedChars = ALLOWED_INTEGER;
        }else if(type == DECIMAL_NUMBER){
            this.type = DECIMAL_NUMBER;
            this.precision = 2;
            this.allowedChars = ALLOWED_DECIMAL;
        }
    }
    /**
     * Sets whether are negative inputs allowed
     * @param value
     */
    public void setNegative(boolean value)
    {
        negative = value;
        if ( value )
            negativeChars = "" + NEGATIVE_SIGN;
        else
            negativeChars = "";
    }
    /**
     * Filter validating input of {@link javax.swing.JTextField}
     */
    class JNumberFieldFilter extends PlainDocument
    {
        public JNumberFieldFilter()
        {
            super();
        }
        /**
         * Methods validates input according to variables set in {@link com.smarthome.emulator.design.JNumberTextField}
         * @param offset
         * @param str
         * @param attr
         * @throws BadLocationException
         */
        public void insertString(int offset, String str, AttributeSet attr) throws BadLocationException
        {
            String text = getText(0,offset) + str + getText(offset,(getLength() - offset));
            if (str == null || text == null)
                return;
            //looking for not allowed characters
            for(int i=0; i<str.length(); i++)
            {
                if((allowedChars + negativeChars).indexOf(str.charAt(i)) == -1)
                    return;
            }
            int precisionLength = 0, dotLength = 0, minusLength = 0;
            int textLength = text.length();
            try
            {
                //if looking for integer number
                if(type == INTEGER_NUMBER)
                {
                    //if not only minus added return number
                    if(!((text.equals(negativeChars)) && (text.length() == 1)))
                        new Long(text);
                }
                else if ( type == DECIMAL_NUMBER)
                {
                    //if not only minus added return number
                    if(!((text.equals(negativeChars)) && (text.length() == 1)))
                        new Double(text);
                    //if found decimal separator
                    int dotIndex = text.indexOf(DECIMAL_SEPARATOR);
                    if( dotIndex != -1 )
                    {
                        //take only given precision
                        dotLength = 1;
                        precisionLength = textLength - dotIndex - dotLength;
                        if(precisionLength > precision)
                            return;
                    }
                }
            }
            catch(Exception ex)
            {
                return;
            }
            //checks if negative number is added
            if (text.startsWith("" + NEGATIVE_SIGN))
            {
                //if negative number is not allowed
                if (!negative)
                    return;
                else
                    minusLength = 1;
            }
            //if maximum length exceeded, do not accept other
            if(maxLength < (textLength - dotLength - precisionLength - minusLength))
                return;
            //insert string to text field
            super.insertString( offset, str, attr );
        }
    }
}
