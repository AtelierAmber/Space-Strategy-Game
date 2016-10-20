#pragma once

	typedef unsigned int TexParam;
	
	enum TexParams{
		/* DEFAULT, PIXELATED, LINEAR, MIPMAP, REPEAT, EDGE_CLAMP, TRANS_BORDER */
		DEFAULT = 0x1,
		PIXELATED = 0x2,
		LINEAR = 0x4,
		MIPMAP = 0x8,
		REPEAT = 0x10,
		EDGE_CLAMP = 0x20,
		TRANS_BORDER = 0x40, /***< IF NECESSARY Use EDGE_CLAMP if possible*/
	};