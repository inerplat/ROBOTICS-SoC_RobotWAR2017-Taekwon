#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

#define SZ(v) ((int)(v).size())
#define ALL(v) (v).begin(),(v).end()
#define one first
#define two second
typedef long long ll;
typedef unsigned long long ull;
typedef pair<double, double> pd;
typedef pair<int, int> pi; typedef pair<ll, int> pli;
typedef pair<ll, ll> pll; typedef pair<ll, pi> plp;
const int INF = 0x3f2f1f0f;
const ll LINF = 1ll * INF * INF;


void rgbToXyz(int rgbI[3], double xyz[3]) {
	double rgb[3]; for (int i = 0; i<3; i++) rgb[i] = (double)rgbI[i] / 256.;
	for (int i = 0; i<3; i++) if (rgb[i] > 0.04045) rgb[i] = pow(((rgb[i] + 0.055) / 1.055), 2.4); else rgb[i] = rgb[i] / 12.92;
	for (int i = 0; i<3; i++) rgb[i] *= 100;
	double mat[3][3] = { { 0.4124, 0.3576, 0.1805 },{ 0.2126, 0.7152, 0.0722 },{ 0.0193, 0.1192, 0.9505 } };
	for (int ik = 0; ik<3; ik++) for (int jk = 0; jk<3; jk++) xyz[ik] += mat[ik][jk] * rgb[jk];
}
void xyzToLab(double xyzD[3], double lab[3]) {
	double xyz[3] = { 0, 0, 0 };
	for (int i = 0; i<3; i++) xyz[i] = xyzD[i];
	double div[3] = { 95.047, 100.000, 108.883 };
	for (int i = 0; i<3; i++) xyz[i] /= div[i];
	for (int i = 0; i<3; i++) if (xyz[i] > 0.008856) xyz[i] = pow(xyz[i], 1. / 3.); else xyz[i] = (7.787 * xyz[i]) + (16. / 116.);
	double x = xyz[0], y = xyz[1], z = xyz[2];
	lab[0] = 116. * y - 16.; lab[1] = 500. * (x - y); lab[2] = 200. * (y - z);
}

const int RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, BLACK = 4, ORANGE = 5, YELLOW = 6, NONE = 7, GRAY =8;


int Color[256 / 8][256 / 4][256 / 8], Ch[9] = { 4, 0, 1, 6, 2, 5, 7, 3 , 8};
// 000, 100, 010, 110, 001, 101, 011, 111
// Bla,   R,   G,   Y, Blu,   O,   -,   W
//   4,   0,   1,   6,   2,   5,   -,   3


int main() {
	freopen("output.txt", "w", stdout);

	int colorRGB[9][3] = { 0, };
	double colorXYZ[9][3] = { 0, };
	double colorLAB[9][3] = { 0, };

	// 000, 100, 010, 110, 001, 101, 011, 111
	// Bla,   R,   G,   Y, Blu,   O,   -,   W
	for (int s = 0; s<8; s++) {
		for (int ik = 0; ik<3; ik++) if (s & (1 << ik)) colorRGB[s][ik] = 200; else colorRGB[s][ik] = 20;
		if (s == 5) {
			colorRGB[s][RED] = 200;
			colorRGB[s][GREEN] = 128;
			colorRGB[s][BLUE] = 20;
		}
		rgbToXyz(colorRGB[s], colorXYZ[s]);
		xyzToLab(colorXYZ[s], colorLAB[s]);
	}
	for (int ik = 0; ik < 3; ik++) colorRGB[8][ik] = 100;
	rgbToXyz(colorRGB[8], colorXYZ[8]);
	xyzToLab(colorXYZ[8], colorLAB[8]);

	printf("{");
	for (int r = 0; r<256; r += 8) {
		if (r != 0) printf(",");
		printf("{");
		for (int g = 0; g<256; g += 4) {
			if (g != 0) printf(",");
			printf("{");
			for (int b = 0; b<256; b += 8) {
				int rgb[3] = { r, g, b };
				double xyz[3] = { 0, 0, 0 }, lab[3] = { 0, 0, 0 };
				rgbToXyz(rgb, xyz);
				xyzToLab(xyz, lab);

				double minV = 1e18; int minIx = -1;
				for (int s = 0; s<=8; s++) {
					int cnt = 0; for (int k = 0; k<3; k++) if (s & (1 << k)) cnt++;
					double nowV = 0, mul = 1;
					// 000, 100, 010, 110, 001, 101, 011, 111
					// Bla,   R,   G,   Y, Blu,   O,   -,   W
					double muls[9] = { 1, 3, 3, 4, 3, 3, 0.001, 3 , 0.5};
					mul = muls[s];
					for (int ik = 0; ik<3; ik++) nowV += (colorLAB[s][ik] - lab[ik]) * (colorLAB[s][ik] - lab[ik]);
					nowV /= mul;
					if (nowV < minV) minV = nowV, minIx = s;
				}

				Color[r / 8][g / 4][b / 8] = Ch[minIx];
				if (b != 0) printf(",");
				printf("%d", Ch[minIx]);
				if (minV > 2000) {
					//something?
				}
			}
			printf("}");
		}
		printf("}");
	}
	printf("}");
	return 0;
}
