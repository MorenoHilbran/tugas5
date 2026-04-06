#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

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
        cout << "Bukan P6\n";
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

    // =========================
    // FORWARD MAPPING (KIRI)
    // =========================
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int xc = x - cx;
            int yc = y - cy;

            int xd = cosT * xc - sinT * yc + cx;
            int yd = sinT * xc + cosT * yc + cy;

            if (xd >= 0 && xd < width && yd >= 0 && yd < height) {
                for (int c = 0; c < 3; c++) {
                    output[(yd * outW + xd) * 3 + c] =
                        input[(y * width + x) * 3 + c];
                }
            }
        }
    }

    // =========================
    // BACKWARD MAPPING (KANAN)
    // =========================
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int xc = x - cx;
            int yc = y - cy;

            int xs =  cosT * xc + sinT * yc + cx;
            int ys = -sinT * xc + cosT * yc + cy;

            if (xs >= 0 && xs < width && ys >= 0 && ys < height) {
                for (int c = 0; c < 3; c++) {
                    output[(y * outW + (x + width)) * 3 + c] =
                        input[(ys * width + xs) * 3 + c];
                }
            }
        }
    }

    ofstream out("mapping_compare.ppm", ios::binary);
    out << "P6\n" << outW << " " << outH << "\n" << maxval << "\n";
    out.write((char*)output.data(), output.size());
    out.close();

    cout << "mapping_compare.ppm berhasil dibuat\n";
    return 0;
}
