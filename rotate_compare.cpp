#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

unsigned char clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (unsigned char)v;
}

int main() {
    ifstream in("input.ppm", ios::binary);
    if (!in) {
        cout << "Gagal membuka input.ppm\n";
        return 1;
    }

    string format;
    int width, height, maxval;
    in >> format >> width >> height >> maxval;
    in.ignore();

    if (format != "P6") {
        cout << "File bukan P6\n";
        return 1;
    }

    vector<unsigned char> input(width * height * 3);
    in.read((char*)input.data(), input.size());
    in.close();

    int outW = width * 2;
    int outH = height;
    vector<unsigned char> output(outW * outH * 3, 0);

    double theta = 30.0 * M_PI / 180.0;
    double cosT = cos(theta);
    double sinT = sin(theta);

    int cx = width / 2;
    int cy = height / 2;

    // BACKWARD MAPPING
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int xc = x - cx;
            int yc = y - cy;

            double xs =  cosT * xc + sinT * yc + cx;
            double ys = -sinT * xc + cosT * yc + cy;

            // ===== NEAREST NEIGHBOR (KIRI) =====
            int xi = round(xs);
            int yi = round(ys);

            if (xi >= 0 && xi < width && yi >= 0 && yi < height) {
                for (int c = 0; c < 3; c++) {
                    output[(y * outW + x) * 3 + c] =
                        input[(yi * width + xi) * 3 + c];
                }
            }

            // ===== BILINEAR (KANAN) =====
            int x0 = floor(xs);
            int y0 = floor(ys);
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            if (x0 >= 0 && x1 < width && y0 >= 0 && y1 < height) {
                double a = xs - x0;
                double b = ys - y0;

                for (int c = 0; c < 3; c++) {
                    int I00 = input[(y0 * width + x0) * 3 + c];
                    int I10 = input[(y0 * width + x1) * 3 + c];
                    int I01 = input[(y1 * width + x0) * 3 + c];
                    int I11 = input[(y1 * width + x1) * 3 + c];

                    int val =
                        (1 - a) * (1 - b) * I00 +
                        a * (1 - b) * I10 +
                        (1 - a) * b * I01 +
                        a * b * I11;

                    output[(y * outW + (x + width)) * 3 + c] =
                        clamp(val);
                }
            }
        }
    }

    ofstream out("comparison.ppm", ios::binary);
    out << "P6\n" << outW << " " << outH << "\n" << maxval << "\n";
    out.write((char*)output.data(), output.size());
    out.close();

    cout << "comparison.ppm berhasil dibuat\n";
    return 0;
}
