//-----------------------------------------------------------------------------
// DIRECTX LIBRARY
// This include file contains functions that access Direct X.
// By Patrick Lester, pwlester@policyalmanac.org

// You also will need the Direct X 7.0 SDK, and to install the lib
// and include files from the SDK to your C++ lib and include
// folders. You can get the DirectX 7.0 SDK (large!) here:
// http://download.microsoft.com/download/win98SE/SDK/7.0/W9X/EN-US/dx7sdk-700.1.exe
//-----------------------------------------------------------------------------

#include <ddraw.h> //Direct Draw header
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>
#define D3D_OVERLOADS
#include <d3d.h>
#include <string> //used by Debuglog


//-----------------------------------------------------------------------------
// Name: Function prototypes (where needed) 
//-----------------------------------------------------------------------------
DWORD ColorMatch(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb);
HRESULT LoadPalette(LPDIRECTDRAWPALETTE* ppPalette,const TCHAR* strBMP );
int ReleaseObjects(void);
static HRESULT CALLBACK TextureSearchCallback(DDPIXELFORMAT* pddpf, VOID* param);
void UpdateMouse (void);
void Debuglog (char* pDebuglog);
void Debuglog (int number);
int SetBrush (int r, int g, int b, int pattern=0, int style=0);
int SetFontColor(int r,int g,int b);
int SetPen (int r, int g, int b, int thickness=1, int style=0);



//-----------------------------------------------------------------------------
// Name: Class Declarations
//-----------------------------------------------------------------------------
class cImage //class containing info about an image
{
public:
	LPDIRECTDRAWSURFACE7 pSurface; //pointer to the image
	int imageWidth;
	int imageHeight;
	int xHandle;
	int yHandle;
	bool isMasked; //0 = no, 1 = yes
	int convertedMaskColor;//mask color converted to used pixel format
	byte memLocation; //0 = VRAM, 1 = RAM
	bool imageIs3D; //0 = false, 1 = true

	cImage* previousImage;//pointer to previous item in linked list
	cImage* nextImage;//pointer to next item in linked list

	//constructor member function
	cImage()
	{
	previousImage = NULL;
	nextImage = NULL;
	}
};


//Concatening string class, CSTR: This class is used to combine
//strings and numbers into a single, usable string for the
//Debuglog and Text commands.
#define combine g_strString-
class CSTR 
{
	public:
		char actualString[255];
		CSTR& operator-(char* nextString);
		CSTR& operator-(int number);
		CSTR& operator-(double number);
		CSTR& operator+(char* nextString);
		CSTR& operator+(int number);
		CSTR& operator+(double number);
};
CSTR g_strString; //declare global CSTR

CSTR& CSTR::operator-(char* nextString){
	strcpy (this->actualString,nextString); 
	return g_strString;}

CSTR& CSTR::operator-(int number){
	char temp [33];
	itoa (number,temp,10); 
	strcpy (this->actualString,temp);
	return g_strString;}

CSTR& CSTR::operator-(double number){
	char temp [20];
	gcvt (number,10,temp);
	strcpy (this->actualString,temp);
	return g_strString;}

CSTR& CSTR::operator+(char* nextString){
	strcat (this->actualString,nextString); 
	return g_strString;}

CSTR& CSTR::operator+(int number){
	char temp [33];
	itoa (number,temp,10); 
	strcat (this->actualString,temp); 
	return g_strString;}
	
CSTR& CSTR::operator+(double number){
	char temp [20];
	gcvt (number,10,temp);
	strcat (this->actualString,temp);
	return g_strString;}

void Debuglog (CSTR pStringDebuglog);

//-----------------------------------------------------------------------------
// Name: Globals
//-----------------------------------------------------------------------------
HWND                        g_hWndGame=NULL; //game's Window handle
LPDIRECTDRAW7               g_pDirectDraw = NULL;
LPDIRECTDRAWSURFACE7        g_pFrontBuffer= NULL; 
LPDIRECTDRAWSURFACE7        g_pBackBuffer = NULL;
LPDIRECTDRAWPALETTE         g_pPalette = NULL;
PALETTEENTRY                g_PaletteEntry[256];
LPDIRECT3D7                 g_pD3D = NULL;
LPDIRECT3DDEVICE7           g_pD3DDevice = NULL;
D3DTLVERTEX                 g_Vertex[4]; //quad vertices for 3D images
LPDIRECTINPUT7              g_pDirectInput = NULL;
LPDIRECTINPUTDEVICE         g_pMouse=NULL;
DIMOUSESTATE                g_pMouseData;
int                         g_MouseX=0;
int                         g_MouseY=0;
bool                        g_MouseDown1=false;
bool                        g_MouseDown2=false;
int                         g_MouseHit1=0;
int                         g_MouseHit2=0;
int                         g_MouseSpeed=2;
DWORD                       g_ClsColor=0;
COLORREF                    g_FontColor=RGB(255,255,255);
HBRUSH                      g_hBrush=NULL;
HFONT                       g_hFont=NULL;
HPEN                        g_hPen=NULL;
int                         g_screenWidth = 0;
int                         g_screenHeight = 0;
int                         g_screenDepth = 0;
short                       g_screenFormat16Bit = 0;
bool                        g_AutoMidHandle=false;
int                         g_Pitch=0;
void*                       g_pSurface = NULL;

//Globals used to keep track of current drawing buffer
char                        g_pCurrentBuffer = 2;//1=front, 2 = back, 3 = image
cImage*                     g_pCurrentImage = NULL;//pointer to the cImage in buffer
LPDIRECTDRAWSURFACE7        g_pCurrentSurface = NULL;//pointer to the surface in buffer

//Globals used by cImage class	
cImage*                     g_pFirstImage = NULL;//pointer to first item in linked list
cImage*                     g_pLastImage = NULL;//pointer to last item in linked list


//-----------------------------------------------------------------------------
// Name: AutoMidHandle
// Desc: Automatically sets the image handle of loaded images to the 
//	middle of the image. Only affects images loaded or created 
//	after this command is called.
//-----------------------------------------------------------------------------
void AutoMidHandle (bool inValue)
{ 	
	g_AutoMidHandle = inValue;
}


//-----------------------------------------------------------------------------
// Name: AvailSysMem()
// Desc: Returns the available system memory (RAM) in bytes.
//  - Example = Draw3DImage demo
//-----------------------------------------------------------------------------
int AvailSysMem(void) 
{ 	
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	return(stat.dwAvailPhys);
}


//-----------------------------------------------------------------------------
// Name: AvailVidMem()
// Desc: Returns the available video memory in bytes
//  - Example = FreeImage demo
//-----------------------------------------------------------------------------
int AvailVidMem(void) 
{
	DWORD totalVRAM=0;
	DWORD freeVRAM=0;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));//clear memory block for structure
	ddscaps.dwCaps = DDSCAPS_VIDEOMEMORY;  
	g_pDirectDraw->GetAvailableVidMem(&ddscaps,&totalVRAM,&freeVRAM);
	return freeVRAM;
} 


//-----------------------------------------------------------------------------
// Name: BackBuffer
// Desc: Returns a "pointer" to the back buffer
//-----------------------------------------------------------------------------
int BackBuffer (void)
{
	return 2;
}


//-----------------------------------------------------------------------------
// Name: Bin()
// Desc: Converts an unsigned integer to its binary equivalent in the 
//	form of a string. It is stored in a global string.
//	Example = ReadPixel
//-----------------------------------------------------------------------------
char g_BinaryString[33];//global string
void Bin(unsigned int number) 
{
	strcpy (g_BinaryString,"00000000000000000000000000000000");
	for (int x=1;x<32;x++)
	{
		unsigned int a = (unsigned int) pow(2,x);
		unsigned int b = (unsigned int) pow(2,x-1);
		if ((number % a)/b == 1) g_BinaryString[32-x] = 49;
	}
}


//-----------------------------------------------------------------------------
// Name: Cls
// Desc: Clears the screen with the designated CLS color, which is black
// unless changed by the ClsColor() command.
//-----------------------------------------------------------------------------
int Cls (void)
{
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = g_ClsColor;

	// Blit the entire current buffer with the specified fill color
	HRESULT hr = g_pCurrentSurface->Blt(
	NULL, //destination rectangle of blit (0 = blit entire surface)
	NULL, //address of source surface blitted from (0 used here)
	NULL, //source rectangle of blit (0 = blit from entire surface)
	DDBLT_COLORFILL | DDBLT_WAIT, //blit flags; DDBLT_COLORFILL = use dwFillColor member
	// of the DDBLTFX structure as the RGB color that fills the destination
	&ddbltfx );//address of special effects ddbltfx structure created above

	if (hr == DD_OK)
		return 1;
	else
		return hr;
}



//-----------------------------------------------------------------------------
// Name: ClsColor
// Desc: Changes the color for the Cls command. Default is black.
//-----------------------------------------------------------------------------
void ClsColor (int r, int g, int b)
{
	COLORREF rgb = RGB(r,g,b);
	if (rgb == 0) g_ClsColor=0;
	if (rgb != 0) g_ClsColor = ColorMatch(g_pFrontBuffer, rgb);
}


//-----------------------------------------------------------------------------
// Name: Color()
// Desc: Changes the current drawing color for text writing and GUI draw
//	commands like Line, Oval, and Rect. You can set each of these
//	separately using SetFontColor, SetBrush and SetPen. 
//-----------------------------------------------------------------------------
void Color(int r, int g, int b)
{
	SetFontColor(r,g,b);
	SetBrush(r,g,b);
	SetPen(r,g,b);
}


//-----------------------------------------------------------------------------
// Name: ColorMatch()
// Desc: This function translates a specified rgb color value into a format
//	appropriate to the color depth being used. For example, if 8 bit color
//	depth is used, it translates it into the appropriate palette entry. 
//	If 16 bit color depth is used, it translates the rgb into a 16 bit 
//	color, etc.
//	
//	Used by MaskImage and Cls
//-----------------------------------------------------------------------------
DWORD ColorMatch(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb)
{
    COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hres;

    // Use GDI SetPixel to color match for us
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->ReleaseDC(hdc);
    }

    //Now lock the surface so we can read back the converted color
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;// Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;// Mask it to bpp
        pdds->Unlock(NULL);
    }

    //Now put the color that was there back.
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}



//-----------------------------------------------------------------------------
// Name: CopyImage
// Desc: Creates a new image that is a copy of another, pre-existing
//	image.
//	Example = CopyRect demo
//-----------------------------------------------------------------------------
cImage* CopyImage (cImage* pImageSource, int loadTo=0)
{
	HRESULT                     hr;

	//Error check
	if(pImageSource == NULL) return 0;
	if(pImageSource->pSurface == NULL) return 0;

	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid
	if (loadTo == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (loadTo == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1
	ddsd.dwWidth = pImageSource->imageWidth;
	ddsd.dwHeight = pImageSource->imageHeight;

	//Create the Direct Draw surface in specified memory area
	LPDIRECTDRAWSURFACE7 pSurfaceNew; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pSurfaceNew, //address of the variable pointing to the surface
	NULL)  //advanced COM stuff
	!= DD_OK) return 0;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=0;
	rectDest.right=pImageSource->imageWidth;
	rectDest.top=0;
	rectDest.bottom=pImageSource->imageHeight;

	//Blit the existing image to the new image copy.
	hr = pSurfaceNew->Blt(
	&rectDest, //Destination RECT
	pImageSource->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //flags
	NULL ); //special effects

	if (hr != DD_OK)
	{
		pSurfaceNew->Release();
		pSurfaceNew = NULL;
		return 0;
	}

	//Create a new cImage class instance to hold image data 
	cImage* pImageNew = new cImage;
	pImageNew->pSurface = pSurfaceNew; //pointer to the image
	pImageNew->imageWidth = ddsd.dwWidth;
	pImageNew->imageHeight = ddsd.dwHeight;
	if (g_AutoMidHandle == true)
	{
	pImageNew->xHandle = pImageNew->imageWidth/2;
	pImageNew->yHandle = pImageNew->imageHeight/2;
	}
	pImageNew->isMasked = false;

	//Update linked list of images
	if (g_pFirstImage == NULL) g_pFirstImage = g_pLastImage = pImageNew;
	if (g_pFirstImage != pImageNew) 
	{
	g_pLastImage->nextImage = pImageNew;//link this item to end of list
	pImageNew->previousImage = g_pLastImage;//do a return link
	g_pLastImage = pImageNew;//make this item the last one on the list
	}

	return pImageNew;//return pointer pImageNew
}


//-----------------------------------------------------------------------------
// Name: CopyRect
// Desc: Copies a portion of one image to another, or to the current
//	drawing buffer
//	Example = CopyRect demo
//-----------------------------------------------------------------------------
int CopyRect (int x1, int y1, int width, int height, int x2, int y2,
			  cImage* pImage1, cImage* pImage2, 
			  int pointer1=-1, int pointer2=-1)
{
	//Get source image pointer
	LPDIRECTDRAWSURFACE7 pSourceImage=NULL;
	if (pointer1 == 0)
		pSourceImage = g_pCurrentSurface;
	else if (pointer1 == 1)
		pSourceImage = g_pFrontBuffer;
	else if (pointer1 == 2)
		pSourceImage = g_pBackBuffer;
	else
		pSourceImage = pImage1->pSurface;

	//Get destination image pointer
	LPDIRECTDRAWSURFACE7 pDestImage=NULL;
	if (pointer2 == 0)
		pDestImage = g_pCurrentSurface;
	else if (pointer2 == 1)
		pDestImage = g_pFrontBuffer;
	else if (pointer2 == 2)
		pDestImage = g_pBackBuffer;
	else
		pDestImage = pImage2->pSurface;

	//Compute source rectangle
	RECT rectSource;
	rectSource.left=x1;
	rectSource.right=x1 + width;
	rectSource.top=y1;
	rectSource.bottom=y1 + height;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=x2;
	rectDest.right=x2 + width;
	rectDest.top=y2;
	rectDest.bottom=y2 + height;

	//Blit to the destination image
	HRESULT hr = pDestImage->Blt(
	&rectDest, //Destination RECT
	pSourceImage,  //lpDDSrcSurface
	&rectSource, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //flags
	NULL ); //special effects

	if (hr == DD_OK) 
		return 1;
	else
		return hr;
}

//-----------------------------------------------------------------------------
// Name: CopyRect
// Desc: Overloaded version of above function.
//-----------------------------------------------------------------------------
int CopyRect (int x1, int y1, int width, int height, int x2, int y2,
			  int pointer1, int pointer2)
{
	return CopyRect (x1, y1, width, height, x2, y2,
			  NULL,NULL, pointer1, pointer2);
}


//-----------------------------------------------------------------------------
// Name: CopyRect
// Desc: Overloaded version of above function.
//-----------------------------------------------------------------------------
int CopyRect (int x1, int y1, int width, int height, int x2, int y2,
			  int pointer1, cImage* image2)
{
	return CopyRect (x1, y1, width, height, x2, y2,
			  NULL,image2, pointer1, -1);
}


//-----------------------------------------------------------------------------
// Name: CopyRect
// Desc: Overloaded version of above function.
//-----------------------------------------------------------------------------
int CopyRect (int x1, int y1, int width, int height, int x2, int y2,
			  cImage* image1, int pointer2)
{
	return CopyRect (x1, y1, width, height, x2, y2,
			  image1,NULL, -1, pointer2);
}


