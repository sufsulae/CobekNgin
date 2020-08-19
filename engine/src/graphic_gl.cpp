#include "include\graphic.h"
#ifdef COBEK_INCLUDE_GRAPHIC_GL
#include "include\window.h"
#include "include\utility\inline.h"
#include "include\utility\sys.h"

using namespace cobek::Utility;
namespace cobek {
	namespace Graphic {
		namespace __internal {
			bool IGraphic_GL::checkShaderCompilation(void* handler, uint kind) {
				int res;
				int InfoLogLength;
				glGetShaderiv((uint)handler, GL_COMPILE_STATUS, &res);
				glGetShaderiv((uint)handler, GL_INFO_LOG_LENGTH, &InfoLogLength);
				if (InfoLogLength > 0) {
					std::vector<char> ErrorMessage(InfoLogLength + 1);
					glGetShaderInfoLog((uint)handler, InfoLogLength, NULL, &ErrorMessage[0]);
					logCallback.invoke(-1, this->get_GraphicType(), strformat("Failed to Compile %s Shader: \n%s", kind == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", &ErrorMessage[0]));
					return false;
				}
				return true;
			}
			bool IGraphic_GL::checkShaderProgram(void* handler, uint kind) {
				GLint res;
				glGetProgramiv((uint)handler, kind, &res);
				if (res != GL_TRUE) {
					int log_length = 1024;
					GLchar message[1024];
					glGetProgramInfoLog((uint)handler, 1024, &log_length, message);
						logCallback.invoke(-1, this->get_GraphicType(), strformat("Failed to %s Shader: \n%s", kind == GL_LINK_STATUS ? "LINK" : "VALIDATE", message));
					glDeleteProgram((uint)handler);
					return false;
				}
				return true;
			}

			inline GraphicShaderInputDataType __getShaderInputDataType(uint code) {
				switch (code) {
					case GL_FLOAT:			return GraphicShaderInputDataType::FLOAT;
					case GL_FLOAT_VEC2:		return GraphicShaderInputDataType::VEC2;
					case GL_FLOAT_VEC3:		return GraphicShaderInputDataType::VEC3;
					case GL_FLOAT_VEC4:		return GraphicShaderInputDataType::VEC4;
					case GL_FLOAT_MAT2:		return GraphicShaderInputDataType::MAT2X2;
					case GL_FLOAT_MAT3:		return GraphicShaderInputDataType::MAT3X3;
					case GL_FLOAT_MAT4:		return GraphicShaderInputDataType::MAT4X4;
				}
				return GraphicShaderInputDataType::UNKNOWN;
			}
			inline uint __getDrawMode(GraphicDrawMode code) {
				switch (code) {
					case GraphicDrawMode::POINTS:			return GL_POINTS;
					case GraphicDrawMode::LINE_STRIP:		return GL_LINE_STRIP;
					case GraphicDrawMode::LINE_LOOP:		return GL_LINE_LOOP;
					case GraphicDrawMode::LINES:			return GL_LINES;
					case GraphicDrawMode::TRIANGLE_STRIP:	return GL_TRIANGLE_STRIP;
					case GraphicDrawMode::TRIANGLE_FAN:		return GL_TRIANGLE_FAN;
					case GraphicDrawMode::TRIANGLES:		return GL_TRIANGLES;
				}
				return GL_FALSE;
			}
			inline void __getErr(Delegate<void(int, int, std::string)>& callBack, const char* funName, uint line) {
				uint code = glGetError();
				if (code != GL_NO_ERROR) {
					callBack.invoke(code, (int)GraphicBackend::OPENGL, strformat("ERROR:%i in %s line %i", code, funName, line - 1));
				}
			}

			IGraphic_GL::IGraphic_GL(Surface* surface) : IGraphic::IGraphic(surface) {
				if (surface != nullptr)
					this->_surface = surface;
				_libShader = std::vector<Shader>();
				_handler = std::unordered_map<const char*, void*>();
				logCallback = Delegate<void(int, int, std::string)>();
			}
			IGraphic_GL::~IGraphic_GL() {
				_libShader.clear();
				_handler.clear();
				_gctx = nullptr;
				_surface = nullptr;
			}

			int IGraphic_GL::get_GraphicType() { return (int)GraphicBackend::OPENGL; }
			bool IGraphic_GL::get_IsInitialized() { return IGraphic::get_IsInitialized(); }
			Surface* IGraphic_GL::get_Surface() { return IGraphic::get_Surface(); }
			std::vector<Shader> IGraphic_GL::get_Shaders() { return IGraphic::get_Shaders(); }
			Shader* IGraphic_GL::get_Shader(id_t shaderID) { return IGraphic::get_Shader(shaderID); }

			int IGraphic_GL::Init() {
				_initialized = false;
				Window::WinPtr* winSurface = (Window::WinPtr*)_surface->winData;
				this->_gctx = Window::WindowMgr::InitilizeGraphicWindow(winSurface, (GraphicBackend)get_GraphicType());

				gladLoadGL();
				//Initializing Some Handler
				int fb;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
				uint vao;
				glGenVertexArrays(1, &vao);
				_handler["fb"] = (void*)(uint)fb;
				_handler["vao"] = (void*)vao;

				//enble some set
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				_surface->gInterface = this;
				_initialized = true;
				_surface->gInterface = this;

				_initialized = true;
				return true;
			}
			int IGraphic_GL::Release() {
				_initialized = false;
				_surface->gInterface = nullptr;
				return true;
			}

			bool IGraphic_GL::CheckFeature(int feature) {
				return glIsEnabled((GLenum)feature);
			}
			int IGraphic_GL::EnableFeature(int feature, void(*fAction)(void*)) {
				glEnable((GLenum)feature);
				if (fAction != nullptr)
					fAction(this->_gctx);
				return true;
			}
			int IGraphic_GL::DisableFeature(int feature, void(*fAction)(void*)) {
				glDisable((GLenum)feature);
				if (fAction != nullptr)
					fAction(this->_gctx);
				return true;
			}

			id_t IGraphic_GL::CreateFrameBuffer() {
				return 0U;
			}
			int IGraphic_GL::DeleteFrameBuffer(id_t handler) {
				return 0;
			}
			int IGraphic_GL::BindFrameBuffer(id_t handler) {
				return 0;
			}

			id_t IGraphic_GL::CreateRenderBuffer() {
				return 0U;
			}
			int IGraphic_GL::DeleteRenderBuffer(id_t handler) {
				return 0;
			}
			int IGraphic_GL::BindRenderBuffer(id_t handler) {
				return 0;
			}

			int IGraphic_GL::BindMesh(Mesh* mesh, bool rebind) {
				if (mesh == nullptr)
					return false;
				glBindBuffer(GL_ARRAY_BUFFER, NULL);
				__getErr(this->logCallback, "glBindBuffer", __LINE__);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
				__getErr(this->logCallback, "glBindBuffer", __LINE__);
				//Check mesh
				/*if (!rebind) {
					for (auto m : _meshHandler) {
						if (m.first == mesh) {
							glBindBuffer(GL_ARRAY_BUFFER, (uint)m.second.vbo);
							__getErr(this->logCallback, "glBindBuffer", __LINE__);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (uint)m.second.ibo);
							__getErr(this->logCallback, "glBindBuffer", __LINE__);
							return true;
						}
					}
				}*/
				uint hPtr[2];
				glGenBuffers(2, hPtr);
				glBindBuffer(GL_ARRAY_BUFFER, hPtr[0]);
				size_t vLen = mesh->get_vertexBufferSize();
				glBufferData(GL_ARRAY_BUFFER, vLen, NULL, mesh->isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
				__getErr(this->logCallback, "glBufferData", __LINE__);
				size_t offset = 0U;
				auto ptrs = mesh->get_vertexBufferPtrs();
				for (auto v : ptrs) {
					auto size = v.size;
					glBufferSubData(GL_ARRAY_BUFFER, offset, size, v.ptr);
					__getErr(this->logCallback, "glBufferSubData", __LINE__);
					offset += size;
				}
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hPtr[1]);

				size_t iLen = mesh->get_indexBufferSize() * sizeof(float);
				auto iPtr = mesh->get_indexBufferPtr();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, iLen, iPtr.ptr, mesh->isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
				__getErr(this->logCallback, "glBufferSubData", __LINE__);
				return true;
			}
			int IGraphic_GL::UnbindMesh(Mesh* mesh, bool release) {
				return false;
			}

			id_t IGraphic_GL::BindTexture(Texture* tex) {
				return false;
			}
			int IGraphic_GL::UnbindTexture(id_t handler) {
				return false;
			}

			Shader* IGraphic_GL::CreateShader(const char* vertexShaderSrc, const char* fragmentShaderSrc, const char* name) {
				if (vertexShaderSrc == nullptr || fragmentShaderSrc == nullptr) {
					return false;
				}
				//Compile Vertex Shader
				auto vHandle = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vHandle, 1, &vertexShaderSrc, NULL);
				glCompileShader(vHandle);
				if (!checkShaderCompilation((void*)vHandle, GL_VERTEX_SHADER))
					return false;

				//Compile Fragment Shader
				auto fHandle = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fHandle, 1, &fragmentShaderSrc, NULL);
				glCompileShader(fHandle);
				if (!checkShaderCompilation((void*)fHandle, GL_FRAGMENT_SHADER))
					return false;

				//Linking Program
				auto shaderHandler = glCreateProgram();
				glAttachShader(shaderHandler, vHandle);
				glAttachShader(shaderHandler, fHandle);
				glLinkProgram(shaderHandler);
				if (!checkShaderProgram((void*)shaderHandler, GL_LINK_STATUS))
					return false;

				//Validating Program
				glValidateProgram(shaderHandler);
				if (!checkShaderProgram((void*)shaderHandler, GL_VALIDATE_STATUS))
					return false;

				glDetachShader(shaderHandler, vHandle);
				glDetachShader(shaderHandler, fHandle);

				glDeleteShader(vHandle);
				glDeleteShader(fHandle);

				Shader newShader = Shader();
				newShader.name = name;
				newShader.handle = (void*)shaderHandler;
				newShader.input = std::unordered_map <std::string, GraphicShaderInputInfo >();

				int maxlength = 0;
				int count = 0;

				//Get Exposed Attributes
				glGetProgramiv(shaderHandler, GL_ACTIVE_ATTRIBUTES, &count);
				glGetProgramiv(shaderHandler, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxlength);
				std::vector<char> buffer(maxlength);
				for (int i = count - 1; i > -1; i--) {
					int size = 0;
					int len = 0;
					uint type = 0;
					glGetActiveAttrib(shaderHandler, i, buffer.size(), &len, &size, &type, &buffer[0]);
					std::string name((char*)&buffer[0], len);
					GraphicShaderInputInfo info = {};
					info.type = GraphicShaderInputType::ATTRIBUTE;
					info.dataType = __getShaderInputDataType(type);
					newShader.input[name] = info;
				}

				//Get Exposed Uniform
				glGetProgramiv(shaderHandler, GL_ACTIVE_UNIFORMS, &count);
				buffer = std::vector<char>(256);
				for (int i = count - 1; i > -1; i--) {
					int size = 0;
					int len = 0;
					uint type = 0;
					glGetActiveUniform(shaderHandler, i, 256, &len, &size, &type, &buffer[0]);
					std::string name((char*)&buffer[0], len - 1);
					GraphicShaderInputInfo info = {};
					info.type = GraphicShaderInputType::UNIFORM;
					info.dataType = __getShaderInputDataType(type);
					newShader.input[name] = info;
				}

				_libShader.push_back(newShader);
				return &_libShader[_libShader.size() - 1];

				return nullptr;
			}
			int IGraphic_GL::AttachShaderToMaterial(Shader* shader, Material* material) {
				if (shader == nullptr || material == nullptr)
					return false;
				for (auto sP : shader->input) {
					uint loc = 0U;
					if (sP.second.type == GraphicShaderInputType::ATTRIBUTE)
						loc = (uint)glGetAttribLocation((uint)shader->handle, sP.first.c_str());
					else
						loc = (uint)glGetUniformLocation((uint)shader->handle, sP.first.c_str());
					material->shaderInputHandler[sP.first] = (void*)loc;
				}
				material->shader = shader;
				return true;
			}
			int IGraphic_GL::DestroyShader(Shader* shader) {
				if (shader == nullptr || shader->handle == nullptr)
					return false;
				return false;
			}

