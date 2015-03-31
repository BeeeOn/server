package com.iha.emulator.utilities;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLPeerUnverifiedException;
import javax.net.ssl.SSLSession;
import javax.security.auth.x500.X500Principal;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.Arrays;


public class WildcardHostnameVerifier implements HostnameVerifier {
    public WildcardHostnameVerifier() {
        super();
    }

    /**
     * Parses the Distinguished Name, specifically looking for Common Names.
     * Attempts to adhere relatively strictly to RFC2253.
     */
    private static class DistinguishedNameParser {
        private CharacterIterator iter;
        private String string;
        private DistinguishedNameParser(String name) {
            string = name;
            iter = new StringCharacterIterator(name);
        }

        public static String parse(String name) throws UnsupportedEncodingException {
            return (new DistinguishedNameParser(name)).parseDistinguishedName();
        }

        /**
         * Expects the iterator to be at the beginning of the string.
         * @return the Common Name contained within this distinguished name
         */
        private String parseDistinguishedName() throws UnsupportedEncodingException {
            // Parse the first component first
            String name = parseNameComponent();
            if (name != null)
                return name;

            // After each iteration, we expect the iterator to be past the end
            // of the component, having hit an end condition. So, test the
            // next one.
            while (iter.next() != CharacterIterator.DONE) {
                name = parseNameComponent();
                if (name != null)
                    return name;
            }
            return null;
        }

        // Characters that signal the end of these fields
        private static final char[] NAME_COMPONENT_END;
        private static final char[] ATTRIBUTE_END;
        private static final char[] ATTRIBUTE_TYPE_END;

        /**
         * Valid hexadecimal values.
         */
        private static final char[] HEX_VALUES;

        static {
            NAME_COMPONENT_END = new char[]{',', CharacterIterator.DONE};
            Arrays.sort(NAME_COMPONENT_END);

            ATTRIBUTE_END = Arrays.copyOf(NAME_COMPONENT_END, 3);
            ATTRIBUTE_END[2] = '+';
            Arrays.sort(ATTRIBUTE_END);

            ATTRIBUTE_TYPE_END = Arrays.copyOf(ATTRIBUTE_END, 4);
            ATTRIBUTE_TYPE_END[3] = '=';
            Arrays.sort(ATTRIBUTE_TYPE_END);

            HEX_VALUES = "0123456789abcdefABCDEF".toCharArray();
            Arrays.sort(HEX_VALUES);
        }

        // Handy shortcut.
        private static final Charset UTF8 = Charset.forName("UTF-8");

        /**
         * Checks if a character is in the given pre-sorted array.
         * Mainly meant to compare with the static arrays above.
         * @param in array to test against
         * @param c character to test
         * @return true if c is in 'in', false otherwise.
         */
        private boolean includes(char[] in, char c) {
            boolean value = Arrays.binarySearch(in, c) >= 0;
            return value;
        }

        /**
         * Parse a name component, looking for a common name. Expects the
         * iterator to be at the beginning of the name at the beginning of the
         * loop. At end, iterator will be at the end character (a comma), thus
         * iter.next should point to the next name component.
         * @return Common name if found, null otherwise.
         */
        private String parseNameComponent() throws UnsupportedEncodingException {
            while (!includes(NAME_COMPONENT_END, iter.current())) {
                // Parse the attribute pair
                String type = parseAttributeType();
                String value = parseAttributeValue();

                // If we find the common name, return it.
                if (type.toUpperCase().equals("CN") || type.equals("2.5.4.3"))
                    return value;
                iter.next();
            }
            return null;
        }

        /**
         * Parse the type component of an attribute pair. Expects iterator
         * to be currently at the beginning. Leaves iterator at the end point
         * -- an equals sign, in a well-formed string.
         * @return the type, as a string.
         */
        private String parseAttributeType() {
            StringBuffer result = new StringBuffer();
            char c = iter.current();
            while (!includes(ATTRIBUTE_TYPE_END, c)) {
                result.append(c);
                c = iter.next();
            }
            return result.toString();
        }