//-----------------------------------------------------------------------------
// Name: CreateImage
// Desc: Creates an image of the specified dimensions, ready for 
//	writing or copying.
//-----------------------------------------------------------------------------
cImage* CreateImage (int width, int height, int loadTo=0)
{
	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid

	//Specify type of surface (plain offscreen) and location (VRAM/RAM)
	if (loadTo == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (loadTo == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1

	//Record image dimensions.
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	//Create the Direct Draw surface in specified memory area
	LPDIRECTDRAWSURFACE7 pSurface; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pSurface, //address of the variable pointing to the surface
	NULL)  //advanced COM stuff
	!= DD_OK) return 0;

	//Fill the surface with the color black.
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	HRESULT hr = pSurface->Blt(
	NULL, //destination rectangle of blit (0 = blit entire surface)
	NULL, //address of source surface blitted from (0 used here)
	NULL, //source rectangle of blit (0 = blit from entire surface)
	DDBLT_COLORFILL | DDBLT_WAIT, //blit flags; DDBLT_COLORFILL = use dwFillColor member
	// of the DDBLTFX structure as the RGB color that fills the destination
	&ddbltfx );//address of special effects ddbltfx structure created above

	//Create a new cImage class instance to hold image data 
	cImage* pImage = new cImage;
	pImage->pSurface = pSurface; //pointer to the image
	pImage->imageWidth = ddsd.dwWidth;
	pImage->imageHeight = ddsd.dwHeight;
	pImage->isMasked = false;
	pImage->memLocation = loadTo;
	pImage->imageIs3D = false;
	pImage->xHandle = 0;
	pImage->yHandle = 0;

	//Update linked list of images
	if (g_pFirstImage == NULL) g_pFirstImage = g_pLastImage = pImage;
	if (g_pFirstImage != pImage) 
	{
	g_pLastImage->nextImage = pImage;//link this item to end of list
	pImage->previousImage = g_pLastImage;//do a return link
	g_pLastImage = pImage;//make this item the last one on the list
	}

	return pImage;//return pointer pImage
}

//-----------------------------------------------------------------------------
// Name: Draw3DImage()
// Desc: Draws a 3D-enabled image, which supports real-time rotations, scaling
//	and alpha blending. Not currently supported in 8 bit (paletted) or 24 bit 
//	display mode.
//	Example = Draw3DImage demo
//-----------------------------------------------------------------------------
int Draw3DImage (cImage* pImage, int x, int y, int alpha=255,
                  int scalePercent=100,int angle=0, int flipped=0)
{
	//Not supported in 8 or 24 bit screen mode
	if (g_screenDepth == 8 || g_screenDepth == 24) return 0; 

	//Error check
	if(pImage == NULL) return E_INVALIDARG;
	if(pImage->pSurface == NULL) return E_INVALIDARG;
	if (alpha <= 0 || scalePercent <= 0) return 1;//don't draw invisible images

	x = x-pImage->xHandle;
	y = y-pImage->yHandle;

	//Set location of 3D image by adjusting image vertices
	if (scalePercent == 100 && angle == 0 && flipped == 0) //simple (no effects)
	{
		    g_Vertex[0].sx = (float) x;
		    g_Vertex[0].sy = (float) y;

		    g_Vertex[1].sx = (float) x+pImage->imageWidth;
		    g_Vertex[1].sy = (float) y;

		    g_Vertex[2].sx = (float) x;
		    g_Vertex[2].sy = (float) y+pImage->imageHeight;

		    g_Vertex[3].sx = (float) x+pImage->imageWidth;
		    g_Vertex[3].sy = (float) y+pImage->imageHeight;
	}

	else if (flipped == 0) //if it is not flipped
	{
		//This rotation method avoids the D3D T&L pipeline. We do our own 
		//transformations and send the transformed vertices to the video card.
		int xRadius = pImage->imageWidth*scalePercent/200;
		int yRadius = pImage->imageHeight*scalePercent/200;
		int midPointX = x+pImage->imageWidth/2;
		int midPointY = y+pImage->imageHeight/2;
		double multiple = 3.14159265/180;

	    g_Vertex[0].sx = (float) (midPointX+1.41*cos((135+angle)*multiple)*xRadius);
	    g_Vertex[0].sy = (float) (midPointY-1.41*sin((135+angle)*multiple)*yRadius);

	    g_Vertex[1].sx = (float) (midPointX+1.41*cos((45+angle)*multiple)*xRadius);
	    g_Vertex[1].sy = (float) (midPointY-1.41*sin((45+angle)*multiple)*yRadius);

	    g_Vertex[2].sx = (float) (midPointX+1.41*cos((225+angle)*multiple)*xRadius);
	    g_Vertex[2].sy = (float) (midPointY-1.41*sin((225+angle)*multiple)*yRadius);

	    g_Vertex[3].sx = (float) (midPointX+1.41*cos((315+angle)*multiple)*xRadius);
	    g_Vertex[3].sy = (float) (midPointY-1.41*sin((315+angle)*multiple)*yRadius);
	}

	else //if image is flipped
	{
		if (scalePercent == 100 && angle == 0) //if it is not rotated or scaled
		{
		    g_Vertex[0].sx = (float) x+pImage->imageWidth;
		    g_Vertex[0].sy = (float) y;

		    g_Vertex[1].sx = (float) x;
		    g_Vertex[1].sy = (float) y;

		    g_Vertex[2].sx = (float) x+pImage->imageWidth;
		    g_Vertex[2].sy = (float) y+pImage->imageHeight;

		    g_Vertex[3].sx = (float) x;
		    g_Vertex[3].sy = (float) y+pImage->imageHeight;
		}

		else //if it is rotated and/or scaled
		{
			//This rotation method avoids the D3D T&L pipeline. We do our own 
			//transformations and send the transformed vertices to the video card.
			int xRadius = pImage->imageWidth*scalePercent/200;
			int yRadius = pImage->imageHeight*scalePercent/200;
			int midPointX = x+pImage->imageWidth/2;
			int midPointY = y+pImage->imageHeight/2;
			double multiple = 3.14159265/180;

		    g_Vertex[0].sx = (float) (midPointX+1.41*cos((315+angle)*multiple)*xRadius);
		    g_Vertex[0].sy = (float) (midPointY-1.41*sin((135+angle)*multiple)*yRadius);

		    g_Vertex[1].sx = (float) (midPointX+1.41*cos((225+angle)*multiple)*xRadius);
		    g_Vertex[1].sy = (float) (midPointY-1.41*sin((45+angle)*multiple)*yRadius);

		    g_Vertex[2].sx = (float) (midPointX+1.41*cos((45+angle)*multiple)*xRadius);
		    g_Vertex[2].sy = (float) (midPointY-1.41*sin((225+angle)*multiple)*yRadius);

		    g_Vertex[3].sx = (float) (midPointX+1.41*cos((135+angle)*multiple)*xRadius);
		    g_Vertex[3].sy = (float) (midPointY-1.41*sin((315+angle)*multiple)*yRadius);
		}
	}

	//Set render state
    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_CLIPPING,false);
    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING,false);
    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_CULLMODE,D3DCULL_NONE);
	if (pImage->isMasked == true)
		g_pD3DDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);
	if (alpha != 255) //if image is alpha blended
	{
	    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,true);
	    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA );
	    g_pD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND,D3DBLEND_INVSRCALPHA);
	    g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTA_TFACTOR);
	}
	g_Vertex[0].color = RGBA_MAKE(255, 255, 255, alpha);
	g_Vertex[1].color = RGBA_MAKE(255, 255, 255, alpha);
	g_Vertex[2].color = RGBA_MAKE(255, 255, 255, alpha);
	g_Vertex[3].color = RGBA_MAKE(255, 255, 255, alpha);

	//Set texture
	g_pD3DDevice->SetTexture(0,pImage->pSurface);

    // Begin the scene
    if(FAILED(g_pD3DDevice->BeginScene())) return -1;

    //DrawPrimitive to render the quad
    g_pD3DDevice->DrawPrimitive(
        D3DPT_TRIANGLESTRIP,
		//D3DFVF_TLVERTEX, //which is the following
		D3DFVF_XYZRHW | 
		D3DFVF_DIFFUSE | 
		D3DFVF_SPECULAR |
        D3DFVF_TEX1, 
		g_Vertex,
		4,
		0);

    // End the scene.
    g_pD3DDevice->EndScene();

	//Free the texture from the 3D device, thereby avoiding memory leaks
	g_pD3DDevice->SetTexture(0,NULL);

	if (alpha != 255)  
		g_pD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,false);

	return 1;
}


//-----------------------------------------------------------------------------
// Name: DrawBlock
// Desc: This function blits an unmasked version of an image on the screen
//	Example = DrawImage demo
//-----------------------------------------------------------------------------
int DrawBlock (cImage* pImage, int x, int y)
{
	//Get incoming info
	if(pImage == NULL) return E_INVALIDARG;
	if(pImage->pSurface == NULL) return E_INVALIDARG;
	x = x-pImage->xHandle; //x position to draw image
	y = y-pImage->yHandle; //y position to draw image

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=x;
	rectDest.right=x + pImage->imageWidth;
	rectDest.top=y;
	rectDest.bottom=y + pImage->imageHeight;

	//Blit to the current buffer
	HRESULT hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //flags
	NULL ); //special effects

	//Return result (should be DD_OK, which is 0, if drawn ok)
	return hr;
}


//-----------------------------------------------------------------------------
// Name: DrawFlipped
// Desc: This function blits a mirror image of an image on the screen
//	- Mode 0 = horizontal mirror; 1 = vertical; 2 = both (180 degree rotate)
// This command may be somewhat slower than Draw3DImage in flipped mode.
//-----------------------------------------------------------------------------
int DrawFlipped (cImage* pImage, int x, int y, int mode=0)
{
	HRESULT                     hr;

	//Get incoming info
	if(pImage == NULL) return E_INVALIDARG;
	if(pImage->pSurface == NULL) return E_INVALIDARG;
	x = x-pImage->xHandle; //x position to draw image
	y = y-pImage->yHandle; //y position to draw image

	//Set up Mirrored Blit stuff
	int effect = 0;
	if (mode == 0) effect = DDBLTFX_MIRRORLEFTRIGHT;
	if (mode == 1) effect = DDBLTFX_MIRRORUPDOWN;
	if (mode == 2) effect = DDBLTFX_MIRRORLEFTRIGHT | DDBLTFX_MIRRORUPDOWN;
	DDBLTFX  ddbltfx;  
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = effect;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=x;
	rectDest.right=x + pImage->imageWidth;
	rectDest.top=y;
	rectDest.bottom=y + pImage->imageHeight;

	//Blit to the current buffer
	//If image is not masked, draw it as-is
	if (pImage->isMasked == false)
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_WAIT | DDBLT_DDFX,//flags
	&ddbltfx ); //special effects
	}

	//If image is masked, draw it masked.
	else
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_KEYSRC | DDBLT_WAIT | DDBLT_DDFX, //flags
	&ddbltfx ); //special effects
	}

	//Return result (should be DD_OK, which is 0, if drawn ok)
	return hr;
}


//-----------------------------------------------------------------------------
// Name: DrawImage
// Desc: This function blits a masked image on the screen.
//	Example = DrawImage demo
//-----------------------------------------------------------------------------
int DrawImage (cImage* pImage, int x, int y)
{
	HRESULT                     hr;
	//Error check
	if(pImage == NULL) return E_INVALIDARG;
	if(pImage->pSurface == NULL) return E_INVALIDARG;

	x = x-pImage->xHandle;
	y = y-pImage->yHandle;
	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=x;
	rectDest.right=x + pImage->imageWidth;
	rectDest.top=y;
	rectDest.bottom=y + pImage->imageHeight;

	//Blit to the backbuffer
	//If image is not masked, draw it as-is
	if (pImage->isMasked == false)
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //DDBLT_WAIT = wait until blitter is free
	NULL ); //special effects
	}

	//If image is masked, draw it masked.
	else
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	NULL, //Source RECT or NULL for entire surface
	DDBLT_KEYSRC | DDBLT_WAIT, //DDBLT_KEYSRC = use source color key
	NULL ); //special effects
	}

	//Return result (should be DD_OK, which is 0, if drawn ok)
	return hr;
}


//-----------------------------------------------------------------------------
// Name: DrawImageRect
// Desc: Draws a portion of an image to the backbuffer. If the original
//	is masked, this will be too. Handle information is ignored.
//  - Example = CopyRect demo.
//-----------------------------------------------------------------------------
int DrawImageRect (cImage* pImage, int x2, int y2, int x1, int y1,
					 int width, int height)
{
	//Get incoming info
	if(pImage == NULL) return -1;
	if(pImage->pSurface == NULL) return -1;

	//Compute source rectangle
	RECT rectSource;
	rectSource.left=x1;
	rectSource.right=x1 + width;
	rectSource.top=y1;
	rectSource.bottom=y1 + height;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=x2;
	rectDest.right=x2 + width;
	rectDest.top=y2;
	rectDest.bottom=y2 + height;

	//Blit to the backbuffer
	//If image is not masked, draw it as-is
	HRESULT hr;
	if (pImage->isMasked == false)
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	&rectSource, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //DDBLT_WAIT = wait until blitter is free
	NULL ); //special effects
	}

	//If image is masked, draw it masked.
	else
	{
	hr = g_pCurrentSurface->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	&rectSource, //Source RECT or NULL for entire surface
	DDBLT_KEYSRC | DDBLT_WAIT, //DDBLT_KEYSRC = use source color key
	NULL ); //special effects
	}

	if (hr == DD_OK) 
		return 1;
	else
		return hr;
}


//-----------------------------------------------------------------------------
// Name: EndGraphics
// Desc: Releases all Direct X objects. Required at the end of each
//	program.
//-----------------------------------------------------------------------------
int EndGraphics (void)
{
	return ReleaseObjects();
}


//-----------------------------------------------------------------------------
// Name: Flip
// Desc: This function flips stuff drawn on the back buffer to the front buffer
//	It also updates the mouse position.
//-----------------------------------------------------------------------------
int Flip (void)
{
	HRESULT                     hr;
	while (TRUE)
	{
        hr = g_pFrontBuffer->Flip(NULL, 0);
        if (hr == DD_OK)
            break;
        if (hr == DDERR_SURFACELOST)
        {
            hr = g_pDirectDraw->RestoreAllSurfaces(); //Reload images
            if (hr != DD_OK) break; 
			hr = g_pMouse->Acquire(); //reaquire mouse for Direct Input
			ShowCursor(FALSE);
			g_MouseX = g_screenWidth/2;
			g_MouseY = g_screenHeight/2;
        }
        if (hr != DDERR_WASSTILLDRAWING)
            break;
	}
    UpdateMouse();
	return (1);
}



