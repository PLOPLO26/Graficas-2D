// Graphics2023I.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "Graphics2023I.h"
#include "CDXManager.h"
#include <math.h>
#include "Matrix4D.h"
#include "CCanvasFont.h"
#include "CSprite.h"
#include "Windowsx.h"
#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                            // instancia actual
WCHAR szTitle[MAX_LOADSTRING];              // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];        // nombre de clase de la ventana principal
CDXManager g_DXManager;
int g_mx;
int g_my;
CCanvas* g_pImage;
CCanvas* g_pImage2;
CCanvas* g_pTexture00;
//DX resource manager
CCanvasFont* g_pFont;
CSprite g_Asteroid;
int             g_nInputType;
CCanvas::VERTEX g_LineStrip[100];
int             g_LineStripCount;
bool            g_bLBPressed;
CCanvas::VERTEX g_TriangleList[300];
int             g_TriangleListCount;
CCanvas::VERTEX g_TriangleStripV[100];
long            g_TriangleStripI[100];
int             g_TriangleStripCount;
MATRIX4D        g_Offset;
MATRIX4D        g_Rotate;
int             g_PosX;
int             g_PosY;
int             g_RotZ;
bool            g_bUp;
bool            g_bLeft;
bool            g_bRight;
bool            g_bDown;
bool            g_bRotLeft;
bool            g_bRotRight;


struct STATS
{
    unsigned long m_nFrameCount; //Frame count
    unsigned long m_nLastFrameCount; //Last Frame Count snapshot
    unsigned long m_nFPS;        //Frames per second
    unsigned long m_sx;
    unsigned long m_sy;
};
struct STATS g_Stats;

