#ifndef image_h_
#define image_h_

namespace air
{
    typedef unsigned long	u32;
	typedef long			s32;
	typedef unsigned short	u16;
	typedef short			s16;
	typedef unsigned char	u8;
	typedef char			s8;
    // 2011-5-7   这种方式交换两数是存在问题的，即不能对同一个变量使用SWAP
	#define SWAP(A, B)		(A) = (A) + (B);	\
							(B) = (A) - (B);	\
							(A) = (A) - (B)
	struct rgb
	{
		u8 r, g, b;
	};
	struct rgba
	{
		u8 r, g, b, a;
	};

	//////////////////////////////////////////////////////////////////////////
	// 支持 24 位未压缩位图
	class CBmp
	{
	private:
		u8*	m_p_rgb;
		u8* m_p_rgba;
		u32	m_w;
		u32	m_h;

		// 位图文件头信息
#pragma pack(1)
		struct head
		{
			u16 id;
			u32 size;
			u16 dummy1;
			u16 dummy2;
			u32 offset_image;
		};
#pragma pack()
		// 位图文件信息
		struct info
		{
			u32 info_size;
			u32 pixel_width;
			u32 pixel_height;
			u16 planes;
			u16 bit_per_pixel;	// 位图位数
			u32 compress;		// 0 表示未压缩
			u32 image_size;
			u32 horizon;
			u32 vertical;
			u32 color_used;
			u32 color_important;
		};

	public:
		CBmp();
		~CBmp();

		/*
			载入位图
			p_file：	位图文件
			返回值：	载入成功 / 失败
		*/
		bool load(const char* p_file);

		/*
			将 RGB 图像数据转换为 RGBA
			r，g，b：	透明色
		*/
		void convert_to_rgba(u8 r, u8 g, u8 b);

		/*
			得到位图相关信息
		*/
		const u8* rgb_data() const;
		const u8* rgba_data() const;
		u32 width() const;
		u32 height() const;

		/*
			释放内存
		*/
		void unload();
	private:
		CBmp(const CBmp&);
		CBmp& operator=(const CBmp&);
	};

	//////////////////////////////////////////////////////////////////////////
	// 支持 32 位（带 alpha 通道）未压缩 tga
	class CTga
	{
	private:
		u32 m_w;
		u32 m_h;
		u8* m_p_rgba;	// rgb or rgba

		// tga 文件头
#pragma pack(1)
		struct head
		{
			u8 info_length;
			u8 color_map_type;
			u8 image_type;
			u8 color_map[5];
			u16 x_origin;
			u16 y_origin;
			u16 image_width;
			u16 image_height;
			u8 bit_per_pixel;
			u8 image_desc;
		};
#pragma pack()

	public:
		CTga();
		~CTga();

		/*
			载入tga
			p_file：	位图文件
			返回值：	载入成功 / 失败
		*/
		bool load(const char* p_file);

		/*
			释放内存
		*/
		void unload();

		/*
			得到图像相关信息
		*/
		const u8* data() const;
		u32 width() const;
		u32 height() const;

	private:
		CTga(const CTga&);
		CTga& operator=(const CTga&);
	};
}

#endif