//-----------------------------------------------------------------------------
// Name: FlipImage
// Desc: This function flips an image horizontally, vertically, or both.
//	Unlike DrawFlipped, which draws a flipped version of the original
//	image on screen, this function flips the original image itself.
//	Example = Scale & Rotate demo
//-----------------------------------------------------------------------------
int FlipImage (cImage* pImage, int mode=0)
{
	//Get incoming info
	if(pImage == NULL) return 0;
	if(pImage->pSurface == NULL) return 0;	
	char loadTo = pImage->memLocation;

	//Create a temporary image to copy flipped image to
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid
	if (loadTo == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (loadTo == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1
	ddsd.dwWidth = pImage->imageWidth;
	ddsd.dwHeight = pImage->imageHeight;
	LPDIRECTDRAWSURFACE7 pTempImage; //pointer to Direct Draw surface
	HRESULT hr = g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pTempImage, //address of the variable pointing to the new image
	NULL);  //advanced COM stuff
	if (hr != DD_OK) return hr;

	//Set up Mirrored Blit stuff
	int effect = 0;
	if (mode == 0) effect = DDBLTFX_MIRRORLEFTRIGHT;
	if (mode == 1) effect = DDBLTFX_MIRRORUPDOWN;
	if (mode == 2) effect = DDBLTFX_MIRRORLEFTRIGHT | DDBLTFX_MIRRORUPDOWN;
	DDBLTFX  ddbltfx;  
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = effect;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=0;
	rectDest.right=pImage->imageWidth;
	rectDest.top=0;
	rectDest.bottom=pImage->imageHeight;

	//Blit it to the temporary image
	pTempImage->Blt(
	    &rectDest, //Destination RECT
		pImage->pSurface,  //lpDDSrcSurface
	    NULL, //Source RECT or NULL for entire surface
	    DDBLT_WAIT | DDBLT_DDFX,//flags
	    &ddbltfx ); //special effects

	//Now delete the original image and put the new image
	//in its place
	pImage->pSurface->Release();
	pImage->pSurface = pTempImage;

	//If appropriate, mask the new image the same as the original
	if (pImage->isMasked == true)
	{
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = pImage->convertedMaskColor;
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	pImage->pSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	return 1;
}


//-----------------------------------------------------------------------------
// Name: FPS() 
// Desc: Returns the current frames per second. Should be called every
//	frame to ensure accuracy.
//	- Example = DrawImage demo
//-----------------------------------------------------------------------------
int FPS (void)
{
	static int time = 0, FPS = 0, frames = 0, frames0 = 0;
	if (timeGetTime() >= time) 
	{
		FPS = frames-frames0;
		time = timeGetTime()+1000;
		frames0 = frames;
	}
	frames = frames+1;
	return FPS;
}


//-----------------------------------------------------------------------------
// Name: FreeImage 
// Desc: Frees the VRAM associated with a previously created image.
//  The command does not delete the image's container class, however, which 
//	consumes only a few bytes of memory. This provides a certain level of
//	error protection against subsequent DrawImage calls using
//	a pointer to an image that no longer exists. If a DrawImage or
//	similar command is called on an image that no longer exists,
//	it will do nothing. Unfortunately, this does not protect against
//	DrawImage calls on images that were never created in the first 
//	place.
//  - Example = FreeImage demo
//-----------------------------------------------------------------------------
int FreeImage (cImage* pImage)
{
	if(pImage == NULL) return 0;
	if(pImage->pSurface == NULL) return 0;

	//Release the image associated with this class pointer
	if (pImage->pSurface != NULL)
	{
	pImage->pSurface->Release();//release VRAM
	pImage->pSurface = NULL;//set pointer to NULL
	}

	return DD_OK;
}


//-----------------------------------------------------------------------------
// Name: FrontBuffer
// Desc: Returns a "pointer" to the front buffer.
//-----------------------------------------------------------------------------
int FrontBuffer (void)
{
	return 1;
}


//-----------------------------------------------------------------------------
// Name: Get16BitFormat
// Desc: Returns the particular 16 bit format used when in 16 bit mode
//-----------------------------------------------------------------------------
int Get16BitFormat (void)
{ 	
	return g_screenFormat16Bit;
}


//-----------------------------------------------------------------------------
// Name: GetMouseSpeed
// Desc: Returns the current mouse speed. This number does not correlate
//	with Windows system mouse speed values.
//-----------------------------------------------------------------------------
int GetMouseSpeed (void)
{ 	
	return g_MouseSpeed;
}


//-----------------------------------------------------------------------------
// Name: GetR
// Desc: Returns the r component of an RGB value.
//-----------------------------------------------------------------------------
int GetR(unsigned int rgb)
{
	return rgb >> 16 & 0xff;
}


//-----------------------------------------------------------------------------
// Name: GetG
// Desc: Returns the r component of an RGB value.
//-----------------------------------------------------------------------------
int GetG(unsigned int rgb)
{
	return rgb >> 8 & 0xff;
}


//-----------------------------------------------------------------------------
// Name: GetB
// Desc: Returns the r component of an RGB value.
//-----------------------------------------------------------------------------
int GetB(unsigned int rgb)
{
	return rgb & 0xff;
}


//-----------------------------------------------------------------------------
// Name: GetRGB
// Desc: Returns the current mouse speed. This number does not correlate
//	with Windows system mouse speed values.
//-----------------------------------------------------------------------------
int GetRGB(byte r, byte g, byte b)
{
	return r << 16 | g << 8 | b;
}


//-----------------------------------------------------------------------------
// Name: Graphics 
// Desc: Creates an instance of Direct Draw.
//-----------------------------------------------------------------------------
int Graphics (int screenWidth, int screenHeight, int screenDepth, 
			  HWND hWnd, char* pPalleteFile = NULL)
{

	//Record screen dimensions as globals.
	g_screenWidth = screenWidth;
	g_screenHeight = screenHeight;
	g_screenDepth = screenDepth;
	g_hWndGame = hWnd;

	//Create the Direct Draw object. Use DirectDrawCreateEx to enable
	//3d stuff.
	HRESULT hr = DirectDrawCreateEx(
         NULL, //use the GUID of the active display
         (VOID**)&g_pDirectDraw,//address of variable representing Direct Draw object
         IID_IDirectDraw7,//Specify Direct Draw 7
         NULL); //Advanced COM stuff
	if (hr != DD_OK) return 2;

	//Set the cooperative level
	g_hWndGame = hWnd; //save this info globally for the debuglog
	hr = g_pDirectDraw->SetCooperativeLevel(
         hWnd, //handle of program's window
         DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if (hr != DD_OK) {ReleaseObjects();return 3;}

	// Set the display mode
	hr = g_pDirectDraw->SetDisplayMode(
         g_screenWidth, g_screenHeight, //set screen width, height
         g_screenDepth,//set the screen bit depth (8,16,24,32)
         0, //screen refresh rate - set to 0 for hardware default rate
         0); //for rarely used standard VGA mode stuff
	if (hr != DD_OK) 
	{
		if (g_pDirectDraw != NULL)
		{
		g_pDirectDraw->Release();
		g_pDirectDraw = NULL;
		}
		return 4;
	}

	//Create the primary surface (frontbuffer)
	DDSURFACEDESC2              ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX |
						  DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;
	hr = g_pDirectDraw->CreateSurface(&ddsd, &g_pFrontBuffer, NULL);
	if (hr != DD_OK) {ReleaseObjects();return 5;}

	//Create the backbuffer
	DDSCAPS2                    ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hr = g_pFrontBuffer->GetAttachedSurface(&ddscaps,&g_pBackBuffer);
	if (hr != DD_OK) {ReleaseObjects();return 6;}

	//Make the back buffer the current buffer.
	g_pCurrentBuffer = 2;
	g_pCurrentSurface = g_pBackBuffer;

	//If in 8 bit mode, set a palette
	if (g_screenDepth == 8)
	{
	LoadPalette(&g_pPalette,pPalleteFile);
	g_pFrontBuffer->SetPalette(g_pPalette);
	g_pBackBuffer->SetPalette(g_pPalette);
	}

	//If in 16 bit mode, save the screen format (555 or 565)
	if (g_screenDepth == 16)
	{
		DDSURFACEDESC2 ddsd2;
		ZeroMemory(&ddsd2, sizeof(ddsd2));
		ddsd2.dwSize = sizeof(ddsd2);
		hr = g_pFrontBuffer->GetSurfaceDesc(&ddsd2); 
		if (ddsd2.ddpfPixelFormat.dwGBitMask == 992) g_screenFormat16Bit = 555;//555 16 bit
		if (ddsd2.ddpfPixelFormat.dwGBitMask == 2016) g_screenFormat16Bit = 565;//565 16 bit
	}

	//Create the clipper object
	LPDIRECTDRAWCLIPPER	pcClipper; //Declare the clipper object
	hr = g_pDirectDraw->CreateClipper( //create the clipper object
	NULL, //unused flag
	&pcClipper, //address of variable pointing to clipper object
	NULL); //advanced COM stuff
	if (hr != DD_OK) {ReleaseObjects();return 8;}

	//Create the clip list, which is a list of RECTS (rectangles) we want to
	//clip to. At this point there is only one item on the list, the screen.
	LPRGNDATA lpClipList //declare pointer to RGNDATA structure containing list
		= (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)//allocate memory (malloc) for
		+ sizeof(RECT)); //header; allocate memory for rect holding screen
	RECT rcBoundary = {0, 0,g_screenWidth,g_screenHeight};//boundaries of screen
	memcpy(lpClipList->Buffer, &rcBoundary, sizeof(RECT));// set clip list
	lpClipList->rdh.dwSize = sizeof(RGNDATAHEADER);   // size of header
	lpClipList->rdh.iType = RDH_RECTANGLES;    // type of clip regions
	lpClipList->rdh.nCount = 1;               // number of rects in clip list
	lpClipList->rdh.nRgnSize = sizeof(RECT); // size of Buffer
	lpClipList->rdh.rcBound = rcBoundary;// bounding RECT for all items on list

	//Set the clip list
	pcClipper->SetClipList(
	lpClipList, //address of a valid RGNDATA structure, created above
	0);//unused flag
	if (hr != DD_OK) 
	{free(lpClipList);ReleaseObjects();return 9;}

	//Attach the clipper to the backbuffer
	if( FAILED( hr = g_pBackBuffer->SetClipper( pcClipper ) ) )
	{
	pcClipper->Release();
	ReleaseObjects();
	return 10;
	}

	//Free the clipper and clip list.
	free(lpClipList);
	pcClipper->Release();

	//Hide the Windows cursor
	ShowCursor(FALSE);

	//Set up Direct Input
	hr = DirectInputCreateEx(
	  GetModuleHandle(NULL),//HINSTANCE hinst,                  
	  DIRECTINPUT_VERSION,                  
	  IID_IDirectInput7,
	  (VOID**)&g_pDirectInput,  
	  NULL);
	if (hr != DD_OK) {ReleaseObjects();return 11;}

	//Set up a Direct Input-enabled mouse
	hr = g_pDirectInput->CreateDeviceEx(
		GUID_SysMouse,// 
		IID_IDirectInputDevice7,
        (void**)&g_pMouse,
		NULL);
	if (hr != DD_OK) {ReleaseObjects();return 12;}
	hr = g_pMouse->SetDataFormat(&c_dfDIMouse);
	if (hr != DD_OK) {ReleaseObjects();return 13;}
	hr = g_pMouse->SetCooperativeLevel(hWnd,
               DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DD_OK) {ReleaseObjects();return 14;}

	//Set up buffer for MouseHit()
	DIPROPDWORD dipdw; //device property header data structure
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = 10; //# of buffered data items
	hr = g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (hr != DI_OK)
	{
	if (hr == DI_PROPNOEFFECT) 
		{ReleaseObjects();return 15;}
	else
		{ReleaseObjects();return hr;}
	}

	//Acquire the mouse
	hr = g_pMouse->Acquire();
	if (hr != DD_OK) {ReleaseObjects();return 16;}
	g_MouseX = g_screenWidth/2;
	g_MouseY = g_screenHeight/2;

	//If not in 8 bit/24 bit mode, activate 3D mode for special effect
	//(fast rotations, alpha blending, etc.).
	if (g_screenDepth != 8 && g_screenDepth != 24)
	{

	//Query DirectDraw for access to Direct3D
    hr = g_pDirectDraw->QueryInterface(IID_IDirect3D7,(VOID**)&g_pD3D);
    if(FAILED(hr)) {ReleaseObjects();return 17;};

	//Create the D3D device using hardware
	hr = g_pD3D->CreateDevice(
		IID_IDirect3DHALDevice, 
		g_pBackBuffer, 
		&g_pD3DDevice);//I'm getting a no palette attached error

	//If no hardware support, create the D3D device using sofware
    if(FAILED(hr))
    {
        hr = g_pD3D->CreateDevice( 
		IID_IDirect3DRGBDevice, 
		g_pBackBuffer,
		&g_pD3DDevice);
	    if(FAILED(hr)) {ReleaseObjects();return 18;}
    }

    //Create the viewport
    D3DVIEWPORT7 vp = {0,0, g_screenWidth, g_screenHeight, 0.0f, 1.0f };
    hr = g_pD3DDevice->SetViewport(&vp);
	if(FAILED(hr)) {ReleaseObjects();return 19;}
	}//if (g_screenDepth != 8)

	//Return function result
	return 1;
}


//-----------------------------------------------------------------------------
// Name: HandleImage
// Desc: Sets a handle for an image.
//-----------------------------------------------------------------------------
int HandleImage (cImage* pImage, int x, int y)
{ 	
	if(pImage == NULL) return -1;
	if(pImage->pSurface == NULL) return -1;
	pImage->xHandle = x; //x handle
	pImage->yHandle = y; //y handle
	return 1;
}


//-----------------------------------------------------------------------------
// Name: HidePointer
// Desc: Hides the Windows cursor (default = hidden)
//-----------------------------------------------------------------------------
void HidePointer (void)
{
	ShowCursor(FALSE);
}


//-----------------------------------------------------------------------------
// Name: ImageHeight
// Desc: Gets the width of a given image.
//-----------------------------------------------------------------------------
int ImageHeight (cImage* pImage)
{
	if(pImage == NULL) return -1;
	return (pImage->imageHeight);
}


//-----------------------------------------------------------------------------
// Name: ImagesCollide
// Desc: Checks to see if two images have collided. This command
//	considers pixel transparency, unlike ImagesOverlap, and it is a 
//	bit slower as a result. It is faster if at least the first of the
//	two images is stored in RAM.
//-----------------------------------------------------------------------------
int ImagesCollide (cImage* pImage1, int x1, int y1,
					 cImage* pImage2, int x2, int y2)
{
	if(pImage1 == NULL) return -1;
	x1 = x1-pImage1->xHandle;
	y1 = y1-pImage1->yHandle;

	if(pImage2 == NULL) return -2;
	x2 = x2-pImage2->xHandle;
	y2 = y2-pImage2->yHandle;

	//Figure out whether there is a simple overlap
	int overlaps = 0;
	if (x2 < x1+pImage1->imageWidth){
		if (x2+pImage2->imageWidth > x1){
			if (y2 < y1+pImage1->imageHeight){
				if (y2+pImage2->imageHeight > y1){
				overlaps = 1;
	}}}}

	//If there is an overlap, figure out the corners,
	//width, and height of the collision box.
	int a=0; int b = 0; int c = 0; int d= 0; 
	int width = 0; int height = 0;
	if (overlaps == 1)
		if (x2 > x1)
		{
			if (y2 > y1)
			{
				width = x1 + pImage1->imageWidth - x2;
				height = y1 + pImage1->imageHeight - y2;
				a = pImage1->imageWidth-width;
				b = pImage1->imageHeight-height;
				c = 0; 
				d = 0;
			}
			else
			{
				width = x1 + pImage1->imageWidth - x2;
				height = y2 + pImage2->imageHeight - y1;
				a = pImage1->imageWidth-width;
				b = 0;
				c = 0; 
				d = pImage2->imageHeight-height;
			}
		}
		else
		{
			if (y2 > y1)
			{
				width = x2 + pImage2->imageWidth - x1;
				height = y1 + pImage1->imageHeight - y2;
				a = 0;
				b = pImage1->imageHeight-height;
				c = pImage2->imageWidth-width; 
				d = 0;
			}
			else
			{
				width = x2 + pImage2->imageWidth - x1;
				height = y2 + pImage2->imageHeight - y1;
				a = 0;
				b = 0;
				c = pImage2->imageWidth-width; 
				d = pImage2->imageHeight-height;
			}
		}
	else //if no overlap, return no collision
	{
	return 0;
	}
	int collision = 0;

	//Lock both surfaces
	DDSURFACEDESC2 ddsd1; //surface description data structure
	ZeroMemory(&ddsd1, sizeof(ddsd1));// zero-out the memory area
	ddsd1.dwSize = sizeof(ddsd1);
	HRESULT hr = pImage1->pSurface->Lock(
		NULL,//lock entire surface
		&ddsd1,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) //couldn't lock surface so exit
		return -3;
	int pitch1 = (int) ddsd1.lPitch; //get the pitch 
	int bytesPerPixel1 = ddsd1.ddpfPixelFormat.dwRGBBitCount/8;

	DDSURFACEDESC2 ddsd2; //surface description data structure
	ZeroMemory(&ddsd2, sizeof(ddsd2));// zero-out the memory area
	ddsd2.dwSize = sizeof(ddsd2);
	hr = pImage2->pSurface->Lock(
		NULL,//lock entire surface
		&ddsd2,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) //couldn't lock surface so exit
	{
		pImage1->pSurface->Unlock(NULL);
		return -4;
	}
	int pitch2 = (int) ddsd2.lPitch; //get the pitch in bytes
	int bytesPerPixel2 = ddsd2.ddpfPixelFormat.dwRGBBitCount/8;

	//Now do pixel detection in that box
	if (g_screenDepth == 8){
	byte* pSurface1 = (byte*) ddsd1.lpSurface; //Grab surface pointer
	byte* pSurface2 = (byte*) ddsd2.lpSurface; //Grab surface pointer
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
		if (*(pSurface1 + a+x + (b+y)*pitch1) != 
			pImage1->convertedMaskColor){
		if (*(pSurface2 + c+x + (d+y)*pitch2) != 
			pImage2->convertedMaskColor){
			collision = 1;
			break;
		}//if (*(pSurface2
		if (collision == 1) break;
		}//if (*(pSurface1
		}//for (int y = 0; y < height; y++){
	}//for (int x = 0; x < width; x++){
	}//if (g_screenDepth == 8){

	if (g_screenDepth == 16){
	short* pSurface1 = (short*) ddsd1.lpSurface; //Grab surface pointer
	short* pSurface2 = (short*) ddsd2.lpSurface; //Grab surface pointer
	pitch1 = pitch1/2; //adjust pitch to short value
	pitch2 = pitch2/2; //adjust pitch to short value
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
		if (*(pSurface1 + a+x + (b+y)*pitch1) != 
			pImage1->convertedMaskColor){
		if (*(pSurface2 + c+x + (d+y)*pitch2) != 
			pImage2->convertedMaskColor){
			collision = 1;
			break;
		}//if (*(pSurface2
		if (collision == 1) break;
		}//if (*(pSurface1
		}//for (int y = 0; y < height; y++){
	}//for (int x = 0; x < width; x++){
	}//if (g_screenDepth == 16){

	if (g_screenDepth == 24){
	byte* pSurface1 = (byte*) ddsd1.lpSurface; //Grab surface pointer
	byte* pSurface2 = (byte*) ddsd2.lpSurface; //Grab surface pointer
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
		if ((*((int*) (pSurface1+3*(a+x)+(b+y)*pitch1)) & 0x00FFFFFF) != 
			pImage1->convertedMaskColor){
		if ((*((int*) (pSurface2+3*(c+x)+(d+y)*pitch2)) & 0x00FFFFFF) != 
			pImage2->convertedMaskColor){
			collision = 1;
			break;
		}//if (*(pSurface2
		if (collision == 1) break;
		}//if (*(pSurface1
		}//for (int y = 0; y < height; y++){
	}//for (int x = 0; x < width; x++){
	}//if (g_screenDepth == 24){

	if (g_screenDepth == 32){
	int* pSurface1 = (int*) ddsd1.lpSurface; //Grab surface pointer
	int* pSurface2 = (int*) ddsd2.lpSurface; //Grab surface pointer
	pitch1 = pitch1/4; //adjust pitch to short value
	pitch2 = pitch2/4; //adjust pitch to short value
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
		if (*(pSurface1 + a+x + (b+y)*pitch1) != 
			pImage1->convertedMaskColor){
		if (*(pSurface2 + c+x + (d+y)*pitch2) != 
			pImage2->convertedMaskColor){
			collision = 1;
			break;
		}//if (*(pSurface2
		if (collision == 1) break;
		}//if (*(pSurface1
		}//for (int y = 0; y < height; y++){
	}//for (int x = 0; x < width; x++){
	}//if (g_screenDepth == 32){

	//Unlock both surfaces
	pImage1->pSurface->Unlock(NULL);
	pImage2->pSurface->Unlock(NULL);

	return collision; //return collision status
}


