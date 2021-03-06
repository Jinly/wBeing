#include <gui.hpp>
#include <ctime>
#include <cstdlib>

static unsigned n = 0;
int rand_init(void) {
	n = 0;
	srand(time(NULL));
}

int r_rand(void) {
	if (n >= RAND_MAX)
		srand(time(NULL));
	return rand();
}

void put_4pixel (GdkPixbuf *pixbuf, int x, int y, Color color) {
	int width, height, rowstride, n_channels;
	Color *pixels;
	n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	pixels = (Color *) (gdk_pixbuf_get_pixels (pixbuf) + y * rowstride + x * n_channels);
	pixels[0] = color;
	pixels[1] = color;
	pixels = (Color *) (((char *)pixels) + rowstride);
	pixels[0] = color;
	pixels[1] = color;
}