CCanvas::PIXEL BmpAlpha(CCanvas::PIXEL Color)
{
    Color.a = 64;
    return Color;
}

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                Render(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    VECTOR4D A = { 1,2,3,1 }, B = { 4,5,6,1 }, C;
    C = A + B;
    C = B - A;
    C = A * B;

    // TODO: Colocar código aquí.

    g_Offset = Identity();
    g_Rotate = Identity();

    CCanvas* pCanvas = CCanvas::Create(640, 480);
    pCanvas->Clear({ 0,0,0,0 });
    pCanvas->Circle(320, 240, 200, { 255,255,255,0 });
    pCanvas->CreateFileFromCanvas("..\\data\\test.bmp");
    CCanvas::Destroy(pCanvas);
    //Carga de una 

   /* g_pImage = CCanvas::CreateCanvasFromFile("..\\data\\hola.bmp");
    if (!g_pImage)
        MessageBoxA(NULL, "No se pudo recargar el recurso ", "Error", MB_ICONERROR);
    g_pImage2 = CCanvas::CreateCanvasFromFile("..\\data\\imagen8bpp.bmp");
    if(!g_pImage2)
       MessageBoxA(NULL, "No se pudo recargar el recurso ", "Error", MB_ICONERROR);*/
    /*g_pTexture00 = CCanvas::CreateCanvasFromFile("..\\data\\texture00.bmp");
    if(!g_pTexture00)
        MessageBoxA(NULL, "No se pudo recargar el recurso ", "Error", MB_ICONERROR);*/
    g_pFont = CCanvasFont::LoadFont("..\\data\\font.bmp");
    if (!g_pFont)
        MessageBoxA(NULL, "No se pudo recargar el recurso ", "Error", MB_ICONERROR);
   if(!g_Asteroid.LoadSprites("..\\data\\Asteroids.bmp", 8, 8, CSprite::ColorKeyWhite))
       MessageBoxA(NULL, "No se pudo recargar el recurso ", "Error", MB_ICONERROR);
       // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICS2023I, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICS2023I));

    MSG msg;

    // Bucle principal de mensajes:
    while (1)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

            }
            if (msg.message == WM_QUIT)
                return msg.wParam;
        }
        if (g_DXManager.Ready())
            Render();
    }
    

    return (int) msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICS2023I));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = 0; // (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0; //MAKEINTRESOURCEW(IDC_GRAPHICS2023I);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   if (!g_DXManager.Initialize(hWnd))
   {
       MessageBox(NULL, L"Error al intentar inicializar Direct3D", L"Error fatal", MB_OK | MB_ICONERROR);
       return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYUP:
        switch (wParam)
        {
        case 'Z': g_bRotLeft = false; break;
        case 'X': g_bRotRight = false; break;
        case VK_LEFT:  g_bLeft = false; break;
        case VK_RIGHT: g_bRight = false; break;
        case VK_UP:    g_bUp = false; break;
        case VK_DOWN:  g_bDown = false; break;
       
        }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case '1':
        case '2':
        case '0':
        case '4':
            g_nInputType = wParam - '0';
            break;
        case 'Z': g_bRotLeft = true; break;
        case 'X': g_bRotRight = true; break;
        case VK_LEFT:  g_bLeft = true; break;
        case VK_RIGHT: g_bRight = true; break;
        case VK_UP:    g_bUp = true; break;
        case VK_DOWN:  g_bDown = true; break;

        case 'C':
            switch (g_nInputType)
            {
            case 1:
                g_LineStripCount = 0;
                break;
            case 2:
                g_TriangleListCount = 0;
                break;
            case 4:
                g_TriangleStripCount = 0;
                break;
            }
            break;
        }
        break;
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        return 0;
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            g_Stats.m_nFPS = g_Stats.m_nFrameCount - g_Stats.m_nLastFrameCount;
            g_Stats.m_nLastFrameCount = g_Stats.m_nFrameCount;
            break;
        }
        break;
    case WM_MOUSEMOVE:
        g_mx = LOWORD(lParam);
        g_my = HIWORD(lParam);
        switch (g_nInputType) 
        {
        case 1:
            if (g_bLBPressed && g_LineStripCount < 100)
            {
                MATRIX4D M, InvM;
                VECTOR4D Input, Mapped;
                M = g_Rotate * g_Offset;
                Inverse(M, InvM);
                Input = { (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), 0, 1.0f };
                Mapped = Input * InvM;

                g_LineStrip[g_LineStripCount].Position = Mapped;
                g_LineStrip[g_LineStripCount].Color = { 255,255,255,255 };
                g_LineStripCount++;

            }
            break;
        
        }
        
        break;
    case  WM_LBUTTONDOWN:
    {
        g_bLBPressed = true;
        switch (g_nInputType) 
        {
        case 2:
            if (g_bLBPressed && g_TriangleListCount < 300)
            {
                MATRIX4D M, InvM;
                VECTOR4D Input, Mapped;
                M = g_Rotate * g_Offset;
                Inverse(M, InvM);
                Input = { (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), 0, 1.0f };
                Mapped = Input * InvM;
                g_TriangleList[g_TriangleListCount].Position = Mapped;
                g_TriangleList[g_TriangleListCount].Color = { 255,255,255,255 };
                g_TriangleListCount++;
            }
            break;
        case 4:
            if (g_bLBPressed && g_TriangleStripCount < 100)
            {
                MATRIX4D M, InvM;
                VECTOR4D Input, Mapped;
                M = g_Rotate * g_Offset;
                Inverse(M, InvM);
                Input = { (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam), 0, 1.0f };
                Mapped = Input * InvM;
                g_TriangleStripV[g_TriangleStripCount].Position = Mapped;
                g_TriangleStripV[g_TriangleStripCount].Color = { 255,255,255,0 };
                g_TriangleStripI[g_TriangleStripCount] = g_TriangleStripCount;
                g_TriangleStripCount++;
            }
            break;
        }
        break;
    }
        break;
    case WM_LBUTTONUP:
    {
        g_bLBPressed = false;
    }
        break;
    case WM_SIZE:
        {
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            g_DXManager.Resize(rcClient.right, rcClient.bottom);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        g_DXManager.Uninitialize();
        PostQuitMessage(0);  
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:  g_bLeft = false; break;
        case VK_RIGHT: g_bRight = false; break;
        case VK_UP:    g_bUp = false; break;
        case VK_DOWN:  g_bDown = false; break;
        }
        break;
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

