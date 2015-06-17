package com.iha.emulator.models.value;

/**
 * Interface providing methods, that needs to be implemented by values, that are using generator
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface HasGenerator {
    /**
     * Gets generator type
     * @return generator type
     */
    public Value.Generator getGeneratorType();

    /**
     * Sets generator type
     * @param generatorType generator type
     */
    public void setGeneratorType(Value.Generator generatorType);

    /**
     * Restarts generator
     */
    public void restartGenerator();
}