			int IGraphic_GL::DrawMesh(Mesh* mesh, GraphicDrawMode drawMode) {
				glUseProgram((uint)mesh->material->shader->handle);
				__getErr(this->logCallback, "glUseProgram", __LINE__);
				BindMesh(mesh);
				size_t offset = 0U;
				for (auto v : mesh->material->shaderInputHandler) {
					auto meshPtr = mesh->get_vertexBufferPtr(v.first.c_str());
					glEnableVertexAttribArray((uint)v.second);
					__getErr(this->logCallback, "glEnableVertexAttribArray", __LINE__);
					glVertexAttribPointer((uint)v.second, meshPtr.stride, GL_FLOAT, GL_FALSE, 0, (void*)offset);
					__getErr(this->logCallback, "glVertexAttribPointer", __LINE__);
					offset += meshPtr.size;
				}
				glDrawElements(__getDrawMode(drawMode), mesh->get_indexBufferSize(), GL_UNSIGNED_INT, (void*)NULL);
				for (auto m : mesh->material->shaderInputHandler) {
					glDisableVertexAttribArray((uint)m.second);
					__getErr(this->logCallback, "glDisableVertexAttribArray", __LINE__);
				}
				glBindBuffer(GL_ARRAY_BUFFER, NULL);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
				glUseProgram(0);
				return false;
			}
			int IGraphic_GL::DrawVertices(Memory::MappedBuffer<float>* vertices, GraphicDrawMode mode) {
				return false;
			}
			int IGraphic_GL::DrawViewPort(RectI rect) {
				//OpenGL always start from bottom-left screen, so we need to flip that
				glViewport(rect.x, rect.height - rect.y, rect.width, rect.height);
				return true;
			}
			int IGraphic_GL::DrawScissor(RectI rect) {
				//OpenGL always start from bottom-left screen, so we need to flip that
				glScissor(rect.x, rect.height - rect.y, rect.width, rect.height);
				return false;
			}

			int IGraphic_GL::ClearColor(const ColorF& color) {
				glClearColor(color.col0, color.col1, color.col2, color.col3);
				return false;
			}
			int IGraphic_GL::ClearDepth(const float& depth) {
				glClearDepthf(depth);
				return false;
			}
			int IGraphic_GL::ClearStencil(const int& stencil) {
				glClearStencil(stencil);
				return false;
			}

			int IGraphic_GL::BeginRender() {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				return true;
			}
			int IGraphic_GL::EndRender() {
				Window::WindowMgr::SwapBufferGraphicWindow(this->_gctx);
				return true;
			}
		}
	}
}

#endif // COBEK_INCLUDE_GRAPHIC
