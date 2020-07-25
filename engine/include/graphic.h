#pragma once
#ifndef COBEK_GRAPHIC_H
#define COBEK_GRAPHIC_H
#include "common.h"

#ifndef COBEK_MEMORY_H
#include "memory.h"
#endif

#define COBEK_IGRAPHIC(className) \
	class className : public IGraphic {\
	protected:\
		bool checkShaderProgram(void* shaderHandle, uint kind) override;\
		bool checkShaderCompilation(void* shaderHandle, uint kind) override;\
	public:\
		className(Surface* surface);\
		~className();\
		int get_GraphicType() override;\
		bool get_IsInitialized() override;\
		Surface* get_Surface() override;\
		std::vector<Shader> get_Shaders() override;\
		Shader* get_Shader(id_t shaderID) override;\
		int Init() override;\
		int Release() override;\
		bool CheckFeature(int feature) override;\
		int EnableFeature(int feature, void(*fAction)(void*)) override;\
		int DisableFeature(int feature, void(*fAction)(void*)) override;\
		id_t CreateFrameBuffer() override;\
		int DeleteFrameBuffer(id_t handler) override;\
		int BindFrameBuffer(id_t handler) override;\
		id_t CreateRenderBuffer() override;\
		int DeleteRenderBuffer(id_t handler) override;\
		int BindRenderBuffer(id_t handler) override;\
		int BindMesh(Mesh* mesh,bool rebind = false) override;\
		int UnbindMesh(Mesh* mesh,bool release = false) override;\
		id_t BindTexture(Texture* texture) override;\
		int UnbindTexture(id_t handler) override;\
		Shader* CreateShader(const char* vertexShaderSrc, const char* fragmentShaderSrc, const char* name = nullptr) override;\
		int AttachShaderToMaterial(Shader* shader, Material* material)override;\
		int DestroyShader(Shader* shader)override;\
		int DrawMesh(Mesh* mesh, GraphicDrawMode drawMode) override;\
		int DrawVertices(MappedBuffer<float>* vertices, GraphicDrawMode drawMode) override;\
		int DrawViewPort(RectI rect) override;\
		int DrawScissor(RectI rect) override;\
		int ClearColor(const Color& color) override;\
		int ClearDepth(const float& depth) override;\
		int ClearStencil(const int& stencil) override;\
		int BeginRender() override;\
		int EndRender() override;\
	};\

namespace cobek {
	namespace Graphic {
		using namespace cobek::Memory;
		namespace __internal__ {
			struct _MeshHandler {
				void* vbo;
				void* ibo;
			};
		}

		class IGraphic;
		typedef FUNC(gLogCallback, void, int, int, std::string);

		//----GRAPHIC PARAMETER---
		enum class GraphicBackend {
			UNKNOWN = -1,
			DEFAULT,
			OPENGL,
			OPENGLES,
			D3D9,
			D3D11,
			D3D12,
			VULKAN,
		};
		enum class GraphicPixelFormat {
			UNKNOWN = -1,
			R1,
			A8,
			R8,
			R8I,
			R8U,
			R8S,
			R16,
			R16I,
			R16U,
			R16F,
			R16S,
			R32I,
			R32U,
			R32F,
			RG8,
			RG8I,
			RG8U,
			RG8S,
			RG16,
			RG16I,
			RG16U,
			RG16F,
			RG16S,
			RG32I,
			RG32U,
			RG32F,
			RGB8,
			RGB8I,
			RGB8U,
			RGB8S,
			RGB9E5F,
			BGRA8,
			RGBA8,
			RGBA8I,
			RGBA8U,
			RGBA8S,
			RGBA16,
			RGBA16I,
			RGBA16U,
			RGBA16F,
			RGBA16S,
			RGBA32I,
			RGBA32U,
			RGBA32F,
			R5G6B5,
			RGBA4,
			RGB5A1,
			RGB10A2,
			RG11B10F,
		};
		enum class GraphicTextureFormat {
			UNKNOWN = -1,
			DXT1,          //!< DXT1
			DXT3,          //!< DXT3
			DXT5,          //!< DXT5
			LATC1,         //!< LATC1/ATI1
			LATC2,         //!< LATC2/ATI2
			BC6H,          //!< BC6H
			BC7,           //!< BC7
			ETC1,          //!< ETC1 RGB8
			ETC2,          //!< ETC2 RGB8
			ETC2A,         //!< ETC2 RGBA8
			ETC2A1,        //!< ETC2 RGB8A1
			PTC12,         //!< PVRTC1 RGB 2BPP
			PTC14,         //!< PVRTC1 RGB 4BPP
			PTC12A,        //!< PVRTC1 RGBA 2BPP
			PTC14A,        //!< PVRTC1 RGBA 4BPP
			PTC22,         //!< PVRTC2 RGBA 2BPP
			PTC24,         //!< PVRTC2 RGBA 4BPP
			ATC,           //!< ATC RGB 4BPP
			ATCE,          //!< ATCE RGBA 8 BPP explicit alpha
			ATCI,          //!< ATCI RGBA 8 BPP interpolated alpha
			ASTC4x4,       //!< ASTC 4x4 8.0 BPP
			ASTC5x5,       //!< ASTC 5x5 5.12 BPP
			ASTC6x6,       //!< ASTC 6x6 3.56 BPP
			ASTC8x5,       //!< ASTC 8x5 3.20 BPP
			ASTC8x6,       //!< ASTC 8x6 2.67 BPP
			ASTC10x5,      //!< ASTC 10x5 2.56 BPP
		};
		enum class GraphicDepthFormat {
			UNKNOWN = -1,
			D16,
			D24,
			D24S8,
			D32,
			D16F,
			D24F,
			D32F,
			D0S8,
		};
		enum class GraphicPresentInterval {
			UNKNOWN = -1,
			DEFAULT,
			ONE,
			TWO,
			THREE,
			FOUR,
			IMMEDIATE
		};
		enum class GraphicDisplayOrientation {
			UNKNOWN = -1,
			DEFAULT,
			LANDSCAPELEFT,
			LANDSCAPERIGHT,
			PORTRAIT
		};
		enum class GraphicBufferHandlerType {
			VERTEXARRAY,
			VERTEXBUFFER,
			INDEXBUFFER,
			FRAMEBUFFER,
			RENDERBUFFER,
			CUSTOM
		};
		enum class GraphicShaderInputDataType {
			UNKNOWN = 0,
			FLOAT = sizeof(float),
			VEC2 = sizeof(Vec2F),
			VEC3 = sizeof(Vec3F),
			VEC4 = sizeof(Vec4F),
			MAT2X2 = VEC2 * 2 + 1,
			MAT3X3 = VEC3 * 3,
			MAT4X4 = VEC4 * 4
		};
		enum class GraphicShaderInputType {
			UNKNOWN = 0,
			ATTRIBUTE,
			UNIFORM
		};
		enum class GraphicDrawMode {
			UNKNOWN = 0,
			POINTS,
			LINE_STRIP,
			LINE_LOOP,
			LINES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
			TRIANGLES,
		};

