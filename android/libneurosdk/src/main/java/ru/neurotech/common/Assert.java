package ru.neurotech.common;

public abstract class Assert {
    public static void expects(boolean condition, String message){
        if (!condition){
            throw new IllegalArgumentException("Contract precondition check failed: " + message);
        }
    }

    public static void ensures(boolean condition, String message){
        if (!condition){
            throw new IllegalArgumentException("Contract postcondition check failed: " + message);
        }
    }
}
