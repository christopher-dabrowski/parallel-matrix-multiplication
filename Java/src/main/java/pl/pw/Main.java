package pl.pw;

import lombok.var;

import java.io.FileNotFoundException;

public class Main {
    public static void main(String[] args) {
        var test = "Hi";
        System.out.println("Hi Gradle. Lest's do matrix multiplication");

        try {
            var matrixA = Matrix.loadFromFile("A.txt");
            System.out.println(matrixA);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (Matrix.InvalidFileFormatException e) {
            e.printStackTrace();
        }
    }
}
