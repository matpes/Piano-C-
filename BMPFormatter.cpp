#include "BMPFormatter.h"





void BMPFormatter::createFile(string x)
{
	int width, height;
	cout << "Insert width and height: ";
	cin >> width >> height;
	bmp = new BMP(width, height);
	bmp->fill_region(compozicija);
	x += ".bmp";
	bmp->write(x.c_str());
}

void BMP::fill_region(Composition * c)
{
	uint32_t channels = bmp_info_header.bit_count / 8;
	uint32_t h = bmp_info_header.width;
	uint32_t w = bmp_info_header.height;
	uint32_t y = 0, x = 0;
	//auto iterL = c->beginL();
	for_each(c->beginR(), c->endR(), [&x, &y, &w, &h, &channels, this](Measure &m) {

		for_each(m.begin(), m.end(), [&x, &y, &w, &h, &channels, this](MusicSymbol* ms) {
			uint8_t B, G, R;

			int i = 1;
			if (ms->getDuration().getDenominator() == 4) { i++; }
			if (y >= h) return;
			while (i > 0) {
				determine_colors(R, G, B, ms);
				data[channels * (y * w + x) + 0] = B;
				data[channels * (y * w + x) + 1] = G;
				data[channels * (y * w + x) + 2] = R;
				++x;
				if (x == w) {
					y ++;
					if (y >= h) {
						break;
					}
					x = 0;
				}
				i--;
			}
		});
		if (y >= h) {
			return;
		}
	});


	for_each(c->beginL(), c->endL(), [&x, &y, &w, &h, &channels, this](Measure &m) {

		for_each(m.begin(), m.end(), [&x, &y, &w, &h, &channels, this](MusicSymbol* ms) {
			uint8_t B, G, R;
			int i = 1;
			if (ms->getDuration().getDenominator() == 4) { i++; }
			if (y >= h) return;
			while (i > 0) {
				determine_colors(R, G, B, ms);
				data[channels * (y * w + x) + 0] = B;
				data[channels * (y * w + x) + 1] = G;
				data[channels * (y * w + x) + 2] = R;
				++x;
				if (x == w) {
					++y;
					if (y >= h) return;
					x = 0;
				}
				i--;
			}
		});


		if (y >= h) {
			return;
		}
	});
	for (; y < h ; y ++) {
		for (; x < w; x++) {
			data[channels * (y * w + x) + 0] = 255;
			data[channels * (y * w + x) + 1] = 255;
			data[channels * (y * w + x) + 2] = 255;
		}
		x = 0;
	}
}

void BMP::determine_colors(uint8_t &R, uint8_t &G, uint8_t &B, MusicSymbol * ms)
{
	if (typeid(*ms) == typeid(Pause)) {
		R = G = B = 255; 
		return;
	}
	if (typeid(*ms) == typeid (Note)) {
		Note * n = dynamic_cast<Note*>(ms);
		Note::det_color(n, R, G, B);
		return;
	}
	Chord * c = dynamic_cast<Chord*>(ms);
	int i = c->getChord()->size();
	int r, g, b;
	r = g = b = 0;
	for (Note *n : *c->getChord()) {
		Note::det_color(n, R, G, B);
		r += R;
		g += G;
		b += B;
	}
	R = r / i;
	G = g / i;
	B = b / i;
}

BMPFormatter::~BMPFormatter()
{
}