//-----------------------------------------------------------------------------
// Name: ImagesOverlap
// Desc: Checks to see if two images overlap. This command does not
//	consider whether pixels are transparent or not.
//-----------------------------------------------------------------------------
int ImagesOverlap (cImage* pImage1, int x1, int y1,
					 cImage* pImage2, int x2, int y2)
{
	if(pImage1 == NULL) return -1;
	x1 = x1-pImage1->xHandle;
	y1 = y1-pImage1->yHandle;

	if(pImage2 == NULL) return -2;
	x2 = x2-pImage2->xHandle;
	y2 = y2-pImage2->yHandle;

	int overlaps = 0;
	if (x2 < x1+pImage1->imageWidth){
		if (x2+pImage2->imageWidth > x1){
			if (y2 < y1+pImage1->imageHeight){
				if (y2+pImage2->imageHeight > y1){
				overlaps = 1;
	}}}}

	return overlaps;
}


//-----------------------------------------------------------------------------
// Name: ImageWidth
// Desc: Gets the width of a given image.
//-----------------------------------------------------------------------------
int ImageWidth (cImage* pImage)
{
	if(pImage == NULL) return -1;
	return(pImage->imageWidth);
}


//-----------------------------------------------------------------------------
// Name: ImageXHandle
// Desc: Returns x handle of an image
//-----------------------------------------------------------------------------
int ImageXHandle (cImage* pImage)
{ 	
	if(pImage == NULL) return -1;
	if(pImage->pSurface == NULL) return -2;
	return (pImage->xHandle);
}


//-----------------------------------------------------------------------------
// Name: ImageYHandle
// Desc: Returns y handle of an image
//-----------------------------------------------------------------------------
int ImageYHandle (cImage* pImage)
{ 	
	if(pImage == NULL) return -1;
    if(pImage->pSurface == NULL) return -2;
	return (pImage->yHandle);
}


