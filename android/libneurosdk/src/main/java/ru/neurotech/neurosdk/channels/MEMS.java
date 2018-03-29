package ru.neurotech.neurosdk.channels;

public class MEMS {
    public final Accelerometer accelerometer;
    public final Gyroscope gyroscope;

    public MEMS(double aX, double aY, double aZ,
                double gX, double gY, double gZ){
        accelerometer = new Accelerometer(aX, aY, aZ);
        gyroscope = new Gyroscope(gX, gY, gZ);
    }

    public class Accelerometer{
        public Accelerometer(double x, double y, double z){
            X = x;
            Y = y;
            Z = z;
        }
        public double X;
        public double Y;
        public double Z;
    }
    public class Gyroscope{
        public Gyroscope(double x, double y, double z){
            X = x;
            Y = y;
            Z = z;
        }
        public double X;
        public double Y;
        public double Z;
    }
}
