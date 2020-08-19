#include "include\graphic.h"
namespace cobek {
	namespace Graphic {
		inline IGraphic* __createGraphicInterface(Surface* surface) {
			switch ((GraphicBackend)surface->param.tgtGraphic) {
#if COBEK_GL
			case GraphicBackend::OPENGL:
				return new __internal::IGraphic_GL(surface);
#endif
#if COBEK_GLES
			case GraphicBackend::OPENGLES:
				return new __internal::IGraphic_GLES(surface);
#endif
#if COBEK_DX9
			case GraphicBackend::D3D9:
				return new __internal::IGraphic_DX9(surface);
#endif
#if COBEK_DX11
			case GraphicBackend::D3D11:
				return new __internal::IGraphic_DX11(surface);
#endif
#if COBEK_VULKAN
			case GraphicBackend::VULKAN:
				return new __internal::IGraphic_VULKAN(surface); */
#endif
			default:
				return nullptr;
			}
		}

		//---GraphicMgr---
		std::vector<Surface> GraphicMgr::surfaces = std::vector<Surface>();
		std::vector<Texture> GraphicMgr::textures = std::vector<Texture>();

		Surface* GraphicMgr::CreateSurface(GraphicSurfaceData* surface) {
			if (surface == nullptr)
				return nullptr;
			Surface newSurface = Surface();
			newSurface.param = *surface;
			surfaces.push_back(newSurface);
			return &surfaces[surfaces.size() - 1];
		}
		IGraphic* GraphicMgr::ConnectSurface(Surface* surface, void* winPtr) {
			if (surface == nullptr || winPtr == nullptr)
				return nullptr;

			surface->winData = winPtr;
			auto gInterface = __createGraphicInterface(surface);
			if (gInterface->Init()) {
				return (IGraphic*)surface->gInterface;
			}
			surface->winData = nullptr;
			return nullptr;
		}
		bool GraphicMgr::DestroySurface(Surface* surface) {
			if (surface == nullptr)
				return false;
			return true;
		}
		//---Mesh---
		int Mesh::set_indexBuffer(ArrayPtr<uint>* indexArray) {
			if (indexArray == nullptr || indexArray->ptr == nullptr) {
				m_index.clear();
				m_index.shrink_to_fit();
				return true;
			}
			m_index.clear();
			m_index.shrink_to_fit();
			m_index.insert(m_index.begin(), indexArray->ptr, indexArray->ptr + indexArray->length());
			return true;
		}
		int Mesh::get_indexBuffer(const uint* buffer, size_t bufferSize) {
			if (buffer == nullptr || bufferSize == 0U)
				return false;
			if (bufferSize > m_index.size())
				bufferSize = m_index.size();
			memcpy(&buffer, &m_index[0], bufferSize);
			return true;
		}
		size_t Mesh::get_indexBufferSize() {
			return m_index.size();
		}
		ArrayPtr<uint> Mesh::get_indexBufferPtr() {
			auto res = ArrayPtr<uint>();
			res.ptr = &m_index[0];
			res.size = m_index.size() * sizeof(uint);
			res.sizePerElement = sizeof(uint);
			return res;
		}

		int Mesh::add_vertexBuffer(const char* name, ArrayPtr<float>* vertexData) {
			if (name == nullptr || vertexData == nullptr || vertexData->ptr == nullptr)
				return false;
			return m_vertex.AddBuffer(name, vertexData);
		}
		int Mesh::get_vertexBuffer(const char* name, const float* buffer, size_t bufferSize) {
			if (name == nullptr || buffer == nullptr)
				return false;
			auto data = m_vertex.GetBuffer(name);
			if (data.ptr == nullptr)
				return false;
			if (bufferSize > data.size)
				bufferSize = data.size;
			memcpy(&buffer, data.ptr, bufferSize);
			return true;
		}
		int Mesh::remove_vertexBuffer(const char* name) {
			if (name == nullptr)
				return false;
			return m_vertex.RemoveBuffer(name);
		}
		size_t Mesh::get_vertexBufferSize() {
			return m_vertex.get_size();
		}
		ArrayPtr<float> Mesh::get_vertexBufferPtr(const char* name) {
			return m_vertex.GetBuffer(name);
		}
		std::vector<ArrayPtr<float>> Mesh::get_vertexBufferPtrs() {
			return m_vertex.GetBufferPtrs();
		}
	}
}