//-----------------------------------------------------------------------------
// Name: KeyDown()
// Desc: Returns whether a specific key is down. The key is the virtual
//	key code of the key. For A through Z, a through z, or 0 through 9,
//	use the ASCII value. For other keys, use the virtual key code.
//  - Example = FreeImage demo
//-----------------------------------------------------------------------------
bool KeyDown (int nVirtKey)
{
	if ((GetKeyState(nVirtKey) & 0x8000) != 0)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// Name: KeyHit()
// Desc: Returns whether a specific key has been pressed and released.
//-----------------------------------------------------------------------------
bool KeyHit (int nVirtKey)
{
	static int keyPressed;
	if ((GetKeyState(nVirtKey) & 0x8000) != 0) //if key is down
	{
		keyPressed = nVirtKey;
		return false;
	}
	else if (keyPressed == nVirtKey) //if key is released after being pressed
	{
		keyPressed = 0;
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Name: Line()
// Desc: Draws a line at the specified location using the current pen.
//	Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Line (int startX, int startY, int endX, int endY)
{
	//Get hDC of backbuffer
	HDC hDC = NULL;
	g_pBackBuffer->Restore(); //Make sure the surface is restored	
	g_pBackBuffer->GetDC(&hDC);//Get the DC of the surface

	if (g_hPen != NULL) SelectObject(hDC, g_hPen); 
	SetBkMode(hDC, TRANSPARENT);

	//Move current drawing location
	MoveToEx(
		hDC,// handle to DC
		startX, // x-coordinate of starting point
		startY, // y-coordinate of starting point
		NULL); //pointer to a POINT structure that receives 
			//the previous current position

	//Draw the line
	LineTo(
		hDC,// handle to DC
		endX, // x-coordinate of ending point
		endY); // y-coordinate of ending point

	g_pBackBuffer->ReleaseDC(hDC);
	return (1);
}


//-----------------------------------------------------------------------------
// Name: Load3DImage()
// Desc: Loads a 3D-enabled sprite or image. Such images have the can use
//	real time rotations, scaling, and alpha blending. They do not
//	work in 8 bit (paletted) display modes, however. The loaded image
//	should also be square, and its width and height both a power of 2.
//	Example dimensions are 32x32, 64x64, 128x128, etc. Some video
//	cards may not be able to load images much larger than that.
//	- Example = Draw3DImage demo
//-----------------------------------------------------------------------------
cImage* Load3DImage (char* szBitmap)
{
	if (g_screenDepth == 8 || g_screenDepth == 24) return 0;

    HRESULT hr;

	//Try to load the bitmap from disk
	HBITMAP                 hbm;  //handle of the loaded bitmap
	BITMAP                  bm;   //bitmap class
    hbm = (HBITMAP) LoadImage( //type cast the handle
        NULL,  //hInstance of image
        szBitmap, //string indicating source of file
        IMAGE_BITMAP,//image type, as opposed to IMAGE_CURSOR or IMAGE_ICON
        0, //desired image width if resizing, 0 if using original width
        0, //desired image width if resizing, 0 if using original height
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//If that fails, exit
	if (hbm == NULL) return 0;

	//Save the bitmap in a Windows GUI-compatible device context
	HDC hdcImage = CreateCompatibleDC(NULL);//create device context for image
	SelectObject( //place the bitmap into the memory DC
	hdcImage,//device context of image created above
	hbm);//handle of the loaded bitmap

	//Set up the Direct Draw surface description
    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
    ddsd.dwSize          = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS       | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT       | //dwHeight member is valid
                 DDSD_WIDTH        | //dwWidth member is valid
                 DDSD_PIXELFORMAT  |
				 DDSD_TEXTURESTAGE | //it's a D3D texture
                 DDSD_CKSRCBLT;      //color key enabled
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE; 
    
	GetObject(hbm, sizeof(bm), &bm); // Get the size of the bitmap
    DWORD dwWidth  = (DWORD)bm.bmWidth;
    DWORD dwHeight = (DWORD)bm.bmHeight;
	ddsd.dwWidth = dwWidth;  //record image width (gotten from GetObject)
	ddsd.dwHeight = dwHeight;  //record image height (gotten from GetObject)

    // Adjust width and height, if the driver requires it
    D3DDEVICEDESC7 ddDesc;
    if(FAILED(g_pD3DDevice->GetCaps(&ddDesc)))
        return 0;
    if(ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2)
    {
        for( ddsd.dwWidth=1;  dwWidth>ddsd.dwWidth;   ddsd.dwWidth<<=1 );
        for( ddsd.dwHeight=1; dwHeight>ddsd.dwHeight; ddsd.dwHeight<<=1 );
    }
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
    {
        if(ddsd.dwWidth > ddsd.dwHeight) ddsd.dwHeight = ddsd.dwWidth;
        else                             ddsd.dwWidth  = ddsd.dwHeight;
    }

    // Enumerate the texture formats, and find the closest device-supported
    // texture pixel format. Use the TextureSearchCallback function.
    g_pD3DDevice->EnumTextureFormats( TextureSearchCallback, &ddsd.ddpfPixelFormat );
    if( 0L == ddsd.ddpfPixelFormat.dwRGBBitCount )
        return 0;

	//Create the Direct Draw surface in specified memory area
	LPDIRECTDRAWSURFACE7 pSurface; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pSurface, //address of the variable pointing to the surface
	NULL)  //advanced COM stuff
	!= DD_OK) return 0;

	//If this fails, exit
	if (pSurface == NULL) return 0;

	// Make sure the Direct Draw surface is restored.
	pSurface->Restore();

	//Get device context of Direct Draw surface
	HDC hdc;  //handle of display version of image
	if ((hr = pSurface->GetDC(&hdc)) == DD_OK)

	//Copy the bitmap to Direct Draw surface.
	{
	BitBlt(
		hdc,	// handle of VRAM destination DC
		0,		// x-coord of destination upper-left corner
		0,		// y-coord of destination upper-left corner
		ddsd.dwWidth,	// width of destination rectangle
		ddsd.dwHeight,	// height of destination rectangle
		hdcImage,	// handle of RAM source DC
		0, // width of source rectangle
		0, // height of source rectangle
		SRCCOPY );	// raster operation code
	}

	//Clean up by releasing stuff we don't need anymore
	if( FAILED( hr = pSurface->ReleaseDC(hdc))) return 0;
	DeleteDC(hdcImage);//delete the GUI device context (DC)
	DeleteObject(hbm);//delete the bitmap object.

	//Create a new cImage class instance to hold image data 
	cImage* pImage = new cImage;
	pImage->pSurface = pSurface; //pointer to the image
	pImage->imageWidth = ddsd.dwWidth;
	pImage->imageHeight = ddsd.dwHeight;
	pImage->isMasked = false;
	pImage->memLocation = 0;
	pImage->imageIs3D = true;
	if (g_AutoMidHandle == false)
	{
	pImage->xHandle = 0;
	pImage->yHandle = 0;
	}
	if (g_AutoMidHandle == true)
	{
	pImage->xHandle = pImage->imageWidth/2;
	pImage->yHandle = pImage->imageHeight/2;
	}

	//Update linked list of images
	if (g_pFirstImage == NULL) g_pFirstImage = g_pLastImage = pImage;
	if (g_pFirstImage != pImage) 
	{
	g_pLastImage->nextImage = pImage;//link this item to end of list
	pImage->previousImage = g_pLastImage;//do a return link
	g_pLastImage = pImage;//make this item the last one on the list
	}

	//dvRHW (D3DVALUE)
	g_Vertex[0].rhw = 1.0f;
	g_Vertex[1].rhw = 1.0f;
	g_Vertex[2].rhw = 1.0f;
	g_Vertex[3].rhw = 1.0f;

	//Base dcColor (D3DCOLOR) -- alpha (last) value can be altered
	g_Vertex[0].color = RGBA_MAKE(255, 255, 255, 127);//RGBA
	g_Vertex[1].color = RGBA_MAKE(255, 255, 255, 127);
	g_Vertex[2].color = RGBA_MAKE(255, 255, 255, 127);
	g_Vertex[3].color = RGBA_MAKE(255, 255, 255, 127);

	//dcSpecular (D3DCOLOR) 
	g_Vertex[0].specular = D3DRGB(1.0,1.0,1.0);
	g_Vertex[1].specular = D3DRGB(1.0,1.0,1.0);
	g_Vertex[2].specular = D3DRGB(1.0,1.0,1.0);
	g_Vertex[3].specular = D3DRGB(1.0,1.0,1.0);

	//Texture alignment (D3DVALUE). Since we are using 2D
	//sprites, we don't change this.
	g_Vertex[0].tu = 0.0f; g_Vertex[0].tv = 0.0f;
	g_Vertex[1].tu = 1.0f; g_Vertex[1].tv = 0.0f;
	g_Vertex[2].tu = 0.0f; g_Vertex[2].tv = 1.0f;
	g_Vertex[3].tu = 1.0f; g_Vertex[3].tv = 1.0f;

	return pImage;//return pointer pImage
}



//-----------------------------------------------------------------------------
// Name: LoadImage()
// Desc: Create a Direct Draw Surface from a bitmap resource.
//	- Example = DrawImage demo
//-----------------------------------------------------------------------------
cImage* LoadImage (char* szBitmap, char loadTo=0)
{
	HRESULT                 hr; //variable to receive results from DD commands

	//Try to load the bitmap from disk
	HBITMAP                 hbm;  //handle of the loaded bitmap
	BITMAP                  bm;   //bitmap class
	hbm = (HBITMAP) LoadImage( //type cast the handle
        NULL,  //hInstance of image
        szBitmap, //string indicating source of file
        IMAGE_BITMAP,//image type, as opposed to IMAGE_CURSOR or IMAGE_ICON
        0, //desired image width if resizing, 0 if using original width
        0, //desired image width if resizing, 0 if using original height
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);
 
	//If that fails, exit
	if (hbm == NULL) return NULL;

	//Save the bitmap in a Windows GUI-compatible device context
	//Note that doing this may slightly change the exact RGB colors used in the
	//original bitmap if the bytes per pixel in the incoming bitmap are
	//higher than the color depth being used in the game. For example,
	//if the game is a 256 color, 8 bit game, and the loaded image is 24 
	//color, "selecting" the bitmap into the compatible DC will involve
	//mapping the 24 color image as well as possible to the 256 color
	//palette that is being used. Conversions the other way (8 bit to 24 bit)
	//should involve no color loss, however.
	HDC hdcImage = CreateCompatibleDC(NULL);//create device context for image
	SelectObject( //place the bitmap into the memory DC
	hdcImage,//device context of image created above
	hbm);//handle of the loaded bitmap

	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid

	//Specify type of surface (plain offscreen) and location (VRAM/RAM)
	if (loadTo == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (loadTo == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1

	GetObject(hbm, sizeof(bm), &bm); // Get the size of the bitmap
	ddsd.dwWidth = bm.bmWidth;  //record image width (gotten from GetObject)
	ddsd.dwHeight = bm.bmHeight;  //record image height (gotten from GetObject)

	//Create the Direct Draw surface in specified memory area
	LPDIRECTDRAWSURFACE7 pSurface; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pSurface, //address of the variable pointing to the surface
	NULL)  //advanced COM stuff
	!= DD_OK) return NULL;

	//If this fails, exit
	if (pSurface == NULL) return NULL;

	// Make sure the Direct Draw surface is restored.
	pSurface->Restore();

	//Get device context of Direct Draw surface
	HDC hdc;  //handle of display version of image
	if ((hr = pSurface->GetDC(&hdc)) == DD_OK)

	//Copy the bitmap to Direct Draw surface.
	{
	BitBlt(
		hdc,	// handle of VRAM destination DC
		0,		// x-coord of destination upper-left corner
		0,		// y-coord of destination upper-left corner
		ddsd.dwWidth,	// width of destination rectangle
		ddsd.dwHeight,	// height of destination rectangle
		hdcImage,	// handle of RAM source DC
		0, // width of source rectangle
		0, // height of source rectangle
		SRCCOPY );	// raster operation code
	}

	//Clean up by releasing stuff we don't need anymore
	if( FAILED( hr = pSurface->ReleaseDC(hdc))) return NULL;
	DeleteDC(hdcImage);//delete the GUI device context (DC)
	DeleteObject(hbm);//delete the bitmap object.

	//Create a new cImage class instance to hold image data 
	cImage* pImage = new cImage;
	pImage->pSurface = pSurface; //pointer to the image
	pImage->imageWidth = ddsd.dwWidth;
	pImage->imageHeight = ddsd.dwHeight;
	pImage->isMasked = false;
	pImage->memLocation = loadTo;
	pImage->imageIs3D = false;
	if (g_AutoMidHandle == false)
	{
	pImage->xHandle = 0;
	pImage->yHandle = 0;
	}
	if (g_AutoMidHandle == true)
	{
	pImage->xHandle = pImage->imageWidth/2;
	pImage->yHandle = pImage->imageHeight/2;
	}

	//Update linked list of images
	if (g_pFirstImage == NULL) g_pFirstImage = g_pLastImage = pImage;
	if (g_pFirstImage != pImage) 
	{
	g_pLastImage->nextImage = pImage;//link this item to end of list
	pImage->previousImage = g_pLastImage;//do a return link
	g_pLastImage = pImage;//make this item the last one on the list
	}

	return pImage;//return pointer pImage
}


//-----------------------------------------------------------------------------
// Name: LoadPalette
// Desc: Loads a palette from a particular bitmap. This function is called
// from the Graphics function.
//-----------------------------------------------------------------------------
HRESULT LoadPalette(
	LPDIRECTDRAWPALETTE* ppPalette,//global variable pointing to palette
	const TCHAR* strBMP )//string representing file location of bitmap
{
	HANDLE            hFile = NULL;//handle of the bitmap file being opened
	DWORD             iColor; //specific color entry in palette
	DWORD             dwColors;//total number of colors used in bitmap
	BITMAPFILEHEADER  bf;//structure that holds bitmap file info
	BITMAPINFOHEADER  bi;//structure that holds bitmap header info
	DWORD             dwBytesRead;//used by ReadFile (see below)

	if( g_pDirectDraw == NULL || strBMP == NULL || ppPalette == NULL )
        return E_INVALIDARG;

	*ppPalette = NULL;

	// Read the bitmap's file, header, and palette information.
	hFile = CreateFile( 
	strBMP, //string representing file location of bitmap
	GENERIC_READ, //declare desired file access rights as generic read-only
	0, //Prevent access to file until handle is closed (see below)
	NULL, //security attributes of file
	OPEN_EXISTING, //action to take on file
	0, //file attributes flags (encripted, read-only, hidden, etc.)
	NULL);//unused file template parameter
	if( NULL == hFile )
        return E_FAIL;

	// Read the BITMAPFILEHEADER, which contains info about the bitmap's
	// type (bitmap) and size.
	ReadFile( 
	hFile, //handle of file to be read
	&bf,   //address of structure that will receive the read info
	sizeof(bf), //number of bytes to read from the file
	&dwBytesRead, //pointer to the variable that receives the number of bytes read.
	NULL );//overlapped bytes stuff (unused)
	if( dwBytesRead != sizeof(bf) )
	{
        CloseHandle( hFile );
        return E_FAIL;
	}


	// Read the BITMAPINFOHEADER, a data structure that contains info
	// about the bitmap's size, width, height, color depth, compression, etc.
	ReadFile( hFile, &bi, sizeof(bi), &dwBytesRead, NULL );
	if( dwBytesRead != sizeof(bi) )
	{
        CloseHandle( hFile );
        return E_FAIL;
	}

	// Read the PALETTEENTRY, read pallete info from the bitmap and 
	// save it in the g_PaletteEntry PALETTEENTRY structure.
	ReadFile( hFile, g_PaletteEntry, sizeof(g_PaletteEntry), &dwBytesRead, NULL );
	if( dwBytesRead != sizeof(g_PaletteEntry) )
	{
        CloseHandle( hFile );
        return E_FAIL;
	}

	CloseHandle( hFile );

	//  Colors stored in a DIB (bitmap) palette are in BGR order.
	//  Colors used in a Direct Draw PALETTEENTRY structure are
	//  in RGB order, so flip them around. The PALETTEENTRY 
	//  structure also holds alpha information, but it's not used here.

	// Figure out how many colors there are by reading it from the 
	// bitmap header.
	if( bi.biSize != sizeof(BITMAPINFOHEADER) )
        dwColors = 0;
	else if (bi.biBitCount > 8) //if bit count of bitmap is > 8
        dwColors = 0;           //set palette colors used to 0
	else if (bi.biClrUsed == 0)
        dwColors = 1 << bi.biBitCount;
	else
        dwColors = bi.biClrUsed; //header info indicating # colors
                 //from header used in bitmap

	//For each color in the g_PaletteEntry PALETTEENTRY structure, swap the red 
	//and blue values so they are in the correct order.
	for( iColor = 0; iColor < dwColors; iColor++ )
	{
        BYTE r = g_PaletteEntry[iColor].peRed; //read red value
        g_PaletteEntry[iColor].peRed  = g_PaletteEntry[iColor].peBlue; //swap blue into red
        g_PaletteEntry[iColor].peBlue = r; //save blue into red position
	}

	//Create the pallete from the g_PaletteEntry PALETTEENTRY structure.	
	return g_pDirectDraw->CreatePalette(
	DDPCAPS_8BIT, //flag indicated its an 8 bit/256 color palette
	g_PaletteEntry, //address of array of PALETTEENTRY structures
	ppPalette, //pointer to palette. This is used by SetPalette()
	NULL ); //advanced COM stuff

}


//-----------------------------------------------------------------------------
// Name: LockBuffer()
// Desc: Locks the current drawing buffer, the backbuffer, or the front
//	buffer.
//	- Example = WritePixelFast demo
//-----------------------------------------------------------------------------
bool LockBuffer(int buffer)
{
	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);

	//Lock the surface
	if (buffer == 0)
		g_pCurrentSurface->Lock(
			NULL,//lock entire surface
			&ddsd,  
			DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
			NULL);
	else if (buffer == 1)
		g_pFrontBuffer->Lock(
			NULL,//lock entire surface
			&ddsd,  
			DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
			NULL);
	else if (buffer == 2)
		g_pBackBuffer->Lock(
			NULL,//lock entire surface
			&ddsd,  
			DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
			NULL);

	//Record surface pitch and pointer as globals.
	g_Pitch = ddsd.lPitch; //get the pitch
	g_pSurface = ddsd.lpSurface;

	if (g_Pitch != 0) 
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// Name: LockBuffer()
// Desc: Locks a specified image buffer.
//	- Example = WritePixelFast demo
//-----------------------------------------------------------------------------
bool LockBuffer(cImage* pImage)
{
	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);

	//Lock the surface
	HRESULT hr = pImage->pSurface->Lock(
		NULL,//lock entire surface
		&ddsd,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) return false;//couldn't lock surface so exit

	//Record surface pitch and pointer as globals.
	g_Pitch = ddsd.lPitch; //get the pitch
	g_pSurface = ddsd.lpSurface;

	if (g_Pitch != 0) 
		return true;
	else
		return false;	
}


//--------------------------------------------------------------------------
// Name: MaskImage
// Desc: Masks an image  
//- 16 bit bitmaps do not mask properly in 32 bit display modes
//- 24 bit bitmaps do not mask properly in 16 and 32 bit display modes
//- this has something to do with the ColorMatch algorithm
//- Example = DrawImage demo
//-----------------------------------------------------------------------------
int MaskImage (cImage* pImage, int r, int g, int b)
{	
	//Get and translate the image pointer
	if(pImage == NULL) return E_INVALIDARG;
	if(pImage->pSurface == NULL) return E_INVALIDARG;

	COLORREF rgb = RGB(r,g,b);

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ColorMatch(pImage->pSurface, rgb);
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	pImage->isMasked = true;
	pImage->convertedMaskColor = ddck.dwColorSpaceLowValue;
	return pImage->pSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
}


//-----------------------------------------------------------------------------
// Name: MouseDown()
// Desc: Returns whether the selected mouse is down. 
// - Example = Mouse demo
//-----------------------------------------------------------------------------
int MouseDown (char button)
{
	UpdateMouse();
	if (button == 1)
		return g_MouseDown1;
	else
		return g_MouseDown2;
}


//-----------------------------------------------------------------------------
// Name: MoveMouse()
// Desc: Moves the mouse to the selected spot on the screen. 
//-----------------------------------------------------------------------------
void MoveMouse (int x, int y)
{
	g_MouseX = x;
	if (g_MouseX < 0) g_MouseX = 0;
	if (g_MouseX >= g_screenWidth) g_MouseX = g_screenWidth-1;

	g_MouseY = y;
	if (g_MouseY < 0) g_MouseY = 0;
	if (g_MouseY >= g_screenHeight) g_MouseY = g_screenHeight-1;
}


//-----------------------------------------------------------------------------
// Name: MouseHitCheck()
// Desc: Checks to see how many times the mouse has been hit since the
//	last time this command was called. This command uses buffered Direct
//	Input data via GetDeviceData.
//-----------------------------------------------------------------------------
int MouseHitCheck (void)
{
	DIDEVICEOBJECTDATA inputBuffer[16]; //array storing buffer items
	DWORD numberItems = 10; // number of items to be retrieved

	HRESULT hr = g_pMouse->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA),          
		inputBuffer,  
		&numberItems,            
		0);//use DIGDD_PEEK to leave buffered items in buffer

	//Unable to read data or no data available
	if (FAILED(hr))
	{
		return -1;
	}		
	if (numberItems == 0) 
	{
		return 0;
	}

	//Check buffer for left and right button presses
	for (DWORD item=0; item < numberItems; item++)
	{
	if ((inputBuffer[item].dwOfs == DIMOFS_BUTTON0) && 
		(inputBuffer[item].dwData & 0x80))
		g_MouseHit1 = g_MouseHit1+1;
	if ((inputBuffer[item].dwOfs == DIMOFS_BUTTON1) && 
		(inputBuffer[item].dwData & 0x80)) 
		g_MouseHit2 = g_MouseHit2+1;
	}

	return 1;
}


//-----------------------------------------------------------------------------
// Name: MouseHit()
// Desc: Checks to see how many times a specific mouse button has been
//	hit since the last time this command was called.
// - Example = Mouse demo
//-----------------------------------------------------------------------------
int MouseHit (char button)
{
	MouseHitCheck();
	if (button == 1)
	{
		int temp = g_MouseHit1;
		g_MouseHit1 = 0;
		return temp;
	}
	else
	{
		int temp = g_MouseHit2;
		g_MouseHit2 = 0;
		return temp;
	}
}


//-----------------------------------------------------------------------------
// Name: MouseX()
// Desc: Returns the current x location of the mouse in screen coordinates.
//-----------------------------------------------------------------------------
int MouseX(void)
{
	return g_MouseX;
}


//-----------------------------------------------------------------------------
// Name: MouseY()
// Desc: Returns the current y location of the mouse in screen coordinates.
//-----------------------------------------------------------------------------
int MouseY(void)
{
	return g_MouseY;
}

