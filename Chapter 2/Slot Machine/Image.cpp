#include "framework.h"
#include "Image.h"

Image::Image()
{

}

Image::~Image()
{
	SafeRelease(m_pD2DBitmap);
}

HRESULT Image::OpenImageFile(HWND hWnd, LPCWSTR lpszFilePath, ID2D1HwndRenderTarget* m_pRT)
{
	IWICImagingFactory* m_pIWICFactory{};
	IWICFormatConverter* m_pConvertedSourceBitmap = NULL;

	HRESULT hr = S_OK;
	// Create WIC factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	// Step 1: Create the open dialog box and locate the image file
	if (PathFileExists(lpszFilePath))
	{
		// Step 2: Decode the source image

		// Create a decoder
		IWICBitmapDecoder* pDecoder = NULL;

		hr = m_pIWICFactory->CreateDecoderFromFilename(
			lpszFilePath,                      // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&pDecoder                        // Pointer to the decoder
		);

		// Retrieve the first frame of the image from the decoder
		IWICBitmapFrameDecode* pFrame = NULL;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		//Step 3: Format convert the frame to 32bppPBGRA
		if (SUCCEEDED(hr))
		{
			SafeRelease(m_pConvertedSourceBitmap);
			hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,                          // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				NULL,                            // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			);
		}

		//Step 4: Create render target and D2D bitmap from IWICBitmapSource
		/*
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources(hWnd, mp_RT, m_pD2DBitmap);
		}
		*/

		if (SUCCEEDED(hr))
		{
			// Need to release the previous D2DBitmap if there is one
			SafeRelease(m_pD2DBitmap);
			hr = m_pRT->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
		}

		SafeRelease(pDecoder);
		SafeRelease(pFrame);

		InvalidateRect(hWnd, 0, TRUE);
	}

	return hr;
}

void Image::OnPaint(HWND hWnd, int xPos, int yPos, ID2D1HwndRenderTarget* m_pRT)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;

	if (BeginPaint(hWnd, &ps))
	{
		// Create render target if not yet created
		//hr = CreateDeviceResources(hWnd);

		if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRT->BeginDraw();

			m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

			// Clear the background
			m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

			// D2DBitmap may have been released due to device loss. 
			// If so, re-create it from the source bitmap
			//if (m_pConvertedSourceBitmap && !m_pD2DBitmap)
			//{
			//	m_pRT->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
			//}
			if (m_pD2DBitmap)
			{
				D2D1_SIZE_F rtBitmapSize = m_pD2DBitmap->GetSize();
				m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, rtBitmapSize.width + xPos, rtBitmapSize.height + yPos));

			}

			hr = m_pRT->EndDraw();

			// In case of device loss, discard D2D render target and D2DBitmap
			// They will be re-create in the next rendering pass
			if (hr == D2DERR_RECREATE_TARGET)
			{
				SafeRelease(m_pD2DBitmap);
				// Force a re-render
				hr = InvalidateRect(hWnd, NULL, TRUE) ? S_OK : E_FAIL;
			}
		}

		EndPaint(hWnd, &ps);
	}
	return;
}

void Image::Draw(HWND hwnd, int xPos, int yPos, int xSize, int ySize, ID2D1HwndRenderTarget* m_pRT)
{
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	if (m_pD2DBitmap)
	{
		m_pRT->DrawBitmap(m_pD2DBitmap, D2D1::RectF(xPos, yPos, xPos + xSize, yPos + ySize));
	}

	return;
}