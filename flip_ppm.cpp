#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
    ifstream in("input.ppm", ios::binary);
    if (!in) {
        cout << "Gagal membuka input.ppm\n";
        return 1;
    }

    string format;
    int width, height, maxval;

    in >> format;
    in >> width >> height;
    in >> maxval;
    in.ignore(); // buang newline setelah header

    if (format != "P6") {
        cout << "Bukan P6\n";
        return 1;
    }

    vector<unsigned char> data(width * height * 3);
    in.read((char*)data.data(), data.size());
    in.close();

    // Flip horizontal
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            int left = (y * width + x) * 3;
            int right = (y * width + (width - 1 - x)) * 3;

            for (int c = 0; c < 3; c++) {
                swap(data[left + c], data[right + c]);
            }
        }
    }

    ofstream out("output.ppm", ios::binary);
    out << "P6\n" << width << " " << height << "\n" << maxval << "\n";
    out.write((char*)data.data(), data.size());
    out.close();

    cout << "Flip horizontal selesai.\n";
    return 0;
}