//-----------------------------------------------------------------------------
// Name: Oval()
// Desc: Draws an oval at the specified location  
//	Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Oval (int x, int y, int width, int height, int solid)
{
	//Get hDC of backbuffer
	HDC hDC = NULL;
	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface

	//Set the border pen and fill brush
	if (g_hPen != NULL) SelectObject(hDC, g_hPen);	
	if (solid==0) SelectObject(hDC, GetStockObject(NULL_BRUSH));
	if (solid==1) SelectObject(hDC, g_hBrush);
	if (solid==1) SetBkMode(hDC, TRANSPARENT);

	Ellipse(
		hDC,// handle to DC
		x, // int x-coord of upper-left corner of oval
		y, // int y-coord of upper-left corner of oval
		x+width, // int x-coord of lower-right corner of oval
		y+height); // int y-coord of lower-right corner of oval

	g_pCurrentSurface->ReleaseDC(hDC);
	return 1;
}


//-----------------------------------------------------------------------------
// Name: Plot()
// Desc: Draws a pixel at the specified location. This command is slow
//	in 8 bit mode. Use WritePixel instead.  
//	Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Plot (int x, int y, int r, int g, int b)
{
	//Get hDC of backbuffer
	HDC hDC = NULL;
	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface

	SetPixel(
		hDC,       // handle to DC
		x,             // x-coordinate of pixel
		y,             // y-coordinate of pixel
		RGB(r,g,b));   // pixel color

	g_pCurrentSurface->ReleaseDC(hDC);
	return 1;
}


//-----------------------------------------------------------------------------
// Name: ReadPaletteRGB
// Desc: Reads the RGB value of a given palette entry in a 256 color palette
// - Example = Palettes demo
//-----------------------------------------------------------------------------
int ReadPaletteRGB (byte iColor)
{
	int r = g_PaletteEntry[iColor].peRed;
	int g = g_PaletteEntry[iColor].peGreen;
	int b = g_PaletteEntry[iColor].peBlue;
	return GetRGB(r,g,b);
} 


//-----------------------------------------------------------------------------
// Name: ReadPaletteR
// Desc: Reads the R value of a given palette entry in a 256 color palette
// - Example = Palettes demo
//-----------------------------------------------------------------------------
byte ReadPaletteR (byte iColor)
{
	return g_PaletteEntry[iColor].peRed;
}


//-----------------------------------------------------------------------------
// Name: ReadPaletteG
// Desc: Reads the G value of a given palette entry in a 256 color palette
// - Example = Palettes demo
//-----------------------------------------------------------------------------
byte ReadPaletteG (byte iColor)
{
	return g_PaletteEntry[iColor].peGreen;
}


//-----------------------------------------------------------------------------
// Name: ReadPaletteB
// Desc: Reads the B value of a given palette entry in a 256 color palette
// - Example = Palettes demo
//-----------------------------------------------------------------------------
byte ReadPaletteB (byte iColor)
{
	return g_PaletteEntry[iColor].peBlue;
}


//-----------------------------------------------------------------------------
// Name: ReadPixel
// Desc: Reads a pixel at a given location in the current drawing buffer.
//	Mode: 0 = returns RGB, 1 = returns raw pixel data
//  Example = ReadPixel demo
//-----------------------------------------------------------------------------
int ReadPixel (int x, int y, int mode=0)
{
	int color=0;
	int rgb = 0;

	//Do some error checking on the coordinates vs. current
	//drawing buffer.
	if (g_pCurrentBuffer == 1 || g_pCurrentBuffer == 2)
	{
		if (x >= g_screenWidth) return -1;		
		if (y >= g_screenHeight) return -1;
	}
	else if (g_pCurrentBuffer == 3)
	{
		if (x >= g_pCurrentImage->imageWidth) return -1;
		if (y >= g_pCurrentImage->imageHeight) return -1;
	}
	else
		return -1;

	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);

	//Lock the surface
	HRESULT hr = g_pCurrentSurface->Lock(
		NULL,//lock entire surface
		&ddsd,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) return -2;//couldn't lock surface so exit
	int bytesPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount/8;
	int pitch = ddsd.lPitch; //get the pitch 

	//Read pixel info
	if (g_screenDepth == 8)
	{
		byte* pSurface = (byte*) ddsd.lpSurface; //Grab surface pointer
		color = *(pSurface+x+y*pitch);
	}

	if (g_screenDepth == 16)
	{
		short* pSurface = (short*) ddsd.lpSurface; //Grab surface pointer
		pitch = pitch/2; //adjust pitch to short value
		color = *(pSurface+x+y*pitch);
	}	

	if (g_screenDepth == 24)
	{
		byte* pSurface = (byte*) ddsd.lpSurface; //Grab surface pointer
		color = *((int*) (pSurface+3*x+y*pitch)) & 0x00FFFFFF;
	}

	if (g_screenDepth == 32)
	{
		int* pSurface = (int*) ddsd.lpSurface; //Grab surface pointer
		pitch = pitch/4; //adjust pitch to int value
		color = *(pSurface+x+y*pitch);
	}

	//Unlock the surface
	g_pCurrentSurface->Unlock(NULL);

	//If returning pixel data
	if (mode == 0 && g_screenDepth == 16) //if 16 bit rgb

		//Return rgb equivalent of naked 16 bit data
		if (g_screenFormat16Bit == 555)	
		{
			int r = 8*((color >> 10) % 32); //pos 10-14
			int g = 8*((color >> 5) % 32);//pos 5-9
			int b = 8*(color % 32);//pos 0-4
			return r << 16 | g << 8 | b;		
		}
		else //if 565
		{
			int r = 8*((color >> 11) % 32); //pos 11-15
			int g = 4*((color >> 5) % 64);//pos 5-10
			int b = 8*(color % 32);//pos 0-4
			return r << 16 | g << 8 | b;
		}

	else //otherwise return raw pixel data
		return (int) color;		
}


//-----------------------------------------------------------------------------
// Name: ReadPixelFast
// Desc: Reads a pixel from the specified location in the spedified drawing
//	buffer. This command only works if the surface has been locked first
//	using LockBuffer().
//  - Example = WritePixelFast demo
//-----------------------------------------------------------------------------
inline int ReadPixelFast(int x, int y, int pitch=g_Pitch, 
						 void* vpSurface=g_pSurface)
{
	//Read pixel info
	if (g_screenDepth == 8)
	{
		byte* pSurface = (byte*) vpSurface; //Grab surface pointer
		return (int) *(pSurface+x+y*pitch);
	}

	if (g_screenDepth == 16)
	{
		short* pSurface = (short*) vpSurface; //Grab surface pointer
		pitch = pitch/2; //adjust pitch to short value
		return (int) *(pSurface+x+y*pitch);
	}	

	if (g_screenDepth == 24)
	{
		byte* pSurface = (byte*) vpSurface; //Grab surface pointer
		return *((int*) (pSurface+3*x+y*pitch)) & 0x00FFFFFF;
	}

	if (g_screenDepth == 32)
	{
		int* pSurface = (int*) vpSurface; //Grab surface pointer
		pitch = pitch/4; //adjust pitch to int value
		return *(pSurface+x+y*pitch);
	}
	return 0;
}


//-----------------------------------------------------------------------------
// Name: Rect()
// Desc: Draws a rectangle at the specified location  
//	Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Rect (int x, int y, int width, int height, int solid=1)
{
	//Get hDC of backbuffer
	HDC hDC = NULL;
	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface
 
	//Set the border pen and fill brush
	if (g_hPen != NULL) SelectObject(hDC, g_hPen);	
	if (solid==0) SelectObject(hDC, GetStockObject(NULL_BRUSH));
	if (solid==1) SetBkMode(hDC, TRANSPARENT); //Make it transparent
	if (solid==1) SelectObject(hDC, g_hBrush);

	Rectangle(
		hDC,// handle to DC
		x, // int x-coord of upper-left corner of rectangle
		y, // int y-coord of upper-left corner of rectangle
		x+width, // int x-coord of lower-right corner of rectangle
		y+height); // int y-coord of lower-right corner of rectangle

	g_pCurrentSurface->ReleaseDC(hDC);
	return 1;
}


//-----------------------------------------------------------------------------
// Name: ReleaseObjects
// Desc: Releases Direct Draw objects. Called by EndGraphics and
//	Graphics functions.
//-----------------------------------------------------------------------------
int ReleaseObjects(void)
{
	int imageCount = 0;//variable recording the number of images
		//excluding the front and backbuffers, that were deleted
		//at exit.

	//Delete GDI objects
	if (g_hBrush != NULL) DeleteObject(g_hBrush);
	if (g_hFont != NULL) DeleteObject(g_hFont);
	if (g_hPen != NULL) DeleteObject(g_hPen);

	//Release Direct3D objects (release before Direct Draw objects)
	if (g_pD3D != NULL)
	{
		//Release Direct3D device
		if (g_pD3DDevice != NULL)
		{
		g_pD3DDevice->Release();//this command causes an error
		g_pD3DDevice = NULL;
		}

		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//Release Direct Draw objects
	if (g_pDirectDraw != NULL)
	{

		//Release all image objects and release their video memory
		cImage* pImage = g_pFirstImage;
		while(pImage != NULL)
		{
		imageCount = imageCount + 1;//keep track of images deleted
		cImage* pTemp = pImage->nextImage;
		if (pImage->pSurface != NULL) pImage->pSurface->Release();//release VRAM
		delete pImage; //delete its container object
		pImage = pTemp;
		}

		//Release palette, if any (8 bit mode)
		if (g_pPalette != NULL)
		{
		g_pPalette->Release();
		g_pPalette = NULL;
		}

		//Release backbuffer
		if (g_pBackBuffer != NULL)
		{
		g_pBackBuffer->Release();
		g_pBackBuffer = NULL;
		}

		//Release primary surface
		if (g_pFrontBuffer!= NULL)
		{
		g_pFrontBuffer->Release();
		g_pFrontBuffer= NULL;
		}

		//Release Direct Draw object
		g_pDirectDraw->Release();
		g_pDirectDraw = NULL;
	}

	//Release all Direct Input objects
	if (g_pDirectInput != NULL)
	{

		//Release Direct Input mouse
		if (g_pMouse != NULL)
		{
		g_pMouse->Unacquire();
		g_pMouse->Release();
		g_pMouse = NULL;
		}

		//Release Direct Input object
		g_pDirectInput->Release();
		g_pDirectInput = NULL;
	}

	DestroyWindow(g_hWndGame);
	PostQuitMessage(0);	
	return imageCount;
}


//-----------------------------------------------------------------------------
// Name: ResizeImage
// Desc: Resizes an image to a specified image size in pixels.
//	Does not work on 3D-enabled images. Use Draw3DImage instead.
//-----------------------------------------------------------------------------
int ResizeImage (cImage* pImage, int width, int height)
{
	if(pImage == NULL) return -1;
	if(pImage->pSurface == NULL) return -1;
	if (pImage->imageIs3D == true) return -2;

	//Create a temporary image to copy scaled image to
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid
	if (pImage->memLocation == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (pImage->memLocation == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	int tempImageWidth = ddsd.dwWidth; //record dimensions for later
	int tempImageHeight = ddsd.dwHeight;

	LPDIRECTDRAWSURFACE7 pTempImage; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pTempImage, //address of the variable pointing to the new image
	NULL)  //advanced COM stuff
	!= DD_OK) return -3;

	//Compute source rectangle
	RECT rectSource;
	rectSource.left=0;
	rectSource.right= pImage->imageWidth;
	rectSource.top=0;
	rectSource.bottom= pImage->imageHeight;

	//Compute destination rectangle
	RECT rectDest;
	rectDest.left=0;
	rectDest.right= tempImageWidth;
	rectDest.top=0;
	rectDest.bottom= tempImageHeight;

	//Blit to the destination image
	HRESULT hr = pTempImage->Blt(
	&rectDest, //Destination RECT
	pImage->pSurface,  //lpDDSrcSurface
	&rectSource, //Source RECT or NULL for entire surface
	DDBLT_WAIT, //flags
	NULL ); //special effects

	//Now delete the original image and put the new image
	//in its place
	pImage->pSurface->Release();
	pImage->pSurface = pTempImage;
	pImage->imageWidth = tempImageWidth;
	pImage->imageHeight = tempImageHeight;
	if (g_AutoMidHandle == true)
	{
	pImage->xHandle = pImage->imageWidth/2;
	pImage->yHandle = pImage->imageHeight/2;
	}

	//If appropriate, mask the new image the same as the original
	if (pImage->isMasked == true)
	{
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = pImage->convertedMaskColor;
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	pImage->pSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	return 1;
}


//-----------------------------------------------------------------------------
// Name: RotateImage
// Desc: Rotates a pre-existing image in a counter-clockwise direction by
// 90, 180, or 270 degrees. This doesn't work on 3D-enabled images.
//	Example = Scale & Rotate demo
//-----------------------------------------------------------------------------
int RotateImage (cImage* pImage, int angle)
{
	//Get incoming info
	if(pImage == NULL) return 0;
	if(pImage->pSurface == NULL) return 0;	
	angle = (angle/90) % 4;//convert angle to 0,1,2,3
	char loadTo = pImage->memLocation;
	if (pImage->imageIs3D == true) return 0;

	//Create a temporary image to copy rotated image to
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | //ddsCaps structure is valid (see below
                 DDSD_HEIGHT | //dwHeight member is valid
                 DDSD_WIDTH;   //dwWidth member is valid
	if (loadTo == 0) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	if (loadTo == 1) ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN 
		| DDSCAPS_SYSTEMMEMORY; //load to RAM if loadTo == 1
	if (angle == 0 || angle == 2){
		ddsd.dwWidth = pImage->imageWidth;
		ddsd.dwHeight = pImage->imageHeight;
	}
	else {
		ddsd.dwWidth = pImage->imageHeight;
		ddsd.dwHeight = pImage->imageWidth;
	}
	int tempImageWidth = ddsd.dwWidth; //record dimensions for later
	int tempImageHeight = ddsd.dwHeight;
	LPDIRECTDRAWSURFACE7 pTempImage; //pointer to Direct Draw surface
	if (g_pDirectDraw->CreateSurface(
	&ddsd, //address of above DDSURFACEDESC2 structure that describes surface
	&pTempImage, //address of the variable pointing to the new image
	NULL)  //advanced COM stuff
	!= DD_OK) return -1;

	//Lock surface of temporary image
	HRESULT hr = pTempImage->Lock(
		NULL,//lock entire surface
		&ddsd,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) //couldn't lock image so delete it and exit
	{
		pTempImage->Release();
		pTempImage = NULL;
		return -2;
	}
	int tempPitch = (int) ddsd.lPitch; //get the image pitch 
	byte* pTempSurface = (byte*) ddsd.lpSurface; //grab surface pointer

	//Lock surface of source image
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);
	hr = pImage->pSurface->Lock(
		NULL,//lock entire surface
		&ddsd,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) //couldn't lock surface so exit
	{
		pTempImage->Unlock(NULL);
		pTempImage->Release();
		return -3;
	}
	int bytesPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount/8;
	int pitch = (int) ddsd.lPitch; //get the pitch 
	byte* pSurface = (byte*) ddsd.lpSurface; //Grab surface pointer

	//Write the pixel info
	if (bytesPerPixel == 1) //slightly optimized 8 bit version
	{
	if (angle == 0) //{ //angle = 0 degrees	
		memcpy(pTempSurface,pSurface,pImage->imageHeight*pitch);
	else if (angle == 1) {//angle = 90 degrees counterclockwise	
		pTempSurface = pTempSurface + (pImage->imageHeight-1)*tempPitch;
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		*(pTempSurface + b - a*tempPitch) = 
			*(pSurface+a+b*pitch);
		}}}
	else if (angle == 2) {//angle = 180 degrees		
		pTempSurface = pTempSurface+pImage->imageWidth-1
		+ (pImage->imageHeight-1)*tempPitch;
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		*(pTempSurface - a - b*tempPitch) = 
			*(pSurface+a+b*pitch);
		}}}
	else if (angle == 3) {//angle = 270 degrees counterclockwise			
		pTempSurface = pTempSurface+pImage->imageWidth-1;
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		*(pTempSurface - b + a*tempPitch) = 
			*(pSurface+a+b*pitch);
		}}}
	}//if (bytesPerPixel == 1)

	if (bytesPerPixel != 1)
	{
	if (angle == 0) { //angle = 0 degrees
		for (int b=0;b<pImage->imageHeight;b++)
		{	
		memcpy(pTempSurface+b*tempPitch,pSurface+b*pitch,pitch);
		}}		
	else if (angle == 1) {//angle = 90 degrees counterclockwise		
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		for (int c=0;c<bytesPerPixel;c++){
		*(pTempSurface + b*bytesPerPixel + 
			(pImage->imageWidth-1-a)*tempPitch + c) = 
			*(pSurface + a*bytesPerPixel + b*pitch + c);
		}}}}
	else if (angle == 2) {//angle = 180 degrees		
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		for (int c=0;c<bytesPerPixel;c++){
		*(pTempSurface + (pImage->imageWidth-1-a)*bytesPerPixel
			+ (pImage->imageHeight-1-b)*tempPitch + c) = 
			*(pSurface + a*bytesPerPixel + b*pitch + c);
		}}}}
	else if (angle == 3) {//angle = 270 degrees counterclockwise
		for (int a=0;a<pImage->imageWidth;a++){
		for (int b=0;b<pImage->imageHeight;b++){
		for (int c=0;c<bytesPerPixel;c++){
		*(pTempSurface + (pImage->imageHeight-1-b)*bytesPerPixel 
			+ a*tempPitch + c) = 
			*(pSurface + a*bytesPerPixel + b*pitch + c);
		}}}}	
	}//if (bytesPerPixel == 2)

	//Unlock the surfaces
	pImage->pSurface->Unlock(NULL);
	pTempImage->Unlock(NULL);

	//Now delete the original image and put the new image
	//in its place
	pImage->pSurface->Release();
	pImage->pSurface = pTempImage;
	pImage->imageWidth = tempImageWidth;
	pImage->imageHeight = tempImageHeight;
	if (g_AutoMidHandle == true)
	{
	pImage->xHandle = pImage->imageWidth/2;
	pImage->yHandle = pImage->imageHeight/2;
	}

	//If appropriate, mask the new image the same as the original
	if (pImage->isMasked == true)
	{
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = pImage->convertedMaskColor;
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	pImage->pSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	return angle;//hr;
}


