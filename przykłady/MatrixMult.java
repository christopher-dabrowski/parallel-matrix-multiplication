import java.io.*;
import java.util.Locale;
import java.util.Scanner;

/**
 * User: szmurlor
 * Date: 17.03.14
 * Time: 12:13
 */
public class MatrixMult {

    public static void main(String[] args) {
        MatrixMult mm = new MatrixMult();

        try {
            mm.start(args);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    protected void start(String[] args) throws FileNotFoundException {
        Matrix A,B;
        A = read(args[0]);
        B = read(args[1]);

        System.out.println("Wczytalem A:");
        print(A);

        System.out.println("\nWczytalem B:");
        print(B);

        Matrix C = mult(A,B);
        System.out.println("A*B = ");

        print(C);

        System.out.println("Jak doszedlem tutaj, to pewnie jest ok :-).");

    }

    private Matrix mult(Matrix A, Matrix B) {
        Matrix C = new Matrix(A.rows(), B.cols());

        for (int r = 0; r < A.rows(); r++) {
            for (int c = 0; c < B.cols(); c++) {
                float s = 0;
                for (int k = 0; k < A.cols(); k++) {
                    s += A.get(r,k) * B.get(k, c);
                }
                C.set(r,c,s);
            }
        }

        return C;
    }

    protected Matrix read(String fname) throws FileNotFoundException {
        File f = new File(fname);
        Scanner scanner = new Scanner(f).useLocale(Locale.ENGLISH);

        int rows  = scanner.nextInt();
        int cols  = scanner.nextInt();
        Matrix res = new Matrix(rows,cols);

        for (int r = 0; r < res.rows(); r++) {
            for (int c = 0; c < res.cols(); c++) {
                res.set(r, c, scanner.nextFloat());
            }
        }
        return res;
    }

    protected void print(Matrix m) {
        System.out.println("[");
        for (int r = 0; r < m.rows(); r++) {

            for (int c = 0; c < m.cols(); c++) {
                System.out.print(m.get(r,c));
                System.out.print(" ");
            }

            System.out.println("");
        }
        System.out.println("]");
    }


    public class Matrix {
        private int ncols;
        private int nrows;
        private float _data[];

        public Matrix(int r, int c) {
            this.ncols = c;
            this.nrows = r;
            _data = new float[c*r];
        }

        public float get(int r, int c) {
            return _data[r*ncols + c];
        }

        public void set(int r, int c, float v) {
            _data[r*ncols +c] = v;
        }

        public int rows() {
            return nrows;
        }

        public int cols() {
            return ncols;
        }
    }


}
