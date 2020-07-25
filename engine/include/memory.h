#pragma once
#ifndef COBEK_MEMORY_H
#define COBEK_MEMORY_H

#include "headers.h"

namespace cobek {
	namespace Memory {
		template<typename T> struct ArrayPtr {
			T* ptr = nullptr;
			size_t size = 0U;
			size_t sizePerElement = sizeof(T);
			uint stride = 1U;

			ArrayPtr(T* ptr = nullptr, size_t size = 0U, uint stride = 1U, size_t sizePerElement = sizeof(T)) {
				this->ptr = ptr;
				this->size = size;
				this->stride = stride;
				this->sizePerElement = sizePerElement;
			}

			size_t length() { return size / sizePerElement; }

		};

		namespace __internal__ {
			template<typename T> struct _MapPtr {
				std::string name;
				size_t offset;
				Memory::ArrayPtr<T> data;
			};
		}

		template<typename T> class MappedBuffer : public BaseObject<id_t, MappedBuffer<T>> {
		private:
			std::vector<T> m_mem;
			std::vector<__internal__::_MapPtr<T>> m_ptrs;
		public:
			MappedBuffer() : BaseObject<id_t, MappedBuffer<T>>() {
				m_mem = std::vector<T>();
				m_ptrs = std::vector<__internal__::_MapPtr<T>>();
			}
			~MappedBuffer() {
				m_ptrs.clear();
				m_mem.clear();
			}

			size_t get_size() { return sizeof(T) * m_mem.size(); }
			size_t get_length() { return m_mem.size(); }
			ArrayPtr<T> get_ptr() {
				auto res = ArrayPtr<T>();
				res.ptr = &m_mem[0];
				res.size = m_mem.size() * sizeof(T);
				res.sizePerElement = sizeof(T);
				return res;
			}

			int AddBuffer(std::string name, ArrayPtr<T>* array) {
				if (array == nullptr)
					return false;
				bool registered = false;
				for (auto& p : m_ptrs) {
					//if data already registered
					if (p.name == name) {
						if (p.data.size != array->size) {
							auto diff = (intptr_t)array->length() - (intptr_t)p.data.length();
							if (diff > 0) {
								auto buff = (T*)alloca(sizeof(T));
								m_mem.insert(m_mem.begin() + p.offset + p.data.length(), diff, *buff);
								free(buff);
							}
							else {
								m_mem.erase(m_mem.begin() + p.offset + p.data.length() + diff, m_mem.begin() + p.offset + p.data.length());
							}
						}
						memcpy(&m_mem[p.offset], array->ptr, array->size);
						p.data = *array;
						registered = true;
						break;
					}
				}

				//if data not registered yet
				if (!registered) {
					m_mem.insert(m_mem.end(), array->ptr, array->ptr + array->length());
					auto newPtr = __internal__::_MapPtr<T>();
					newPtr.data = *array;
					newPtr.name = name;
					newPtr.offset = NULL;
					m_ptrs.push_back(newPtr);
				}

				m_mem.shrink_to_fit();

				//Re-Map our ptr
				size_t offset = 0U;
				for (auto& p : m_ptrs) {
					p.data.ptr = &m_mem[offset];
					p.offset = offset;
					offset += p.data.length();
				}
				return true;
			}
			int RemoveBuffer(std::string name) {
				auto len = m_ptrs.size();
				for (size_t i = 0; i < len; i++) {
					auto& mem = m_ptrs[i];
					if (mem.name == name) {
						auto dataLength = mem.data.length();

						//Erase/Dealocate Data
						m_mem.erase(m_mem.begin() + mem.offset, m_mem.begin() + mem.offset + dataLength);
						m_mem.shrink_to_fit();

						//Subtract the offset after this pointer
						for (size_t j = i + 1; j < len; j++) {
							m_ptrs[j].offset -= dataLength;
						}

						//Erase/Dealocate Pointer
						m_ptrs.erase(m_ptrs.begin() + i);
						m_ptrs.shrink_to_fit();

						//Re-Map our ptr
						size_t offset = 0U;
						for (auto& p : m_ptrs) {
							p.data.ptr = &m_mem[offset];
							p.offset = offset;
							offset += p.data.length();
						}
						return true;
					}
				}
				return false;
			}
			ArrayPtr<T> GetBuffer(std::string name) {
				auto arr = ArrayPtr<T>();
				auto len = m_ptrs.size();
				for (size_t i = 0; i < len; i++)
				{
					if (m_ptrs[i].name == name) {
						return m_ptrs[i].data;
					}
				}
				return arr;
			}
			std::vector<std::string> GetBufferNames() {
				auto len = m_ptrs.size();
				auto res = std::vector<std::string>(len);
				for (size_t i = 0; i < len; i++) {
					res[i] = m_ptrs[i].name;
				}
				return res;
			}
			std::vector<ArrayPtr<T>> GetBufferPtrs() {
				auto len = m_ptrs.size();
				auto res = std::vector<ArrayPtr<T>>(len);
				for (size_t i = 0; i < len; i++) {
					res[i] = m_ptrs[i].data;
				}
				return res;
			}
		};
	}
}
#endif