CCanvas::PIXEL ConstantShader(int x, int y)
{
    return { 255,0,0,255 };
}

CCanvas::PIXEL Noise(int x, int y)
{
    unsigned char c = (unsigned char)rand();
    return { c,c,c,c };
}

CCanvas::PIXEL Chess(int x, int y)
{
    int i = x / 64;
    int j = y / 64;

    if ((i%2) ^ (j%2) )
        return { 0,0,0,255 };
    else
        return { 255,255,255, 0 };
}



void  Render(void)
{
    /*Control de vista del documento */
    if (g_bLeft)
        g_PosX += 10;
    if (g_bRight)
        g_PosX -= 10;
    if (g_bUp)
        g_PosY += 10;
    if (g_bDown)
        g_PosY -= 10;
    if (g_bRotLeft)
        g_RotZ += 1;
    if (g_bRotRight)
        g_RotZ -= 1;
    g_Offset = Translation(g_PosX, g_PosY, 0);
    g_Rotate = RotationZ(g_RotZ * 3.141592f / 180);


    char szText[256];
    static float t = 0.0f;
    CCanvas* pCanvas ;
    pCanvas = g_DXManager.GetCanvas();
    g_Stats.m_sx = pCanvas->GetSizeX();
    g_Stats.m_sy = pCanvas->GetSizeY();
    pCanvas->Clear({ 64,64,64,0 });
    
    wsprintfA(szText, "Frm #:%d", g_Stats.m_nFrameCount);
    MATRIX4D M = Scaling(40, 40, 1) * Translation(20, 20, 0);
    g_pFont->PutText(pCanvas, M, szText);
    M = Scaling(40, 40, 1) * Translation(20, 60, 0);
    wsprintfA(szText, "FPS :%d", g_Stats.m_nFPS);
    g_pFont->PutText(pCanvas, M, szText);

    
    CCanvas::VERTEX* pVertex = new CCanvas::VERTEX[300];
    M = g_Rotate * g_Offset;
    if (g_LineStripCount > 1)
    {
        for (int i = 0; i < g_LineStripCount; i++)
        {
            pVertex[i].Position = g_LineStrip[i].Position * M;
            pVertex[i].TexCoord = g_LineStrip[i].TexCoord;
            pVertex[i].Color = g_LineStrip[i].Color;
        }
        pCanvas->DrawLineStrip(pVertex, g_LineStripCount);
    }
    
    if (g_TriangleListCount > 2)
    {
        for (int i = 0; i < g_TriangleListCount; i++)
        {
            pVertex[i].Position = g_TriangleList[i].Position * M;
            pVertex[i].TexCoord = g_TriangleList[i].TexCoord;
            pVertex[i].Color = g_TriangleList[i].Color;
        }
        pCanvas->DrawTriangleList(pVertex, (g_TriangleListCount / 3) * 3);
    }
   
    if (g_TriangleStripCount > 2)
    {
        for (int i = 0; i < g_TriangleStripCount ; i++)
        {
            pVertex[i].Position = g_TriangleStripV[i].Position * M;
            pVertex[i].TexCoord = g_TriangleStripV[i].TexCoord;
            pVertex[i].Color = g_TriangleStripV[i].Color;
        }
        pCanvas->DrawTriangleStripIndexed(pVertex, g_TriangleStripI, g_TriangleStripCount);
    }
    
    delete[] pVertex;

    
    g_DXManager.Show(pCanvas);
    g_Stats.m_nFrameCount++;
    t += 1.0f / 60;
    CCanvas::Destroy(pCanvas);

}
