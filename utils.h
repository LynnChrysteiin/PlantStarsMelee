#ifndef _UTILS_H_
#define _UTILS_H_

#include <graphics.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buf = GetImageBuffer(src);
	DWORD* dst_buf = GetImageBuffer(dst);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w - x - 1);
			dst_buf[idx_dst] = src_buf[idx_src];
		}
	}
}

inline void putimage_with_alpha(int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(
		GetImageHDC(NULL), dst_x, dst_y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }
	);
}

inline void putimage_with_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	AlphaBlend(
		GetImageHDC(NULL), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y), w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }
	);
}

inline void putimage_with_alpha(int dst_x, int dst_y, int width, int height, IMAGE* image, int src_x, int src_y)
{
	int w = width > 0 ? width : image->getwidth();
	int h = height > 0 ? height : image->getheight();
	AlphaBlend(
		GetImageHDC(NULL), dst_x, dst_y, w, h,
		GetImageHDC(image), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }
	);
}

inline void sketch_frame(IMAGE* src, IMAGE* dst, COLORREF color_ref = RGB(255, 255, 255))
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buf = GetImageBuffer(src);
	DWORD* dst_buf = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx = y * w + x;
			dst_buf[idx] = BGR(color_ref) | (src_buf[idx] & 0xFF000000);
		}
	}
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	const Vector2& pos_camera = camera.get_position();
	::line(x1 - (int)pos_camera.x, y1 - (int)pos_camera.y, x2 - (int)pos_camera.x, y2 - (int)pos_camera.y);
}

inline void rectangle(const Camera& camera, int left, int top, int right, int bottom)
{
	const Vector2& pos_camera = camera.get_position();
	::rectangle(left - (int)pos_camera.x, top - (int)pos_camera.y, right - (int)pos_camera.x, bottom - (int)pos_camera.y);
}

inline void solidcircle(const Camera& camera, int x, int y, int radius)
{
	const Vector2& pos_camera = camera.get_position();
	::solidcircle(x - (int)pos_camera.x, y - (int)pos_camera.y, radius);
}

#endif // !_UTILS_H_
