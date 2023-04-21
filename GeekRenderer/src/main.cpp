#include "render/renderer.h"

#include <thread>

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

int fps = 0;
int width = 800 , height = 600;

Window* win;
Renderer* renderer;

void cameraInput(Camera* camera);
void drawLoop();
void showFPS();

int main()
{
	//create an visual window
	win = new Window(width, height);

	vector<vec3> pv =
	{
		vec3(-2.0f, -1.0f, -2.0f),
		vec3(-2.0f, -1.0f, 2.0f),
		vec3(2.0f, -1.0f, 2.0f),
		vec3(2.0f, -1.0f, -2.0f)
	};

	Mesh planeMesh = createPlane(pv, vec3(0.0f, 1.0f, 0.0f));
	Mesh boxMesh = createBox(vec3(0.0, 0.0, 0.0), 0.5);

	Material wood;
	Texture boxTexture("D:\\Projects\\GeekRenderer\\GeekRenderer\\assets\\texture\\wood.png");
	wood.setTexture(&boxTexture);

	boxMesh.addMesh(planeMesh);
	Object Box(boxMesh, wood);

	Model* model = new Model();
	model->objects.push_back(Box);
	model->setMaterial(0, wood);

	Camera* camera = new Camera(
		vec3(0.0f, 2.0f, 4.0f),
		vec3(0.0f, 1.0f, 0.0f),
		45.0, (float)width / (float)height,
		1.0f, 20.f,
		-90.0f, -45.0f);

	Shader* shader = new Shader();

	renderer = new Renderer(width, height, model, camera, shader, win);

	//attach the fps
	std::thread t(showFPS);
	t.detach();

	//enter the main loop
	drawLoop();

	delete renderer;
	return 0;
}

void cameraInput(Camera* camera)
{
	float moveSpeed = 0.1f;
	if (IS_KEY_DOWN('A'))
	{
		camera->moveRight(-moveSpeed);
	}

	if (IS_KEY_DOWN('S'))
	{
		camera->moveForward(-moveSpeed);
	}

	if (IS_KEY_DOWN('D'))
	{
		camera->moveRight(moveSpeed);
	}

	if (IS_KEY_DOWN('W'))
	{
		camera->moveForward(moveSpeed);
	}

	if (IS_KEY_DOWN(' '))
	{
		camera->moveUp(moveSpeed);
	}

	if (IS_KEY_DOWN('Z'))
	{
		camera->moveUp(-moveSpeed);
	}

	if (IS_KEY_DOWN('I'))
	{
		camera->rotatePitch(moveSpeed);
	}

	if (IS_KEY_DOWN('K'))
	{
		camera->rotatePitch(-moveSpeed);
	}

	if (IS_KEY_DOWN('J'))
	{
		camera->rotateYaw(-moveSpeed);
	}

	if (IS_KEY_DOWN('L'))
	{
		camera->rotateYaw(moveSpeed);
	}

	if (IS_KEY_DOWN('Q'))
	{
		renderer->changeRenderMode();
	}
}

void drawLoop()
{
	//开始消息循环
	//定义消息结构体
	MSG msg = { 0 };

	//如果PeekMessage函数不等于0，说明没有接受到WM_QUIT
	while (msg.message != WM_QUIT) {

		//处理按键输入
		cameraInput(renderer->camera);

		//如果有窗口消息就进行处理
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);	//键盘按键转换，将虚拟键消息转换为字符消息
			DispatchMessage(&msg);	//把消息分派给相应的窗口过程
		}
		//else里将放置渲染内容，没有窗口信息需要处理时进行渲染
		else {
			renderer->clearBuffer();

			renderer->render();

			//在这里画到设备上，hMem相当于缓冲区
			BitBlt(win->hDC, 0, 0, width, height, win->hMem, 0, 0, SRCCOPY);//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境
			if (fps < 144)
				fps++;
		}
	}
}

void showFPS()
{
	while (1) {
		Sleep(1000);

		wchar_t titleBuffer[30] = L"SoftRenderV0.1 FPS: ";
		wchar_t frameCount[10];
		_itow_s(fps, frameCount, 10, 10);

		titleBuffer[20] += frameCount[0];
		titleBuffer[21] += frameCount[1];
		titleBuffer[22] += frameCount[2];

		//在窗口标题显示
		SetWindowText(win->hWnd, titleBuffer);
		//每一秒重置fps
		fps = 0;
	}
}