        /**
         * Parse the value of an attribute pair. Expects iterator to be
         * just before the beginning (on the equals sign). Leaves the iterator
         * just past the end, as usual -- likely on a + or comma.
         * @return the value, as a string.
         */
        private String parseAttributeValue() throws UnsupportedEncodingException {
            char c = iter.next();
            if (includes(ATTRIBUTE_END, c))
                return "";
            switch (c) {
                case '#':
                    return parseHexString();
                case '"':
                    return parseQuotation();
                default:
                    return parseString();
            }
        }

        /**
         * Parse a normal attribute string. See parseAttributeValue().
         */
        private String parseString() {
            StringBuffer result = new StringBuffer();
            char c = iter.current();
            while(!includes(ATTRIBUTE_END, c)) {
                if (c == '\\')
                    result.append(parsePair());
                else
                    result.append(c);
                c = iter.next();
            }
            return result.toString();
        }

        /**
         * Parse a hex-encoded attribute string. See parseAttributeValue.
         */
        private String parseHexString() throws UnsupportedEncodingException {
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            while (includes(HEX_VALUES, iter.next())) {
                bytes.write(parseByte());
            }
            return bytes.toString("UTF-8");
        }

        /**
         * Parse a quoted string. See parseAttributeValue.
         */
        private String parseQuotation() {
            StringBuffer result = new StringBuffer();
            while (iter.next() != '"') {
                char c = iter.current();
                if (c == '\\')
                    result.append(parsePair());
                else
                    result.append(c);
            }
            iter.next();
            return result.toString();
        }

        /**
         * Parse a backslash-escaped character, leaving the iterator just
         * beyond it.
         */
        private char parsePair() {
            char c = iter.next();
            if (includes(HEX_VALUES, c))
                return new String(new byte[]{parseByte()}, UTF8).charAt(0);
            return c;
        }

        /**
         * Parse a hex-encoded byte, leaving the iterator just beyond it.
         * In all usages of this method, the result is interpreted as
         * a UTF8 character, which will likely break on actual UTF8 (non-ascii)
         * characters. Shouldn't be too difficult to fix, if anyone needs it.
         */
        private byte parseByte() {
            int i = iter.getIndex();
            byte result = (byte)Integer.parseInt(string.substring(i, i+2), 16);
            iter.next();
            return result;
        }

    }

    /**
     * Checks the given hostname against the CommonName value of the
     * certificate in the given session.
     */
    public boolean verify(String hostname, SSLSession session) {
        Certificate cert;
        // Re-parse as X509
        try {
            cert = session.getPeerCertificates()[0];
        } catch (SSLPeerUnverifiedException e) {
            return false;
        }
        if (!(cert instanceof X509Certificate)) {
            try {
                cert = CertificateFactory.getInstance("X.509").generateCertificate(
                        new ByteArrayInputStream(cert.getEncoded())
                );
            } catch (CertificateException e) {
                throw new IllegalArgumentException("Certificate was invalid", e);
            }
        }

        // Parse the distinguished name -- see above for details.
        String distinguishedName = ((X509Certificate) cert).getSubjectX500Principal()
                .getName(X500Principal.RFC2253);

        String commonName;

        try {
            commonName = DistinguishedNameParser.parse(distinguishedName);
        } catch (UnsupportedEncodingException e) {
            // How should I handle this, really?
            throw new RuntimeException("UTF8 wasn't found!");
        }

        // Is it a wildcard cert?
        if (commonName.substring(0, 2).equals("*.")) {
            // Slice off the first part of our common name, then compare the
            // end of both strings.
            int index = hostname.indexOf(".");
            if (index > 0) {
                return hostname.substring(index+1).equals(commonName.substring(2));
            }
        }

        // Not a wildcard cert. Just compare.
        return hostname.equals(commonName);
    }
}