		struct GraphicSurfaceData {
			//Surface Param
			GraphicBackend tgtGraphic;
			GraphicPixelFormat pixFormat;
			GraphicDepthFormat depthFmt;
			GraphicPresentInterval presentInterval;
			GraphicDisplayOrientation dispOrient;
			SizeI backBuffSize;
			int multiSampleCount;
			bool fullScreen;

			GraphicSurfaceData() {
				tgtGraphic = (GraphicBackend)0;
				pixFormat = (GraphicPixelFormat)0;
				depthFmt = (GraphicDepthFormat)0;
				presentInterval = (GraphicPresentInterval)0;
				dispOrient = (GraphicDisplayOrientation)0;
				multiSampleCount = 0;
				backBuffSize = SizeI();
				fullScreen = true;
			}
		};
		struct GraphicShaderInputInfo {
			GraphicShaderInputType type = GraphicShaderInputType::UNKNOWN;
			GraphicShaderInputDataType dataType = GraphicShaderInputDataType::UNKNOWN;
		};

		//----GRAPHIC RESOURCE---
		class Surface : public BaseObject<ulong, Surface> {
		public:
			GraphicSurfaceData param;
			void* winData;
			void* gInterface;
			RectI rect;
			Surface() {
				gInterface = nullptr;
				rect = RectI(0, 0, 0, 0);
			}
		};

		class Shader : public BaseObject<id_t, Shader> {
		public:
			const char* name = nullptr;
			void* handle = nullptr;
			std::unordered_map<std::string, GraphicShaderInputInfo> input;
		};

		class Material : public BaseObject<id_t, Material> {
		public:
			Shader* shader;
			std::unordered_map<std::string, void*> shaderInputHandler;

			Material() {
				shader = nullptr;
				shaderInputHandler = std::unordered_map <std::string, void*>();
			}
			~Material() {
				shader = nullptr;
				shaderInputHandler.clear();
			}
		};

		class Mesh : public BaseObject<id_t, Mesh> {
		private:
			MappedBuffer<float> m_vertex;
			std::vector<uint> m_index;
		public:
			Material* material;
			bool isStatic = false;

			Mesh() : BaseObject() {
				m_index = std::vector<uint>();

				material = nullptr;
				isStatic = false;
			}
			~Mesh() {
				m_index.clear();
			}

			int get_index(const uint* buffer, size_t bufferSize);
			int set_index(ArrayPtr<uint>* indexArray);
			ArrayPtr<uint> get_indexPtr();

			int get_vertex(const char* name, const float* buffer, size_t bufferSize);
			int add_vertex(const char* name, ArrayPtr<float>* vertexData);
			int remove_vertex(const char* name);

			size_t get_vertexSize();
			size_t get_indexSize();
			ArrayPtr<float> get_vertexPtr(const char* name);
			std::vector<ArrayPtr<float>> get_vertexPtrs();
		};

