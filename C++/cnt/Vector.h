/**
 * @file cnt/console.h
 * Copyright 2025, aplcexenicesetrl project
 * This project and document files are maintained by CNT Development Team (under the APlcexenicesetrl studio),
 * and according to the project license (MIT license) agreement,
 * the project and documents can be used, modified, merged, published, branched, etc.
 * provided that the project is developed and open-source maintained by CNT Development Team.
 * At the same time,
 * project and documents can be used for commercial purposes under the condition of informing the development source,
 * but it is not allowed to be closed source, but it can be partially source.
 *
 * The project and documents will be updated and maintained from time to time,
 * and any form of dispute event, CNT Development Team.
 * and APlcexicesetrl shall not be liable for any damages,
 * and any compensation shall not be borne by the APlcexenicesetrl studio.
 */
 /* Written by Anders Norlander <taim_way@aplcexenicesetrl.com> */

#pragma once

namespace cnt {
	template<int _VECTOR_BIN>
	class Vector {
	public:
		Vector() : _size(0), _capacity(0), _data(nullptr) {}
		Vector(int size) : _size(size), _capacity(size), _data(new int[size]) {}
		Vector(const Vector& other) : _size(other._size), _capacity(other._capacity), _data(new int[other._capacity]) {
			for (int i = 0; i < _size; ++i) {
				_data[i] = other._data[i];
			}
		}
		Vector& operator=(const Vector& other) {
			if (this != &other) {
				delete[] _data;
				_size = other._size;
				_capacity = other._capacity;
				_data = new int[_capacity];
				for (int i = 0; i < _size; ++i) {
					_data[i] = other._data[i];
				}
			}
			return *this;
		}
		~Vector() { delete[] _data; }
	};
}