//-----------------------------------------------------------------------------
// Name: SaveBuffer
// Desc: Saves the current image (or buffer) as a bmp file.
//	Based on code by Michael Fötsch, which can be found at 
//	http://www.geocities.com/foetsch/screenshot/screenshot.htm
//	- Example = DrawImage demo.
//-----------------------------------------------------------------------------
int SaveBuffer (char* FileName)
{
	LPDIRECTDRAWSURFACE7 lpDDS = g_pCurrentSurface;

	if (!FileName || !lpDDS) return false;
	int ErrorCode = 1;

	HDC SurfDC=NULL;        // GDI-compatible device context for the surface
	HBITMAP OffscrBmp=NULL; // bitmap that is converted to a DIB
	HBITMAP OldBmp=NULL;
	int Width = 0; //image width
	int Height = 0; //image height
	HDC OffscrDC=NULL;      // offscreen DC that we can select OffscrBmp into
	LPBITMAPINFO lpbi=NULL; // bitmap format info; used by GetDIBits
	LPVOID lpvBits=NULL;    // pointer to bitmap bits array
	HANDLE BmpFile=INVALID_HANDLE_VALUE;    // destination .bmp file
	BITMAPFILEHEADER bmfh;  // .bmp file header

	// Get dimensions of Surface:
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (FAILED(lpDDS->GetSurfaceDesc(&ddsd)))
		{ErrorCode = -1; goto cleanUp;}
	Width = ddsd.dwWidth;
	Height = ddsd.dwHeight;

	// Create a GDI-compatible device context for the surface:
	if (FAILED(lpDDS->GetDC(&SurfDC))) 
		{ErrorCode = -2; goto cleanUp;}

	// We need an HBITMAP to convert it to a DIB:
	if ((OffscrBmp = CreateCompatibleBitmap(SurfDC, Width, Height)) == NULL)
        {ErrorCode = -3; goto cleanUp;}

	// The bitmap is empty, so let's copy the contents of the surface to it.
	// For that we need to select it into a device context. We create one.
	if ((OffscrDC = CreateCompatibleDC(SurfDC)) == NULL) 
		{ErrorCode = -4; goto cleanUp;}
	// Select OffscrBmp into OffscrDC:
	OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
	// Now we can copy the contents of the surface to the offscreen bitmap:
	BitBlt(OffscrDC, 0, 0, Width, Height, SurfDC, 0, 0, SRCCOPY);

	// We don't need SurfDC anymore. Free it:
	lpDDS->ReleaseDC(SurfDC); SurfDC = NULL;

	// GetDIBits requires format info about the bitmap. We can have GetDIBits
	// fill a structure with that info if we pass a NULL pointer for lpvBits:
	// Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible
	// palette):
	if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) +
        256 * sizeof(RGBQUAD)])) == NULL) 
		{ErrorCode = -5; goto cleanUp;}
	ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Get info but first de-select OffscrBmp because GetDIBits requires it:
	SelectObject(OffscrDC, OldBmp);
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, Height, NULL, lpbi, DIB_RGB_COLORS))
        {ErrorCode = -6; goto cleanUp;}

	// Reserve memory for bitmap bits:
	if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
        {ErrorCode = -7; goto cleanUp;}

	// Have GetDIBits convert OffscrBmp to a DIB (device-independent bitmap):
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, Height, lpvBits, lpbi,
        DIB_RGB_COLORS)) {ErrorCode = -8; goto cleanUp;}

	// Create a file to save the DIB to:
	if ((BmpFile = CreateFile(FileName,
                              GENERIC_WRITE,
                              0, NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL)) == INVALID_HANDLE_VALUE)
			{ErrorCode = -9; goto cleanUp;}

	DWORD Written;    // number of bytes written by WriteFile
    
	// Write a file header to the file:
	bmfh.bfType = 19778;        // 'BM'
	// bmfh.bfSize = ???        // we'll write that later
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	// bmfh.bfOffBits = ???     // we'll write that later
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        {ErrorCode = -10; goto cleanUp;}
	if (Written < sizeof(bmfh)) {ErrorCode = -11; goto cleanUp;}

	// Write BITMAPINFOHEADER to the file:
	if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER),
        &Written, NULL)) {ErrorCode = -12; goto cleanUp;}
	if (Written < sizeof(BITMAPINFOHEADER)) 
		{ErrorCode = -13; goto cleanUp;}

	// Calculate size of palette:
	int PalEntries;
	// 16-bit or 32-bit bitmaps require bit masks:
	if (lpbi->bmiHeader.biCompression == BI_BITFIELDS) PalEntries = 3;
	else
        // bitmap is palettized?
        PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
		// 2^biBitCount palette entries max.:
            (int)(1 << lpbi->bmiHeader.biBitCount)
		// bitmap is TrueColor -> no palette:
        : 0;
	// If biClrUsed use only biClrUsed palette entries:
	if (lpbi->bmiHeader.biClrUsed) PalEntries = lpbi->bmiHeader.biClrUsed;

	// Write palette to the file:
	if (PalEntries)
	{
        if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD),
            &Written, NULL)) {ErrorCode = -14; goto cleanUp;}
        if (Written < PalEntries * sizeof(RGBQUAD)) 
			{ErrorCode = -15; goto cleanUp;}
	}

	// The current position in the file (at the beginning of the bitmap bits)
	// will be saved to the BITMAPFILEHEADER:
	bmfh.bfOffBits = SetFilePointer(BmpFile, 0, 0, FILE_CURRENT);

	// Write bitmap bits to the file:
	if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage,
        &Written, NULL)) {ErrorCode = -16; goto cleanUp;}
	if (Written < lpbi->bmiHeader.biSizeImage) 
		{ErrorCode = -17; goto cleanUp;}

	// The current pos. in the file is the final file size and will be saved:
	bmfh.bfSize = SetFilePointer(BmpFile, 0, 0, FILE_CURRENT);

	// We have all the info for the file header. Save the updated version:
	SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
        {ErrorCode = -18; goto cleanUp;}
	if (Written < sizeof(bmfh)) {ErrorCode = -19; goto cleanUp;}

  cleanUp:

	if (SurfDC) lpDDS->ReleaseDC(SurfDC);
	if (OffscrDC) DeleteDC(OffscrDC);
	if (OffscrBmp) DeleteObject(OffscrBmp);
	if (lpbi) delete[] lpbi;
	if (lpvBits) delete[] lpvBits;
	if (BmpFile != INVALID_HANDLE_VALUE) CloseHandle(BmpFile);
    
	return ErrorCode;//returns 1 if everything's ok.
}


//-----------------------------------------------------------------------------
// Name: ScaleImage
// Desc: Scales an image to a new size using fractions.
//	i.e., 0.5 = 50%, 1 = 100%, 2 = 200%, etc.
//	Does not work on 3D-enabled images. Use Draw3DImage instead.
//	Example = Scale & Rotate demo
//-----------------------------------------------------------------------------
int ScaleImage (cImage* pImage, float xScale, float yScale)
{
	return ResizeImage (pImage, pImage->imageWidth*xScale, 
		pImage->imageHeight*yScale);
}


//-----------------------------------------------------------------------------
// Name: SetBrush()
// Desc: Sets the fill color/pattern used by GDI drawing operations 
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int SetBrush (int r, int g, int b, int pattern, int style)
{
	int usedStyle = 0;

	if (g_hBrush != NULL) DeleteObject(g_hBrush);
	g_hBrush = NULL;
	if (pattern == 0) 
		g_hBrush = CreateSolidBrush(RGB(r,g,b));
	if (pattern == 1) 
	{
		if (style == 0) usedStyle = HS_BDIAGONAL;
		if (style == 1) usedStyle = HS_CROSS;
		if (style == 2) usedStyle = HS_DIAGCROSS;
		if (style == 3) usedStyle = HS_FDIAGONAL;
		if (style == 4) usedStyle = HS_HORIZONTAL;
		if (style == 5) usedStyle = HS_VERTICAL;
		g_hBrush = CreateHatchBrush(usedStyle,RGB(r,g,b));
	}
	return 1;
}


//-----------------------------------------------------------------------------
// Name: SetBuffer
// Desc: Changes the current drawing buffer to the front buffer, back
// buffer, or an image buffer.
//-----------------------------------------------------------------------------
void SetBuffer (int pointer, cImage* pImage = NULL)
{
	if (pointer == 1)//if setting buffer to front buffer
	{
		g_pCurrentBuffer = 1;
		g_pCurrentImage = NULL;//pointer to the cImage in buffer
		g_pCurrentSurface = g_pFrontBuffer;//pointer to the surface in buffer
		return;
	}

	if (pointer == 2) //if setting buffer to back buffer
	{
		g_pCurrentBuffer = 2;
		g_pCurrentImage = NULL;//pointer to the cImage in buffer
		g_pCurrentSurface = g_pBackBuffer;//pointer to the surface in buffer
		return;
	}

	//If setting buffer to image buffer
	if(pImage == NULL) return;//error check
	if(pImage->pSurface == NULL) return;//error check

	g_pCurrentBuffer = 3;
	g_pCurrentImage = pImage;//pointer to the cImage in buffer
	g_pCurrentSurface = pImage->pSurface;//pointer to the surface in buffer
}


//-----------------------------------------------------------------------------
// Name: SetFont()
// Desc: Designates a font typeface for use by text functions 
// Some typical typefaces are Arial, Courier New, Garamond, Times New Roman 
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int SetFont (char* typeface, int typesize, int bold=0, int italic=0, 
			 int underline=0)
{
	int weight = 0;
	if (bold==0) weight = 200;
	if (bold==1) weight = 700;

	if (g_hFont != NULL) DeleteObject(g_hFont);
	g_hFont = NULL;
	g_hFont = CreateFont(
		typesize,//height of font
		0,//int average character width
		0,//int angle of escapement
		0,//int base-line orientation angle
		weight,//font weight 0-1000 
		italic,//DWORD italic attribute option
		underline,//DWORD underline attribute option
		0,//DWORD strikeout attribute option
		0,//DEFAULT_CHARSET,//DWORD character set identifier
		0,//OUT_DEFAULT_PRECIS,//DWORD output precision
		0,//CLIP_DEFAULT_PRECIS,//DWORD clipping precision
		0,//DWORD output quality
		0,//DWORD pitch and family
		typeface);//LPCTSTR typeface name

	HDC hdc = GetDC (g_hWndGame);
	SelectObject(hdc,g_hFont);
	ReleaseDC (g_hWndGame, hdc);

	return 1;
}


//-----------------------------------------------------------------------------
// Name: SetFontColor()
// Desc: Sets the default text color  
//-----------------------------------------------------------------------------
int SetFontColor (int r, int g, int b)
{
	g_FontColor = RGB(r,g,b); 
	return(1);
}


//-----------------------------------------------------------------------------
// Name: SetMouseSpeed()
// Desc: Set the current mouse speed. Starting value is 2. 
//-----------------------------------------------------------------------------
void SetMouseSpeed (int speed)
{
	g_MouseSpeed = speed;
}


//-----------------------------------------------------------------------------
// Name: SetPalette
// Desc: Sets the palette after it has been changed using WritePalette
//	The default settings set the entire palette. You can also just change
//	a single entry in the palette or a range of entries, and only those
//	entries in the g_PaletteEntry array will be changed.
//  - Example = Palettes demo
//-----------------------------------------------------------------------------
int SetPalette (byte startingEntry=0, byte entryCount=255)
{
	HRESULT hr=0;
	if (g_pPalette != NULL) 
	{
		hr = g_pPalette->
		SetEntries(0,startingEntry,entryCount,&g_PaletteEntry[startingEntry]);
	}

	if (hr == DD_OK) 
		return 1;
	else
		return -1;	
}


