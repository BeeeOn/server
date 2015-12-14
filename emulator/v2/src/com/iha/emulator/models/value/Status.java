package com.iha.emulator.models.value;

/**
 * Interface used for values represented as status. Providing methods to identify specific status.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Status {
    /**
     * Gets status code
     * @return status code
     */
    public int getCode();

    /**
     * Gets status description
     * @return status description
     */
    public String getDescription();
}