		class Texture : public BaseObject<id_t, Shader> {
		public:
			SizeI size = SizeI();
			GraphicPixelFormat pixFmt;
		};

		//----GRAPHIC ABSTRACTION---
		class IGraphic {
		protected:
			bool _initialized;
			void* _gctx;
			Surface* _surface;
			std::vector<Shader> _libShader;
			std::unordered_map<const char*, void*> _handler;
			std::unordered_map<Mesh*, __internal__::_MeshHandler> _meshHandler;

			virtual bool checkShaderProgram(void* shaderHandle, uint kind) { return false; };
			virtual bool checkShaderCompilation(void* shaderHandle, uint kind) { return false; };
		public:
			gLogCallback logCallback;

			IGraphic(Surface* surface) {}
			virtual ~IGraphic() {}

			//Property-Like Function
			virtual int get_GraphicType() { return -1; }
			virtual bool get_IsInitialized() { return _initialized; }
			virtual Surface* get_Surface() { return _surface; }
			virtual std::vector<Shader> get_Shaders() { return _libShader; }
			virtual Shader* get_Shader(id_t shaderID) {
				auto len = _libShader.size();
				for (size_t i = 0; i < len; i++)
				{
					if (_libShader[i].get_id() == shaderID)
						return &_libShader[i];
				}
				return nullptr;
			}

			//Base Function
			virtual int Init() { return false; }
			virtual int Release() { return false; }
			//Backend Feature
			virtual bool CheckFeature(int feature) { return false; }
			virtual int EnableFeature(int feature, void(*fAction)(void*)) { return false; }
			virtual int DisableFeature(int feature, void(*fAction)(void*)) { return false; }
			//Frame Array
			virtual id_t CreateFrameBuffer() { return 0U; }
			virtual int DeleteFrameBuffer(id_t handler) { return false; }
			virtual int BindFrameBuffer(id_t handler) { return false; }
			//Render Array
			virtual id_t CreateRenderBuffer() { return 0U; }
			virtual int DeleteRenderBuffer(id_t handler) { return false; }
			virtual int BindRenderBuffer(id_t handler) { return false; }
			//Mesh Creation
			virtual int BindMesh(Mesh* mesh, bool rebind = false) { return false; }
			virtual int UnbindMesh(Mesh* mesh, bool release = false) { return false; }
			//Texture
			virtual id_t BindTexture(Texture* texture) { return 0U; }
			virtual int UnbindTexture(id_t handler) { return false; }
			//Shader
			virtual Shader* CreateShader(const char* vertexShaderSrc, const char* fragmentShaderSrc, const char* name = nullptr) { return false; }
			virtual int AttachShaderToMaterial(Shader* shader, Material* material) { return false; }
			virtual int DestroyShader(Shader* shader) { return false; }
			//Render
			virtual int DrawMesh(Mesh* mesh, GraphicDrawMode drawMode) { return false; }
			virtual int DrawVertices(Memory::MappedBuffer<float>* vertices, GraphicDrawMode mode) { return false; }
			virtual int DrawViewPort(RectI rect) { return false; }
			virtual int DrawScissor(RectI rect) { return false; }
			virtual int ClearColor(const ColorF& color) { return false; };
			virtual int ClearDepth(const float& depth) { return false; };
			virtual int ClearStencil(const int& stencil) { return false; };
			virtual int Clear(const ColorF& color, float depth, int stencil) {
				ClearColor(color);
				ClearDepth(depth);
				ClearStencil(stencil);
				return true;
			}
			virtual int BeginRender() { return false; }
			virtual int EndRender() { return false; }
		};

		namespace __internal__ {
#ifdef COBEK_INCLUDE_GRAPHIC_GL
			//Generated Script for OpenGL Graphic
			COBEK_IGRAPHIC(IGraphic_OGL);
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_GLES
			//Generated Script for OpenGL ES Graphic
			COBEK_IGRAPHIC(IGraphic_GLES);
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_VULKAN
			//Generated Script for Vulkan Graphic
			COBEK_IGRAPHIC(IGraphic_VULKAN);
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_DX9
			//Generated Script for DirectX 9 Graphic
			COBEK_IGRAPHIC(IGraphic_DX9);
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_DX11
			//Generated Script for DirectX 11 Graphic
			COBEK_IGRAPHIC(IGraphic_DX11);
#endif
#ifdef COBEK_INCLUDE_GRAPHIC_DX12
			//Generated Script for DirectX 12 Graphic
			COBEK_IGRAPHIC(IGraphic_DX12);
#endif
		}


		//----GRAPHIC MANAGER----
		class GraphicMgr final {
		public:
			static std::vector<Surface> surfaces;
			static std::vector<Texture> textures;

			static Surface* CreateSurface(GraphicSurfaceData* surface);
			static IGraphic* ConnectSurface(Surface* surface, void* winPtr);
			static bool DestroySurface(Surface* surface);
		};
	}
}

#undef COBEK_IGRAPHIC

#endif