//-----------------------------------------------------------------------------
// Name: SetPen()
// Desc: Sets the current pen used by GDI drawing operations 
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int SetPen (int r, int g, int b, int thickness, int style)
{
	int usedStyle = 0;
	if (style == 0) usedStyle = PS_SOLID;
	if (style == 1) usedStyle = PS_DASH;//thickness = 1 pixel 
	if (style == 2) usedStyle = PS_DOT;//thickness = 1 pixel 
	if (style == 3) usedStyle = PS_DASHDOT;//thickness = 1 pixel 
	if (style == 4) usedStyle = PS_DASHDOTDOT;//thickness = 1 pixel 
	if (style == 5) usedStyle = PS_NULL;

	if (g_hPen != NULL) DeleteObject(g_hPen);
	g_hPen = NULL;
	g_hPen = CreatePen(
		usedStyle, //int pen style
		thickness, //int pen width; 0 = 1 pixel
		RGB(r,g,b)); //COLORREF pen color 
	return 1;
}


//-----------------------------------------------------------------------------
// Name: ShowPointer
// Desc: Shows the Windows cursor (default = off)
//-----------------------------------------------------------------------------
void ShowPointer (void)
{
	ShowCursor(TRUE);
}


//-----------------------------------------------------------------------------
// Name: StringHeight
// Desc: Returns the height, in pixels, of a string in the currently
//	selected font.
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int StringHeight (char* textString)
{
	SIZE size;
	HDC hdc = GetDC (g_hWndGame);
	if (g_hFont) SelectObject(hdc,g_hFont);//Use the selected font, if any
	GetTextExtentPoint32(hdc, textString, 
		strlen (textString), &size);
	ReleaseDC (g_hWndGame, hdc);
	return size.cy;
}


//-----------------------------------------------------------------------------
// Name: StringWidth
// Desc: Returns the width, in pixels, of a string in the currently
//	selected font.
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int StringWidth (char* textString)
{
	SIZE size;
	HDC hdc = GetDC (g_hWndGame);
	if (g_hFont) SelectObject(hdc,g_hFont);//Use the selected font, if any
	GetTextExtentPoint32(hdc, textString, 
		strlen (textString), &size);
	ReleaseDC (g_hWndGame, hdc);
	return size.cx;
}


//-----------------------------------------------------------------------------
// Name: Text()
// Desc: Draws a text string on the current DirectDraw surface using 
//	g_hFont or the default GDI font if g_hFont is NULL.
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Text (int x, int y, char* pText)
{	
	HDC     hDC = NULL;

	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface
	SetTextColor( hDC, g_FontColor);// Set text color
	SetBkMode(hDC, TRANSPARENT); //Make it transparent
	if (g_hFont) SelectObject(hDC,g_hFont);//Use the selected font, if any
	TextOut( hDC, x, y, pText, strlen(pText)); 
	g_pCurrentSurface->ReleaseDC( hDC );
	return 1;
}


//-----------------------------------------------------------------------------
// Name: Text()
// Desc: Overloaded integer version
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Text (int x, int y, DWORD number)
{	
 	HDC     hDC = NULL;
	char	pText [33];
 	itoa(number,pText,10);
	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface
	SetTextColor( hDC, g_FontColor);// Set text color
	SetBkMode(hDC, TRANSPARENT); //Make it transparent
	if (g_hFont) SelectObject(hDC,g_hFont);//Use the selected font, if any
	TextOut( hDC, x, y, pText, strlen(pText)); 
	g_pCurrentSurface->ReleaseDC( hDC );
	return 1;
}


//-----------------------------------------------------------------------------
// Name: Text()
// Desc: Overloaded CSTR version
//	- Example = GUI Commands demo
//-----------------------------------------------------------------------------
int Text (int x, int y, CSTR cstr)
{	
	HDC     hDC = NULL;

	char* pText = cstr.actualString;

	g_pCurrentSurface->Restore(); //Make sure the surface is restored	
	g_pCurrentSurface->GetDC(&hDC);//Get the DC of the surface
	SetTextColor( hDC, g_FontColor);// Set text color
	SetBkMode(hDC, TRANSPARENT); //Make it transparent
	if (g_hFont) SelectObject(hDC,g_hFont);//Use the selected font, if any
	TextOut( hDC, x, y, pText, strlen(pText)); 
	g_pCurrentSurface->ReleaseDC( hDC );
	return 1;
}


//-----------------------------------------------------------------------------
// Name: TextureSearchCallback()
// Desc: Enumeration callback routine to find a 16-bit texture format.
//	Needs to be upgraded.
//-----------------------------------------------------------------------------
static HRESULT CALLBACK TextureSearchCallback(DDPIXELFORMAT* pddpf, VOID* param)
{
    // Note: Return with DDENUMRET_OK to continue enumerating more formats.

    // Skip any funky modes
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;
    
    // Skip any FourCC formats
    if( pddpf->dwFourCC != 0 )
        return DDENUMRET_OK;

    // Skip alpha modes
    if( pddpf->dwFlags&DDPF_ALPHAPIXELS )
        return DDENUMRET_OK;

    // We only want screen-depth's format, so skip all others
    if( pddpf->dwRGBBitCount != (DWORD) g_screenDepth)
        return DDENUMRET_OK;

    // We found a good match. Copy the current pixel format to our output
    // parameter
    memcpy( (DDPIXELFORMAT*)param, pddpf, sizeof(DDPIXELFORMAT) );

    // Return with DDENUMRET_CANCEL to end enumeration.
    return DDENUMRET_CANCEL;
}



//-----------------------------------------------------------------------------
// Name: TotalSysMem
// Desc: Returns the total physical system memory in bytes.
//  - Example = Draw3DImage demo
//-----------------------------------------------------------------------------
int TotalSysMem (void)
{ 	
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	return(stat.dwTotalPhys);
}


//-----------------------------------------------------------------------------
// Name: TotalVidMem()
// Desc: Returns the available video memory in bytes
//  - Example = FreeImage demo
//-----------------------------------------------------------------------------
int TotalVidMem (void) 
{
	DWORD totalVRAM=0;
	DWORD freeVRAM=0;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));//clear memory block for structure
	ddscaps.dwCaps = DDSCAPS_VIDEOMEMORY;  
	g_pDirectDraw->GetAvailableVidMem(&ddscaps,&totalVRAM,&freeVRAM);
	return totalVRAM;
}  


//-----------------------------------------------------------------------------
// Name: UnlockBuffer() -- overloaded version
// Desc: Unlocks the current drawing buffer, the backbuffer, or the front
//	buffer.
//	Example = WritePixelFast
//-----------------------------------------------------------------------------
inline void UnlockBuffer(int buffer=0)
{
	if (buffer == 0)
		g_pCurrentSurface->Unlock(NULL);
	else if (buffer == 1)
		g_pFrontBuffer->Unlock(NULL);
	else if (buffer == 2)
		g_pBackBuffer->Unlock(NULL);
}


//-----------------------------------------------------------------------------
// Name: UnlockBuffer()
// Desc: Unlocks a specified image buffer.
//	Example = WritePixelFast
//-----------------------------------------------------------------------------
inline void UnlockBuffer(cImage* pImage)
{
	pImage->pSurface->Unlock(NULL);
}


//-----------------------------------------------------------------------------
// Name: UpdateMouse
// Desc: Updates the mouse by using Direct Input. This function can be 
//	called directly. It is also called automatically by the Flip() function.
//-----------------------------------------------------------------------------
void UpdateMouse (void)
{
	g_pMouse->GetDeviceState(sizeof(g_pMouseData), &g_pMouseData);

	//Get and calculate current mouse position info.
	if (abs(g_pMouseData.lX) <= 10) 
		g_MouseX = g_MouseX+g_pMouseData.lX;
	else
		g_MouseX = g_MouseX+g_MouseSpeed*g_pMouseData.lX;
	if (g_MouseX < 0) g_MouseX = 0;
	if (g_MouseX >= g_screenWidth) g_MouseX = g_screenWidth-1;

	if (abs(g_pMouseData.lY) <= 10) 
		g_MouseY = g_MouseY+g_pMouseData.lY;
	else 
		g_MouseY = g_MouseY+g_MouseSpeed*g_pMouseData.lY;
	if (g_MouseY < 0) g_MouseY = 0;
	if (g_MouseY >= g_screenHeight) g_MouseY = g_screenHeight-1;

	//Return left button status
	if (g_pMouseData.rgbButtons[0] & 0x80)
		g_MouseDown1 = true;
	else
		g_MouseDown1 = false;

	//Return right button status
	if (g_pMouseData.rgbButtons[1] & 0x80)
		g_MouseDown2 = true;
	else
		g_MouseDown2 = false;
}


//-----------------------------------------------------------------------------
// Name: WritePalette
// Desc: Writes an RGB value to a given palette entry in a 256 color palette.
//	This command will not actually change the palette itself. To change the
//	palette, you need to use SetPalette after you have changed the palette
//	entries to whatever you want. 
//	- Example = Palettes demo
//-----------------------------------------------------------------------------
void WritePalette (int iColor, int r, int g, int b)
{
	g_PaletteEntry[iColor].peRed = r;
	g_PaletteEntry[iColor].peGreen = g;
	g_PaletteEntry[iColor].peBlue = b;
}


//-----------------------------------------------------------------------------
// Name: WritePixel
// Desc: Writes a pixel to the specified location in the current drawing
// buffer. If in 16 bit display mode or greater, the specified rgb color
// is written. If in 8 bit mode, the first parameter is used and treated
// as a palette entry (0-255).
//-----------------------------------------------------------------------------
int WritePixel (int x, int y, int r, int g, int b)
{
	//Do some error checking on the coordinates vs. current
	//drawing buffer.
	if (g_pCurrentBuffer == 1 || g_pCurrentBuffer == 2)
	{
		if (x >= g_screenWidth) return -1;		
		if (y >= g_screenHeight) return -1;
	}
	else if (g_pCurrentBuffer == 3)
	{
		if (x >= g_pCurrentImage->imageWidth) return -1;
		if (y >= g_pCurrentImage->imageHeight) return -1;
	}
	else
		return -1;

	//Set up the Direct Draw surface description
	DDSURFACEDESC2 ddsd; //surface description data structure
	ZeroMemory(&ddsd, sizeof(ddsd));// zero-out the memory area
	ddsd.dwSize = sizeof(ddsd);

	//Lock the surface
	HRESULT hr = g_pCurrentSurface->Lock(
		NULL,//lock entire surface
		&ddsd,  
		DDLOCK_WAIT | DDLOCK_NOSYSLOCK,                    
		NULL);
	if (hr != DD_OK) return -3;//couldn't lock surface so exit

	int pitch = ddsd.lPitch; //get the pitch 

	//Write the pixel
	if (g_screenDepth == 8)
	{
		byte* pSurface = (byte*) ddsd.lpSurface;
		*(pSurface+x+y*pitch) = (byte) r;
	}
	else if (g_screenDepth == 16)
	{
		short pixel=0;
		if (g_screenFormat16Bit == 555)
			pixel = ((r/8)<<10) + ((g/8)<<5) + (b/8);  
		else if (g_screenFormat16Bit == 565)
			pixel = ((r/8)<<11) + ((g/4)<<5) + (b/8);
		else 
			pixel = 0; //error
		short* pSurface = (short*) ddsd.lpSurface;
		*(pSurface+x+y*pitch/2) = pixel;
	}
	else if (g_screenDepth == 24)
	{
		byte* pSurface = (byte*) ddsd.lpSurface;
		*(pSurface+x*3+y*pitch+2) = (byte) r;
		*(pSurface+x*3+y*pitch+1) = (byte) g;
		*(pSurface+x*3+y*pitch+0) = (byte) b;
	}
	else if (g_screenDepth == 32)
	{
		byte* pSurface = (byte*) ddsd.lpSurface;
		*(pSurface+x*4+y*pitch+3) = 255;//unused alpha
		*(pSurface+x*4+y*pitch+2) = (byte) r;
		*(pSurface+x*4+y*pitch+1) = (byte) g;
		*(pSurface+x*4+y*pitch+0) = (byte) b;
	}
	else 
		b = 0; //dummy statement
	
	//Unlock the surface
	g_pCurrentSurface->Unlock(NULL);
	return 0;
}



//-----------------------------------------------------------------------------
// Name: WritePixelFast
// Desc: Writes a pixel to the specified location in the specified drawing
//	buffer. This command only works if the surface has been locked first
//	using LockBuffer(), and then followed with UnlockBuffer(). If in 
//	8-bit paletted mode, only the first value in the rgb triplet is
//	used, and it is treated as a palette entry.
//  - Example = WritePixelFast demo
//-----------------------------------------------------------------------------
inline void WritePixelFast(int x, int y, int r, int g=0, int b=0, 
			int pitch=g_Pitch, void* vpSurface=g_pSurface)
{
	if (g_screenDepth == 8)
	{
		byte* pSurface = (byte*) vpSurface;
		*(pSurface+x+y*pitch) = (byte) r;
	}
	else if (g_screenDepth == 16)
	{
		short pixel=0;
		if (g_screenFormat16Bit == 555)
			pixel = ((r/8)<<10) + ((g/8)<<5) + (b/8);  
		else if (g_screenFormat16Bit == 565)
			pixel = ((r/8)<<11) + ((g/4)<<5) + (b/8);
		else 
			pixel = 0; //error
		short* pSurface = (short*) vpSurface;
		*(pSurface+x+y*pitch/2) = pixel;
	}
	else if (g_screenDepth == 24)
	{
		byte* pSurface = (byte*) vpSurface;
		*(pSurface+x*3+y*pitch+2) = (byte) r;
		*(pSurface+x*3+y*pitch+1) = (byte) g;
		*(pSurface+x*3+y*pitch+0) = (byte) b;
	}
	else if (g_screenDepth == 32)
	{
		byte* pSurface = (byte*) vpSurface;
		*(pSurface+x*4+y*pitch+3) = 255;//unused alpha
		*(pSurface+x*4+y*pitch+2) = (byte) r;
		*(pSurface+x*4+y*pitch+1) = (byte) g;
		*(pSurface+x*4+y*pitch+0) = (byte) b;
	}
}	


//-----------------------------------------------------------------------------
// Name: WritePixelFast
// Desc: This version of WritePixelFast uses naked pixel data and is
//	usually combined with ReadPixelFast. This command only works if the 
//	surface has been locked first using LockBuffer(), and then followed
//	with UnlockBuffer(). If in 8-bit paletted mode, only the first
//	value in the rgb triplet is used, and it is treated as a palette entry.
//  - Example = WritePixelFast demo
//-----------------------------------------------------------------------------
inline void WritePixelFast(int x, int y, int inPixel, 
			int pitch=g_Pitch, void* vpSurface=g_pSurface)
{
	//Write the pixel
	if (g_screenDepth == 8)
	{
		byte* pSurface = (byte*) vpSurface;
		*(pSurface+x+y*pitch) = (byte) inPixel;
	}
	else if (g_screenDepth == 16)
	{
		short* pSurface = (short*) vpSurface;
		*(pSurface+x+y*pitch/2) = (short) inPixel;
	}
	else if (g_screenDepth == 24)
	{
		byte* pSurface = (byte*) vpSurface;
		*(pSurface+x*3+y*pitch+2) = (byte) RGB_GETRED(inPixel);
		*(pSurface+x*3+y*pitch+1) = (byte) RGB_GETGREEN(inPixel);
		*(pSurface+x*3+y*pitch+0) = (byte) RGB_GETBLUE(inPixel);
	}
	else if (g_screenDepth == 32)
	{
		int* pSurface = (int*) vpSurface;
		*(pSurface+x+y*pitch/4) = inPixel;
	}
}	




